#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH3F.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TMath.h>
#include <Riostream.h>
#include "../utils/Config.h"
#include "TRandom3.h"
#include "TROOT.h"

#define CLOSURE_TEST

#ifdef __CINT__
#pragma link C++ class std::vector<MiniKaon>+;
#pragma link C++ class std::vector<MiniKaonMC>+;
#pragma link C++ class std::vector<MiniXi>+;
#pragma link C++ class std::vector<MiniXiMC>+;
#endif

void ReadTreeEffCorrCpy(const char* fname = "tree_data_full/part_merging_True/%s_AnalysisResults", const char* ofname = "o", const bool isMC = false, const int tree_number = 1)
{

  TFile *fEffK = TFile::Open(Form("%s/%s.root", kDataDir, kEffKFile));
  TFile *fEffXi = TFile::Open(Form("%s/%s.root", kDataDir, kEffXiFile));
  TFile *fEffBDTXi = TFile::Open(Form("%s/%s.root", kDataDir, kEffBDTXiFile));
  TFile f(Form("%s/%s.root", kDataDir, fname));
  TFile o(Form("%s.root", ofname), "recreate");

  #ifndef CLOSURE_TEST
    std::vector<MiniKaon> *k = nullptr;
    std::vector<MiniXi> *xi = nullptr;
  #endif // CLOSURE_TEST
  #ifdef CLOSURE_TEST
    std::vector<MiniKaonMC> *k = nullptr;
    std::vector<MiniXiMC> *xi = nullptr;
    float index = 0.;
    TBranch *bindex = nullptr;
  #endif // CLOSURE_TEST
  MiniCollision *c = new MiniCollision();
  c->fZ = 0.;
  c->fCent = 0.;
  c->fTrigger = 0x00;
  TTree *t = (TTree*)f.Get("StrangenessTree");
  TBranch *bk = nullptr;
  TBranch *bxi = nullptr;
  if (isMC){
    t->SetBranchAddress("MiniKaonMC", &k, &bk);
    t->SetBranchAddress("MiniXiMC", &xi, &bxi);
    #ifdef CLOSURE_TEST
      t->SetBranchAddress("index", &index, &bindex);
    #endif
  }
  else{
    t->SetBranchAddress("MiniKaon", &k, &bk);
    t->SetBranchAddress("MiniXi", &xi, &bxi);
  }
  TBranch *be = t->GetBranch("MiniCollision");
  be->SetAddress(&c);
  
  TH1D *hCent[N_SAMPLE];
  #ifdef CLOSURE_TEST
    TH1D *hKaonQ1_Gen[2][N_SAMPLE];
    TH1D *hKaonQ1Sq_Gen[2][N_SAMPLE];
    TH1D *hKaonQ2_Gen[2][N_SAMPLE];
    TH1D *hXiQ1_Gen[2][N_SAMPLE];
    TH1D *hXiQ1Sq_Gen[2][N_SAMPLE];
    TH1D *hXiQ2_Gen[2][N_SAMPLE];
    TH1D *hSameKaonXiQ11_Gen[2][N_SAMPLE];
    TH1D *hOppKaonXiQ11_Gen[2][N_SAMPLE];
  #endif // CLOSURE_TEST 
  TH1D *hKaonQ1[2][2][N_SAMPLE];
  TH1D *hKaonQ1Sq[2][2][N_SAMPLE];
  TH1D *hKaonQ2[2][2][N_SAMPLE];
  TH1D *hXiQ1[2][2][N_SAMPLE];
  TH1D *hXiQ1Sq[2][2][N_SAMPLE];
  TH1D *hXiQ2[2][2][N_SAMPLE];
  TH1D *hSameKaonXiQ11[2][2][N_SAMPLE];
  TH1D *hOppKaonXiQ11[2][2][N_SAMPLE];
  
  TH3F *hNsigmaTPC[2];
  TH3F *hNsigmaTOF[2];
  TH3F *hMass[2];

  TH1D *hEffK[2][kNCentBinsSmall][N_SAMPLE];
  TH1D *hEffXi[2][kNCentBinsSmall][N_SAMPLE]; // TO BE IMPROVED -> CENTRALITY DIFFERENTIAL ESTIMATE OF XI EFFICIENCY (ALSO BDT)
  TH3F *hBDTEffXi; // TO BE IMPROVED -> CENTRALITY DIFFERENTIAL ESTIMATE OF XI EFFICIENCY (ALSO BDT -> SEPARATELY FOR CHARGES)

  for (int iS = 0; iS < N_SAMPLE; ++iS){
    hCent[iS] = new TH1D(Form("hCent"), ";Centrality (%);Entries", kNCentBinsSmall, kCentBinsSmall);
  }
  for (int iC = 0; iC < 2; ++iC){
    for (int iS = 0; iS < N_SAMPLE; ++iS){
      #ifdef CLOSURE_TEST
        hKaonQ1_Gen[iC][iS] = new TH1D(Form("h%sKaonQ1_Gen", kAntiMatterLabel[iC]), ";Centrality (%);q_{1}^{K}", kNCentBinsSmall, kCentBinsSmall);
        hKaonQ1Sq_Gen[iC][iS] = new TH1D(Form("h%sKaonQ1Sq_Gen", kAntiMatterLabel[iC]), ";Centrality (%);(q_{1}^{K})^{2}", kNCentBinsSmall, kCentBinsSmall);
        hKaonQ2_Gen[iC][iS] = new TH1D(Form("h%sKaonQ2_Gen", kAntiMatterLabel[iC]), ";Centrality (%);q_{2}^{K}", kNCentBinsSmall, kCentBinsSmall);
        hXiQ1_Gen[iC][iS] = new TH1D(Form("h%sXiQ1_Gen", kAntiMatterLabel[iC]), ";Centrality (%);q_{1}^{#Xi}", kNCentBinsSmall, kCentBinsSmall);
        hXiQ1Sq_Gen[iC][iS] = new TH1D(Form("h%sXiQ1Sq_Gen", kAntiMatterLabel[iC]), ";Centrality (%);(q_{1}^{#Xi})^{2}", kNCentBinsSmall, kCentBinsSmall);
        hXiQ2_Gen[iC][iS] = new TH1D(Form("h%sXiQ2_Gen", kAntiMatterLabel[iC]), ";Centrality (%);q_{2}^{#Xi}", kNCentBinsSmall, kCentBinsSmall);
        hSameKaonXiQ11_Gen[iC][iS] = new TH1D(Form("h%sSameKaonXiQ11_Gen", kAntiMatterLabel[iC]), ";Centrality (%);q_{11}^{K^{-}#Xi^{-}}", kNCentBinsSmall, kCentBinsSmall);
        hOppKaonXiQ11_Gen[iC][iS] = new TH1D(Form("h%sOppKaonXiQ11_Gen", kAntiMatterLabel[iC]), ";Centrality (%);q_{11}^{K^{+}#Xi^{-}}", kNCentBinsSmall, kCentBinsSmall);
      #endif // CLOSURE_TEST
      for (int iCorr = 0; iCorr < 2; ++iCorr){
        hKaonQ1[iC][iCorr][iS] = new TH1D(Form("h%sKaonQ1_%s", kAntiMatterLabel[iC], kCorrLabel[iCorr]), ";Centrality (%);q_{1}^{K}", kNCentBinsSmall, kCentBinsSmall);
        hKaonQ1Sq[iC][iCorr][iS] = new TH1D(Form("h%sKaonQ1Sq_%s", kAntiMatterLabel[iC], kCorrLabel[iCorr]), ";Centrality (%);(q_{1}^{K})^{2}", kNCentBinsSmall, kCentBinsSmall);
        hKaonQ2[iC][iCorr][iS] = new TH1D(Form("h%sKaonQ2_%s", kAntiMatterLabel[iC], kCorrLabel[iCorr]), ";Centrality (%);q_{2}^{K}", kNCentBinsSmall, kCentBinsSmall);
        hXiQ1[iC][iCorr][iS] = new TH1D(Form("h%sXiQ1_%s", kAntiMatterLabel[iC], kCorrLabel[iCorr]), ";Centrality (%);q_{1}^{#Xi}", kNCentBinsSmall, kCentBinsSmall);
        hXiQ1Sq[iC][iCorr][iS] = new TH1D(Form("h%sXiQ1Sq_%s", kAntiMatterLabel[iC], kCorrLabel[iCorr]), ";Centrality (%);(q_{1}^{#Xi})^{2}", kNCentBinsSmall, kCentBinsSmall);
        hXiQ2[iC][iCorr][iS] = new TH1D(Form("h%sXiQ2_%s", kAntiMatterLabel[iC], kCorrLabel[iCorr]), ";Centrality (%);q_{2}^{#Xi}", kNCentBinsSmall, kCentBinsSmall);
        hSameKaonXiQ11[iC][iCorr][iS] = new TH1D(Form("h%sSameKaonXiQ11_%s", kAntiMatterLabel[iC], kCorrLabel[iCorr]), ";Centrality (%);q_{11}^{K^{-}#Xi^{-}}", kNCentBinsSmall, kCentBinsSmall);
        hOppKaonXiQ11[iC][iCorr][iS] = new TH1D(Form("h%sOppKaonXiQ11_%s", kAntiMatterLabel[iC], kCorrLabel[iCorr]), ";Centrality (%);q_{11}^{K^{+}#Xi^{-}}", kNCentBinsSmall, kCentBinsSmall);
      }
    }
    for (int iCent = 0; iCent < kNCentBinsSmall; ++iCent){
      for (int iS = 0; iS < N_SAMPLE; ++iS){
        if (fEffK){
          hEffK[iC][iCent][iS] = (TH1D*)fEffK->Get(Form("subsample_%d/h%sEff%s_%d", iS + 1, kAntiMatterLabel[iC], kPartLabel[0], iCent));
        }
        // TO BE IMPROVED -> CENTRALITY DIFFERENTIAL ESTIMATE OF XI EFFICIENCY (ALSO BDT)
        if (fEffXi){
          hEffXi[iC][iCent][iS] = (TH1D*)fEffK->Get(Form("subsample_%d/h%sEff%s_%d", iS + 1, kAntiMatterLabel[iC], kPartLabel[1], iCent)); // (TH1D*)fEffXi->Get(Form("fPreselEff_vs_pt_%s_%.0f_%.0f", kAntiMatterLabelML[iC], kCentBins[iCent], kCentBins[iCent + 1]));
        }
      }
    }
  }
  hBDTEffXi = (TH3F*)fEffBDTXi->Get("hBDTEff");

  #ifdef CLOSURE_TEST
    TH3F *hNKaonXi_Gen[2][2][N_SAMPLE];
    TH2D *hRecKaon[2];
    TH2D *hRecXi[2];
    TH2D *hGenXi[2];
    double ptBins[kNBinsPt + 1];
    for (int iB = 0; iB < kNBinsPt + 1; ++iB){
      ptBins[iB] = kMinPt + kDeltaPt * iB;
    }
  #endif // CLOSURE_TEST
  TH3F *hNKaonXi[2][2][N_SAMPLE];

  for (int iC = 0; iC < 2; ++iC){
    #ifdef CLOSURE_TEST
      hRecKaon[iC] = new TH2D(Form("h%sRecKaon", kAntiMatterLabel[iC]), ";Centrality (%);#it{p}_{T} (GeV/#it{c})", kNCentBins, kCentBins, kNBinsPt, ptBins);
      hGenXi[iC] = new TH2D(Form("h%sGenXi", kAntiMatterLabel[iC]), ";Centrality (%);#it{p}_{T} (GeV/#it{c})", kNCentBins, kCentBins, kNBinsPt, ptBins);
      hRecXi[iC] = new TH2D(Form("h%sRecXi", kAntiMatterLabel[iC]), ";Centrality (%);#it{p}_{T} (GeV/#it{c})", kNCentBins, kCentBins, kNBinsPt, ptBins);
    #endif
    for (int iS = 0; iS < N_SAMPLE; ++iS){
      for (int iM = 0; iM < 2; ++iM){
        #ifdef CLOSURE_TEST
          hNKaonXi_Gen[iC][iM][iS] = new TH3F(Form("h%sNKaonXi%s_Gen", kChargeLabel[iC], kXiChargeLabel[iM]), ";Centrality (%);K;#Xi", 100, 0, 100, 1000, 0, 1000, 20, 0, 20);
        #endif // CLOSURE_TEST
        hNKaonXi[iC][iM][iS] = new TH3F(Form("h%sNKaonXi%s", kChargeLabel[iC], kXiChargeLabel[iM]), ";Centrality (%);K;#Xi", 100, 0, 100, 1000, 0, 1000, 20, 0, 20);
      }
    }
    hNsigmaTPC[iC] = new TH3F(Form("h%sNsigmaTPC", kAntiMatterLabel[iC]), ";Centrality (%);#it{p}_{T} (GeV/#it{c});#it{n}#sigma_{TPC} (a.u.)", 10, 0, 10, 30, 0, 3, 120, -6, 6);
    hNsigmaTOF[iC] = new TH3F(Form("h%sNsigmaTOF", kAntiMatterLabel[iC]), ";Centrality (%);#it{p}_{T} (GeV/#it{c});#it{n}#sigma_{TOF} (a.u.)", 10, 0, 10, 30, 0, 3, 120, -6, 6);
    hMass[iC] = new TH3F(Form("h%sMass", kAntiMatterLabel[iC]), ";Centrality (%);#it{p}_{T} (GeV/#it{c});#it{M}(#pi + #Lambda) (GeV/#it{c}^{2})", 10, 0, 10, 8, 0, 4, 200, 1.29, 1.34);
  }

  Long64_t nEntries = kLimitSample ? kLimitedSample : t->GetEntries();
  Long64_t nEntriesSample = nEntries/N_SAMPLE;
  Long64_t nEntriesSampleRest = nEntries - nEntriesSample*N_SAMPLE;
  // std::cout << "nEntriesSample = " << nEntriesSample << "; nEntriesSampleRest = " << nEntriesSampleRest << std::endl;
  for (int iS = 0; iS < N_SAMPLE; ++iS){
    TH1D hCentTmp("hCentTmp", "hCentTmp", kNCentBins, kCentBins);
    TH1D hCentSmallTmp("hCentSmallTmp", "hCentSmallTmp", kNCentBinsSmall, kCentBinsSmall);
    #ifdef CLOSURE_TEST
      double qK_1_Gen[kNCentBinsSmall][2];
      double qK_1Sq_Gen[kNCentBinsSmall][2];
      double qXi_1_Gen[kNCentBinsSmall][2];
      double qXi_1Sq_Gen[kNCentBinsSmall][2];
      double qK_2_Gen[kNCentBinsSmall][2];
      double qXi_2_Gen[kNCentBinsSmall][2];
      double qKXi_11Same_Gen[kNCentBinsSmall][2];
      double qKXi_11Opp_Gen[kNCentBinsSmall][2];
    #endif // CLOSURE_TEST
    double qK_1[kNCentBinsSmall][2][2];
    double qK_1Sq[kNCentBinsSmall][2][2];
    double qXi_1[kNCentBinsSmall][2][2];
    double qXi_1Sq[kNCentBinsSmall][2][2];
    double qK_2[kNCentBinsSmall][2][2];
    double qXi_2[kNCentBinsSmall][2][2];
    double qKXi_11Same[kNCentBinsSmall][2][2];
    double qKXi_11Opp[kNCentBinsSmall][2][2];

    for (int i = 0; i < 2; ++i){
      for (int j = 0; j < kNCentBinsSmall; ++j){
        #ifdef CLOSURE_TEST
          qK_1_Gen[j][i] = 0;       
          qK_1Sq_Gen[j][i] = 0;       
          qXi_1_Gen[j][i] = 0;      
          qXi_1Sq_Gen[j][i] = 0;      
          qK_2_Gen[j][i] = 0;    
          qXi_2_Gen[j][i] = 0;       
          qKXi_11Same_Gen[j][i] = 0;      
          qKXi_11Opp_Gen[j][i] = 0;
        #endif // CLOSURE_TEST
        for (int k = 0; k < 2; ++k){ // efficiency corrected and w/o correction
          qK_1[j][i][k] = 0;       
          qK_1Sq[j][i][k] = 0;       
          qXi_1[j][i][k] = 0;      
          qXi_1Sq[j][i][k] = 0;      
          qK_2[j][i][k] = 0;    
          qXi_2[j][i][k] = 0;       
          qKXi_11Same[j][i][k] = 0;      
          qKXi_11Opp[j][i][k] = 0;
        }
      }
    }

    #ifdef CLOSURE_TEST
      for (Long64_t i = 0; i < nEntries; ++i){
    #endif

    #ifndef CLOSURE_TEST
    for (Long64_t i = iS * nEntriesSample; i < nEntriesSample * (iS + 1); ++i){
    #endif
      Long64_t e = i;
      
      Long64_t tentry = t->LoadTree(e);
      be->GetEntry(tentry);
      bk->GetEntry(tentry);
      bxi->GetEntry(tentry);
      #ifdef CLOSURE_TEST
        bindex->GetEntry(tentry);
        if (index * nEntries < iS * nEntriesSample || index * nEntries > nEntriesSample * (iS + 1)) continue;
        // std::cout << "index = " << index << std::endl;
      #endif

      if (c->fCent > 90) continue;
      int ic = hCentTmp.FindBin(c->fCent);
      int ic_sm = hCentSmallTmp.FindBin(c->fCent);

      if ( !isMC && ( ( c->fCent > 9.9 && c->fCent < 30 && ( ( c->fTrigger & 1 ) != 1 ) ) || ( c->fCent > 49.9 && ( ( c->fTrigger & 1 ) != 1 ) ) ) ){
        continue;
      }

      hCent[iS]->Fill(c->fCent);
      hCentSmallTmp.Fill(c->fCent);

      #ifdef CLOSURE_TEST
        double qK_1_gen_tmp[] = {0, 0};
        double qXi_1_gen_tmp[] = {0, 0};
        double qK_2_gen_tmp[] = {0, 0};
        double qXi_2_gen_tmp[] = {0, 0};
        Long64_t nK_gen[] = {0, 0};
        Long64_t nXi_gen[] = {0, 0};
      #endif // CLOSURE_TEST
      double qK_1_tmp[][2] = {{0, 0}, {0, 0}};
      double qXi_1_tmp[][2] = {{0, 0}, {0, 0}};
      double qK_1_tmp_update[][2] = {{0, 0}, {0, 0}};
      double qXi_1_tmp_update[][2] = {{0, 0}, {0, 0}};
      double qK_1_sq_tmp[][2] = {{0, 0}, {0, 0}};
      double qXi_1_sq_tmp[][2] = {{0, 0}, {0, 0}};
      double qK_2_tmp[][2] = {{0, 0}, {0, 0}};
      double qXi_2_tmp[][2] = {{0, 0}, {0, 0}};
      Long64_t nK[] = {0, 0};
      Long64_t nXi[] = {0, 0};
      for (UInt_t iK = 0; iK < k->size(); ++iK){
        #ifdef CLOSURE_TEST
        if (std::abs(k->at(iK).fPtMC) > kTOFptCut || std::abs(k->at(iK).fPtMC) < kPtLowLimitK || k->at(iK).fFlag != 1 || std::abs(k->at(iK).fEtaMC) > kEtaCut ) continue;
          int im_MC = k->at(iK).fPtMC > 0 ? 1 : 0;
          qK_1_gen_tmp[im_MC] += 1.;
          qK_2_gen_tmp[im_MC] += 1.;
          nK_gen[im_MC] += 1;
        #endif // CLOSURE_TEST
        if ( std::abs(k->at(iK).fPt) > kPtLowLimitK && std::abs(k->at(iK).fPt) < kTOFptCut &&
            (std::abs(k->at(iK).fEta) < kEtaCut) &&
            ((std::abs(k->at(iK).fPt) < kTPCptCut && (std::abs(k->at(iK).fNsigmaTPC) < kNsigmaTPCcut) ) ||
            (std::abs(k->at(iK).fPt) > kTPCptCut && (std::abs(k->at(iK).fPt) < kTOFptCut) && (std::abs(k->at(iK).fNsigmaTPC) < kNsigmaTPCcutPresel) && (std::abs(k->at(iK).fNsigmaTOF) < kNsigmaTOFcut)))
          )
        {
          #ifdef CLOSURE_TEST
            int im_ = k->at(iK).fPt > 0 ? 1 : 0;
            double eff_ = fEffK ? hEffK[im_][ic_sm - 1][iS]->GetBinContent(hEffK[im_][ic_sm - 1][iS]->FindBin(std::abs(k->at(iK).fPt))) : kDummyEffK;
            if (!k->at(iK).fIsReconstructed || k->at(iK).fFlag != 1) continue;
            int im_tmp = k->at(iK).fPt > 0 ? 1 : 0;
            hRecKaon[im_tmp]->Fill(c->fCent, std::abs(k->at(iK).fPt), 1./eff_);
          #endif // CLOSURE_TEST
          int im = k->at(iK).fPt > 0 ? 1 : 0;
          //std::cout << "im = " << im << ", ic = " << ic - 1 << std::endl;
          double eff = fEffK ? hEffK[im][ic_sm - 1][iS]->GetBinContent(hEffK[im][ic_sm - 1][iS]->FindBin(std::abs(k->at(iK).fPt))) : kDummyEffK;
          //std::cout << "pt = " << std::abs(k->at(iK).fPt) << "; bin = " << hEffK[im][ic - 1]->GetXaxis()->FindBin(std::abs(k->at(iK).fPt)) << "eff = " << eff << std::endl;
          qK_1_tmp[im][0] += 1.;
          qK_1_tmp_update[im][0] += 1.;
          qK_2_tmp[im][0] += 1.;
          qK_1_tmp[im][1] += (1./eff);
          qK_1_tmp_update[im][1] += (1./eff);
          double q2 = 1./eff/eff;
          qK_2_tmp[im][1] += q2;
          nK[im] += 1;
          hNsigmaTPC[im]->Fill(c->fCent, std::abs(k->at(iK).fPt), k->at(iK).fNsigmaTPC);
          hNsigmaTOF[im]->Fill(c->fCent, std::abs(k->at(iK).fPt), k->at(iK).fNsigmaTOF);
        }
      }
      for (int iM = 0; iM < 2; ++iM){
        for (int iCorr = 0; iCorr < 2; ++iCorr){
          double q1_sq = qK_1_tmp[iM][iCorr] * qK_1_tmp[iM][iCorr];
          qK_1_sq_tmp[iM][iCorr] += q1_sq;
          //std::cout << "qK_1_sq_tmp[" << iM << "][" << iCorr << "] = " << qK_1_tmp_update[iM][iCorr] * qK_1_tmp_update[iM][iCorr] << std::endl;
          //std::cout << "qK_1_tmp[" << iM << "][" << iCorr << "] = " << qK_1_tmp[iM][iCorr] << std::endl;
          qK_1_tmp_update[iM][iCorr] = 0.;
        }
      }
      for (UInt_t iXi = 0; iXi < xi->size(); ++iXi){
        #ifdef CLOSURE_TEST
          if (std::abs(xi->at(iXi).fPtMC) > kXiUpPtCut || std::abs(xi->at(iXi).fPtMC) < kXiLowPtCut || std::abs(xi->at(iXi).fEtaMC) > kEtaCut) continue;
          int im_MC = xi->at(iXi).fPtMC > 0;
          qXi_1_gen_tmp[im_MC] += 1.;
          qXi_2_gen_tmp[im_MC] += 1.;
          nXi_gen[im_MC] += 1;
          hGenXi[im_MC]->Fill(c->fCent, std::abs(xi->at(iXi).fPtMC));
        #endif // CLOSURE_TEST
        if (
            std::abs(xi->at(iXi).fEta) < kEtaCut &&
            std::abs(xi->at(iXi).fPt) > kXiLowPtCut && std::abs(xi->at(iXi).fPt) < kXiUpPtCut &&
            std::abs(xi->at(iXi).fMass - kXiMass) < kXiMassCut
          )
        {
          #ifdef CLOSURE_TEST
            if (!xi->at(iXi).fIsReconstructed || xi->at(iXi).fFlag != 1) continue;
          #endif // CLOSURE_TEST
          double bdtEff = 1.;
          if (!isMC){
            bdtEff = kBdtEffCut;
            double bdtScoreCut = hBDTEffXi->GetBinContent(hBDTEffXi->FindBin(c->fCent, std::abs(xi->at(iXi).fPt), bdtEff));
            if (xi->at(iXi).fBdtOut < bdtScoreCut) continue;
            // std::cout << "bdtScoreCut = " << bdtScoreCut << std::endl;
          }
          int im = xi->at(iXi).fPt > 0 ? 1 : 0;
          #ifdef CLOSURE_TEST
            hRecXi[im]->Fill(c->fCent, std::abs(xi->at(iXi).fPt));
          #endif // CLOSURE_TEST
          double eff = fEffXi ? hEffXi[im][ic_sm - 1][iS]->GetBinContent(hEffXi[im][ic_sm - 1][iS]->FindBin(std::abs(xi->at(iXi).fPt))) : kDummyEffXi;
          // std::cout << "pt = " << std::abs(xi->at(iXi).fPt) << "; bin = " << hEffXi[im][ic - 1]->FindBin(std::abs(xi->at(iXi).fPt)) << "eff = " << eff << std::endl;
          qXi_1_tmp[im][0] += 1.;
          qXi_1_tmp_update[im][0] += 1.;
          qXi_2_tmp[im][0] += 1.;
          qXi_1_tmp[im][1] += (1./eff/bdtEff);
          qXi_1_tmp_update[im][1] += (1./eff/bdtEff);
          qXi_2_tmp[im][1] += (1./eff/eff/bdtEff/bdtEff);
          nXi[im] += 1;
          hMass[im]->Fill(c->fCent, std::abs(xi->at(iXi).fPt), xi->at(iXi).fMass);
        }
      }
      for (int iM = 0; iM < 2; ++iM){
        for (int iCorr = 0; iCorr < 2; ++iCorr){
          qXi_1_sq_tmp[iM][iCorr] += (qXi_1_tmp_update[iM][iCorr] * qXi_1_tmp_update[iM][iCorr]);
          qXi_1_tmp_update[iM][iCorr] = 0.;
        }
      }

      for (int iM = 0; iM < 2; ++iM){
        #ifdef CLOSURE_TEST
          qK_1_Gen[ic_sm-1][iM] += qK_1_gen_tmp[iM];
          qK_1Sq_Gen[ic_sm-1][iM] += (qK_1_gen_tmp[iM]*qK_1_gen_tmp[iM]);
          qK_2_Gen[ic_sm-1][iM] += qK_2_gen_tmp[iM];
          qXi_1_Gen[ic_sm-1][iM] += qXi_1_gen_tmp[iM];
          qXi_1Sq_Gen[ic_sm-1][iM] += (qXi_1_gen_tmp[iM]*qXi_1_gen_tmp[iM]);
          //std::cout << "qXi_1_gen_tmp[iM][iC] = " << qXi_1_gen_tmp[iM] << "; square = " << qXi_1_gen_tmp[iM]*qXi_1_gen_tmp[iM] << std::endl;
          qXi_2_Gen[ic_sm-1][iM] += qXi_2_gen_tmp[iM];
          qKXi_11Same_Gen[ic_sm-1][iM] += (qK_1_gen_tmp[iM]*qXi_1_gen_tmp[1-iM]);
          qKXi_11Opp_Gen[ic_sm-1][iM] += (qK_1_gen_tmp[1-iM]*qXi_1_gen_tmp[1-iM]);
          hNKaonXi_Gen[0][iM][iS]->Fill(c->fCent, nK_gen[iM], nXi_gen[1-iM]);
          hNKaonXi_Gen[1][iM][iS]->Fill(c->fCent, nK_gen[1-iM], nXi_gen[1-iM]);
        #endif // CLOSURE_TEST
        for (int iC = 0; iC < 2; ++iC){ // loop over efficiency correction
          qK_1[ic_sm-1][iC][iM] += qK_1_tmp[iM][iC];
          qK_1Sq[ic_sm-1][iC][iM] += qK_1_sq_tmp[iM][iC];
          qK_2[ic_sm-1][iC][iM] += qK_2_tmp[iM][iC];
          qXi_1[ic_sm-1][iC][iM] += qXi_1_tmp[iM][iC];
          qXi_1Sq[ic_sm-1][iC][iM] += qXi_1_sq_tmp[iM][iC];
          //std::cout << "qXi_1_tmp[iM][iC] = " << qXi_1_tmp[iM][iC] << "; square = " << qXi_1_tmp[iM][iC]*qXi_1_tmp[iM][iC] << std::endl;
          qXi_2[ic_sm-1][iC][iM] += qXi_2_tmp[iM][iC];
          qKXi_11Same[ic_sm-1][iC][iM] += (qK_1_tmp[iM][iC]*qXi_1_tmp[1-iM][iC]);
          qKXi_11Opp[ic_sm-1][iC][iM] += (qK_1_tmp[1-iM][iC]*qXi_1_tmp[1-iM][iC]);
        }
        hNKaonXi[0][iM][iS]->Fill(c->fCent, nK[iM], nXi[1-iM]);
        hNKaonXi[1][iM][iS]->Fill(c->fCent, nK[1-iM], nXi[1-iM]);
      }
      
    }

    #ifdef CLOSURE_TEST
      o.mkdir(Form("subsample_%d", iS + 1));
      o.cd(Form("subsample_%d", iS + 1));
    #endif
    #ifndef CLOSURE_TEST
    o.mkdir(Form("subsample_%s", ofname));
    o.cd(Form("subsample_%s", ofname));
    #endif
    for (int iM = 0; iM < 2; ++iM){
      for (int iC = 0; iC < kNCentBinsSmall; ++iC){
        double ev = hCentSmallTmp.GetBinContent(iC+1);
        // std::cout << "n_ev = " << ev << std::endl;
        #ifdef CLOSURE_TEST
          hKaonQ1_Gen[iM][iS]->SetBinContent(iC + 1, ev > 0 ? qK_1_Gen[iC][iM]/ev : 0);
          hKaonQ1Sq_Gen[iM][iS]->SetBinContent(iC + 1, ev > 0 ? qK_1Sq_Gen[iC][iM]/ev : 0);
          hKaonQ2_Gen[iM][iS]->SetBinContent(iC + 1, ev > 0 ? qK_2_Gen[iC][iM]/ev : 0);
          hXiQ1_Gen[iM][iS]->SetBinContent(iC + 1, ev > 0 ? qXi_1_Gen[iC][iM]/ev : 0);
          hXiQ1Sq_Gen[iM][iS]->SetBinContent(iC + 1, ev > 0 ? qXi_1Sq_Gen[iC][iM]/ev : 0);
          hXiQ2_Gen[iM][iS]->SetBinContent(iC + 1, ev > 0 ? qXi_2_Gen[iC][iM]/ev : 0);
          hSameKaonXiQ11_Gen[iM][iS]->SetBinContent(iC + 1, ev > 0 ? qKXi_11Same_Gen[iC][iM]/ev : 0);
          hOppKaonXiQ11_Gen[iM][iS]->SetBinContent(iC + 1, ev > 0 ? qKXi_11Opp_Gen[iC][iM]/ev : 0);
        #endif // CLOSURE_TEST
        for (int iCorr = 0; iCorr < 2; ++iCorr){
          hKaonQ1[iM][iCorr][iS]->SetBinContent(iC + 1, ev > 0 ? qK_1[iC][iCorr][iM]/ev : 0);
          hKaonQ1Sq[iM][iCorr][iS]->SetBinContent(iC + 1, ev > 0 ? qK_1Sq[iC][iCorr][iM]/ev : 0);
          hKaonQ2[iM][iCorr][iS]->SetBinContent(iC + 1, ev > 0 ? qK_2[iC][iCorr][iM]/ev : 0);
          hXiQ1[iM][iCorr][iS]->SetBinContent(iC + 1, ev > 0 ? qXi_1[iC][iCorr][iM]/ev : 0);
          hXiQ1Sq[iM][iCorr][iS]->SetBinContent(iC + 1, ev > 0 ? qXi_1Sq[iC][iCorr][iM]/ev : 0);
          hXiQ2[iM][iCorr][iS]->SetBinContent(iC + 1, ev > 0 ? qXi_2[iC][iCorr][iM]/ev : 0);
          hSameKaonXiQ11[iM][iCorr][iS]->SetBinContent(iC + 1, ev > 0 ? qKXi_11Same[iC][iCorr][iM]/ev : 0);
          hOppKaonXiQ11[iM][iCorr][iS]->SetBinContent(iC + 1, ev > 0 ? qKXi_11Opp[iC][iCorr][iM]/ev : 0);
        }
      }
      #ifdef CLOSURE_TEST
        hKaonQ1_Gen[iM][iS]->Write();
        hKaonQ1Sq_Gen[iM][iS]->Write();
        hKaonQ2_Gen[iM][iS]->Write();
        hXiQ1_Gen[iM][iS]->Write();
        hXiQ1Sq_Gen[iM][iS]->Write();
        hXiQ2_Gen[iM][iS]->Write();
        hSameKaonXiQ11_Gen[iM][iS]->Write();
        hOppKaonXiQ11_Gen[iM][iS]->Write();
        for (int iC = 0; iC < 2; ++iC)
          hNKaonXi_Gen[iM][iC][iS]->Write();
      #endif // CLOSURE_TEST
      for (int iC = 0; iC < 2; ++iC){
        hKaonQ1[iM][iC][iS]->Write();
        hKaonQ1Sq[iM][iC][iS]->Write();
        hKaonQ2[iM][iC][iS]->Write();
        hXiQ1[iM][iC][iS]->Write();
        hXiQ1Sq[iM][iC][iS]->Write();
        hXiQ2[iM][iC][iS]->Write();
        hSameKaonXiQ11[iM][iC][iS]->Write();
        hOppKaonXiQ11[iM][iC][iS]->Write();
        hNKaonXi[iC][iM][iS]->Write();
      }
    }
    hCent[iS]->Write();
  }


  o.cd();
  #ifdef CLOSURE_TEST
    for (int iM = 0; iM < 2; ++iM){
      hRecKaon[iM]->Write();
      hRecXi[iM]->Write();
      hGenXi[iM]->Write();
    }
  #endif
  for (int iC = 0; iC < 2; ++iC){
    hNsigmaTOF[iC]->Write();
    hNsigmaTPC[iC]->Write();
    hMass[iC]->Write();
  }

  // close stream
  o.Close();
  f.Close();
}

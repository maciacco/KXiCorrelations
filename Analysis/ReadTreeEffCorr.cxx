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

//#define CLOSURE_TEST

#ifdef __CINT__
#pragma link C++ class std::vector<MiniKaon>+;
#pragma link C++ class std::vector<MiniKaonMC>+;
#pragma link C++ class std::vector<MiniXi>+;
#pragma link C++ class std::vector<MiniXiMC>+;
#endif

void ReadTreeEffCorr(const char* fname = "tree_data_full/AnalysisResults_child_1", const char* ofname = "o", const bool isMC = false){

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

  TH1D *hEffK[2][kNCentBinsSmall];
  TH1D *hEffXi[2][1]; // TO BE IMPROVED -> CENTRALITY DIFFERENTIAL ESTIMATE OF XI EFFICIENCY (ALSO BDT)
  TH3F *hBDTEffXi; // TO BE IMPROVED -> CENTRALITY DIFFERENTIAL ESTIMATE OF XI EFFICIENCY (ALSO BDT -> SEPARATELY FOR CHARGES)

  for (int iS = 0; iS < N_SAMPLE; ++iS){
    hCent[iS] = new TH1D(Form("hCent_%d", iS), ";Centrality (%);Entries", kNCentBinsSmall, kCentBinsSmall);
  }
  for (int iC = 0; iC < 2; ++iC){
    for (int iS = 0; iS < N_SAMPLE; ++iS){
      #ifdef CLOSURE_TEST
        hKaonQ1_Gen[iC][iS] = new TH1D(Form("h%sKaonQ1_Gen_%d", kAntiMatterLabel[iC], iS), ";Centrality (%);q_{1}^{K}", kNCentBinsSmall, kCentBinsSmall);
        hKaonQ1Sq_Gen[iC][iS] = new TH1D(Form("h%sKaonQ1Sq_Gen_%d", kAntiMatterLabel[iC], iS), ";Centrality (%);(q_{1}^{K})^{2}", kNCentBinsSmall, kCentBinsSmall);
        hKaonQ2_Gen[iC][iS] = new TH1D(Form("h%sKaonQ2_Gen_%d", kAntiMatterLabel[iC], iS), ";Centrality (%);q_{2}^{K}", kNCentBinsSmall, kCentBinsSmall);
        hXiQ1_Gen[iC][iS] = new TH1D(Form("h%sXiQ1_Gen_%d", kAntiMatterLabel[iC], iS), ";Centrality (%);q_{1}^{#Xi}", kNCentBinsSmall, kCentBinsSmall);
        hXiQ1Sq_Gen[iC][iS] = new TH1D(Form("h%sXiQ1Sq_Gen_%d", kAntiMatterLabel[iC], iS), ";Centrality (%);(q_{1}^{#Xi})^{2}", kNCentBinsSmall, kCentBinsSmall);
        hXiQ2_Gen[iC][iS] = new TH1D(Form("h%sXiQ2_Gen_%d", kAntiMatterLabel[iC], iS), ";Centrality (%);q_{2}^{#Xi}", kNCentBinsSmall, kCentBinsSmall);
        hSameKaonXiQ11_Gen[iC][iS] = new TH1D(Form("h%sSameKaonXiQ11_Gen_%d", kAntiMatterLabel[iC], iS), ";Centrality (%);q_{11}^{K^{-}#Xi^{-}}", kNCentBinsSmall, kCentBinsSmall);
        hOppKaonXiQ11_Gen[iC][iS] = new TH1D(Form("h%sOppKaonXiQ11_Gen_%d", kAntiMatterLabel[iC], iS), ";Centrality (%);q_{11}^{K^{+}#Xi^{-}}", kNCentBinsSmall, kCentBinsSmall);
      #endif // CLOSURE_TEST
      for (int iCorr = 0; iCorr < 2; ++iCorr){
        hKaonQ1[iC][iCorr][iS] = new TH1D(Form("h%sKaonQ1_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iS), ";Centrality (%);q_{1}^{K}", kNCentBinsSmall, kCentBinsSmall);
        hKaonQ1Sq[iC][iCorr][iS] = new TH1D(Form("h%sKaonQ1Sq_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iS), ";Centrality (%);(q_{1}^{K})^{2}", kNCentBinsSmall, kCentBinsSmall);
        hKaonQ2[iC][iCorr][iS] = new TH1D(Form("h%sKaonQ2_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iS), ";Centrality (%);q_{2}^{K}", kNCentBinsSmall, kCentBinsSmall);
        hXiQ1[iC][iCorr][iS] = new TH1D(Form("h%sXiQ1_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iS), ";Centrality (%);q_{1}^{#Xi}", kNCentBinsSmall, kCentBinsSmall);
        hXiQ1Sq[iC][iCorr][iS] = new TH1D(Form("h%sXiQ1Sq_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iS), ";Centrality (%);(q_{1}^{#Xi})^{2}", kNCentBinsSmall, kCentBinsSmall);
        hXiQ2[iC][iCorr][iS] = new TH1D(Form("h%sXiQ2_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iS), ";Centrality (%);q_{2}^{#Xi}", kNCentBinsSmall, kCentBinsSmall);
        hSameKaonXiQ11[iC][iCorr][iS] = new TH1D(Form("h%sSameKaonXiQ11_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iS), ";Centrality (%);q_{11}^{K^{-}#Xi^{-}}", kNCentBinsSmall, kCentBinsSmall);
        hOppKaonXiQ11[iC][iCorr][iS] = new TH1D(Form("h%sOppKaonXiQ11_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iS), ";Centrality (%);q_{11}^{K^{+}#Xi^{-}}", kNCentBinsSmall, kCentBinsSmall);
      }
    }
    for (int iCent = 0; iCent < kNCentBins; ++iCent){
      if (fEffK){
        hEffK[iC][iCent] = (TH1D*)fEffK->Get(Form("h%sEff_%d", kAntiMatterLabel[iC], iCent));
      }
    }
    // TO BE IMPROVED -> CENTRALITY DIFFERENTIAL ESTIMATE OF XI EFFICIENCY (ALSO BDT)
    if (fEffXi){
      hEffXi[iC][0] = (TH1D*)fEffXi->Get(Form("fPreselEff_vs_pt_%s_0_90", kAntiMatterLabelML[iC]));
    }
  }
  hBDTEffXi = (TH3F*)fEffBDTXi->Get("hBDTEff");

  #ifdef CLOSURE_TEST
    TH3F *hNKaonXi_Gen[2][N_SAMPLE];
  #endif // CLOSURE_TEST
  TH3F *hNKaonXi[2][N_SAMPLE];

  for (int iC = 0; iC < 2; ++iC){
    for (int iS = 0; iS < N_SAMPLE; ++iS){
      #ifdef CLOSURE_TEST
        hNKaonXi_Gen[iC][iS] = new TH3F(Form("h%sNKaonXi_Gen_%d", kChargeLabel[iC], iS), ";Centrality (%);K;#Xi", 100, 0, 100, 500, 0, 500, 20, 0, 20);
      #endif // CLOSURE_TEST
      hNKaonXi[iC][iS] = new TH3F(Form("h%sNKaonXi_%d", kChargeLabel[iC], iS), ";Centrality (%);K;#Xi", 100, 0, 100, 500, 0, 500, 20, 0, 20);
    }
  }

  Long64_t nEntries = t->GetEntries();
  int nEntriesSample = nEntries/N_SAMPLE;
  int nEntriesSampleRest = nEntries - nEntriesSample*N_SAMPLE;
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

    for (int i = iS * nEntriesSample; i < nEntriesSample * (iS + 1); ++i){
      int e = isMC ? (int)(gRandom->Rndm() * nEntries) : i;
      
      Long64_t tentry = t->LoadTree(e);
      be->GetEntry(tentry);
      bk->GetEntry(tentry);
      bxi->GetEntry(tentry);

      if (c->fCent > 90) continue;
      int ic = hCentTmp.FindBin(c->fCent);
      int ic_sm = hCentSmallTmp.FindBin(c->fCent);

      if ( !isMC && ( ( c->fCent > 9.9 && c->fCent < 30 && ( ( c->fTrigger & 1 ) != 1 ) ) || ( c->fCent > 49.9 && ( ( c->fTrigger & 1 ) != 1 ) ) ) ){
        continue;
      }

      hCent[iS]->Fill(c->fCent);

      #ifdef CLOSURE_TEST
        double qK_1_gen_tmp[] = {0, 0};
        double qXi_1_gen_tmp[] = {0, 0};
        double qK_2_gen_tmp[] = {0, 0};
        double qXi_2_gen_tmp[] = {0, 0};
        int nK_gen[] = {0, 0};
        int nXi_gen[] = {0, 0};
      #endif // CLOSURE_TEST
      double qK_1_tmp[][2] = {{0, 0}, {0, 0}};
      double qXi_1_tmp[][2] = {{0, 0}, {0, 0}};
      double qK_2_tmp[][2] = {{0, 0}, {0, 0}};
      double qXi_2_tmp[][2] = {{0, 0}, {0, 0}};
      int nK[] = {0, 0};
      int nXi[] = {0, 0};
      for (UInt_t iK = 0; iK < k->size(); ++iK){
        #ifdef CLOSURE_TEST
          if (std::abs(k->at(iK).fPtMC) > 1.5 || std::abs(k->at(iK).fPtMC) < 0.2 || k->at(iK).fFlag != 1) continue;
          int im_MC = k->at(iK).fPtMC > 0;
          qK_1_gen_tmp[im_MC] += 1.;
          qK_2_gen_tmp[im_MC] += 1.;
          nK_gen[im_MC] += 1;
        #endif // CLOSURE_TEST
        if ( std::abs(k->at(iK).fPt) > 0.2 &&
            (std::abs(k->at(iK).fEta) < kEtaCut) &&
            ((std::abs(k->at(iK).fPt) < kTPCptCut && (std::abs(k->at(iK).fNsigmaTPC) < kNsigmaTPCcut) ) ||
            (std::abs(k->at(iK).fPt) > kTPCptCut && (std::abs(k->at(iK).fPt) < kTOFptCut) && (std::abs(k->at(iK).fNsigmaTPC) < kNsigmaTPCcut) && (std::abs(k->at(iK).fNsigmaTOF) < kNsigmaTOFcut)))
          )
        {
          #ifdef CLOSURE_TEST
            if (!k->at(iK).fIsReconstructed || k->at(iK).fFlag != 1) continue;
          #endif // CLOSURE_TEST
          int im = k->at(iK).fPt > 0;
          // std::cout << "im = " << im << ", ic = " << ic - 1 << std::endl;
          double eff = fEffK ? hEffK[im][ic - 1]->GetBinContent(hEffK[im][ic - 1]->FindBin(std::abs(k->at(iK).fPt))) : kDummyEffK;
          qK_1_tmp[im][0] += 1.;
          qK_2_tmp[im][0] += 1.;
          qK_1_tmp[im][1] += 1./eff;
          qK_2_tmp[im][1] += 1./eff/eff;
          nK[im] += 1;
        }
      }
      for (UInt_t iXi = 0; iXi < xi->size(); ++iXi){
        #ifdef CLOSURE_TEST
          if (std::abs(xi->at(iXi).fPtMC) > 4 || std::abs(xi->at(iXi).fPtMC) < 1) continue;
          int im_MC = xi->at(iXi).fPtMC > 0;
          qXi_1_gen_tmp[im_MC] += 1.;
          qXi_2_gen_tmp[im_MC] += 1.;
          nXi_gen[im_MC] += 1;
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
          int im = xi->at(iXi).fPt > 0;
          double eff = fEffXi ? hEffXi[im][0]->GetBinContent(hEffXi[im][0]->FindBin(std::abs(xi->at(iXi).fPt))) : kDummyEffXi;
          qXi_1_tmp[im][0] += 1.;
          qXi_2_tmp[im][0] += 1.;
          qXi_1_tmp[im][1] += 1./eff/bdtEff;
          qXi_2_tmp[im][1] += 1./eff/eff/bdtEff/bdtEff;
          nXi[im] += 1;
        }
      }

      for (int iM = 0; iM < 2; ++iM){
        #ifdef CLOSURE_TEST
          qK_1_Gen[ic_sm-1][iM] += qK_1_gen_tmp[iM];
          qK_1Sq_Gen[ic_sm-1][iM] += (qK_1_gen_tmp[iM]*qK_1_gen_tmp[iM]);
          qK_2_Gen[ic_sm-1][iM] += qK_2_gen_tmp[iM];
          qXi_1_Gen[ic_sm-1][iM] += qXi_1_gen_tmp[iM];
          qXi_1Sq_Gen[ic_sm-1][iM] += (qXi_1_gen_tmp[iM]*qXi_1_gen_tmp[iM]);
          qXi_2_Gen[ic_sm-1][iM] += qXi_2_gen_tmp[iM];
          qKXi_11Same_Gen[ic_sm-1][iM] += (qK_1_gen_tmp[iM]*qXi_1_gen_tmp[1-iM]);
          qKXi_11Opp_Gen[ic_sm-1][iM] += (qK_1_gen_tmp[iM]*qXi_1_gen_tmp[iM]);
          hNKaonXi_Gen[0][iS]->Fill(c->fCent, nK_gen[iM], nXi_gen[1-iM]);
          hNKaonXi_Gen[1][iS]->Fill(c->fCent, nK_gen[iM], nXi_gen[iM]);
        #endif // CLOSURE_TEST
        for (int iC = 0; iC < 2; ++iC){ // loop over efficiency correction
          qK_1[ic_sm-1][iC][iM] += qK_1_tmp[iM][iC];
          qK_1Sq[ic_sm-1][iC][iM] += (qK_1_tmp[iM][iC]*qK_1_tmp[iM][iC]);
          qK_2[ic_sm-1][iC][iM] += qK_2_tmp[iM][iC];
          qXi_1[ic_sm-1][iC][iM] += qXi_1_tmp[iM][iC];
          qXi_1Sq[ic_sm-1][iC][iM] += (qXi_1_tmp[iM][iC]*qXi_1_tmp[iM][iC]);
          qXi_2[ic_sm-1][iC][iM] += qXi_2_tmp[iM][iC];
          qKXi_11Same[ic_sm-1][iC][iM] += (qK_1_tmp[iM][iC]*qXi_1_tmp[1-iM][iC]);
          qKXi_11Opp[ic_sm-1][iC][iM] += (qK_1_tmp[iM][iC]*qXi_1_tmp[iM][iC]);
        }
        hNKaonXi[0][iS]->Fill(c->fCent, nK[iM], nXi[1-iM]);
        hNKaonXi[1][iS]->Fill(c->fCent, nK[iM], nXi[iM]);
      }
      
      hCentSmallTmp.Fill(c->fCent);
    }

    o.mkdir(Form("subsample_%d", iS));
    o.cd(Form("subsample_%d", iS));
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
        hNKaonXi_Gen[iM][iS]->Write();
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
      }
      hNKaonXi[iM][iS]->Write();
    }
    hCent[iS]->Write();
  }

  // close stream
  o.Close();
  f.Close();
}

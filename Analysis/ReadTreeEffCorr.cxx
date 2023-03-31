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

void ReadTreeEffCorr(const char* fname = "tree_data_full/part_merging_True/%s_AnalysisResults", const char* ofname = "o", const int tree_number = 1)
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
  #endif // CLOSURE_TEST
  MiniCollision *c = new MiniCollision();
  c->fZ = 0.;
  c->fCent = 0.;
  c->fTrigger = 0x00;
  TTree *t = (TTree*)f.Get("StrangenessTree");
  TBranch *bk = nullptr;
  TBranch *bxi = nullptr;  float index = 0.;
  TBranch *bindex = nullptr;
  if (isMC){
    t->SetBranchAddress("MiniKaonMC", &k, &bk);
    t->SetBranchAddress("MiniXiMC", &xi, &bxi);
  }
  else{
    t->SetBranchAddress("MiniKaon", &k, &bk);
    t->SetBranchAddress("MiniXi", &xi, &bxi);
  }
  if (kUseIndex)
    t->SetBranchAddress("index", &index, &bindex);
  TBranch *be = t->GetBranch("MiniCollision");
  be->SetAddress(&c);
  
  TH1D *hCent[N_SAMPLE];
  #ifdef CLOSURE_TEST
    TH1D *hKaonQ1_Gen[2][N_SAMPLE];
    TH1D *hKaonQ11_Gen[2][N_SAMPLE];
    TH1D *hKaonQ1Sq_Gen[2][N_SAMPLE];
    TH1D *hKaonQ2_Gen[2][N_SAMPLE];
    TH1D *hXiQ1_Gen[2][N_SAMPLE];
    TH1D *hXiQ11_Gen[2][N_SAMPLE];
    TH1D *hXiQ1Sq_Gen[2][N_SAMPLE];
    TH1D *hXiQ2_Gen[2][N_SAMPLE];
    TH1D *hSameKaonXiQ11_Gen[2][N_SAMPLE];
    TH1D *hOppKaonXiQ11_Gen[2][N_SAMPLE];
  #endif // CLOSURE_TEST 
  TH1D *hKaonQ1[2][2][N_SAMPLE];
  TH1D *hKaonQ11[2][2][N_SAMPLE];
  TH1D *hKaonQ1Sq[2][2][N_SAMPLE];
  TH1D *hKaonQ2[2][2][N_SAMPLE];
  TH1D *hXiQ1[2][2][N_SAMPLE];
  TH1D *hXiQ11[2][2][N_SAMPLE];
  TH1D *hXiQ1Sq[2][2][N_SAMPLE];
  TH1D *hXiQ2[2][2][N_SAMPLE];
  TH1D *hSameKaonXiQ11[2][2][N_SAMPLE];
  TH1D *hOppKaonXiQ11[2][2][N_SAMPLE];
  
  TH3F *hNsigmaTPC[2];
  TH3F *hNsigmaTOF[2];
  TH3F *hMass[2];

  TH1D *hEffK[2][kNCentBins][kNEtaBins][N_SAMPLE];
  TH1D *hEffXi[2][kNCentBins][kNEtaBins][N_SAMPLE]; // TO BE IMPROVED -> CENTRALITY DIFFERENTIAL ESTIMATE OF XI EFFICIENCY (ALSO BDT)
  TH3F *hBDTEffXi; // TO BE IMPROVED -> CENTRALITY DIFFERENTIAL ESTIMATE OF XI EFFICIENCY (ALSO BDT -> SEPARATELY FOR CHARGES)

  for (int iS = 0; iS < N_SAMPLE; ++iS){
    hCent[iS] = new TH1D(Form("hCent"), ";Centrality (%);Entries", kNCentBinsSmall, kCentBinsSmall);
  }
  for (int iC = 0; iC < 2; ++iC){
    for (int iS = 0; iS < N_SAMPLE; ++iS){
      #ifdef CLOSURE_TEST
        hKaonQ1_Gen[iC][iS] = new TH1D(Form("h%sKaonQ1_Gen", kAntiMatterLabel[iC]), ";Centrality (%);q_{1}^{K}", kNCentBinsSmall, kCentBinsSmall);
        hKaonQ11_Gen[iC][iS] = new TH1D(Form("h%sKaonQ11_Gen", kAntiMatterLabel[iC]), ";Centrality (%);q_{1}^{K}", kNCentBinsSmall, kCentBinsSmall);
        hKaonQ1Sq_Gen[iC][iS] = new TH1D(Form("h%sKaonQ1Sq_Gen", kAntiMatterLabel[iC]), ";Centrality (%);(q_{1}^{K})^{2}", kNCentBinsSmall, kCentBinsSmall);
        hKaonQ2_Gen[iC][iS] = new TH1D(Form("h%sKaonQ2_Gen", kAntiMatterLabel[iC]), ";Centrality (%);q_{2}^{K}", kNCentBinsSmall, kCentBinsSmall);
        hXiQ1_Gen[iC][iS] = new TH1D(Form("h%sXiQ1_Gen", kAntiMatterLabel[iC]), ";Centrality (%);q_{1}^{#Xi}", kNCentBinsSmall, kCentBinsSmall);
        hXiQ11_Gen[iC][iS] = new TH1D(Form("h%sXiQ11_Gen", kAntiMatterLabel[iC]), ";Centrality (%);q_{1}^{#Xi}", kNCentBinsSmall, kCentBinsSmall);
        hXiQ1Sq_Gen[iC][iS] = new TH1D(Form("h%sXiQ1Sq_Gen", kAntiMatterLabel[iC]), ";Centrality (%);(q_{1}^{#Xi})^{2}", kNCentBinsSmall, kCentBinsSmall);
        hXiQ2_Gen[iC][iS] = new TH1D(Form("h%sXiQ2_Gen", kAntiMatterLabel[iC]), ";Centrality (%);q_{2}^{#Xi}", kNCentBinsSmall, kCentBinsSmall);
        hSameKaonXiQ11_Gen[iC][iS] = new TH1D(Form("h%sSameKaonXiQ11_Gen", kAntiMatterLabel[iC]), ";Centrality (%);q_{11}^{K^{-}#Xi^{-}}", kNCentBinsSmall, kCentBinsSmall);
        hOppKaonXiQ11_Gen[iC][iS] = new TH1D(Form("h%sOppKaonXiQ11_Gen", kAntiMatterLabel[iC]), ";Centrality (%);q_{11}^{K^{+}#Xi^{-}}", kNCentBinsSmall, kCentBinsSmall);
      #endif // CLOSURE_TEST
      for (int iCorr = 0; iCorr < 2; ++iCorr){
        hKaonQ1[iC][iCorr][iS] = new TH1D(Form("h%sKaonQ1_%s", kAntiMatterLabel[iC], kCorrLabel[iCorr]), ";Centrality (%);q_{1}^{K}", kNCentBinsSmall, kCentBinsSmall);
        hKaonQ11[iC][iCorr][iS] = new TH1D(Form("h%sKaonQ11_%s", kAntiMatterLabel[iC], kCorrLabel[iCorr]), ";Centrality (%);q_{1}^{K}", kNCentBinsSmall, kCentBinsSmall);
        hKaonQ1Sq[iC][iCorr][iS] = new TH1D(Form("h%sKaonQ1Sq_%s", kAntiMatterLabel[iC], kCorrLabel[iCorr]), ";Centrality (%);(q_{1}^{K})^{2}", kNCentBinsSmall, kCentBinsSmall);
        hKaonQ2[iC][iCorr][iS] = new TH1D(Form("h%sKaonQ2_%s", kAntiMatterLabel[iC], kCorrLabel[iCorr]), ";Centrality (%);q_{2}^{K}", kNCentBinsSmall, kCentBinsSmall);
        hXiQ1[iC][iCorr][iS] = new TH1D(Form("h%sXiQ1_%s", kAntiMatterLabel[iC], kCorrLabel[iCorr]), ";Centrality (%);q_{1}^{#Xi}", kNCentBinsSmall, kCentBinsSmall);
        hXiQ11[iC][iCorr][iS] = new TH1D(Form("h%sXiQ11_%s", kAntiMatterLabel[iC], kCorrLabel[iCorr]), ";Centrality (%);q_{1}^{#Xi}", kNCentBinsSmall, kCentBinsSmall);
        hXiQ1Sq[iC][iCorr][iS] = new TH1D(Form("h%sXiQ1Sq_%s", kAntiMatterLabel[iC], kCorrLabel[iCorr]), ";Centrality (%);(q_{1}^{#Xi})^{2}", kNCentBinsSmall, kCentBinsSmall);
        hXiQ2[iC][iCorr][iS] = new TH1D(Form("h%sXiQ2_%s", kAntiMatterLabel[iC], kCorrLabel[iCorr]), ";Centrality (%);q_{2}^{#Xi}", kNCentBinsSmall, kCentBinsSmall);
        hSameKaonXiQ11[iC][iCorr][iS] = new TH1D(Form("h%sSameKaonXiQ11_%s", kAntiMatterLabel[iC], kCorrLabel[iCorr]), ";Centrality (%);q_{11}^{K^{-}#Xi^{-}}", kNCentBinsSmall, kCentBinsSmall);
        hOppKaonXiQ11[iC][iCorr][iS] = new TH1D(Form("h%sOppKaonXiQ11_%s", kAntiMatterLabel[iC], kCorrLabel[iCorr]), ";Centrality (%);q_{11}^{K^{+}#Xi^{-}}", kNCentBinsSmall, kCentBinsSmall);
      }
    }
    for (int iCent = 0; iCent < kNCentBins; ++iCent){
      for (int iEta = 0; iEta < kNEtaBins; ++iEta){
        for (int iS = 0; iS < N_SAMPLE; ++iS){
          if (fEffK){
            hEffK[iC][iCent][iEta][iS] = (TH1D*)fEffK->Get(Form("subsample_%d/h%sEff%s_%d_%d", 1, kAntiMatterLabel[iC], kPartLabel[0], iCent, iEta));
          }
          // TO BE IMPROVED -> CENTRALITY DIFFERENTIAL ESTIMATE OF XI EFFICIENCY (ALSO BDT)
          if (fEffXi){
            hEffXi[iC][iCent][iEta][iS] = kUseBdtInMC || kUseKaonXiEff ? (TH1D*)fEffK->Get(Form("subsample_%d/h%sEff%s_%d_%d", 1, kAntiMatterLabel[iC], kPartLabel[1], iCent, iEta)) : (TH1D*)fEffXi->Get(Form("fPreselEff_vs_pt_%s_%.0f_%.0f", kAntiMatterLabelML[iC], 0., 90.)); //kCentBins[iCent], kCentBins[iCent + 1]));
          }
        }
      }
    }
  }
  if (fEffBDTXi && !kUseBdtInMC)
    hBDTEffXi = (TH3F*)fEffBDTXi->Get("hBDTEff");

  #ifdef CLOSURE_TEST
    TH3F *hNKaonXi_Gen[2][2][N_SAMPLE];
    TH3F *hGenRecKaon[2];
    TH3F *hGenRecXi[2];
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
      hGenRecKaon[iC] = new TH3F(Form("h%sGenRecKaon", kAntiMatterLabel[iC]), ";Centrality (%);#it{N}_{gen};#it{N_rec}", kNCentBins, 0, 100, 200, 0, 200, 200, 0, 200);
      hGenRecXi[iC] = new TH3F(Form("h%sGenRecXi", kAntiMatterLabel[iC]), ";Centrality (%);#it{N}_{gen};#it{N_rec}", kNCentBins, 0, 100, 50, 0, 50, 50, 0, 50);
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
    hNsigmaTPC[iC] = new TH3F(Form("h%sNsigmaTPC", kAntiMatterLabel[iC]), ";Centrality (%);#it{p}_{T} (GeV/#it{c});#it{n}#sigma_{TPC} (a.u.)", 10, 0, 100, 30, 0, 3, 120, -6, 6);
    hNsigmaTOF[iC] = new TH3F(Form("h%sNsigmaTOF", kAntiMatterLabel[iC]), ";Centrality (%);#it{p}_{T} (GeV/#it{c});#it{n}#sigma_{TOF} (a.u.)", 10, 0, 100, 30, 0, 3, 120, -6, 6);
    
    hMass[iC] = new TH3F(Form("h%sMass", kAntiMatterLabel[iC]), ";Centrality (%);#it{p}_{T} (GeV/#it{c});#it{M}(#pi + #Lambda) (GeV/#it{c}^{2})", 10, 0, 10, 8, 0, 4, 200, 1.29, 1.34);
  }

  Long64_t nEntries = kLimitSample ? kLimitedSample : t->GetEntries();
  // Long64_t nEntriesSample = nEntries/N_SAMPLE;
  // Long64_t nEntriesSampleRest = nEntries - nEntriesSample*N_SAMPLE;
  // std::cout << "nEntriesSample = " << nEntriesSample << "; nEntriesSampleRest = " << nEntriesSampleRest << std::endl;
  TH1D hCentTmp("hCentTmp", "hCentTmp", kNCentBins, kCentBins);
  TH1D hCentSmallTmp("hCentSmallTmp", "hCentSmallTmp", kNCentBinsSmall, kCentBinsSmall);
  TH1D hEtaTmp("hEtaTmp", "hEtaTmp", kNBinsPt, kMinEta, kMinEta + kDeltaEta * kNBinsPt);

  #ifdef CLOSURE_TEST
    double qK_1_Gen[kNCentBinsSmall][2][N_SAMPLE];
    double qK_11_Gen[kNCentBinsSmall][2][N_SAMPLE];
    double qK_1Sq_Gen[kNCentBinsSmall][2][N_SAMPLE];
    double qXi_1_Gen[kNCentBinsSmall][2][N_SAMPLE];
    double qXi_11_Gen[kNCentBinsSmall][2][N_SAMPLE];
    double qXi_1Sq_Gen[kNCentBinsSmall][2][N_SAMPLE];
    double qK_2_Gen[kNCentBinsSmall][2][N_SAMPLE];
    double qXi_2_Gen[kNCentBinsSmall][2][N_SAMPLE];
    double qKXi_11Same_Gen[kNCentBinsSmall][2][N_SAMPLE];
    double qKXi_11Opp_Gen[kNCentBinsSmall][2][N_SAMPLE];
  #endif // CLOSURE_TEST
  double qK_1[kNCentBinsSmall][2][2][N_SAMPLE];
  double qK_11[kNCentBinsSmall][2][2][N_SAMPLE];
  double qK_1Sq[kNCentBinsSmall][2][2][N_SAMPLE];
  double qXi_1[kNCentBinsSmall][2][2][N_SAMPLE];
  double qXi_11[kNCentBinsSmall][2][2][N_SAMPLE];
  double qXi_1Sq[kNCentBinsSmall][2][2][N_SAMPLE];
  double qK_2[kNCentBinsSmall][2][2][N_SAMPLE];
  double qXi_2[kNCentBinsSmall][2][2][N_SAMPLE];
  double qKXi_11Same[kNCentBinsSmall][2][2][N_SAMPLE];
  double qKXi_11Opp[kNCentBinsSmall][2][2][N_SAMPLE];

  for (int i = 0; i < 2; ++i){
    for (int j = 0; j < kNCentBinsSmall; ++j){
      for (int is = 0; is < N_SAMPLE; ++is){
        #ifdef CLOSURE_TEST
          qK_1_Gen[j][i][is] = 0;  
          qK_11_Gen[j][i][is] = 0;        
          qK_1Sq_Gen[j][i][is] = 0;       
          qXi_1_Gen[j][i][is] = 0;        
          qXi_11_Gen[j][i][is] = 0;      
          qXi_1Sq_Gen[j][i][is] = 0;      
          qK_2_Gen[j][i][is] = 0;    
          qXi_2_Gen[j][i][is] = 0;       
          qKXi_11Same_Gen[j][i][is] = 0;      
          qKXi_11Opp_Gen[j][i][is] = 0;
        #endif // CLOSURE_TEST
        for (int k = 0; k < 2; ++k){ // efficiency corrected and w/o correction
          qK_1[j][i][k][is] = 0;
          qK_11[j][i][k][is] = 0;   
          qK_1Sq[j][i][k][is] = 0;       
          qXi_1[j][i][k][is] = 0;          
          qXi_11[j][i][k][is] = 0;    
          qXi_1Sq[j][i][k][is] = 0;      
          qK_2[j][i][k][is] = 0;    
          qXi_2[j][i][k][is] = 0;       
          qKXi_11Same[j][i][k][is] = 0;      
          qKXi_11Opp[j][i][k][is] = 0;
        }
      }
    }
  }

  for (Long64_t i = 0; i < nEntries; ++i){
    const int iS = (int)(gRandom->Rndm() * N_SAMPLE);

    Long64_t e = i;
    if (!(i%10000000)) std::cout << "n_ev = " << i << std::endl;
    
    Long64_t tentry = t->LoadTree(e);
    be->GetEntry(tentry);
    bk->GetEntry(tentry);
    bxi->GetEntry(tentry);

    double cent = c->fCent;
    if (cent > 90) continue;
    int ic = hCentTmp.FindBin(cent);
    int ic_sm = hCentSmallTmp.FindBin(cent);

    if ( !isMC && k2018 && ( ( cent > 9.9 && cent < 30 && ( ( c->fTrigger & 1 ) != 1 ) ) || ( cent > 49.9 && ( ( c->fTrigger & 1 ) != 1 ) ) ) ){
      continue;
    }

    hCent[iS]->Fill(cent);
    //hCentSmallTmp.Fill(cent);

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

    UInt_t l_k = 0;
    for (auto k_tmp : *k){
      l_k++;
    }

    for (auto k_tmp : *k){

      #ifdef CLOSURE_TEST
      if ( std::abs(k_tmp.fPtMC) > kTOFptCut || std::abs(k_tmp.fPtMC) < kPtLowLimitK || k_tmp.fFlag != 1 || std::abs(k_tmp.fEtaMC) > kEtaCut ) continue;
        int im_MC = k_tmp.fPtMC > 0 ? 1 : 0;
        qK_1_gen_tmp[im_MC] += 1.;
        qK_2_gen_tmp[im_MC] += 1.;
        nK_gen[im_MC] += 1;
      #endif // CLOSURE_TEST
      if (
          (k_tmp.fCutBitMap == 1 || k_tmp.fCutBitMap == 2) &&
          std::abs(k_tmp.fPt) > kPtLowLimitK && std::abs(k_tmp.fPt) < kTOFptCut &&
          (std::abs(k_tmp.fEta) < kEtaCut) &&
          ((std::abs(k_tmp.fPt) < kTPCptCut && (k_tmp.fNsigmaTPC > kNsigmaTPCcutAsym[0] && k_tmp.fNsigmaTPC < kNsigmaTPCcutAsym[1]) ) ||
          (std::abs(k_tmp.fPt) > kTPCptCut && (std::abs(k_tmp.fPt) < kTOFptCut) && (std::abs(k_tmp.fNsigmaTPC) < kNsigmaTPCcutPresel) && (k_tmp.fNsigmaTOF > kNsigmaTOFcutAsym[0] && k_tmp.fNsigmaTOF < kNsigmaTOFcutAsym[1])))
        )
      {
        #ifdef CLOSURE_TEST
          int im_ = k_tmp.fPt > 0 ? 1 : 0;
          int ie_ = hEtaTmp.FindBin(k_tmp.fEtaMC);
          double eff_ = fEffK ? hEffK[im_][ic - 1][ie_ - 1][iS]->GetBinContent(hEffK[im_][ic - 1][ie_ - 1][iS]->FindBin(std::abs(k_tmp.fPtMC))) : kDummyEffK;
          if (!k_tmp.fIsReconstructed || k_tmp.fFlag != 1) continue;
          int im_tmp = k_tmp.fPt > 0 ? 1 : 0;
          hRecKaon[im_tmp]->Fill(cent, std::abs(k_tmp.fPt), 1./eff_);
        #endif // CLOSURE_TEST
        int im = k_tmp.fPt > 0 ? 1 : 0;
        int ie = hEtaTmp.FindBin(k_tmp.fEta);
        //std::cout << "im = " << im << ", ic = " << ic - 1 << std::endl;
        double eff = fEffK ? hEffK[im][ic - 1][ie - 1][iS]->GetBinContent(hEffK[im][ic - 1][ie - 1][iS]->FindBin(std::abs(k_tmp.fPt))) : kDummyEffK;
        //std::cout << "pt = " << std::abs(k_tmp.fPt) << "; bin = " << hEffK[im][ic - 1][ie - 1][iS]->GetXaxis()->FindBin(std::abs(k_tmp.fPt)) << "eff = " << eff << std::endl;
        qK_1_tmp[im][0] += 1.;
        qK_1_tmp_update[im][0] += 1.;
        qK_2_tmp[im][0] += 1.;
        qK_1_tmp[im][1] += (1./eff);
        qK_1_tmp_update[im][1] += (1./eff);
        double q2 = 1./eff/eff;
        qK_2_tmp[im][1] += q2;
        nK[im] += 1;
        hNsigmaTPC[im]->Fill(cent, std::abs(k_tmp.fPt), k_tmp.fNsigmaTPC);
        hNsigmaTOF[im]->Fill(cent, std::abs(k_tmp.fPt), k_tmp.fNsigmaTOF);
      }
    }
    for (int iM = 0; iM < 2; ++iM){
      #ifdef CLOSURE_TEST
        hGenRecKaon[iM]->Fill(cent, nK_gen[iM], nK[iM]);
      #endif
      for (int iCorr = 0; iCorr < 2; ++iCorr){
        double q1_sq = qK_1_tmp[iM][iCorr] * qK_1_tmp[iM][iCorr];
        qK_1_sq_tmp[iM][iCorr] += q1_sq;
        //std::cout << "qK_1_sq_tmp[" << iM << "][" << iCorr << "] = " << qK_1_tmp_update[iM][iCorr] * qK_1_tmp_update[iM][iCorr] << std::endl;
        //std::cout << "qK_1_tmp[" << iM << "][" << iCorr << "] = " << qK_1_tmp[iM][iCorr] << std::endl;
        qK_1_tmp_update[iM][iCorr] = 0.;
      }
    }

    for (auto xi_tmp : *xi){
      try {
        double pt = xi_tmp.fPt;
      }
      catch (const std::out_of_range& e) {
        continue;
      }
      //std::cout << "iXi = " << iXi << "; size = " << (UInt_t)(xi->size()) << std::endl;
      #ifdef CLOSURE_TEST
        if ( std::abs(xi_tmp.fPtMC) > kXiUpPtCut || std::abs(xi_tmp.fPtMC) < kXiLowPtCut || std::abs(xi_tmp.fEtaMC) > kEtaCut || (xi_tmp.fRecFlag & BIT(0)) != 1 || (xi_tmp.fRecFlag & BIT(1)) != 2 ) continue;
        int im_MC = xi_tmp.fPtMC > 0;
        qXi_1_gen_tmp[im_MC] += 1.;
        qXi_2_gen_tmp[im_MC] += 1.;
        nXi_gen[im_MC] += 1;
        hGenXi[im_MC]->Fill(cent, std::abs(xi_tmp.fPtMC));
      #endif // CLOSURE_TEST
      if (
          std::abs(xi_tmp.fEta) < kEtaCut &&
          std::abs(xi_tmp.fPt) > kXiLowPtCut && std::abs(xi_tmp.fPt) < kXiUpPtCut &&
          std::abs(xi_tmp.fMass - kXiMass) < kXiMassCut
        )
      {
        if (kUseBdtInMC){
          if (xi_tmp.fBdtOut < kBdtScoreCut) continue;
        }
        #ifdef CLOSURE_TEST
          if (!xi_tmp.fIsReconstructed || xi_tmp.fFlag != 1) continue;
        #endif // CLOSURE_TEST
        double bdtEff = 1.;
        if (!isMC && !kUseBdtInMC){
          bdtEff = kBdtEffCut;
          double bdtScoreCut = fEffBDTXi ? hBDTEffXi->GetBinContent(hBDTEffXi->FindBin(cent, std::abs(xi_tmp.fPt), bdtEff)) : kDummyBdtScoreXi;
          if (xi_tmp.fBdtOut < bdtScoreCut) continue;
          //std::cout << "bdtScoreCut = " << bdtScoreCut << std::endl;
        }
        int im = xi_tmp.fPt > 0 ? 1 : 0;
        int ie = hEtaTmp.FindBin(xi_tmp.fEta);
        //std::cout << "xi: " << cent << "\t" << xi_tmp.fPt << "\t" << bdtEff << std::endl;
        #ifdef CLOSURE_TEST
          hRecXi[im]->Fill(cent, std::abs(xi_tmp.fPt));
        #endif // CLOSURE_TEST
        //std::cout << hEffXi[im][ic - 1][ie - 1][iS]->GetName() << std::endl;
        double eff = fEffXi ? hEffXi[im][ic - 1][ie - 1][iS]->GetBinContent(hEffXi[im][ic - 1][ie - 1][iS]->FindBin(std::abs(xi_tmp.fPt))) : kDummyEffXi;
        //std::cout << "pt = " << std::abs(xi_tmp.fPt) << "; bin = " << hEffXi[im][ic - 1][ie - 1][iS]->FindBin(std::abs(xi_tmp.fPt)) << "eff = " << eff << std::endl;
        qXi_1_tmp[im][0] += 1.;
        qXi_1_tmp_update[im][0] += 1.;
        qXi_2_tmp[im][0] += 1.;
        qXi_1_tmp[im][1] += (1./eff/bdtEff);
        qXi_1_tmp_update[im][1] += (1./eff/bdtEff);
        qXi_2_tmp[im][1] += (1./eff/eff/bdtEff/bdtEff);
        nXi[im] += 1;
        hMass[im]->Fill(cent, std::abs(xi_tmp.fPt), xi_tmp.fMass);
      }
    }
    for (int iM = 0; iM < 2; ++iM){
      #ifdef CLOSURE_TEST
        hGenRecXi[iM]->Fill(cent, nXi_gen[iM], nXi[iM]);
      #endif
      for (int iCorr = 0; iCorr < 2; ++iCorr){
        qXi_1_sq_tmp[iM][iCorr] += (qXi_1_tmp_update[iM][iCorr] * qXi_1_tmp_update[iM][iCorr]);
        qXi_1_tmp_update[iM][iCorr] = 0.;
      }
    }

    for (int iM = 0; iM < 2; ++iM){
      #ifdef CLOSURE_TEST
        qK_1_Gen[ic_sm-1][iM][iS] += qK_1_gen_tmp[iM];
        qK_11_Gen[ic_sm-1][iM][iS] += (qK_1_gen_tmp[iM]*qK_1_gen_tmp[1-iM]);
        qK_1Sq_Gen[ic_sm-1][iM][iS] += (qK_1_gen_tmp[iM]*qK_1_gen_tmp[iM]);
        qK_2_Gen[ic_sm-1][iM][iS] += qK_2_gen_tmp[iM];
        qXi_1_Gen[ic_sm-1][iM][iS] += qXi_1_gen_tmp[iM];
        qXi_11_Gen[ic_sm-1][iM][iS] += (qXi_1_gen_tmp[iM]*qXi_1_gen_tmp[1-iM]);
        qXi_1Sq_Gen[ic_sm-1][iM][iS] += (qXi_1_gen_tmp[iM]*qXi_1_gen_tmp[iM]);
        //std::cout << "qXi_1_gen_tmp[iM][iC] = " << qXi_1_gen_tmp[iM] << "; square = " << qXi_1_gen_tmp[iM]*qXi_1_gen_tmp[iM] << std::endl;
        qXi_2_Gen[ic_sm-1][iM][iS] += qXi_2_gen_tmp[iM];
        qKXi_11Same_Gen[ic_sm-1][iM][iS] += (qK_1_gen_tmp[iM]*qXi_1_gen_tmp[1-iM]);
        qKXi_11Opp_Gen[ic_sm-1][iM][iS] += (qK_1_gen_tmp[1-iM]*qXi_1_gen_tmp[1-iM]);
        hNKaonXi_Gen[0][iM][iS]->Fill(cent, nK_gen[iM], nXi_gen[1-iM]);
        hNKaonXi_Gen[1][iM][iS]->Fill(cent, nK_gen[1-iM], nXi_gen[1-iM]);
      #endif // CLOSURE_TEST
      for (int iC = 0; iC < 2; ++iC){ // loop over efficiency correction
        qK_1[ic_sm-1][iC][iM][iS] += qK_1_tmp[iM][iC];
        qK_11[ic_sm-1][iC][iM][iS] += (qK_1_tmp[iM][iC]*qK_1_tmp[1-iM][iC]);
        qK_1Sq[ic_sm-1][iC][iM][iS] += qK_1_sq_tmp[iM][iC];
        qK_2[ic_sm-1][iC][iM][iS] += qK_2_tmp[iM][iC];
        qXi_1[ic_sm-1][iC][iM][iS] += qXi_1_tmp[iM][iC];
        qXi_11[ic_sm-1][iC][iM][iS] += (qXi_1_tmp[iM][iC]*qXi_1_tmp[1-iM][iC]);
        //std::cout <<  qXi_1[ic_sm-1][iC][iM] << std::endl;
        qXi_1Sq[ic_sm-1][iC][iM][iS] += qXi_1_sq_tmp[iM][iC];
        //std::cout << "qXi_1_tmp[iM][iC] = " << qXi_1_tmp[iM][iC] << "; square = " << qXi_1_tmp[iM][iC]*qXi_1_tmp[iM][iC] << std::endl;
        qXi_2[ic_sm-1][iC][iM][iS] += qXi_2_tmp[iM][iC];
        qKXi_11Same[ic_sm-1][iC][iM][iS] += (qK_1_tmp[iM][iC]*qXi_1_tmp[1-iM][iC]);
        qKXi_11Opp[ic_sm-1][iC][iM][iS] += (qK_1_tmp[1-iM][iC]*qXi_1_tmp[1-iM][iC]);
      }
      hNKaonXi[0][iM][iS]->Fill(cent, nK[iM], nXi[1-iM]);
      hNKaonXi[1][iM][iS]->Fill(cent, nK[1-iM], nXi[1-iM]);
    }
  }


  for (int iS = 0; iS < N_SAMPLE; ++iS){
    if (isMC || (!isMC && kUseIndex)){
      o.mkdir(Form("subsample_%d", iS + 1));
      o.cd(Form("subsample_%d", iS + 1));
    }
    else {
      o.mkdir(Form("subsample_%s", ofname));
      o.cd(Form("subsample_%s", ofname));
    }
    for (int iM = 0; iM < 2; ++iM){
      for (int iC = 0; iC < kNCentBinsSmall; ++iC){
        double ev = hCent[iS]->GetBinContent(iC+1);
        // std::cout << "n_ev = " << ev << std::endl;
        #ifdef CLOSURE_TEST
          hKaonQ1_Gen[iM][iS]->SetBinContent(iC + 1, ev > 0 ? qK_1_Gen[iC][iM][iS]/ev : 0);
          hKaonQ11_Gen[iM][iS]->SetBinContent(iC + 1, ev > 0 ? qK_11_Gen[iC][iM][iS]/ev : 0);
          hKaonQ1Sq_Gen[iM][iS]->SetBinContent(iC + 1, ev > 0 ? qK_1Sq_Gen[iC][iM][iS]/ev : 0);
          hKaonQ2_Gen[iM][iS]->SetBinContent(iC + 1, ev > 0 ? qK_2_Gen[iC][iM][iS]/ev : 0);
          hXiQ1_Gen[iM][iS]->SetBinContent(iC + 1, ev > 0 ? qXi_1_Gen[iC][iM][iS]/ev : 0);
          hXiQ11_Gen[iM][iS]->SetBinContent(iC + 1, ev > 0 ? qXi_11_Gen[iC][iM][iS]/ev : 0);
          hXiQ1Sq_Gen[iM][iS]->SetBinContent(iC + 1, ev > 0 ? qXi_1Sq_Gen[iC][iM][iS]/ev : 0);
          hXiQ2_Gen[iM][iS]->SetBinContent(iC + 1, ev > 0 ? qXi_2_Gen[iC][iM][iS]/ev : 0);
          hSameKaonXiQ11_Gen[iM][iS]->SetBinContent(iC + 1, ev > 0 ? qKXi_11Same_Gen[iC][iM][iS]/ev : 0);
          hOppKaonXiQ11_Gen[iM][iS]->SetBinContent(iC + 1, ev > 0 ? qKXi_11Opp_Gen[iC][iM][iS]/ev : 0);
        #endif // CLOSURE_TEST
        for (int iCorr = 0; iCorr < 2; ++iCorr){
          hKaonQ1[iM][iCorr][iS]->SetBinContent(iC + 1, ev > 0 ? qK_1[iC][iCorr][iM][iS]/ev : 0);
          hKaonQ11[iM][iCorr][iS]->SetBinContent(iC + 1, ev > 0 ? qK_11[iC][iCorr][iM][iS]/ev : 0);
          hKaonQ1Sq[iM][iCorr][iS]->SetBinContent(iC + 1, ev > 0 ? qK_1Sq[iC][iCorr][iM][iS]/ev : 0);
          hKaonQ2[iM][iCorr][iS]->SetBinContent(iC + 1, ev > 0 ? qK_2[iC][iCorr][iM][iS]/ev : 0);
          hXiQ1[iM][iCorr][iS]->SetBinContent(iC + 1, ev > 0 ? qXi_1[iC][iCorr][iM][iS]/ev : 0);
          hXiQ11[iM][iCorr][iS]->SetBinContent(iC + 1, ev > 0 ? qXi_11[iC][iCorr][iM][iS]/ev : 0);
          hXiQ1Sq[iM][iCorr][iS]->SetBinContent(iC + 1, ev > 0 ? qXi_1Sq[iC][iCorr][iM][iS]/ev : 0);
          hXiQ2[iM][iCorr][iS]->SetBinContent(iC + 1, ev > 0 ? qXi_2[iC][iCorr][iM][iS]/ev : 0);
          hSameKaonXiQ11[iM][iCorr][iS]->SetBinContent(iC + 1, ev > 0 ? qKXi_11Same[iC][iCorr][iM][iS]/ev : 0);
          hOppKaonXiQ11[iM][iCorr][iS]->SetBinContent(iC + 1, ev > 0 ? qKXi_11Opp[iC][iCorr][iM][iS]/ev : 0);
        }
      }
      #ifdef CLOSURE_TEST
        hKaonQ1_Gen[iM][iS]->Write();
        hKaonQ11_Gen[iM][iS]->Write();
        hKaonQ1Sq_Gen[iM][iS]->Write();
        hKaonQ2_Gen[iM][iS]->Write();
        hXiQ1_Gen[iM][iS]->Write();
        hXiQ11_Gen[iM][iS]->Write();
        hXiQ1Sq_Gen[iM][iS]->Write();
        hXiQ2_Gen[iM][iS]->Write();
        hSameKaonXiQ11_Gen[iM][iS]->Write();
        hOppKaonXiQ11_Gen[iM][iS]->Write();
        for (int iC = 0; iC < 2; ++iC)
          hNKaonXi_Gen[iM][iC][iS]->Write();
      #endif // CLOSURE_TEST
      for (int iC = 0; iC < 2; ++iC){
        hKaonQ1[iM][iC][iS]->Write();
        hKaonQ11[iM][iC][iS]->Write();
        hKaonQ1Sq[iM][iC][iS]->Write();
        hKaonQ2[iM][iC][iS]->Write();
        hXiQ1[iM][iC][iS]->Write();
        hXiQ11[iM][iC][iS]->Write();
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
      hGenRecKaon[iM]->Write();
      hGenRecXi[iM]->Write();
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

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH3F.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TMath.h>
#include <Riostream.h>
#include "../utils/Config_tests.h"
#include <TRandom3.h>
#include <TROOT.h>
#include <TNtuple.h>
#include <TStopwatch.h>

#define FILL_HIST
// #define CLOSURE_TEST

#ifdef __CINT__
#pragma link C++ class std::vector<MiniKaon>+;
#pragma link C++ class std::vector<MiniKaonMC>+;
#pragma link C++ class std::vector<MiniXi>+;
#pragma link C++ class std::vector<MiniXiMC>+;
#endif

void ReadTreeEffCorr(const char* fname = "tree_data_full/part_merging_True/%s_AnalysisResults", const char* ofname = "o15_", const double bdtCut = 0.3, const int iVarMin = 0, const int iVarMax = 90, const int tree_number = 1)
{

  TStopwatch w;
  w.Start();

  TFile *fEffK = TFile::Open(Form("%s/%s.root", kDataDir, kEffKFile));
  TFile *fEffXi = TFile::Open(Form("%s/%s.root", kDataDir, kEffXiFile));
  TFile *fEffBDTXi = TFile::Open(Form("%s/%s.root", kDataDir, kEffBDTXiFile));
  TFile f(Form("%s/%s.root", kDataDir, fname));
  //TFile o(Form("%s.root"/* , kResDir */, ofname), "recreate");
  TFile fBdtmap(Form("results/%s.root", kBdtmapName));

  // double bdtScoreCuts[kNBinsPtXi][kNBdtCuts];
  // for (int i{0}; i < kNBinsPtXi; ++i){
  //   auto h = (TH1D*)fBdtmap.Get(Form("hEffBDT_%d", i + 1));
  //   for (int j{0}; j < kNBdtCuts; ++j){
  //     for (int iB{1}; iB < h->GetNbinsX(); ++iB){
  //       double eff_shift = 0.; //(i == 2 ? 0. : (i == 3 ? 1. : 2.));
  //       if ( std::abs(h->GetBinContent(iB) - bdtCut - eff_shift * k_eff_shift) < 0.005 ){
  //         bdtScoreCuts[i][j] = h->GetXaxis()->GetBinLowEdge(iB);
  //         std::cout << bdtCut + eff_shift * k_eff_shift << "\t" << bdtScoreCuts[i][j] << std::endl;
  //         break;
  //       }
  //     }
  //   }
  // }
    
  double bdtScoreCuts[kNBinsPtXi][kNBdtCuts];
  for (int i{0}; i < kNBinsPtXi; ++i){
    auto h = (TH1D*)fBdtmap.Get(Form("hEffBDT_%d", i + 1));
    for (int j{0}; j < kNBdtCuts; ++j){
      for (int iB{1}; iB < h->GetNbinsX(); ++iB){
        double eff_shift = (i == 2 ? 0. : (i == 3 ? 1. : 2.));
        if ( std::abs(h->GetBinContent(iB) - kBdtEffCuts[j] - eff_shift * k_eff_shift) < 0.005 ){
          bdtScoreCuts[i][j] = h->GetXaxis()->GetBinLowEdge(iB);
          std::cout << kBdtEffCuts[j] + eff_shift * k_eff_shift << "\t" << bdtScoreCuts[i][j] << std::endl;
          break;
        }
      }
    }
  }

  const int nF = iVarMax - iVarMin;
  TFile *o[nF];
  for (int i{iVarMin}; i < iVarMax; ++i){
    o[i - iVarMin] = new TFile(Form("%s_var_%d.root"/* , kResDir */, ofname, i), "recreate");
  }

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
  //if (kUseIndex)
    //t->SetBranchAddress("index", &index, &bindex);
  TBranch *be = t->GetBranch("MiniCollision");
  be->SetAddress(&c);
  
  TH1D *hCent[N_SAMPLE];
  #ifdef CLOSURE_TEST
    TH1D *hKaonQ1_Gen[2][N_SAMPLE][nF];
    TH1D *hKaonQ11_Gen[2][N_SAMPLE][nF];
    TH1D *hKaonQ1Sq_Gen[2][N_SAMPLE][nF];
    TH1D *hKaonQ2_Gen[2][N_SAMPLE][nF];
    TH1D *hXiQ1_Gen[2][N_SAMPLE][nF];
    TH1D *hXiQ11_Gen[2][N_SAMPLE][nF];
    TH1D *hXiQ1Sq_Gen[2][N_SAMPLE][nF];
    TH1D *hXiQ2_Gen[2][N_SAMPLE][nF];
    TH1D *hSameKaonXiQ11_Gen[2][N_SAMPLE][nF];
    TH1D *hOppKaonXiQ11_Gen[2][N_SAMPLE][nF];
  #endif // CLOSURE_TEST 
  TH1D *hKaonQ1[2][2][N_SAMPLE][nF];
  TH1D *hKaonQ11[2][2][N_SAMPLE][nF];
  TH1D *hKaonQ1Sq[2][2][N_SAMPLE][nF];
  TH1D *hKaonQ2[2][2][N_SAMPLE][nF];
  TH1D *hXiQ1[2][2][N_SAMPLE][nF];
  TH1D *hXiQ11[2][2][N_SAMPLE][nF];
  TH1D *hXiQ1Sq[2][2][N_SAMPLE][nF];
  TH1D *hXiQ2[2][2][N_SAMPLE][nF];
  TH1D *hSameKaonXiQ11[2][2][N_SAMPLE][nF];
  TH1D *hOppKaonXiQ11[2][2][N_SAMPLE][nF];
  
  TH3F *hNsigmaITS[2][kNEtaBins][nF];
  TH3F *hNsigmaTPC[2][kNEtaBins][nF];
  TH3F *hNsigmaTOF[2][kNEtaBins][nF];
  TH3F *hBDTOut[2][nF];
  TH3F *hMass[2][nF];

  const int kXiCut = kNMassCuts * kNBdtCuts;
  TH1D *hEffK[2][kNCentBins][kNEtaBins][N_SAMPLE][nF];
  TH1D *hEffXi[2][kNCentBins][kNEtaBins][N_SAMPLE][kXiCut]; // TO BE IMPROVED -> CENTRALITY DIFFERENTIAL ESTIMATE OF XI EFFICIENCY (ALSO BDT)
  TH3F *hBDTEffXi; // TO BE IMPROVED -> CENTRALITY DIFFERENTIAL ESTIMATE OF XI EFFICIENCY (ALSO BDT -> SEPARATELY FOR CHARGES)

  TNtuple *evtTuple[nF];
  
  for (int i{iVarMin}; i < iVarMax; ++i)
  {
    if (i < kNMassCuts * kNBdtCuts){
      evtTuple[i - iVarMin] = new TNtuple(Form("evtTuple_%d", i), Form("evtTuple_%d", i), "cent:q1kP:q1kN:q2kP:q2kN:q1xiP:q1xiN:q2xiP:q2xiN");
    }
    else{
      evtTuple[i - iVarMin] = new TNtuple(Form("evtTuple_%d", i), Form("evtTuple_%d", i), "cent:q1kP:q1kN:q2kP:q2kN");
    }
    evtTuple[i - iVarMin]->SetDirectory(o[i - iVarMin]);
  }

  for (int iS = 0; iS < N_SAMPLE; ++iS){
    hCent[iS] = new TH1D(Form("hCent"), ";Centrality (%);Entries", kNCentBinsSmall, kCentBinsSmall);
  }
  for (int iC = 0; iC < 2; ++iC){
    for (int iS = 0; iS < N_SAMPLE; ++iS){
      for (int iVar = iVarMin; iVar < iVarMax; ++iVar){
        #ifdef CLOSURE_TEST
          hKaonQ1_Gen[iC][iS][iVar] = new TH1D(Form("h%sKaonQ1_Gen_%d", kAntiMatterLabel[iC], iVar), ";Centrality (%);q_{1}^{K}", kNCentBinsSmall, kCentBinsSmall);
          hKaonQ11_Gen[iC][iS][iVar] = new TH1D(Form("h%sKaonQ11_Gen_%d", kAntiMatterLabel[iC], iVar), ";Centrality (%);q_{1}^{K}", kNCentBinsSmall, kCentBinsSmall);
          hKaonQ1Sq_Gen[iC][iS][iVar] = new TH1D(Form("h%sKaonQ1Sq_Gen_%d", kAntiMatterLabel[iC], iVar), ";Centrality (%);(q_{1}^{K})^{2}", kNCentBinsSmall, kCentBinsSmall);
          hKaonQ2_Gen[iC][iS][iVar] = new TH1D(Form("h%sKaonQ2_Gen_%d", kAntiMatterLabel[iC], iVar), ";Centrality (%);q_{2}^{K}", kNCentBinsSmall, kCentBinsSmall);
          hXiQ1_Gen[iC][iS][iVar] = new TH1D(Form("h%sXiQ1_Gen_%d", kAntiMatterLabel[iC], iVar), ";Centrality (%);q_{1}^{#Xi}", kNCentBinsSmall, kCentBinsSmall);
          hXiQ11_Gen[iC][iS][iVar] = new TH1D(Form("h%sXiQ11_Gen_%d", kAntiMatterLabel[iC], iVar), ";Centrality (%);q_{1}^{#Xi}", kNCentBinsSmall, kCentBinsSmall);
          hXiQ1Sq_Gen[iC][iS][iVar] = new TH1D(Form("h%sXiQ1Sq_Gen_%d", kAntiMatterLabel[iC], iVar), ";Centrality (%);(q_{1}^{#Xi})^{2}", kNCentBinsSmall, kCentBinsSmall);
          hXiQ2_Gen[iC][iS][iVar] = new TH1D(Form("h%sXiQ2_Gen", kAntiMatterLabel[iC], iVar), ";Centrality (%);q_{2}^{#Xi}", kNCentBinsSmall, kCentBinsSmall);
          hSameKaonXiQ11_Gen[iC][iS][iVar] = new TH1D(Form("h%sSameKaonXiQ11_Gen_%d", kAntiMatterLabel[iC], iVar), ";Centrality (%);q_{11}^{K^{-}#Xi^{-}}", kNCentBinsSmall, kCentBinsSmall);
          hOppKaonXiQ11_Gen[iC][iS][iVar] = new TH1D(Form("h%sOppKaonXiQ11_Gen_%d", kAntiMatterLabel[iC], iVar), ";Centrality (%);q_{11}^{K^{+}#Xi^{-}}", kNCentBinsSmall, kCentBinsSmall);
        #endif // CLOSURE_TEST
        for (int iCorr = 0; iCorr < 2; ++iCorr){
          hKaonQ1[iC][iCorr][iS][iVar] = new TH1D(Form("h%sKaonQ1_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iVar), ";Centrality (%);q_{1}^{K}", kNCentBinsSmall, kCentBinsSmall);
          hKaonQ11[iC][iCorr][iS][iVar] = new TH1D(Form("h%sKaonQ11_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iVar), ";Centrality (%);q_{1}^{K}", kNCentBinsSmall, kCentBinsSmall);
          hKaonQ1Sq[iC][iCorr][iS][iVar] = new TH1D(Form("h%sKaonQ1Sq_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iVar), ";Centrality (%);(q_{1}^{K})^{2}", kNCentBinsSmall, kCentBinsSmall);
          hKaonQ2[iC][iCorr][iS][iVar] = new TH1D(Form("h%sKaonQ2_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iVar), ";Centrality (%);q_{2}^{K}", kNCentBinsSmall, kCentBinsSmall);
          hXiQ1[iC][iCorr][iS][iVar] = new TH1D(Form("h%sXiQ1_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iVar), ";Centrality (%);q_{1}^{#Xi}", kNCentBinsSmall, kCentBinsSmall);
          hXiQ11[iC][iCorr][iS][iVar] = new TH1D(Form("h%sXiQ11_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iVar), ";Centrality (%);q_{1}^{#Xi}", kNCentBinsSmall, kCentBinsSmall);
          hXiQ1Sq[iC][iCorr][iS][iVar] = new TH1D(Form("h%sXiQ1Sq_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iVar), ";Centrality (%);(q_{1}^{#Xi})^{2}", kNCentBinsSmall, kCentBinsSmall);
          hXiQ2[iC][iCorr][iS][iVar] = new TH1D(Form("h%sXiQ2_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iVar), ";Centrality (%);q_{2}^{#Xi}", kNCentBinsSmall, kCentBinsSmall);
          hSameKaonXiQ11[iC][iCorr][iS][iVar] = new TH1D(Form("h%sSameKaonXiQ11_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iVar), ";Centrality (%);q_{11}^{K^{-}#Xi^{-}}", kNCentBinsSmall, kCentBinsSmall);
          hOppKaonXiQ11[iC][iCorr][iS][iVar] = new TH1D(Form("h%sOppKaonXiQ11_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iVar), ";Centrality (%);q_{11}^{K^{+}#Xi^{-}}", kNCentBinsSmall, kCentBinsSmall);
        }
      }
    }
    for (int iCent = 0; iCent < kNCentBins; ++iCent){
      for (int iEta = 0; iEta < kNEtaBins; ++iEta){
        for (int iS = 0; iS < N_SAMPLE; ++iS){
          for (int iVar{iVarMin}; iVar < iVarMax; ++iVar){
            if (fEffK){
              hEffK[iC][iCent][iEta][iS][iVar - iVarMin] = (TH1D*)fEffK->Get(Form("subsample_%d_var_%d/h%sEff%s_%d_%d_%d", 1, 82, kAntiMatterLabel[iC], kPartLabel[0], iCent, 0, 82));
            }
          }
          // TO BE IMPROVED -> CENTRALITY DIFFERENTIAL ESTIMATE OF XI EFFICIENCY (ALSO BDT)
          for (int iVar{iVarMin}; iVar < kNMassCuts * kNBdtCuts; ++iVar){
            if (fEffXi){
              hEffXi[iC][iCent][iEta][iS][iVar - iVarMin] = kUseBdtInMC || kUseKaonXiEff ? (TH1D*)fEffK->Get(Form("subsample_%d_var_%d/h%sEff%s_%d_%d_%d", 1, 4, kAntiMatterLabel[iC], kPartLabel[1], iCent, 0, 4)) : (TH1D*)fEffXi->Get(Form("fPreselEff_vs_pt_%s_%.0f_%.0f", kAntiMatterLabelML[iC], 0., 90.)); //kCentBins[iCent], kCentBins[iCent + 1]));
            }
          }
        }
      }
    }
  }
  if (fEffBDTXi && !kUseBdtInMC)
    hBDTEffXi = (TH3F*)fEffBDTXi->Get("hBDTEff");

  #ifdef CLOSURE_TEST
    TH3F *hNKaonXi_Gen[2][2][N_SAMPLE][nF];
    TH3F *hGenRecKaon[2][nF];
    TH3F *hGenRecXi[2][nF];
    TH2D *hRecKaon[2][nF];
    TH2D *hRecXi[2][nF];
    TH2D *hGenXi[2][nF];
    double ptBins[kNBinsPt + 1];
    for (int iB = 0; iB < kNBinsPt + 1; ++iB){
      ptBins[iB] = kMinPt + kDeltaPt * iB;
    }
  #endif // CLOSURE_TEST
  TH3F *hNKaonXi[2][2][N_SAMPLE][nF];

  for (int iVar{iVarMin}; iVar < iVarMax; ++iVar){
    for (int iC = 0; iC < 2; ++iC){
      #ifdef CLOSURE_TEST
        hRecKaon[iC][iVar] = new TH2D(Form("h%sRecKaon_%d", kAntiMatterLabel[iC], iVar), ";Centrality (%);#it{p}_{T} (GeV/#it{c})", kNCentBins, kCentBins, kNBinsPt, ptBins);
        hGenRecKaon[iC][iVar] = new TH3F(Form("h%sGenRecKaon_%d", kAntiMatterLabel[iC], iVar), ";Centrality (%);#it{N}_{gen};#it{N_rec}", kNCentBins, 0, 100, 200, 0, 200, 200, 0, 200);
        hGenRecXi[iC][iVar] = new TH3F(Form("h%sGenRecXi_%d", kAntiMatterLabel[iC], iVar), ";Centrality (%);#it{N}_{gen};#it{N_rec}", kNCentBins, 0, 100, 50, 0, 50, 50, 0, 50);
        hGenXi[iC][iVar] = new TH2D(Form("h%sGenXi_%d", kAntiMatterLabel[iC], iVar), ";Centrality (%);#it{p}_{T} (GeV/#it{c})", kNCentBins, kCentBins, kNBinsPt, ptBins);
        hRecXi[iC][iVar] = new TH2D(Form("h%sRecXi_%d", kAntiMatterLabel[iC], iVar), ";Centrality (%);#it{p}_{T} (GeV/#it{c})", kNCentBins, kCentBins, kNBinsPt, ptBins);
      #endif
      for (int iS = 0; iS < N_SAMPLE; ++iS){
        for (int iM = 0; iM < 2; ++iM){
          #ifdef CLOSURE_TEST
            hNKaonXi_Gen[iC][iM][iS][iVar] = new TH3F(Form("h%sNKaonXi%s_Gen_%d", kChargeLabel[iC], kXiChargeLabel[iM], iVar), ";Centrality (%);K;#Xi", 100, 0, 100, 1000, 0, 1000, 20, 0, 20);
          #endif // CLOSURE_TEST
          hNKaonXi[iC][iM][iS][iVar] = new TH3F(Form("h%sNKaonXi%s_%d", kChargeLabel[iC], kXiChargeLabel[iM], iVar), ";Centrality (%);K;#Xi", 100, 0, 100, 1000, 0, 1000, 20, 0, 20);
        }
      }
      for (int iE = 0; iE < kNEtaBins; ++iE){
        hNsigmaITS[iC][iE][iVar] = new TH3F(Form("h%sNsigmaITS_%d_%d", kAntiMatterLabel[iC], iE, iVar), ";Centrality (%);#it{p}_{T} (GeV/#it{c});#it{n}#sigma_{ITS} (a.u.)", 10, 0, 100, 30, 0, 3, 120, -6, 6);
        hNsigmaTPC[iC][iE][iVar] = new TH3F(Form("h%sNsigmaTPC_%d_%d", kAntiMatterLabel[iC], iE, iVar), ";Centrality (%);#it{p}_{T} (GeV/#it{c});#it{n}#sigma_{TPC} (a.u.)", 10, 0, 100, 30, 0, 3, 120, -6, 6);
        hNsigmaTOF[iC][iE][iVar] = new TH3F(Form("h%sNsigmaTOF_%d_%d", kAntiMatterLabel[iC], iE, iVar), ";Centrality (%);#it{p}_{T} (GeV/#it{c});#it{n}#sigma_{TOF} (a.u.)", 10, 0, 100, 30, 0, 3, 120, -6, 6);
      }

      hBDTOut[iC][iVar] = new TH3F(Form("h%sBDTOutXi_%d", kAntiMatterLabel[iC], iVar), ";Centrality (%);#it{p}_{T} (GeV/#it{c});BDT out", 10, 0, 100, 8, 0, 4, 1000, 0, 1);
      hMass[iC][iVar] = new TH3F(Form("h%sMass_%d", kAntiMatterLabel[iC], iVar), ";Centrality (%);#it{p}_{T} (GeV/#it{c});#it{M}(#pi + #Lambda) (GeV/#it{c}^{2})", 10, 0, 10, 8, 0, 4, 200, 1.29, 1.34);
    }
  }

  Long64_t nEntries = kLimitSample ? kLimitedSample : t->GetEntries();
  // Long64_t nEntriesSample = nEntries/N_SAMPLE;
  // Long64_t nEntriesSampleRest = nEntries - nEntriesSample*N_SAMPLE;
  // std::cout << "nEntriesSample = " << nEntriesSample << "; nEntriesSampleRest = " << nEntriesSampleRest << std::endl;
  TH1D hCentTmp("hCentTmp", "hCentTmp", kNCentBins, kCentBins);
  TH1D hCentSmallTmp("hCentSmallTmp", "hCentSmallTmp", kNCentBinsSmall, kCentBinsSmall);
  TH1D hEtaTmp("hEtaTmp", "hEtaTmp", kNBinsPt, kMinEta, kMinEta + kDeltaEta * kNBinsPt);

  #ifdef CLOSURE_TEST
    double qK_1_Gen[kNCentBinsSmall][2][N_SAMPLE][nF];
    double qK_11_Gen[kNCentBinsSmall][2][N_SAMPLE][nF];
    double qK_1Sq_Gen[kNCentBinsSmall][2][N_SAMPLE][nF];
    double qXi_1_Gen[kNCentBinsSmall][2][N_SAMPLE][nF];
    double qXi_11_Gen[kNCentBinsSmall][2][N_SAMPLE][nF];
    double qXi_1Sq_Gen[kNCentBinsSmall][2][N_SAMPLE][nF];
    double qK_2_Gen[kNCentBinsSmall][2][N_SAMPLE][nF];
    double qXi_2_Gen[kNCentBinsSmall][2][N_SAMPLE][nF];
    double qKXi_11Same_Gen[kNCentBinsSmall][2][N_SAMPLE][nF];
    double qKXi_11Opp_Gen[kNCentBinsSmall][2][N_SAMPLE][nF];
  #endif // CLOSURE_TEST
  double qK_1[kNCentBinsSmall][2][2][N_SAMPLE][nF];
  double qK_11[kNCentBinsSmall][2][2][N_SAMPLE][nF];
  double qK_1Sq[kNCentBinsSmall][2][2][N_SAMPLE][nF];
  double qXi_1[kNCentBinsSmall][2][2][N_SAMPLE][nF];
  double qXi_11[kNCentBinsSmall][2][2][N_SAMPLE][nF];
  double qXi_1Sq[kNCentBinsSmall][2][2][N_SAMPLE][nF];
  double qK_2[kNCentBinsSmall][2][2][N_SAMPLE][nF];
  double qXi_2[kNCentBinsSmall][2][2][N_SAMPLE][nF];
  double qKXi_11Same[kNCentBinsSmall][2][2][N_SAMPLE][nF];
  double qKXi_11Opp[kNCentBinsSmall][2][2][N_SAMPLE][nF];

  for (int i = 0; i < 2; ++i){
    for (int j = 0; j < kNCentBinsSmall; ++j){
      for (int is = 0; is < N_SAMPLE; ++is){
        for (int i_f = 0; i_f < nF; ++i_f){
          #ifdef CLOSURE_TEST
            qK_1_Gen[j][i][is][i_f] = 0;  
            qK_11_Gen[j][i][is][i_f] = 0;        
            qK_1Sq_Gen[j][i][is][i_f] = 0;       
            qXi_1_Gen[j][i][is][i_f] = 0;        
            qXi_11_Gen[j][i][is][i_f] = 0;      
            qXi_1Sq_Gen[j][i][is][i_f] = 0;      
            qK_2_Gen[j][i][is][i_f] = 0;    
            qXi_2_Gen[j][i][is][i_f] = 0;       
            qKXi_11Same_Gen[j][i][is][i_f] = 0;      
            qKXi_11Opp_Gen[j][i][is][i_f] = 0;
          #endif // CLOSURE_TEST
          for (int k = 0; k < 2; ++k){ // efficiency corrected and w/o correction
            qK_1[j][i][k][is][i_f] = 0;
            qK_11[j][i][k][is][i_f] = 0;   
            qK_1Sq[j][i][k][is][i_f] = 0;       
            qXi_1[j][i][k][is][i_f] = 0;          
            qXi_11[j][i][k][is][i_f] = 0;    
            qXi_1Sq[j][i][k][is][i_f] = 0;      
            qK_2[j][i][k][is][i_f] = 0;    
            qXi_2[j][i][k][is][i_f] = 0;       
            qKXi_11Same[j][i][k][is][i_f] = 0;      
            qKXi_11Opp[j][i][k][is][i_f] = 0;
          }
        }
      }
    }
  }

  gRandom->SetSeed(42);
  for (Long64_t i = 0; i < nEntries; ++i){
    const int iS = (int)(gRandom->Rndm() * N_SAMPLE);

    Long64_t e = i;
    if (!(i%1000000)) std::cout << "n_ev = " << i << std::endl;
    
    Long64_t tentry = t->LoadTree(e);
    be->GetEntry(tentry);
    bk->GetEntry(tentry);
    bxi->GetEntry(tentry);

    float cent = c->fCent;
    if (cent > kMaxCent) continue;
    int ic = hCentTmp.FindBin(cent);
    // if (ic > 9) std::cout << "ic = " << ic << std::endl;
    int ic_sm = hCentSmallTmp.FindBin(cent);

    if (kRejectMultXiFromSameDaughters && (c->fTrigger & 16) == 16){
      continue;
    }

    if ( !isMC && k2018 && ( ( cent > 9.9 && cent < 30 && ( ( c->fTrigger & 1 ) != 1 ) ) || ( cent > 49.9 && ( ( c->fTrigger & 1 ) != 1 ) ) ) ){
      continue;
    }

    hCent[iS]->Fill(cent);
    //hCentSmallTmp.Fill(cent);

    for (int iVar{iVarMin}; iVar < iVarMax; ++iVar)
    {
      int iTpcClsCut = (iVar / 1) % kNTpcClsCuts;
      int iPidCut = (iVar / kNTpcClsCuts) % kNPidCuts;
      int iDcaCut = (iVar / kNTpcClsCuts / kNPidCuts) % kNDcaCuts;
      int iChi2Cut = (iVar / kNTpcClsCuts / kNPidCuts / kNDcaCuts) % kNChi2Cuts;
      int iMassCut = iTpcClsCut;
      int iBdtScoreCut = iPidCut;
      //if (!(i%10000)) std::cout << "iTpcClsCut = " << iTpcClsCut << ", iDcaCut = " << iDcaCut << ", iChi2Cut = " << iChi2Cut << ", iPidCut = " << iPidCut << std::endl;

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
            ( ( ( ((k_tmp.fCutBitMap & kCutDCA[iDcaCut]) == kCutDCA[iDcaCut]) || ((k_tmp.fCutBitMap & kCutDCA2[iDcaCut]) == kCutDCA2[iDcaCut]) ) && kRequireDCAcut[iDcaCut] ) || !kRequireDCAcut[iDcaCut] ) &&
            ( ( ((k_tmp.fCutBitMap & kCutTPCcls[iTpcClsCut]) == kCutTPCcls[iTpcClsCut] || (k_tmp.fCutBitMap & kCutTPCcls2[iTpcClsCut]) == kCutTPCcls2[iTpcClsCut]) && kRequireTPCclsCut[iTpcClsCut] ) || !kRequireTPCclsCut[iTpcClsCut]) &&
            ( ( ((k_tmp.fCutBitMap & kCutChi2[iChi2Cut]) == kCutChi2[iChi2Cut] || (k_tmp.fCutBitMap & kCutChi22[iChi2Cut]) == kCutChi22[iChi2Cut]) && kRequireChi2Cut[iChi2Cut] ) || !kRequireChi2Cut[iChi2Cut] ) &&
            std::abs(k_tmp.fPt) > kPtLowLimitK && std::abs(k_tmp.fPt) < kTOFptCut &&
            (std::abs(k_tmp.fEta) < kEtaCut)
            &&
            ((std::abs(k_tmp.fPt) < kTPCptCut && (k_tmp.fNsigmaTPC > kNsigmaTPCcutAsym[iPidCut][0] && k_tmp.fNsigmaTPC < kNsigmaTPCcutAsym[iPidCut][1])) ||
            (std::abs(k_tmp.fPt) > kTPCptCut && (std::abs(k_tmp.fPt) < kTOFptCut) && (std::abs(k_tmp.fNsigmaTPC) < kNsigmaTPCcutPresel) && (k_tmp.fNsigmaTOF > kNsigmaTOFcutAsym[iPidCut][0] && k_tmp.fNsigmaTOF < kNsigmaTOFcutAsym[iPidCut][1])))
          )
        {
          if (std::abs(k_tmp.fPt) < kITSptCut && (k_tmp.fNsigmaITS < kNsigmaITScutAsym[0] || k_tmp.fNsigmaITS > kNsigmaITScutAsym[1])) continue;
          if (kUseCircularPID)
          {
            double nsigmaITS = k_tmp.fNsigmaITS;
            double nsigmaTPC = k_tmp.fNsigmaTPC;
            double nsigmaTOF = k_tmp.fNsigmaTOF;
            if (TMath::Sqrt(nsigmaITS * nsigmaITS + nsigmaTPC * nsigmaTPC) > kCircularCuts[0] && std::abs(k_tmp.fPt) < kTPCptCut) continue;
            else if (TMath::Sqrt(nsigmaTPC * nsigmaTPC + nsigmaTOF * nsigmaTOF) > kCircularCuts[0] && std::abs(k_tmp.fPt) > kTPCptCut && std::abs(k_tmp.fPt) < kTOFptCut) continue;
          }
          #ifdef CLOSURE_TEST
            int im_ = k_tmp.fPt > 0 ? 1 : 0;
            int ie_ = hEtaTmp.FindBin(k_tmp.fEtaMC);
            double eff_ = fEffK ? hEffK[im_][ic - 1][ie_ - 1][iS][iVar - iVarMin]->GetBinContent(hEffK[im_][ic - 1][ie_ - 1][iS][iVar - iVarMin]->FindBin(std::abs(k_tmp.fPtMC))) : kDummyEffK;
            if (!k_tmp.fIsReconstructed || k_tmp.fFlag != 1) continue;
            int im_tmp = k_tmp.fPt > 0 ? 1 : 0;
            if (iDcaCut == 1 && iTpcClsCut == 1 && iChi2Cut == 1 && iPidCut == 2) hRecKaon[im_tmp]->Fill(cent, std::abs(k_tmp.fPt), 1./eff_);
          #endif // CLOSURE_TEST
          int im = k_tmp.fPt > 0 ? 1 : 0;
          int ie = hEtaTmp.FindBin(k_tmp.fEta);
          //std::cout << "im = " << im << ", ic = " << ic - 1 << std::endl;
          double eff = fEffK ? hEffK[im][ic - 1][ie - 1][iS][iVar - iVarMin]->GetBinContent(hEffK[im][ic - 1][ie - 1][iS][iVar - iVarMin]->FindBin(std::abs(k_tmp.fPt))) : kDummyEffK;
          //std::cout << "pt = " << std::abs(k_tmp.fPt) << "; bin = " << hEffK[im][ic - 1][ie - 1][iS]->GetXaxis()->FindBin(std::abs(k_tmp.fPt)) << "eff = " << eff << std::endl;
          qK_1_tmp[im][0] += 1.;
          qK_1_tmp_update[im][0] += 1.;
          qK_2_tmp[im][0] += 1.;
          qK_1_tmp[im][1] += (1./eff);
          qK_1_tmp_update[im][1] += (1./eff);
          double q2 = 1./eff/eff;
          qK_2_tmp[im][1] += q2;
          nK[im] += 1;
          #ifdef FILL_HIST
            if (/* iDcaCut == 1 && iTpcClsCut == 1 && iChi2Cut == 1 && iPidCut == 2 */true){
              hNsigmaITS[im][ie - 1][iVar]->Fill(cent, std::abs(k_tmp.fPt), k_tmp.fNsigmaITS);
              hNsigmaTPC[im][ie - 1][iVar]->Fill(cent, std::abs(k_tmp.fPt), k_tmp.fNsigmaTPC);
              hNsigmaTOF[im][ie - 1][iVar]->Fill(cent, std::abs(k_tmp.fPt), k_tmp.fNsigmaTOF);
            }
          #endif // FILLHIST
        }
      }
      for (int iM = 0; iM < 2; ++iM){
        #ifdef CLOSURE_TEST
          if (iDcaCut == 1 && iTpcClsCut == 1 && iChi2Cut == 1 && iPidCut == 2) hGenRecKaon[iM]->Fill(cent, nK_gen[iM], nK[iM]);
        #endif
        for (int iCorr = 0; iCorr < 2; ++iCorr){
          double q1_sq = qK_1_tmp[iM][iCorr] * qK_1_tmp[iM][iCorr];
          qK_1_sq_tmp[iM][iCorr] += q1_sq;
          //std::cout << "qK_1_sq_tmp[" << iM << "][" << iCorr << "] = " << qK_1_tmp_update[iM][iCorr] * qK_1_tmp_update[iM][iCorr] << std::endl;
          //std::cout << "qK_1_tmp[" << iM << "][" << iCorr << "] = " << qK_1_tmp[iM][iCorr] << std::endl;
          qK_1_tmp_update[iM][iCorr] = 0.;
        }
      }

      if (iVar < kNMassCuts * kNBdtCuts){
        for (auto xi_tmp : *xi){
          try {
            double pt = xi_tmp.fPt;
          }
          catch (const std::out_of_range& e) {
            continue;
          }
          //std::cout << "iXi = " << iXi << "; size = " << (UInt_t)(xi->size()) << std::endl;
          #ifdef CLOSURE_TEST
            if ( std::abs(xi_tmp.fPtMC) > kXiUpPtCut || std::abs(xi_tmp.fPtMC) < kXiLowPtCut || std::abs(xi_tmp.fEtaMC) > kEtaCut || xi_tmp.fFlag != 1) continue;
            int im_MC = xi_tmp.fPtMC > 0 ? 1 : 0;
            qXi_1_gen_tmp[im_MC] += 1.;
            qXi_2_gen_tmp[im_MC] += 1.;
            nXi_gen[im_MC] += 1;
            if (iDcaCut == 1 && iTpcClsCut == 1 && iChi2Cut == 1 && iPidCut == 2) hGenXi[im_MC]->Fill(cent, std::abs(xi_tmp.fPtMC));
          #endif // CLOSURE_TEST
          if (
              std::abs(xi_tmp.fEta) < kEtaCut &&
              std::abs(xi_tmp.fPt) > kXiLowPtCut && std::abs(xi_tmp.fPt) < kXiUpPtCut &&
              std::abs(xi_tmp.fMass - kXiMass) < kXiMassCuts[iMassCut]
            )
          {
            if (kUseBdtInMC){
              //if (xi_tmp.fBdtOut < kBdtScoreCuts[iBdtScoreCut]) continue;
              int ptBin = hMass[0][iVar]->GetYaxis()->FindBin(std::abs(xi_tmp.fPt));
              if (xi_tmp.fBdtOut < bdtScoreCuts[ptBin - 1][iBdtScoreCut]) continue;
              //if (xi_tmp.fBdtOut < 0.995) continue;
            }
            if ((xi_tmp.fRecFlag & BIT(0)) != 1 || (xi_tmp.fRecFlag & BIT(1)) != 2) continue;
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
              if (iDcaCut == 1 && iTpcClsCut == 1 && iChi2Cut == 1 && iPidCut == 2) hRecXi[im]->Fill(cent, std::abs(xi_tmp.fPt));
            #endif // CLOSURE_TEST
            //std::cout << hEffXi[im][ic - 1][ie - 1][iS]->GetName() << std::endl;
            double eff = fEffXi ? hEffXi[im][ic - 1][ie - 1][iS][iVar - iVarMin]->GetBinContent(hEffXi[im][ic - 1][ie - 1][iS][iVar - iVarMin]->FindBin(std::abs(xi_tmp.fPt))) : kDummyEffXi;
            //std::cout << "pt = " << std::abs(xi_tmp.fPt) << "; bin = " << hEffXi[im][ic - 1][ie - 1][iS]->FindBin(std::abs(xi_tmp.fPt)) << "eff = " << eff << std::endl;
            qXi_1_tmp[im][0] += 1.;
            qXi_1_tmp_update[im][0] += 1.;
            qXi_2_tmp[im][0] += 1.;
            qXi_1_tmp[im][1] += (1./eff/bdtEff);
            qXi_1_tmp_update[im][1] += (1./eff/bdtEff);
            qXi_2_tmp[im][1] += (1./eff/eff/bdtEff/bdtEff);
            nXi[im] += 1;
            /* if (iDcaCut == 1 && iTpcClsCut == 1 && iChi2Cut == 1 && iPidCut == 2)  */hMass[im][iVar]->Fill(cent, std::abs(xi_tmp.fPt), xi_tmp.fMass);
            /* if (iDcaCut == 1 && iTpcClsCut == 1 && iChi2Cut == 1 && iPidCut == 2)  */hBDTOut[im][iVar]->Fill(cent, std::abs(xi_tmp.fPt), xi_tmp.fBdtOut);
          }
        }
        for (int iM = 0; iM < 2; ++iM){
          #ifdef CLOSURE_TEST
            if (iDcaCut == 1 && iTpcClsCut == 1 && iChi2Cut == 1 && iPidCut == 2) hGenRecXi[iM][iVar]->Fill(cent, nXi_gen[iM], nXi[iM]);
          #endif
          for (int iCorr = 0; iCorr < 2; ++iCorr){
            qXi_1_sq_tmp[iM][iCorr] += (qXi_1_tmp_update[iM][iCorr] * qXi_1_tmp_update[iM][iCorr]);
            qXi_1_tmp_update[iM][iCorr] = 0.;
          }
        }
      }

      #ifdef FILL_HIST
/*         if (iDcaCut == 1 && iTpcClsCut == 1 && iChi2Cut == 1 && iPidCut == 2)
        { */
        int iF = iVar;
          for (int iM = 0; iM < 2; ++iM){
            #ifdef CLOSURE_TEST
              qK_1_Gen[ic_sm-1][iM][iS][iVar] += qK_1_gen_tmp[iM];
              qK_11_Gen[ic_sm-1][iM][iS][iVar] += (qK_1_gen_tmp[iM]*qK_1_gen_tmp[1-iM]);
              qK_1Sq_Gen[ic_sm-1][iM][iS][iVar] += (qK_1_gen_tmp[iM]*qK_1_gen_tmp[iM]);
              qK_2_Gen[ic_sm-1][iM][iS][iVar] += qK_2_gen_tmp[iM];
              qXi_1_Gen[ic_sm-1][iM][iS][iVar] += qXi_1_gen_tmp[iM];
              qXi_11_Gen[ic_sm-1][iM][iS][iVar] += (qXi_1_gen_tmp[iM]*qXi_1_gen_tmp[1-iM]);
              qXi_1Sq_Gen[ic_sm-1][iM][iS][iVar] += (qXi_1_gen_tmp[iM]*qXi_1_gen_tmp[iM]);
              //std::cout << "qXi_1_gen_tmp[iM][iC] = " << qXi_1_gen_tmp[iM] << "; square = " << qXi_1_gen_tmp[iM]*qXi_1_gen_tmp[iM] << std::endl;
              qXi_2_Gen[ic_sm-1][iM][iS][iVar] += qXi_2_gen_tmp[iM];
              qKXi_11Same_Gen[ic_sm-1][iM][iS][iVar] += (qK_1_gen_tmp[iM]*qXi_1_gen_tmp[1-iM]);
              qKXi_11Opp_Gen[ic_sm-1][iM][iS][iVar] += (qK_1_gen_tmp[1-iM]*qXi_1_gen_tmp[1-iM]);
              hNKaonXi_Gen[0][iM][iS][iVar]->Fill(cent, nK_gen[iM], nXi_gen[1-iM]);
              hNKaonXi_Gen[1][iM][iS][iVar]->Fill(cent, nK_gen[1-iM], nXi_gen[1-iM]);
            #endif // CLOSURE_TEST
            for (int iC = 0; iC < 2; ++iC){ // loop over efficiency correction
              qK_1[ic_sm-1][iC][iM][iS][iVar] += qK_1_tmp[iM][iC];
              qK_11[ic_sm-1][iC][iM][iS][iVar] += (qK_1_tmp[iM][iC]*qK_1_tmp[1-iM][iC]);
              qK_1Sq[ic_sm-1][iC][iM][iS][iVar] += qK_1_sq_tmp[iM][iC];
              qK_2[ic_sm-1][iC][iM][iS][iVar] += qK_2_tmp[iM][iC];
              qXi_1[ic_sm-1][iC][iM][iS][iVar] += qXi_1_tmp[iM][iC];
              qXi_11[ic_sm-1][iC][iM][iS][iVar] += (qXi_1_tmp[iM][iC]*qXi_1_tmp[1-iM][iC]);
              //if (!(i%1000000))std::cout <<  qXi_1[ic_sm-1][iC][iM][iS][iVar] << std::endl;
              qXi_1Sq[ic_sm-1][iC][iM][iS][iVar] += qXi_1_sq_tmp[iM][iC];
              //std::cout << "qXi_1_tmp[iM][iC] = " << qXi_1_tmp[iM][iC] << "; square = " << qXi_1_tmp[iM][iC]*qXi_1_tmp[iM][iC] << std::endl;
              qXi_2[ic_sm-1][iC][iM][iS][iVar] += qXi_2_tmp[iM][iC];
              qKXi_11Same[ic_sm-1][iC][iM][iS][iVar] += (qK_1_tmp[iM][iC]*qXi_1_tmp[1-iM][iC]);
              qKXi_11Opp[ic_sm-1][iC][iM][iS][iVar] += (qK_1_tmp[1-iM][iC]*qXi_1_tmp[1-iM][iC]);
            }
            hNKaonXi[0][iM][iS][iVar]->Fill(cent, nK[iM], nXi[1-iM]);
            hNKaonXi[1][iM][iS][iVar]->Fill(cent, nK[1-iM], nXi[1-iM]);
          }
        // }
      #endif // FILL_HIST

      if (iVar < kNMassCuts * kNBdtCuts)
        evtTuple[iVar - iVarMin]->Fill(cent, qK_1_tmp[1][1], qK_1_tmp[0][1], qK_2_tmp[1][1], qK_2_tmp[0][1], qXi_1_tmp[0][1], qXi_1_tmp[1][1], qXi_2_tmp[0][1], qXi_2_tmp[1][1]);
      else
        evtTuple[iVar - iVarMin]->Fill(cent, qK_1_tmp[1][1], qK_1_tmp[0][1], qK_2_tmp[1][1], qK_2_tmp[0][1]);
    }
  }


  for (int i{iVarMin}; i < iVarMax; ++i)
  {
    #ifdef FILL_HIST
      for (int iS = 0; iS < N_SAMPLE; ++iS){
        if (isMC || (!isMC && kUseIndex)){
          o[i - iVarMin]->mkdir(Form("subsample_%s%d", kSubsampleFlag, iS + 1));
          o[i - iVarMin]->cd(Form("subsample_%s%d", kSubsampleFlag, iS + 1));
        }
        else {
          o[i - iVarMin]->mkdir(Form("subsample_%s", ofname));
          o[i - iVarMin]->cd(Form("subsample_%s", ofname));
        }
        for (int iM = 0; iM < 2; ++iM){
          for (int iC = 0; iC < kNCentBinsSmall; ++iC){
            double ev = hCent[iS]->GetBinContent(iC+1);
            // std::cout << "n_ev = " << ev << std::endl;
            #ifdef CLOSURE_TEST
              hKaonQ1_Gen[iM][iS][i]->SetBinContent(iC + 1, ev > 0 ? qK_1_Gen[iC][iM][iS][i]/ev : 0);
              hKaonQ11_Gen[iM][iS][i]->SetBinContent(iC + 1, ev > 0 ? qK_11_Gen[iC][iM][iS][i]/ev : 0);
              hKaonQ1Sq_Gen[iM][iS][i]->SetBinContent(iC + 1, ev > 0 ? qK_1Sq_Gen[iC][iM][iS][i]/ev : 0);
              hKaonQ2_Gen[iM][iS][i]->SetBinContent(iC + 1, ev > 0 ? qK_2_Gen[iC][iM][iS][i]/ev : 0);
              hXiQ1_Gen[iM][iS][i]->SetBinContent(iC + 1, ev > 0 ? qXi_1_Gen[iC][iM][iS][i]/ev : 0);
              hXiQ11_Gen[iM][iS][i]->SetBinContent(iC + 1, ev > 0 ? qXi_11_Gen[iC][iM][iS][i]/ev : 0);
              hXiQ1Sq_Gen[iM][iS][i]->SetBinContent(iC + 1, ev > 0 ? qXi_1Sq_Gen[iC][iM][iS][i]/ev : 0);
              hXiQ2_Gen[iM][iS][i]->SetBinContent(iC + 1, ev > 0 ? qXi_2_Gen[iC][iM][iS][i]/ev : 0);
              hSameKaonXiQ11_Gen[iM][iS][i]->SetBinContent(iC + 1, ev > 0 ? qKXi_11Same_Gen[iC][iM][iS][i]/ev : 0);
              hOppKaonXiQ11_Gen[iM][iS][i]->SetBinContent(iC + 1, ev > 0 ? qKXi_11Opp_Gen[iC][iM][iS][i]/ev : 0);
            #endif // CLOSURE_TEST
            for (int iCorr = 0; iCorr < 2; ++iCorr){
              hKaonQ1[iM][iCorr][iS][i]->SetBinContent(iC + 1, ev > 0 ? qK_1[iC][iCorr][iM][iS][i]/ev : 0);
              hKaonQ11[iM][iCorr][iS][i]->SetBinContent(iC + 1, ev > 0 ? qK_11[iC][iCorr][iM][iS][i]/ev : 0);
              hKaonQ1Sq[iM][iCorr][iS][i]->SetBinContent(iC + 1, ev > 0 ? qK_1Sq[iC][iCorr][iM][iS][i]/ev : 0);
              hKaonQ2[iM][iCorr][iS][i]->SetBinContent(iC + 1, ev > 0 ? qK_2[iC][iCorr][iM][iS][i]/ev : 0);
              hXiQ1[iM][iCorr][iS][i]->SetBinContent(iC + 1, ev > 0 ? qXi_1[iC][iCorr][iM][iS][i]/ev : 0);
              hXiQ11[iM][iCorr][iS][i]->SetBinContent(iC + 1, ev > 0 ? qXi_11[iC][iCorr][iM][iS][i]/ev : 0);
              hXiQ1Sq[iM][iCorr][iS][i]->SetBinContent(iC + 1, ev > 0 ? qXi_1Sq[iC][iCorr][iM][iS][i]/ev : 0);
              hXiQ2[iM][iCorr][iS][i]->SetBinContent(iC + 1, ev > 0 ? qXi_2[iC][iCorr][iM][iS][i]/ev : 0);
              hSameKaonXiQ11[iM][iCorr][iS][i]->SetBinContent(iC + 1, ev > 0 ? qKXi_11Same[iC][iCorr][iM][iS][i]/ev : 0);
              hOppKaonXiQ11[iM][iCorr][iS][i]->SetBinContent(iC + 1, ev > 0 ? qKXi_11Opp[iC][iCorr][iM][iS][i]/ev : 0);
            }
          }
          #ifdef CLOSURE_TEST
            hKaonQ1_Gen[iM][iS][i]->Write();
            hKaonQ11_Gen[iM][iS][i]->Write();
            hKaonQ1Sq_Gen[iM][iS][i]->Write();
            hKaonQ2_Gen[iM][iS][i]->Write();
            hXiQ1_Gen[iM][iS][i]->Write();
            hXiQ11_Gen[iM][iS][i]->Write();
            hXiQ1Sq_Gen[iM][iS][i]->Write();
            hXiQ2_Gen[iM][iS][i]->Write();
            hSameKaonXiQ11_Gen[iM][iS][i]->Write();
            hOppKaonXiQ11_Gen[iM][iS][i]->Write();
            for (int iC = 0; iC < 2; ++iC)
              hNKaonXi_Gen[iM][iC][iS][i]->Write();
          #endif // CLOSURE_TEST
          for (int iC = 0; iC < 2; ++iC){
            hKaonQ1[iM][iC][iS][i]->Write();
            hKaonQ11[iM][iC][iS][i]->Write();
            hKaonQ1Sq[iM][iC][iS][i]->Write();
            hKaonQ2[iM][iC][iS][i]->Write();
            hXiQ1[iM][iC][iS][i]->Write();
            hXiQ11[iM][iC][iS][i]->Write();
            hXiQ1Sq[iM][iC][iS][i]->Write();
            hXiQ2[iM][iC][iS][i]->Write();
            hSameKaonXiQ11[iM][iC][iS][i]->Write();
            hOppKaonXiQ11[iM][iC][iS][i]->Write();
            hNKaonXi[iC][iM][iS][i]->Write();
          }
        }
        hCent[iS]->Write();
      }

      o[i - iVarMin]->cd();
      #ifdef CLOSURE_TEST
        for (int iM = 0; iM < 2; ++iM){
          hRecKaon[iM][i]->Write();
          hGenRecKaon[iM][i]->Write();
          hGenRecXi[iM][i]->Write();
          hRecXi[iM][i]->Write();
          hGenXi[iM][i]->Write();
        }
      #endif
      for (int iC = 0; iC < 2; ++iC){
        for (int iE = 0; iE < kNEtaBins; ++iE){
          hNsigmaTOF[iC][iE][i]->Write();
          hNsigmaTPC[iC][iE][i]->Write();
          hNsigmaITS[iC][iE][i]->Write();
        }
        hBDTOut[iC][i]->Write();
        hMass[iC][i]->Write();
      }
    #endif // FILL_HIST

    o[i - iVarMin]->cd();
    evtTuple[i - iVarMin]->Write();
  }

  for (int i{iVarMin}; i < iVarMax; ++i){
    o[i - iVarMin]->Close();
  }

  w.Stop();
  w.Print();

  // close stream
  f.Close();
}

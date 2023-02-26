#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH3F.h>
#include <TCanvas.h>
#include <TMath.h>
#include <Riostream.h>
#include "../utils/Config.h"
#include "TRandom3.h"
#include "TROOT.h"

#ifdef __CINT__
#pragma link C++ class std::vector<MiniKaon>+;
#pragma link C++ class std::vector<MiniKaonMC>+;
#pragma link C++ class std::vector<MiniXi>+;
#pragma link C++ class std::vector<MiniXiMC>+;
#endif

void ReadTree(const char* fname = "tree/data_test_18q_pass3/AnalysisResults", const char* ofname = "o", const bool isMC = false){
  ROOT::EnableImplicitMT(10);
  TFile f(Form("%s/%s.root", kDataDir, fname));
  std::vector<MiniKaon> *k = nullptr;
  std::vector<MiniXi> *xi = nullptr;
  // std::vector<MiniKaonMC> *k = nullptr;
  // std::vector<MiniXiMC> *xi = nullptr;
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
  
  TH3I *hNKaonXi[2][N_SAMPLE];

  for (int iC = 0; iC < 2; ++iC){
    for (int iS = 0; iS < N_SAMPLE; ++iS){
      hNKaonXi[iC][iS] = new TH3I(Form("h%sNKaonXi_%d", kChargeLabel[iC], iS), ";Centrality (%);K;#Xi", 100, 0, 100, 200, 0, 200, 100, 0, 100);
    }
  }

  TFile o(Form("%s.root", ofname), "recreate");
  Long64_t nEntries = t->GetEntries();
  int nEntriesSample = nEntries/N_SAMPLE;
  int nEntriesSampleRest = nEntries - nEntriesSample*N_SAMPLE;
  std::cout << "nEntriesSample = " << nEntriesSample << "; nEntriesSampleRest = " << nEntriesSampleRest << std::endl;
  for (int iS = 0; iS < N_SAMPLE; ++iS){
    int nXiTOT = 0;
    for (int i = iS * nEntriesSample; i < nEntriesSample * (iS + 1); ++i){ 
      Long64_t tentry = t->LoadTree(i);
      be->GetEntry(tentry);
      bk->GetEntry(tentry);
      bxi->GetEntry(tentry);

      if ( !isMC && ( ( c->fCent > 9.9 && c->fCent < 30 && ( ( c->fTrigger & 2 ) == 2 ) ) || ( c->fCent > 49.9 && ( ( c->fTrigger & 4 ) == 4 ) ) ) ){
        continue;
      }

      int nK[] = {0, 0};
      int nXi[] = {0, 0};
      for (UInt_t iK = 0; iK < k->size(); ++iK){
        if ( //(isMC && k->at(iK).fIsReconstructed && k->at(iK).fFlag == 1) &&
            (std::abs(k->at(iK).fEta) < kEtaCut) &&
            ((std::abs(k->at(iK).fPt) < kTPCptCut && (std::abs(k->at(iK).fNsigmaTPC) < kNsigmaTPCcut) ) ||
            (std::abs(k->at(iK).fPt) > kTPCptCut && (std::abs(k->at(iK).fPt) < kTOFptCut) && (std::abs(k->at(iK).fNsigmaTPC) < kNsigmaTPCcut) && (std::abs(k->at(iK).fNsigmaTOF) < kNsigmaTOFcut))) )
        {
          k->at(iK).fPt > 0 ? nK[1] += 1 : nK[0] += 1;
        }
      }
      for (UInt_t iXi = 0; iXi < xi->size(); ++iXi){
        if ( //(isMC && xi->at(iXi).fIsReconstructed) &&
          std::abs(xi->at(iXi).fEta) < kEtaCut &&
          std::abs(xi->at(iXi).fPt) > kXiLowPtCut && std::abs(xi->at(iXi).fPt) < kXiUpPtCut &&
          std::abs(xi->at(iXi).fMass - kXiMass) < kXiMassCut)
        {
          if (!isMC && xi->at(iXi).fBdtOut < 0.97) continue;
          xi->at(iXi).fPt > 0 ? nXi[1] += 1 : nXi[0] += 1;
          nXiTOT++;
        }
      }

      hNKaonXi[0][iS]->Fill(c->fCent, nK[0], nXi[1]);
      hNKaonXi[0][iS]->Fill(c->fCent, nK[1], nXi[0]);
      hNKaonXi[1][iS]->Fill(c->fCent, nK[0], nXi[0]);
      hNKaonXi[1][iS]->Fill(c->fCent, nK[1], nXi[1]);
    }
    hNKaonXi[0][iS]->Write();
    hNKaonXi[1][iS]->Write();
    std::cout << "nXiTOT = " << nXiTOT << std::endl;
  }

  // close stream
  o.Close();
  f.Close();
}

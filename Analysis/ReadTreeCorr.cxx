#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TMath.h>
#include <Riostream.h>
#include "../utils/Config.h"
#include "TRandom3.h"
#include "TROOT.h"

#ifdef __CINT__
#pragma link C++ class std::vector<MiniKaonMC>+;
#pragma link C++ class std::vector<MiniXiMC>+;
#endif

void ReadTreeCorr(const char* fname = "mc_tree/AnalysisResults_mc_LHC21l5", const char* ofname = "oo"){ // mc_tree/AnalysisResults_mc
  ROOT::EnableImplicitMT(10);
  TFile f(Form("%s/%s.root", kDataDir, fname));
  std::vector<MiniKaonMC> *k = nullptr;
  std::vector<MiniXiMC> *xi = nullptr;
  MiniCollision *c = new MiniCollision();
  c->fZ = 0.;
  c->fCent = 0.;
  c->fTrigger = 0x00;
  TTree *t = (TTree*)f.Get("StrangenessTree");
  TBranch *bk = nullptr;
  TBranch *bxi = nullptr;
  t->SetBranchAddress("MiniKaonMC", &k, &bk);
  t->SetBranchAddress("MiniXiMC", &xi, &bxi);
  TBranch *be = t->GetBranch("MiniCollision");
  be->SetAddress(&c);
  
  TH2D *hGenKaon[2];
  TH2D *hRecKaon[2];
  double ptBins[kNBinsPt + 1];
  for (int iB = 0; iB < kNBinsPt + 1; ++iB){
    ptBins[iB] = kMinPt + kDeltaPt * iB;
  }
  for (int iC = 0; iC < 2; ++iC){
    hGenKaon[iC] = new TH2D(Form("h%sGenKaon", kAntiMatterLabel[iC]), ";Centrality (%);#it{p}_{T} (GeV/#it{c})", kNCentBins, kCentBins, kNBinsPt, ptBins);
    hRecKaon[iC] = new TH2D(Form("h%sRecKaon", kAntiMatterLabel[iC]), ";Centrality (%);#it{p}_{T} (GeV/#it{c})", kNCentBins, kCentBins, kNBinsPt, ptBins);
  }

  TFile o(Form("%s.root", ofname), "recreate");
  Long64_t nEntries = t->GetEntries();

  for (int i = 0; i < nEntries; ++i){
    Long64_t tentry = i;
    be->GetEntry(tentry);
    bk->GetEntry(tentry);
    bxi->GetEntry(tentry);

    int nK[] = {0, 0};
    int nXi[] = {0, 0};
    for (UInt_t iK = 0; iK < k->size(); ++iK){
      if (!(k->at(iK).fFlag == 1)) continue;
      int im_MC = k->at(iK).fPtMC > 0 ? 1 : 0;
      hGenKaon[im_MC]->Fill(c->fCent, std::abs(k->at(iK).fPtMC));
      if (
          k->at(iK).fIsReconstructed &&
          (std::abs(k->at(iK).fEtaMC) < kEtaCut) &&
          ((std::abs(k->at(iK).fPt) < kTPCptCut && (std::abs(k->at(iK).fNsigmaTPC) < kNsigmaTPCcut) ) ||
          (std::abs(k->at(iK).fPt) > kTPCptCut && (std::abs(k->at(iK).fPt) < kTOFptCut) && (std::abs(k->at(iK).fNsigmaTPC) < kNsigmaTPCcut) && (std::abs(k->at(iK).fNsigmaTOF) < kNsigmaTOFcut)))
        )
      {
        int im = k->at(iK).fPt > 0 ? 1 : 0;
        hRecKaon[im]->Fill(c->fCent, std::abs(k->at(iK).fPt));
      }
    }
  }

  for (int iC = 0; iC < 2; ++iC){
    hGenKaon[iC]->Write();
    hRecKaon[iC]->Write();
  }

  // close stream
  o.Close();
  f.Close();
}

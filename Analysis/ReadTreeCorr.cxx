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

bool limit_tree = false;

#ifdef __CINT__
#pragma link C++ class std::vector<MiniKaonMC>+;
#pragma link C++ class std::vector<MiniXiMC>+;
#endif

void ReadTreeCorr(const char* fname = "mc_tree/AnalysisResults_LHC21l5_child1_cpy", const char* ofname = "oo_limit"){ // mc_tree/AnalysisResults_mc
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
  float index = 0.;
  TBranch *bindex = nullptr;
  t->SetBranchAddress("MiniKaonMC", &k, &bk);
  t->SetBranchAddress("MiniXiMC", &xi, &bxi);
  TBranch *be = t->GetBranch("MiniCollision");
  t->SetBranchAddress("index", &index, &bindex);
  be->SetAddress(&c);
  
  TH2D *hGenKaon[2];
  TH2D *hGenXi[2];
  TH2D *hRecKaon[2];
  TH2D *hRecXi[2];
  double ptBins[kNBinsPt + 1];
  for (int iB = 0; iB < kNBinsPt + 1; ++iB){
    ptBins[iB] = kMinPt + kDeltaPt * iB;
  }
  double ptBinsXi[kNBinsPtXi + 1];
  for (int iB = 0; iB < kNBinsPtXi + 1; ++iB){
    ptBinsXi[iB] = kMinPt + kDeltaPtXi * iB;
  }
  for (int iC = 0; iC < 2; ++iC){
    hGenKaon[iC] = new TH2D(Form("h%sGenKaon", kAntiMatterLabel[iC]), ";Centrality (%);#it{p}_{T} (GeV/#it{c})", kNCentBins, kCentBins, kNBinsPt, ptBins);
    hGenXi[iC] = new TH2D(Form("h%sGenXi", kAntiMatterLabel[iC]), ";Centrality (%);#it{p}_{T} (GeV/#it{c})", kNCentBins, kCentBins, kNBinsPtXi, ptBinsXi);
    hRecKaon[iC] = new TH2D(Form("h%sRecKaon", kAntiMatterLabel[iC]), ";Centrality (%);#it{p}_{T} (GeV/#it{c})", kNCentBins, kCentBins, kNBinsPt, ptBins);
    hRecXi[iC] = new TH2D(Form("h%sRecXi", kAntiMatterLabel[iC]), ";Centrality (%);#it{p}_{T} (GeV/#it{c})", kNCentBins, kCentBins, kNBinsPtXi, ptBinsXi);
  }

  TFile o(Form("%s.root", ofname), "recreate");
  Long64_t nEntries = limit_tree ? kLimitedSample : t->GetEntries();
  Long64_t nEntriesSample = nEntries/N_SAMPLE;
  for (int iS = 0; iS < N_SAMPLE; ++iS){
    for (int iC = 0; iC < 2; ++iC){
      hGenKaon[iC]->Reset();
      hRecKaon[iC]->Reset();
      hGenXi[iC]->Reset();
      hRecXi[iC]->Reset();
    }
    for (Long64_t i = 0; i < nEntries; ++i){
      Long64_t tentry = i;
      be->GetEntry(tentry);
      bk->GetEntry(tentry);
      bxi->GetEntry(tentry);
      bindex->GetEntry(tentry);
      //if (index * nEntries < iS * nEntriesSample || index * nEntries > nEntriesSample * (iS + 1)) continue;
      if (c->fCent > 90) continue;

      int nK[] = {0, 0};
      int nXi[] = {0, 0};
      for (UInt_t iK = 0; iK < k->size(); ++iK){
        if (std::abs(k->at(iK).fPtMC) > kTOFptCut || std::abs(k->at(iK).fPtMC) < kPtLowLimitK || k->at(iK).fFlag != 1 || std::abs(k->at(iK).fEtaMC) > kEtaCut) continue;
        int im_MC = k->at(iK).fPtMC > 0 ? 1 : 0;
        hGenKaon[im_MC]->Fill(c->fCent, std::abs(k->at(iK).fPtMC));
        if (
            k->at(iK).fIsReconstructed &&
            std::abs(k->at(iK).fPt) > kPtLowLimitK && std::abs(k->at(iK).fPt) < kTOFptCut &&
            (std::abs(k->at(iK).fEta) < kEtaCut) &&
            ((std::abs(k->at(iK).fPt) < kTPCptCut && (std::abs(k->at(iK).fNsigmaTPC) < kNsigmaTPCcut) ) ||
            (std::abs(k->at(iK).fPt) > kTPCptCut && (std::abs(k->at(iK).fPt) < kTOFptCut) && (std::abs(k->at(iK).fNsigmaTPC) < kNsigmaTPCcutPresel) && (std::abs(k->at(iK).fNsigmaTOF) < kNsigmaTOFcut)))
          )
        {
          int im = k->at(iK).fPt > 0 ? 1 : 0;
          hRecKaon[im]->Fill(c->fCent, std::abs(k->at(iK).fPt));
        }
      }

      for (UInt_t iXi = 0; iXi < xi->size(); ++iXi){
        if (std::abs(xi->at(iXi).fPtMC) > kXiUpPtCut || std::abs(xi->at(iXi).fPtMC) < kXiLowPtCut || std::abs(xi->at(iXi).fEtaMC) > kEtaCut) continue;
        int im_MC = xi->at(iXi).fPtMC > 0;
        hGenXi[im_MC]->Fill(c->fCent, std::abs(xi->at(iXi).fPtMC));
        if (
            std::abs(xi->at(iXi).fEta) < kEtaCut &&
            std::abs(xi->at(iXi).fPt) > kXiLowPtCut && std::abs(xi->at(iXi).fPt) < kXiUpPtCut &&
            std::abs(xi->at(iXi).fMass - kXiMass) < kXiMassCut
          )
        {
          if (!xi->at(iXi).fIsReconstructed || xi->at(iXi).fFlag != 1 || std::abs(xi->at(iXi).fEtaMC) > kEtaCut) continue;
          int im = xi->at(iXi).fPt > 0;
          hRecXi[im]->Fill(c->fCent, std::abs(xi->at(iXi).fPt));
        }
      }
    }

    o.mkdir(Form("subsample_%d", iS + 1));
    o.cd(Form("subsample_%d", iS + 1));
    for (int iC = 0; iC < 2; ++iC){
      hGenKaon[iC]->Write();
      hRecKaon[iC]->Write();
      hGenXi[iC]->Write();
      hRecXi[iC]->Write();
    }
  }

  // close stream
  o.Close();
  f.Close();
}

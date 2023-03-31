#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH3D.h>
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

void ReadTreeCorr(const char* fname = "mc_tree/AnalysisResults_LHC21d6_strinj", const char* ofname = "oo_limit_21d6_strinj"){ // mc_tree/AnalysisResults_mc
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
  if (kUseIndex) t->SetBranchAddress("index", &index, &bindex);
  be->SetAddress(&c);
  
  TH3D *hGenKaon[2];
  TH3D *hGenXi[2];
  TH3D *hRecKaon[2];
  TH3D *hRecXi[2];
  TH3D *hBDTOut[2];
  double ptBins[kNBinsPt + 1];
  for (int iB = 0; iB < kNBinsPt + 1; ++iB){
    ptBins[iB] = kMinPt + kDeltaPt * iB;
  }
  double ptBinsXi[kNBinsPtXi + 1];
  for (int iB = 0; iB < kNBinsPtXi + 1; ++iB){
    double f = iB < kNBinsPtXi ? 0. : 1.;
    ptBinsXi[iB] = kMinPt + kDeltaPtXi * iB + f * kDeltaPtXi;
  }
  double etaBins[kNEtaBins + 1];
  for (int iB = 0; iB < kNEtaBins + 1; ++iB){
    etaBins[iB] = kMinEta + kDeltaEta * iB;
  }
  double bdtBins[101];
  for (int iB = 0; iB < 101; ++iB){
    bdtBins[iB] = iB / 100.;
  }
  for (int iC = 0; iC < 2; ++iC){
    hGenKaon[iC] = new TH3D(Form("h%sGenKaon", kAntiMatterLabel[iC]), ";Centrality (%);#eta;#it{p}_{T} (GeV/#it{c})", kNCentBins, kCentBins, kNEtaBins, etaBins, kNBinsPt, ptBins);
    hGenXi[iC] = new TH3D(Form("h%sGenXi", kAntiMatterLabel[iC]), ";Centrality (%);#eta;#it{p}_{T} (GeV/#it{c})", kNCentBins, kCentBins, kNEtaBins, etaBins, kNBinsPtXi, ptBinsXi);
    hRecKaon[iC] = new TH3D(Form("h%sRecKaon", kAntiMatterLabel[iC]), ";Centrality (%);#eta;#it{p}_{T} (GeV/#it{c})", kNCentBins, kCentBins, kNEtaBins, etaBins, kNBinsPt, ptBins);
    hRecXi[iC] = new TH3D(Form("h%sRecXi", kAntiMatterLabel[iC]), ";Centrality (%);#eta;#it{p}_{T} (GeV/#it{c})", kNCentBins, kCentBins, kNEtaBins, etaBins, kNBinsPtXi, ptBinsXi);
    hBDTOut[iC] = new TH3D(Form("h%sBDTOutXi", kAntiMatterLabel[iC]), ";Centrality (%);#it{p}_{T} (GeV/#it{c});BDT out", kNCentBins, kCentBins, kNBinsPtXi, ptBinsXi, 100, bdtBins);
  }

  TFile o(Form("%s.root", ofname), "recreate");
  Long64_t nEntries = kLimitSample ? kLimitedSample : t->GetEntries();
  Long64_t nEntriesSample = nEntries/N_SAMPLE;
  for (int iS = 0; iS < N_SAMPLE; ++iS){
    for (int iC = 0; iC < 2; ++iC){
      hGenKaon[iC]->Reset();
      hRecKaon[iC]->Reset();
      hGenXi[iC]->Reset();
      hRecXi[iC]->Reset();
      hBDTOut[iC]->Reset();
    }
    for (Long64_t i = 0; i < nEntries; ++i){
      Long64_t tentry = i;
      be->GetEntry(tentry);
      bk->GetEntry(tentry);
      bxi->GetEntry(tentry);
      if (kUseIndex){
        bindex->GetEntry(tentry);
        if (index * nEntries < iS * nEntriesSample || index * nEntries > nEntriesSample * (iS + 1)) continue;
      }
      if (c->fCent > 90) continue;

      int nK[] = {0, 0};
      int nXi[] = {0, 0};
      for (UInt_t iK = 0; iK < k->size(); ++iK){
        if (std::abs(k->at(iK).fPtMC) > kTOFptCut || std::abs(k->at(iK).fPtMC) < kPtLowLimitK || k->at(iK).fFlag != 1 || std::abs(k->at(iK).fEtaMC) > kEtaCut) continue;
        int im_MC = k->at(iK).fPtMC > 0 ? 1 : 0;
        hGenKaon[im_MC]->Fill(c->fCent, k->at(iK).fEtaMC, std::abs(k->at(iK).fPtMC));
        if (
            (k->at(iK).fCutBitMap == 1 || k->at(iK).fCutBitMap == 2) &&
            k->at(iK).fIsReconstructed &&
            std::abs(k->at(iK).fPt) > kPtLowLimitK && std::abs(k->at(iK).fPt) < kTOFptCut &&
            (std::abs(k->at(iK).fEta) < kEtaCut) &&
            ((std::abs(k->at(iK).fPt) < kTPCptCut && (k->at(iK).fNsigmaTPC > kNsigmaTPCcutAsym[0] && k->at(iK).fNsigmaTPC < kNsigmaTPCcutAsym[1]) ) ||
            (std::abs(k->at(iK).fPt) > kTPCptCut && (std::abs(k->at(iK).fPt) < kTOFptCut) && (std::abs(k->at(iK).fNsigmaTPC) < kNsigmaTPCcutPresel) && (k->at(iK).fNsigmaTOF > kNsigmaTOFcutAsym[0] && k->at(iK).fNsigmaTOF < kNsigmaTOFcutAsym[1])))
          )
        {
          int im = k->at(iK).fPt > 0 ? 1 : 0;
          hRecKaon[im]->Fill(c->fCent, k->at(iK).fEta, std::abs(k->at(iK).fPt));
        }
      }

      for (UInt_t iXi = 0; iXi < xi->size(); ++iXi){
        if (std::abs(xi->at(iXi).fPtMC) > kXiUpPtCut || std::abs(xi->at(iXi).fPtMC) < kXiLowPtCut || std::abs(xi->at(iXi).fEtaMC) > kEtaCut) continue;
        int im_MC = xi->at(iXi).fPtMC > 0;
        hGenXi[im_MC]->Fill(c->fCent, xi->at(iXi).fEtaMC, std::abs(xi->at(iXi).fPtMC));
        if (
            std::abs(xi->at(iXi).fEta) < kEtaCut &&
            std::abs(xi->at(iXi).fPt) > kXiLowPtCut && std::abs(xi->at(iXi).fPt) < kXiUpPtCut &&
            std::abs(xi->at(iXi).fMass - kXiMass) < kXiMassCut
          )
        {
          if (!xi->at(iXi).fIsReconstructed || xi->at(iXi).fFlag != 1 || std::abs(xi->at(iXi).fEtaMC) > kEtaCut || (xi->at(iXi).fRecFlag & BIT(0)) != 1 || (xi->at(iXi).fRecFlag & BIT(1)) != 2) continue;
          if (kUseBdtInMC){
            if (xi->at(iXi).fBdtOut < kBdtScoreCut) continue;
          }
          int im = xi->at(iXi).fPt > 0;
          hRecXi[im]->Fill(c->fCent, xi->at(iXi).fEta, std::abs(xi->at(iXi).fPt));
          hBDTOut[im]->Fill(c->fCent, std::abs(xi->at(iXi).fPt), xi->at(iXi).fBdtOut);
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
      hBDTOut[iC]->Write();
    }
  }

  // close stream
  o.Close();
  f.Close();
}

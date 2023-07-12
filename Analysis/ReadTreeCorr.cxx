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
#include <TStopwatch.h>

#ifdef __CINT__
#pragma link C++ class std::vector<MiniKaonMC>+;
#pragma link C++ class std::vector<MiniXiMC>+;
#endif

void ReadTreeCorr(const char* fname = "pPb/mc_tree/AnalysisResults_LHC20f11_postCalib" /* "pp/mc_train/dataset_after_calib/calib_fast_mc/AnalysisResults_LHC22l5_fast" */ /* "mc_tree/dataset_after_calib/merged/AnalysisResults_LHC21d6_postCalib" */, const char* ofname = "oo_limit_lhc20f11_postCalib_finalBinning"){ // pp/mc_train/dataset_after_calib/AnalysisResults_LHC22l5 mc_tree/dataset_after_calib/merged/AnalysisResults_LHC22l5_postCalib mc_tree/AnalysisResults_mc; pp/mc_train/AnalysisResults_1, oo_limit_pp
  TTree::SetMaxTreeSize( 1000000000000LL ); // 1 TB
  TStopwatch w;
  w.Start();
  
  ROOT::EnableImplicitMT(10);
  TFile f(Form("%s/%s.root", kDataDir, fname));
  TFile fBdtmap(Form("results/%s.root", kBdtmapName));
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

  const int kKCut = kNTpcClsCuts * kNDcaCuts * kNChi2Cuts * kNPidCuts;
  const int kXiCut = kNMassCuts * kNBdtCuts;
  TH3D *hGenKaon[2][kKCut];
  TH3D *hGenXi[2][kXiCut];
  TH3D *hRecKaon[2][kKCut];
  TH3D *hRecXi[2][kXiCut];
  TH3D *hBDTOut[2][kXiCut];
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
  double bdtBins[10001];
  for (int iB = 0; iB < 10001; ++iB){
    bdtBins[iB] = iB / 10000.;
  }
  for (int iVar{0}; iVar < kNTpcClsCuts * kNDcaCuts * kNChi2Cuts * kNPidCuts; iVar++){
    for (int iC = 0; iC < 2; ++iC){
      hGenKaon[iC][iVar] = new TH3D(Form("h%sGenKaon_%d", kAntiMatterLabel[iC], iVar), ";Centrality (%);#eta;#it{p}_{T} (GeV/#it{c})", kNCentBins, kCentBins, kNEtaBins, etaBins, kNBinsPt, ptBins);
      hRecKaon[iC][iVar] = new TH3D(Form("h%sRecKaon_%d", kAntiMatterLabel[iC], iVar), ";Centrality (%);#eta;#it{p}_{T} (GeV/#it{c})", kNCentBins, kCentBins, kNEtaBins, etaBins, kNBinsPt, ptBins);
      if (iVar < kNMassCuts * kNBdtCuts){
        hGenXi[iC][iVar] = new TH3D(Form("h%sGenXi_%d", kAntiMatterLabel[iC], iVar), ";Centrality (%);#eta;#it{p}_{T} (GeV/#it{c})", kNCentBins, kCentBins, kNEtaBins, etaBins, kNBinsPtXi, ptBinsXi);
        hRecXi[iC][iVar] = new TH3D(Form("h%sRecXi_%d", kAntiMatterLabel[iC], iVar), ";Centrality (%);#eta;#it{p}_{T} (GeV/#it{c})", kNCentBins, kCentBins, kNEtaBins, etaBins, kNBinsPtXi, ptBinsXi);
        hBDTOut[iC][iVar] = new TH3D(Form("h%sBDTOutXi_%d", kAntiMatterLabel[iC], iVar), ";Centrality (%);#it{p}_{T} (GeV/#it{c});BDT out", kNCentBins, kCentBins, kNBinsPtXi, ptBinsXi, 10000, bdtBins);
      }
    }
  }

  TFile o(Form("%s/%s.root", kResDir, ofname), "recreate");
  Long64_t nEntries = kLimitSample ? kLimitedSample : t->GetEntries();
  Long64_t nEntriesSample = nEntries/N_SAMPLE;
  for (int iS = 0; iS < N_SAMPLE; ++iS){
    
    for (int iVar{0}; iVar < kNTpcClsCuts * kNDcaCuts * kNChi2Cuts * kNPidCuts; iVar++){
      for (int iC = 0; iC < 2; ++iC){
        hGenKaon[iC][iVar]->Reset();
        hRecKaon[iC][iVar]->Reset();
        if (iVar < kNMassCuts * kNBdtCuts){
          hGenXi[iC][iVar]->Reset();
          hRecXi[iC][iVar]->Reset();
          hBDTOut[iC][iVar]->Reset();
        }
      }
    }
    for (Long64_t i = 0; i < nEntries; ++i){

      Long64_t tentry = i;
      if (be->GetEntry(tentry) < 0 || bk->GetEntry(tentry) < 0 || bxi->GetEntry(tentry) < 0) continue;
      if (kUseIndex){
        bindex->GetEntry(tentry);
        if (index * nEntries < iS * nEntriesSample || index * nEntries > nEntriesSample * (iS + 1)) continue;
      }
      if (c->fCent > kMaxCent) continue;

      if (!(i%10000)) std::cout << "n_ev = " << i << std::endl;

      for (int iVar{0}; iVar < kNTpcClsCuts * kNDcaCuts * kNChi2Cuts * kNPidCuts; ++iVar)
      {
        if (iVar != 7 && kGetBDTEffMap) continue;
        int iTpcClsCut = (iVar / 1) % kNTpcClsCuts;
        int iPidCut = (iVar / kNTpcClsCuts) % kNPidCuts;
        int iDcaCut = (iVar / kNTpcClsCuts / kNPidCuts) % kNDcaCuts;
        int iChi2Cut = (iVar / kNTpcClsCuts / kNPidCuts / kNDcaCuts) % kNChi2Cuts;
        int iMassCut = (iVar / 1 ) % kNMassCuts;
        int iBdtScoreCut = (iVar / kNMassCuts) % kNBdtCuts;

        int nK[] = {0, 0};
        int nXi[] = {0, 0};
        for (UInt_t iK = 0; iK < k->size(); ++iK){
          if (std::abs(k->at(iK).fPtMC) > kTOFptCut || std::abs(k->at(iK).fPtMC) < kPtLowLimitK || k->at(iK).fFlag != 1 || std::abs(k->at(iK).fEtaMC) > kEtaCut) continue;
          int im_MC = k->at(iK).fPtMC > 0 ? 1 : 0;
          hGenKaon[im_MC][iVar]->Fill(c->fCent, k->at(iK).fEtaMC, std::abs(k->at(iK).fPtMC));
          if (
              ( ( (k->at(iK).fCutBitMap & kCutDCA[1]) == kCutDCA[1]) || ((k->at(iK).fCutBitMap & kCutDCA2[1]) == kCutDCA2[1]) ) &&
              ( ( ( ((k->at(iK).fCutBitMap & kCutDCA[iDcaCut]) == kCutDCA[iDcaCut]) || ((k->at(iK).fCutBitMap & kCutDCA2[iDcaCut]) == kCutDCA2[iDcaCut]) ) && kRequireDCAcut[iDcaCut] && std::abs(k->at(iK).fPtMC) > kDCAcutPt) || (!kRequireDCAcut[iDcaCut]) || std::abs(k->at(iK).fPtMC) < kDCAcutPt) &&
              ( ( ((k->at(iK).fCutBitMap & kCutTPCcls[iTpcClsCut]) == kCutTPCcls[iTpcClsCut] || (k->at(iK).fCutBitMap & kCutTPCcls2[iTpcClsCut]) == kCutTPCcls2[iTpcClsCut]) && kRequireTPCclsCut[iTpcClsCut] ) || !kRequireTPCclsCut[iTpcClsCut]) &&
              ( ( ((k->at(iK).fCutBitMap & kCutChi2[iChi2Cut]) == kCutChi2[iChi2Cut] || (k->at(iK).fCutBitMap & kCutChi22[iChi2Cut]) == kCutChi22[iChi2Cut]) && kRequireChi2Cut[iChi2Cut] ) || !kRequireChi2Cut[iChi2Cut] ) &&
              k->at(iK).fIsReconstructed &&
              std::abs(k->at(iK).fPt) > kPtLowLimitK && std::abs(k->at(iK).fPt) < kTOFptCut &&
              (std::abs(k->at(iK).fEta) < kEtaCut)
              &&
              ((std::abs(k->at(iK).fPt) < kTPCptCut && (k->at(iK).fNsigmaTPC > kNsigmaTPCcutAsym[iPidCut][0] && k->at(iK).fNsigmaTPC < kNsigmaTPCcutAsym[iPidCut][1])) ||
              (std::abs(k->at(iK).fPt) > kTPCptCut && (std::abs(k->at(iK).fPt) < kTOFptCut) && (std::abs(k->at(iK).fNsigmaTPC) < kNsigmaTPCcutPresel) && (k->at(iK).fNsigmaTOF > kNsigmaTOFcutAsym[iPidCut][0] && k->at(iK).fNsigmaTOF < kNsigmaTOFcutAsym[iPidCut][1])))
            )
          {
            if (std::abs(k->at(iK).fPt) < kITSptCut && (k->at(iK).fNsigmaITS < kNsigmaITScutAsym[0] || k->at(iK).fNsigmaITS > kNsigmaITScutAsym[1])) continue;
            if (kUseCircularPID)
            {
              double nsigmaITS = k->at(iK).fNsigmaITS;
              double nsigmaTPC = k->at(iK).fNsigmaTPC;
              double nsigmaTOF = k->at(iK).fNsigmaTOF;
              if (TMath::Sqrt(nsigmaITS * nsigmaITS + nsigmaTPC * nsigmaTPC) > kCircularCuts[0] && std::abs(k->at(iK).fPt) < kTPCptCut) continue;
              else if (TMath::Sqrt(nsigmaTPC * nsigmaTPC + nsigmaTOF * nsigmaTOF) > kCircularCuts[0] && std::abs(k->at(iK).fPt) > kTPCptCut && std::abs(k->at(iK).fPt) < kTOFptCut) continue;
            }
            int im = k->at(iK).fPt > 0 ? 1 : 0;
            hRecKaon[im][iVar]->Fill(c->fCent, k->at(iK).fEta, std::abs(k->at(iK).fPt));
          }
        }
        if (iVar < kNMassCuts * kNBdtCuts){
          for (UInt_t iXi = 0; iXi < xi->size(); ++iXi){
            if (std::abs(xi->at(iXi).fPtMC) > kXiUpPtCut || std::abs(xi->at(iXi).fPtMC) < kXiLowPtCut || std::abs(xi->at(iXi).fEtaMC) > kEtaCut) continue;
            int im_MC = xi->at(iXi).fPtMC > 0 ? 1 : 0;
            hGenXi[im_MC][iVar]->Fill(c->fCent, xi->at(iXi).fEtaMC, std::abs(xi->at(iXi).fPtMC));
            if (
                std::abs(xi->at(iXi).fEta) < kEtaCut &&
                std::abs(xi->at(iXi).fPt) > kXiLowPtCut && std::abs(xi->at(iXi).fPt) < kXiUpPtCut &&
                std::abs(xi->at(iXi).fMass - kXiMass) < kXiMassCuts[iMassCut]
              )
            {
              if (!xi->at(iXi).fIsReconstructed || xi->at(iXi).fFlag != 1 || std::abs(xi->at(iXi).fEtaMC) > kEtaCut || (xi->at(iXi).fRecFlag & BIT(0)) != 1 || (xi->at(iXi).fRecFlag & BIT(1)) != 2) continue;
              if (kUseBdtInMC){
                if (kGetBDTEffMap){
                  if (xi->at(iXi).fBdtOut < 0) continue;
                }
                else {
                  int ptBin = hRecXi[0][0]->GetZaxis()->FindBin(std::abs(xi->at(iXi).fPt));
                  if (xi->at(iXi).fBdtOut < bdtScoreCuts[ptBin - 1][iBdtScoreCut]) continue;
                  //if (xi->at(iXi).fBdtOut < kBdtScoreCuts[iBdtScoreCut]) continue;
                }
              }
              int im = xi->at(iXi).fPt > 0 ? 1 : 0;
              hRecXi[im][iVar]->Fill(c->fCent, xi->at(iXi).fEta, std::abs(xi->at(iXi).fPt));
              hBDTOut[im][iVar]->Fill(c->fCent, std::abs(xi->at(iXi).fPt), xi->at(iXi).fBdtOut);
            }
          }
        }
      }
    }

    for (int iVar{0}; iVar < kNTpcClsCuts * kNDcaCuts * kNChi2Cuts * kNPidCuts; iVar++){
      o.mkdir(Form("subsample_%d_var_%d", iS + 1, iVar));
      o.cd(Form("subsample_%d_var_%d", iS + 1, iVar));
      for (int iC = 0; iC < 2; ++iC){
        hGenKaon[iC][iVar]->Write();
        hRecKaon[iC][iVar]->Write();
        if (iVar < kNMassCuts * kNBdtCuts){
          hGenXi[iC][iVar]->Write();
          hRecXi[iC][iVar]->Write();
          hBDTOut[iC][iVar]->Write();
        }
      }
    }
  }

  // close stream
  o.Close();
  f.Close();


  // close stream
  o.Close();
  f.Close();

  w.Stop();
  w.Print();
}

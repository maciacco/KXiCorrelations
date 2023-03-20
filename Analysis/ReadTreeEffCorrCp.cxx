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
#include "TProfile.h"

#define CLOSURE_TEST

#ifdef __CINT__
#pragma link C++ class std::vector<MiniKaon>+;
#pragma link C++ class std::vector<MiniKaonMC>+;
#pragma link C++ class std::vector<MiniXi>+;
#pragma link C++ class std::vector<MiniXiMC>+;
#endif

void ReadTreeEffCorrCp(const char* fname = "tree_data_full/part_merging_True/%s_AnalysisResults", const char* ofname = "o", const bool isMC = false, const int tree_number = 1)
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
    TH1D *hKaonC1_Gen[2][N_SAMPLE];
  #endif
  TH1D *hKaonC1[2][2][N_SAMPLE];
  TH1D *hKaonC2[2][2][N_SAMPLE];
  
  TH3F *hNsigmaTPC[2];
  TH3F *hNsigmaTOF[2];
  TH3F *hMass[2];

  TH1D *hEffK[2][kNCentBins][kNEtaBins][N_SAMPLE];
  TH1D *hEffXi[2][kNCentBins][kNEtaBins][N_SAMPLE]; // TO BE IMPROVED -> CENTRALITY DIFFERENTIAL ESTIMATE OF XI EFFICIENCY (ALSO BDT)
  TH3F *hBDTEffXi; // TO BE IMPROVED -> CENTRALITY DIFFERENTIAL ESTIMATE OF XI EFFICIENCY (ALSO BDT -> SEPARATELY FOR CHARGES)

  for (int iS = 0; iS < N_SAMPLE; ++iS){
    hCent[iS] = new TH1D(Form("hCent_%d", iS + 1), ";Centrality (%);Entries", kNCentBinsSmall, kCentBinsSmall);
  }
  for (int iC = 0; iC < 2; ++iC){
    for (int iS = 0; iS < N_SAMPLE; ++iS){
      #ifdef CLOSURE_TEST
        hKaonC1_Gen[iC][iS] = new TH1D(Form("h%sKaonC1_Gen_%d", kAntiMatterLabel[iC], iS), ";Centrality (%);#it{C}_{1}^{K}", kNCentBinsSmall, kCentBinsSmall);
      #endif // CLOSURE_TEST
      for (int iCorr = 0; iCorr < 2; ++iCorr){
        hKaonC1[iC][iCorr][iS] = new TProfile(Form("h%sKaonC1_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iS + 1), ";Centrality (%);#it{C}_{1}^{K}", kNCentBinsSmall, kCentBinsSmall);
        hKaonC2[iC][iCorr][iS] = new TH1D(Form("h%sKaonC2_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iS + 1), ";Centrality (%);#it{C}_{1}^{K}", kNCentBinsSmall, kCentBinsSmall);
        for (int iB = 1; iB <= hKaonC1[iC][iCorr][iS]->GetNbinsX(); ++iB){
          hKaonC2[iC][iCorr][iS]->SetBinContent(iB, 0.);
        }
      }
    }
    for (int iCent = 0; iCent < kNCentBins; ++iCent){
      for (int iEta = 0; iEta < kNEtaBins; ++iEta){
        for (int iS = 0; iS < N_SAMPLE; ++iS){
          if (fEffK){
            hEffK[iC][iCent][iEta][iS] = (TH1D*)fEffK->Get(Form("subsample_%d/h%sEff%s_%d_%d", iS + 1, kAntiMatterLabel[iC], kPartLabel[0], iCent, iEta));
          }
          // TO BE IMPROVED -> CENTRALITY DIFFERENTIAL ESTIMATE OF XI EFFICIENCY (ALSO BDT)
          if (fEffXi){
            hEffXi[iC][iCent][iEta][iS] = (TH1D*)fEffK->Get(Form("subsample_%d/h%sEff%s_%d_%d", iS + 1, kAntiMatterLabel[iC], kPartLabel[1], iCent, iEta)); // (TH1D*)fEffXi->Get(Form("fPreselEff_vs_pt_%s_%.0f_%.0f", kAntiMatterLabelML[iC], kCentBins[iCent], kCentBins[iCent + 1]));
          }
        }
      }
    }
  }
  hBDTEffXi = (TH3F*)fEffBDTXi->Get("hBDTEff");

  #ifdef CLOSURE_TEST
    TH3F *hNKaonXi_Gen[2][2][N_SAMPLE];
    TH2D *hGenKaon[2];
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
      hGenKaon[iC] = new TH2D(Form("h%sGenKaon", kAntiMatterLabel[iC]), ";Centrality (%);#it{p}_{T} (GeV/#it{c})", kNCentBins, kCentBins, kNBinsPt, ptBins);
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
    TH1D hEtaTmp("hEtaTmp", "hEtaTmp", kNBinsPt, kMinEta, kMinEta + kDeltaEta * kNBinsPt);

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

      double q1_gen[2] = {0., 0.};
      double q1[2][2] = {{0., 0.}, {0., 0.}};
      double q1sq[2][2] = {{0., 0.}, {0., 0.}};
      double q2_gen[2] = {0., 0.};
      double q2[2][2] = {{0., 0.}, {0., 0.}};
      for (UInt_t iK = 0; iK < k->size(); ++iK){
        #ifdef CLOSURE_TEST
        if (std::abs(k->at(iK).fPtMC) > kTOFptCut || std::abs(k->at(iK).fPtMC) < kPtLowLimitK || k->at(iK).fFlag != 1 || std::abs(k->at(iK).fEtaMC) > kEtaCut ) continue;
          int im_MC = k->at(iK).fPtMC > 0 ? 1 : 0;
          q1_gen[im_MC] += 1;
          hGenKaon[im_MC]->Fill(c->fCent, std::abs(k->at(iK).fPtMC), 1.);
        #endif // CLOSURE_TEST
        if ( std::abs(k->at(iK).fPt) > kPtLowLimitK && std::abs(k->at(iK).fPt) < kTOFptCut &&
            (std::abs(k->at(iK).fEta) < kEtaCut) &&
            ((std::abs(k->at(iK).fPt) < kTPCptCut && (std::abs(k->at(iK).fNsigmaTPC) < kNsigmaTPCcut) ) ||
            (std::abs(k->at(iK).fPt) > kTPCptCut && (std::abs(k->at(iK).fPt) < kTOFptCut) && (std::abs(k->at(iK).fNsigmaTPC) < kNsigmaTPCcutPresel) && (std::abs(k->at(iK).fNsigmaTOF) < kNsigmaTOFcut)))
          )
        {
          #ifdef CLOSURE_TEST
            int im_ = k->at(iK).fPt > 0 ? 1 : 0;
            if (!k->at(iK).fIsReconstructed || k->at(iK).fFlag != 1) continue;
            int im_tmp = k->at(iK).fPt > 0 ? 1 : 0;
            hRecKaon[im_tmp]->Fill(c->fCent, std::abs(k->at(iK).fPt), 1.);
          #endif // CLOSURE_TEST
          int im = k->at(iK).fPt > 0 ? 1 : 0;
          int ie = hEtaTmp.FindBin(k->at(iK).fEta);
          //std::cout << "im = " << im << ", ic = " << ic - 1 << std::endl;
          double eff = fEffK ? hEffK[im][ic - 1][ie - 1][iS]->GetBinContent(hEffK[im][ic - 1][ie - 1][iS]->FindBin(std::abs(k->at(iK).fPt))) : kDummyEffK;
          //std::cout << "pt = " << std::abs(k->at(iK).fPt) << "; bin = " << hEffK[im][ic - 1]->GetXaxis()->FindBin(std::abs(k->at(iK).fPt)) << "eff = " << eff << std::endl;
          
          q1[im][0] += 1.;
          q1[im][1] += 1./eff;
          q2[im][0] += 1.;
          q2[im][1] += 1./TMath::Power(eff, 2.);

          hNsigmaTPC[im]->Fill(c->fCent, std::abs(k->at(iK).fPt), k->at(iK).fNsigmaTPC);
          hNsigmaTOF[im]->Fill(c->fCent, std::abs(k->at(iK).fPt), k->at(iK).fNsigmaTOF);
        }
      }

      for (UInt_t iXi = 0; iXi < xi->size(); ++iXi){
        #ifdef CLOSURE_TEST
          if (std::abs(xi->at(iXi).fPtMC) > kXiUpPtCut || std::abs(xi->at(iXi).fPtMC) < kXiLowPtCut || std::abs(xi->at(iXi).fEtaMC) > kEtaCut) continue;
          int im_MC = xi->at(iXi).fPtMC > 0;

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
          int ie = hEtaTmp.FindBin(xi->at(iXi).fEta);
          #ifdef CLOSURE_TEST
            hRecXi[im]->Fill(c->fCent, std::abs(xi->at(iXi).fPt));
          #endif // CLOSURE_TEST
          double eff = fEffXi ? hEffXi[im][ic - 1][ie - 1][iS]->GetBinContent(hEffXi[im][ic - 1][ie - 1][iS]->FindBin(std::abs(xi->at(iXi).fPt))) : kDummyEffXi;
          // std::cout << "pt = " << std::abs(xi->at(iXi).fPt) << "; bin = " << hEffXi[im][ic - 1]->FindBin(std::abs(xi->at(iXi).fPt)) << "eff = " << eff << std::endl;
          hMass[im]->Fill(c->fCent, std::abs(xi->at(iXi).fPt), xi->at(iXi).fMass);
        }
      }

      for (int iM = 0; iM < 2; ++iM){
        for (int iCorr = 0; iCorr < 2; ++iCorr){
          q1sq[iM][iCorr] = pow(q1[iM][iCorr], 2.);
          //if (iM == 0 && iCorr == 1)
          // std::cout << "q1[" << iM << "][" << iCorr << "] = " << q1[iM][iCorr] << ", bin " << ic_sm << " content = " << hKaonC1[iM][iCorr][iS]->GetBinContent(ic_sm) << std::endl;
          hKaonC1[iM][iCorr][iS]->Fill(c->fCent, q1[iM][iCorr]);
          hKaonC2[iM][iCorr][iS]->SetBinContent(ic_sm, hKaonC2[iM][iCorr][iS]->GetBinContent(ic_sm) + q1sq[iM][iCorr] + q1[iM][iCorr] - q2[iM][iCorr]);
        }
      }
    }

    for (int iM = 0; iM < 2; ++iM){
      for (int iCorr = 0; iCorr < 2; ++iCorr){
        for (int iB = 1; iB <= hKaonC1[iM][iCorr][iS]->GetNbinsX(); ++iB){
          if (hCent[iS]->GetBinContent(iB) > 1.e-10){
            //hKaonC1[iM][iCorr][iS]->SetBinContent(iB, hKaonC1[iM][iCorr][iS]->GetBinContent(iB) / hCent[iS]->GetBinContent(iB));
            hKaonC2[iM][iCorr][iS]->SetBinContent(iB, hKaonC2[iM][iCorr][iS]->GetBinContent(iB) / hCent[iS]->GetBinContent(iB) - pow(hKaonC1[iM][iCorr][iS]->GetBinContent(iB), 2.));
          }
        }
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
    hCent[iS]->Write();
    for (int iM = 0; iM < 2; ++iM){
      for (int iCorr = 0; iCorr < 2; ++iCorr){
        hKaonC1[iM][iCorr][iS]->Write();
        hKaonC2[iM][iCorr][iS]->Write();
      }
    }
  }

  o.cd();
  #ifdef CLOSURE_TEST
    for (int iM = 0; iM < 2; ++iM){
      hGenKaon[iM]->Write();
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

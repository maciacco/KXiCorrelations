#include "../utils/Config.h"
#include "../utils/Utils.h"

using namespace utils;

void KaonEff(const char* inFileName = "oo", const char* outFileName = "KaonEff"){
  TFile *fMC = TFile::Open(Form("%s.root", inFileName));
  TFile *fOut = TFile::Open(Form("%s.root", outFileName), "recreate");
  for (int iM = 0; iM < 2; ++iM){
    // TTList *l = (TTList*)fMC->Get("nuclei_kaon_mcTrue_");
    // auto hGenKaon = (TH2D*)l->Get(Form("f%sTotal", kAntiMatterLabel[iM]));
    // auto hRecKaon = (TH2D*)l->Get(Form("f%sITS_TPC", kAntiMatterLabel[iM]));
    auto hGenKaon = (TH2D*)fMC->Get(Form("h%sGenKaon", kAntiMatterLabel[iM]));
    auto hRecKaon = (TH2D*)fMC->Get(Form("h%sRecKaon", kAntiMatterLabel[iM]));
    for (int iC = 0; iC < kNCentBins; ++iC){
      auto hGenProj = (TH1D*)hGenKaon->ProjectionY(Form("h%sGenProj_%d", kAntiMatterLabel[iM], iC), iC + 1, iC + 1);
      auto hRecProj = (TH1D*)hRecKaon->ProjectionY(Form("h%sRecProj_%d", kAntiMatterLabel[iM], iC), iC + 1, iC + 1);
      TH1D hEff(*hRecProj);
      hEff.SetName(Form("h%sEff_%d", kAntiMatterLabel[iM], iC));
      hEff.Divide(hRecProj, hGenProj, 1, 1, "B");
      fOut->cd();
      hEff.Write();
      hGenProj->Write();
      hRecProj->Write();
    }
  }
  fMC->Close();
  fOut->Close();
}
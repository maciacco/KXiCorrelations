#include "../utils/Config.h"
#include "../utils/Utils.h"

using namespace utils;

void KaonXiEff(const char* inFileName = "oo_limit_2ad6", const char* outFileName = "KaonXiEff_21d6_1"){
  TFile *fMC = TFile::Open(Form("%s.root", inFileName));
  TFile *fOut = TFile::Open(Form("%s.root", outFileName), "recreate");
  for (int iS = 0; iS < N_SAMPLE; ++iS){
    fOut->mkdir(Form("subsample_%d", iS + 1));
    fOut->cd(Form("subsample_%d", iS + 1));
    for (int iP = 0; iP < 2; ++iP){
      for (int iM = 0; iM < 2; ++iM){
        // TTList *l = (TTList*)fMC->Get("nuclei_kaon_mcTrue_");
        // auto hGenKaon = (TH2D*)l->Get(Form("f%sTotal", kAntiMatterLabel[iM]));
        // auto hRecKaon = (TH2D*)l->Get(Form("f%sITS_TPC", kAntiMatterLabel[iM]));
        auto hGen = (TH3D*)fMC->Get(Form("subsample_%d/h%sGen%s", iS + 1, kAntiMatterLabel[iM], kPartLabelExtend[iP]));
        auto hRec = (TH3D*)fMC->Get(Form("subsample_%d/h%sRec%s", iS + 1, kAntiMatterLabel[iM], kPartLabelExtend[iP]));
        for (int iC = 0; iC < kNCentBins; ++iC){
          for (int iE = 0; iE < kNEtaBins; ++iE){
            auto hGenProj = (TH1D*)hGen->ProjectionZ(Form("h%sGen%sProj_%d_%d", kAntiMatterLabel[iM], kPartLabel[iP], iC, iE), iC + 1, iC + 1, iE + 1, iE + 1);
            auto hRecProj = (TH1D*)hRec->ProjectionZ(Form("h%sRec%sProj_%d_%d", kAntiMatterLabel[iM], kPartLabel[iP], iC, iE), iC + 1, iC + 1, iE + 1, iE + 1);
            TH1D hEff(*hRecProj);
            hEff.SetName(Form("h%sEff%s_%d_%d", kAntiMatterLabel[iM], kPartLabel[iP], iC, iE));
            hEff.Divide(hRecProj, hGenProj, 1, 1, "B");
            fOut->cd();
            fOut->cd(Form("subsample_%d", iS + 1));
            hEff.Write();
            hGenProj->Write();
            hRecProj->Write();
          }
        }
      }
    }
  }
  fMC->Close();
  fOut->Close();
}
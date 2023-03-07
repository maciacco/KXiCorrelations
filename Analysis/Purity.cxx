#include "../utils/Config.h"

void Purity(const char* inFileName = "o_merge_parallel", const char* outFileName = "Purity"){
  gStyle->SetOptStat(0);
  
  TFile f(Form("%s.root", inFileName));
  TFile o(Form("%s.root", outFileName), "recreate");

  TH3F *hNsigmaTPC = (TH3F*)f.Get("hANsigmaTPC");
  TH3F *hNsigmaTOF = (TH3F*)f.Get("hANsigmaTOF");
  TH3F *hMass = (TH3F*)f.Get("hAMass");

  for (int iCent = 1; iCent < kNCentBins + 1; ++iCent){
    for (int iPart = 0; iPart < 1; ++iPart){
      for (int iP = 1; iP < 10; ++iP){
        TH1D* hNsigmaTPCProj = (TH1D*)hNsigmaTPC->ProjectionZ(Form("hNsigmaTPCProj_%d_%d", iCent, iP), iCent, iCent, iP, iP);
        TH1D* hNsigmaTOFProj = (TH1D*)hNsigmaTOF->ProjectionZ(Form("hNsigmaTOFProj_%d_%d", iCent, iP), iCent, iCent, iP, iP);
        TH1D* hMassProj = (TH1D*)hMass->ProjectionZ(Form("hMassProj_%d_%d", iCent, iP), iCent, iCent, iP, iP);
        TF1 fTOF("fTOF", "gaus(0) + [3]*TMath::Exp([4]*x)", -4., 4.);
        TF1 fTOFBkg("fTOFBkg", "[0]*TMath::Exp([1]*x)", -4., 4.);
        fTOF.SetParLimits(0, 0., 1.e6);
        fTOF.SetParLimits(1, -1., 1.);
        fTOF.SetParLimits(2, 0.005, 2.);
        fTOF.SetParLimits(3, 0., 1.e3);
        fTOF.SetParLimits(4, -10., 0.);
        hNsigmaTOFProj->Fit("fTOF", "MRL+", "", -4., 4.);
        fTOFBkg.SetParameter(0, fTOF.GetParameter(3));
        fTOFBkg.SetParameter(1, fTOF.GetParameter(4));
        fTOFBkg.Write();
        hNsigmaTPCProj->Write();
        hNsigmaTOFProj->Write();
        hMassProj->Write();
      }
    }
  }

  o.cd();
  hNsigmaTPC->Write();
  hNsigmaTOF->Write();
  hMass->Write();
  f.Close();
  o.Close();
}
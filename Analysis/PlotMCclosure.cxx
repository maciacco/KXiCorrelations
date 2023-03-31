#include "../utils/Config.h"

Color_t colors1[] = {kRed, kBlue};
const char* kCorrLabelUnderscore[] = {"", ""};

void PlotMCclosure(const char* inFileName = "outMC_LHC20e3", const char* outFileName = "mcClosure_LHC0e3"){
  TFile in(Form("%s.root", inFileName));
  TFile out(Form("%s.root", outFileName), "recreate");
  for (int iM = 0; iM < 2; iM++){
    for (int iC = 0; iC < 2; ++iC){
      for (int iP = 0; iP < 2; ++iP){
        TH1D* h[2];
        TCanvas c(Form("h%s%s%s", kAntiMatterLabel[iM], kAvgVarLabel[iC], kPartLabel[iP]), "c");
        for (int iGR = 0; iGR < 2; ++iGR){
          h[iGR] = (TH1D*)in.Get(Form("h%s%s%sCentEffCorrCBWC_%s%s", kAntiMatterLabel[iM], kAvgVarLabel[iC], kPartLabel[iP], kCorrLabelUnderscore[iGR], kCorrLabel[iGR == 0 ? 2 : 1]));
          if (iGR == 0){
            h[iGR]->GetYaxis()->SetTitle(Form("C_{%d}(%s)", iC + 1, kParticleAntiparticleTextAM[iP][iM]));
          }
          h[iGR]->SetLineColor(colors1[iGR]);
          h[iGR]->SetMarkerColor(colors1[iGR]);
          c.cd();
          h[iGR]->Draw(iGR == 0 ? "" : "same");
        }
        out.cd();
        c.Write();
      }
    }
  }

  for (int iP = 0; iP < 2; ++iP){
    TH1D* h[2];
    TCanvas c(Form("hNet%s%s", "Var", kPartLabel[iP]), "c");
    for (int iGR = 0; iGR < 2; ++iGR){
      h[iGR] = (TH1D*)in.Get(Form("hNet%s%sCentEffCorrCBWC_%s%s", "Var", kPartLabel[iP], kCorrLabelUnderscore[iGR], kCorrLabel[iGR == 0 ? 2 : 1]));
      if (iGR == 0){
        h[iGR]->GetYaxis()->SetTitle(Form("C_{2}(#Delta%s)", kPartLabel[iP]));
      }
      h[iGR]->SetLineColor(colors1[iGR]);
      h[iGR]->SetMarkerColor(colors1[iGR]);
      c.cd();
      h[iGR]->Draw(iGR == 0 ? "" : "same");
    }
    out.cd();
    c.Write();
  }

  in.Close();
  out.Close();
}
#include "../utils/Config.h"

Color_t colors1[] = {kRed, kBlue};
const char* kCorrLabelUnderscore[] = {"", ""};
constexpr const char* kPartHash[] = {"", "#"};

void PlotCumulants(const char* inFileName = "out_pp_full", const char* outFileName = "plot_res_pp"){
  gStyle->SetOptStat(0);

  // corrected cumulants
  TFile in(Form("%s.root", inFileName));
  TFile out(Form("%s.root", outFileName), "recreate");
  for (int iM = 0; iM < 2; iM++){
    for (int iC = 0; iC < 2; ++iC){
      for (int iP = 0; iP < 2; ++iP){
        TH1D* h[2];
        TCanvas c(Form("h%s%s%s", kAntiMatterLabel[iM], kAvgVarLabel[iC], kPartLabel[iP]), "c", 500, 600);
        TCanvas cRatio(Form("h%s%s%s_Ratio", kAntiMatterLabel[iM], kAvgVarLabel[iC], kPartLabel[iP]), "c", 500, 600);
        TLegend leg(0.5, 0.6, 0.8, 0.8);
        leg.SetTextFont(44);
        leg.SetTextSize(25);
        for (int iGR = 0; iGR < 2; ++iGR){
          h[iGR] = (TH1D*)in.Get(Form("h%s%s%sCentEffCorrCBWC_%s%s", kAntiMatterLabel[iM], kAvgVarLabel[iC], kPartLabel[iP], kCorrLabelUnderscore[iGR], kCorrLabel[iGR == 0 ? 1 : 0]));
          if (iGR == 0){
            h[iGR]->GetYaxis()->SetTitle(Form("C_{%d}(%s)", iC + 1, kParticleAntiparticleTextAM[iP][iM]));
          }
          h[iGR]->GetYaxis()->SetRangeUser(0., iP == 0 ? 2. : .02);
          h[iGR]->SetLineWidth(2);
          h[iGR]->SetMarkerStyle(20);
          h[iGR]->SetMarkerSize(1.1);
          h[iGR]->GetXaxis()->SetTitle("V0M Multiplicity Percentile (%)");
          h[iGR]->SetLineColor(colors1[iGR]);
          h[iGR]->SetMarkerColor(colors1[iGR]);
          c.cd();
          h[iGR]->Draw(iGR == 0 ? "" : "same");
          leg.AddEntry(h[iGR], iGR == 0 ? "corr." : "raw");
        }
        out.cd();
        leg.Draw("same");
        c.Write();
        c.Print(Form("hData%s%s%sCentEffCorrCBWC.pdf", kAntiMatterLabel[iM], kAvgVarLabel[iC], kPartLabel[iP]));
      }
    }
  }

  // net variance
  for (int iP = 0; iP < 2; ++iP){
    TH1D* h[2];
    TCanvas c(Form("hNet%s%s", "Var", kPartLabel[iP]), "c", 500, 600);
    TCanvas cRatio(Form("h%s%s_Ratio", "Var", kPartLabel[iP]), "c", 500, 600);
    TLegend leg(0.5, 0.6, 0.8, 0.8);
    leg.SetTextFont(44);
    leg.SetTextSize(25);
    for (int iGR = 0; iGR < 2; ++iGR){
      h[iGR] = (TH1D*)in.Get(Form("hNet%s%sCentEffCorrCBWC_%s%s", "Var", kPartLabel[iP], kCorrLabelUnderscore[iGR], kCorrLabel[iGR == 0 ? 1 : 0]));
      if (iGR == 0){
        h[iGR]->GetYaxis()->SetTitle(Form("C_{2}(#Delta%s%s)", kPartHash[iP], kPartLabel[iP]));
      }
      h[iGR]->GetYaxis()->SetRangeUser(0., iP == 0 ? 2. : .02);
      h[iGR]->SetLineWidth(2);
      h[iGR]->SetMarkerStyle(20);
      h[iGR]->SetMarkerSize(1.1);
      h[iGR]->GetXaxis()->SetTitle("V0M Multiplicity Percentile (%)");
      h[iGR]->SetLineColor(colors1[iGR]);
      h[iGR]->SetMarkerColor(colors1[iGR]);
      c.cd();
      h[iGR]->Draw(iGR == 0 ? "" : "same");
      leg.AddEntry(h[iGR], iGR == 0 ? "corr." : "raw");
    }
    out.cd();
    leg.Draw("same");
    c.Print(Form("hDataNet%s%sCentEffCorrCBWC.pdf", "Var", kPartLabel[iP]));
    c.Write();
  }

  // net correlation
  TH1D* h[2];
  TCanvas c("hNetCorrelation", "c", 500, 600);
  TCanvas cRatio("h%NetCorrelation_Ratio", "c", 500, 600);
  TLegend leg(0.5, 0.65, 0.8, 0.85);
  leg.SetTextFont(44);
  leg.SetTextSize(25);
  for (int iGR = 0; iGR < 2; ++iGR){
    h[iGR] = (TH1D*)in.Get(Form("hOppCorrCentEffCorrCBWC_%s%s", kCorrLabelUnderscore[iGR], kCorrLabel[iGR == 0 ? 1 : 0]));
    if (iGR == 0){
      h[iGR]->GetYaxis()->SetTitle(Form("#rho_{#Delta%s%s #Delta%s%s}", kPartHash[1], kPartLabel[1], kPartHash[0], kPartLabel[0]));
    }
    h[iGR]->GetYaxis()->SetRangeUser(-0.06, 0.03);
    h[iGR]->SetLineWidth(2);
    h[iGR]->SetMarkerStyle(20);
    h[iGR]->SetMarkerSize(1.1);
    h[iGR]->SetLineColor(colors1[iGR]);
    h[iGR]->GetXaxis()->SetTitle("V0M Multiplicity Percentile (%)");
    h[iGR]->SetMarkerColor(colors1[iGR]);
    c.cd();
    h[iGR]->Draw(iGR == 0 ? "" : "same");
    leg.AddEntry(h[iGR], iGR == 0 ? "corr." : "raw");
  }
  out.cd();
  leg.Draw("same");
  c.Print("hDataOppCorrCentEffCorrCBWC.pdf");
  c.Write();


  in.Close();
  out.Close();
}
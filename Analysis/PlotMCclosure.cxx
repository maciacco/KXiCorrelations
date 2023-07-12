#include "../utils/Config.h"

Color_t colors1[] = {kRed, kBlue};
const char* kCorrLabelUnderscore[] = {"", ""};
constexpr const char* kPartHash[] = {"", "#"};

void PlotMCclosure(const char* inFileName = "mcClosure_pPb", const char* outFileName = "mcClosure_pPb_plots"){
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
          h[iGR] = (TH1D*)in.Get(Form("h%s%s%sCentEffCorrCBWC_%s%s", kAntiMatterLabel[iM], kAvgVarLabel[iC], kPartLabel[iP], kCorrLabelUnderscore[iGR], kCorrLabel[iGR == 0 ? 2 : 1]));
          if (iGR == 0){
            h[iGR]->GetYaxis()->SetTitle(Form("C_{%d}(%s)", iC + 1, kParticleAntiparticleTextAM[iP][iM]));
          }
          h[iGR]->GetYaxis()->SetRangeUser(0., iP == 0 ? 110. : 1.2);
          h[iGR]->SetLineWidth(2);
          h[iGR]->SetMarkerStyle(20);
          h[iGR]->SetMarkerSize(1.1);
          h[iGR]->SetLineColor(colors1[iGR]);
          h[iGR]->SetMarkerColor(colors1[iGR]);
          c.cd();
          h[iGR]->Draw(iGR == 0 ? "" : "same");
          leg.AddEntry(h[iGR], iGR == 0 ? "gen." : "rec");
        }
        out.cd();
        leg.Draw("same");
        c.Write();
        c.Print(Form("h%s%s%sCentEffCorrCBWC.pdf", kAntiMatterLabel[iM], kAvgVarLabel[iC], kPartLabel[iP]));
        
        TH1D hh(*h[0]);
        hh.SetName(Form("h%s%s%sCentEffCorrCBWC_ratio", kAntiMatterLabel[iM], kAvgVarLabel[iC], kPartLabel[iP]));
        hh.SetTitle(";V0M Multiplicity Percentile (%);gen / rec");
        hh.Divide(h[0], h[1]);
        hh.Fit("pol0");
        //hh.Write();
        cRatio.cd();
        hh.Draw();
        cRatio.Write();
        cRatio.Print(Form("h%s%s%sCentEffCorrCBWC_Ratio.pdf", kAntiMatterLabel[iM], kAvgVarLabel[iC], kPartLabel[iP]));
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
      h[iGR] = (TH1D*)in.Get(Form("hNet%s%sCentEffCorrCBWC_%s%s", "Var", kPartLabel[iP], kCorrLabelUnderscore[iGR], kCorrLabel[iGR == 0 ? 2 : 1]));
      if (iGR == 0){
        h[iGR]->GetYaxis()->SetTitle(Form("C_{2}(#Delta%s%s)", kPartHash[iP], kPartLabel[iP]));
      }
      h[iGR]->GetYaxis()->SetRangeUser(0., iP == 0 ? 150. : 2.);
      h[iGR]->SetLineWidth(2);
      h[iGR]->SetMarkerStyle(20);
      h[iGR]->SetMarkerSize(1.1);
      h[iGR]->SetLineColor(colors1[iGR]);
      h[iGR]->SetMarkerColor(colors1[iGR]);
      c.cd();
      h[iGR]->Draw(iGR == 0 ? "" : "same");
      leg.AddEntry(h[iGR], iGR == 0 ? "gen." : "rec");
    }
    out.cd();
    leg.Draw("same");
    c.Print(Form("hNet%s%sCentEffCorrCBWC.pdf", "Var", kPartLabel[iP]));
    c.Write();

    TH1D hh(*h[0]);
    hh.SetName(Form("h%s%sCentEffCorrCBWC_ratio", "Var", kPartLabel[iP]));
    hh.SetTitle(";V0M Multiplicity Percentile (%);gen / rec");
    hh.Divide(h[0], h[1]);
    hh.Fit("pol0");
    //hh.Write();
    cRatio.cd();
    hh.Draw();
    cRatio.Write();
    cRatio.Print(Form("h%s%sCentEffCorrCBWC_Ratio.pdf", "Var", kPartLabel[iP]));
  }

  // net correlation
  TH1D* h[2];
  TCanvas c("hNetCorrelation", "c", 500, 600);
  TCanvas cRatio("h%NetCorrelation_Ratio", "c", 500, 600);
  TLegend leg(0.5, 0.6, 0.8, 0.8);
  leg.SetTextFont(44);
  leg.SetTextSize(25);
  for (int iGR = 0; iGR < 2; ++iGR){
    h[iGR] = (TH1D*)in.Get(Form("hNetCorrCentEffCorrCBWC_%s%s", kCorrLabelUnderscore[iGR], kCorrLabel[iGR == 0 ? 2 : 1]));
    if (iGR == 0){
      h[iGR]->GetYaxis()->SetTitle(Form("#rho_{#Delta%s%s #Delta%s%s}", kPartHash[1], kPartLabel[1], kPartHash[0], kPartLabel[0]));
    }
    h[iGR]->GetYaxis()->SetRangeUser(-0.05, 0.);
    h[iGR]->SetLineWidth(2);
    h[iGR]->SetMarkerStyle(20);
    h[iGR]->SetMarkerSize(1.1);
    h[iGR]->SetLineColor(colors1[iGR]);
    h[iGR]->SetMarkerColor(colors1[iGR]);
    c.cd();
    h[iGR]->Draw(iGR == 0 ? "" : "same");
    leg.AddEntry(h[iGR], iGR == 0 ? "gen." : "rec");
  }
  out.cd();
  leg.Draw("same");
  c.Print("hNetCorrCentEffCorrCBWC.pdf");
  c.Write();

  TH1D hh(*h[0]);
  hh.SetName("hNetCorrCentEffCorrCBWC_ratio");
  hh.SetTitle(";V0M Multiplicity Percentile (%);gen - rec");
  hh.Add(h[0], h[1], 1., -1.);
  hh.Fit("pol0");
  hh.GetYaxis()->SetRangeUser(-0.02,0.02);
  //hh.Write();
  cRatio.cd();
  hh.Draw();
  cRatio.Write();
  cRatio.Print("hNetCorrCentEffCorrCBWC_Ratio.pdf");

  in.Close();
  out.Close();
}
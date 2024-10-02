
constexpr double mult[] = {1947.6828, 1578.5132, 1180, 786, 512, 318, 183, 96.3};
constexpr double multSHM[] = {1735.1, 1180, 786, 512, 318, 183, 96.3, 44.9};
constexpr double value[]={0.9379,0.9392,0.9397,0.9365,0.9336,0.9290,0.9271,0.9262};
constexpr double valueError[]={0.013,0.014,0.01,0.006,0.008,0.008,0.01,0.01};
constexpr double valueSHM[]={0.9409,0.941537,0.94335,0.941024,0.938725,0.93725,0.9354,0.933};
constexpr double valueSHMError[]={0.0011,0.00145,0.00127,0.00085,0.00155,0.00132,0.0008,0.0013};

void Plot()
{
  gStyle->SetOptStat(0);

  TH2D frame("frame", ";#LTd#it{N}_{ch}/d#it{#eta}#GT;#it{#kappa}_{2}(p - #bar{p})/#it{#kappa}_{1}(p + #bar{p})", 1, 75, 2500, 1, 0.9, 1.01);

  double mult_err[8] = {0};
  double multSHM_err[8] = {0};
  for (int i{0}; i < 8; ++i){
    mult_err[i] = mult[i] * 7.e-2;
    multSHM_err[i] = multSHM[i] * 7.e-2;
  }
  TGraphErrors gData(8, mult, value, mult_err, valueError);
  TGraphErrors gModel(7, multSHM, valueSHM, multSHM_err, valueSHMError);

  gData.SetLineWidth(2);
  gData.SetFillStyle(0);
  gData.SetMarkerStyle(20);
  gData.SetMarkerSize(1.);
  gData.SetLineColor(kBlue);
  gData.SetMarkerColor(kBlue);

  gModel.SetLineWidth(2);
  gModel.SetLineColor(kOrange - 3);
  gModel.SetFillColor(kOrange - 3);
  gModel.SetFillStyle(3002);

  TLine line(75., 1., 2500., 1.);
  line.SetLineWidth(2);
  line.SetLineStyle(kDashed);

  TLegend leg(0.17, 0.65, 0.5, 0.85);
  leg.SetTextFont(44);
  leg.SetTextSize(29);
  leg.AddEntry(&gData, "#splitline{ALICE, Pb#minusPb, #sqrt{#it{s}_{NN}} = 5.02 TeV, |#it{#eta}| < 0.8}{Phys. Lett. B 844 (2023) 137545}", "pe");
  leg.AddEntry(&gModel, "#splitline{TheFIST CE SHM}{#it{T}_{chem} = 155 MeV, #it{V}_{c} = 3 d#it{V}/d#it{y}}", "lf");
  TLatex t;
  t.SetTextFont(44);
  t.SetTextSize(29);
  t.SetNDC();

  TCanvas c("cProton", "cProton", 800, 800);
  c.cd();
  c.SetLogx();
  frame.GetYaxis()->SetNdivisions(210);
  frame.GetYaxis()->SetTitleOffset(1.45);
  c.SetLeftMargin(0.15);
  c.SetRightMargin(0.02);
  c.SetTopMargin(0.03);
  frame.Draw("");
  line.Draw("same");
  leg.Draw("same");
  t.DrawLatex(0.255, 0.59 /* 0.935 */, "0.6 < #it{p} < 1.5 GeV/#it{c}");
  gModel.Draw("samee3l");
  gData.Draw("samepe5");

  c.Print("cProton.pdf");
}

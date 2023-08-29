constexpr bool fullRange = true;

void CombineChi2(){

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

  TGaxis::SetMaxDigits(3);

  TFile f(fullRange ? "final_plot_chi2_combined.root" : "final_plot_chi2_combined_zoom.root", "recreate");
  TFile f1("final_plot_c2c1_volumes.root");
  TFile f2("final_plot_rho_volumes.root");
  // double vols_bin[] = {0.55, 1.45, 1.55, 2.45, 2.55, 2.65, 2.95, 3.05, 3.35, 3.65, 4.35};
  // TH1D h("h", ";#it{V}_{c} (d#it{V}/d#it{y});#chi^{2};", 10, vols_bin);
  TH1D h("h", ";#it{V}_{c} (d#it{V}/d#it{y});#chi^{2};", 7, 0.75, 4.25);
  TGraphErrors *g1 = (TGraphErrors*)f1.Get(";2");
  TGraphErrors *g2 = (TGraphErrors*)f2.Get(";2");
  
  // double vols[] = {1., 1.5, 2., 2.5, 2.6, 2.8, 3., 3.2, 3.5, 4.};
  double vols[] = {1., 1.5, 2., 2.5, 3., 3.5, 4.};
  for (int i{0}; i < g1->GetN(); ++i){
    g1->SetPointY(i, g1->GetPointY(i) + g2->GetPointY(i));
    g1->SetPointError(i, 0., sqrt(g1->GetPointY(i))/* sqrt(g1->GetErrorY(i) * g1->GetErrorY(i) + g2->GetErrorY(i) * g2->GetErrorY(i)) */);
    h.SetBinContent(i + 1, g1->GetPointY(i));
    //h.SetBinError(i + 1, g1->GetErrorY(i));
  }
  TF1 profile("profile",  /* "pol4" */ "[0]+[1]*TMath::Power(x-[2],2)+[3]*TMath::Power(x-[2],4)" /*+[3]*x*x*x*/);// "[0]+[1]*TMath::Power(x-[2],2)+[3]*TMath::Power(x-[2],4);"
  h.Fit("profile", "N", "", 2., 4.5);
  h.Fit("profile", "RM+", "", 0.5, 4.5);
  double minX = profile.GetMinimumX(1., 4., 1.e-4, 1000000.);
  double min = profile.GetMinimum(1., 4., 1.e-4, 1000000.);
  double left = profile.GetX(min + 1, 1., minX, 1.e-4, 1000000.);
  double right = profile.GetX(min + 1, minX, 4., 1.e-4, 1000000.);
  std::cout << "V_c = " << minX << " +/- " << 0.5 * (right - left) << "; chi2_min = " << min << std::endl;
  TCanvas c("c", "c", 500, 500);
  c.SetTopMargin(0.06);
  c.SetRightMargin(0.04);
  c.cd();
  h.SetMarkerStyle(21);
  h.SetMarkerSize(1.4);
  h.SetMarkerColor(kBlue);
  h.SetLineColor(kBlue);
  h.SetLineWidth(2);
  h.GetFunction("profile")->SetLineStyle(7);
  h.GetFunction("profile")->SetLineWidth(2);
  h.Draw("psx0");
  h.GetXaxis()->SetRangeUser(fullRange ? 0.85 : 2.25, 4.25);
  h.GetYaxis()->SetRangeUser(10, fullRange ? 1050 : 55);
  //g1->Draw("aplk");
  TLatex t;
  t.SetTextFont(44);
  t.SetTextSize(20);
  t.SetNDC();
  t.DrawLatex(fullRange ? 0.5 : 0.35, 0.85, "ALICE Preliminary");
  t.DrawLatex(fullRange ? 0.5 : 0.35, 0.78, "Pb#minusPb, #sqrt{#it{s}_{NN}} = 5.02 TeV");
  TLegend leg(fullRange ? 0.5 : 0.62, fullRange ? 0.6 : 0.14, fullRange ? 0.8 : 0.93, fullRange ? 0.74 : 0.28);
  //t.SetNDC(false);
  leg.SetTextFont(44);
  leg.SetTextSize(20);
  leg.AddEntry(&h, "Observed #chi^{2}", "p");
  leg.AddEntry(h.GetFunction("profile"), "Fit", "l");
  leg.Draw("same");
  if (!fullRange){
    t.DrawLatex(0.35, 0.69, Form("#it{V}_{c} = %.2f #pm %.2f d#it{V}/d#it{y}", minX, 0.5 * (right - left)));
    t.DrawLatex(0.35, 0.62, Form("#chi^{2}/ndf = %.2f / 12", min));
    t.DrawLatex(0.35, 0.55, Form("Probability = %.2f", TMath::Prob(min, 12)));
  }
  f.cd();
  c.Write();
  c.Print(fullRange ? "combineChi2.eps"/* .pdf" */ : "combineChi2_zoom.eps"/* .pdf" */);
  f.Close();
  f1.Close();
  f2.Close();
}
void Chi2Inset(){
  TGaxis::SetMaxDigits(3);
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);
  TFile f1("final_plot_chi2_combined_test.root");
  TFile f2("final_plot_chi2_combined_zoom_test.root");
  TFile fout("final_chi2_inset.root", "recreate");  
  TCanvas c0("c0", "c0", 500, 500);
  auto c = (TCanvas*)f1.Get("c");
  c->SetName("c1");
  auto c1 = (TCanvas*)f2.Get("c");
  c1->SetName("c2");
  TPad pad0("pad0", "pad0", 0., 0., 1., 1., 0);
  TPad pad1("pad1", "pad1", 0.38, 0.22, 0.88, 0.63, 0);
  c0.cd();
  
  pad0.Draw();
  pad0.cd();
  pad0.SetTopMargin(0.06);
  pad0.SetRightMargin(0.04);
  TH1D* fr = (TH1D*)c->GetPrimitive("frame");
  fr->SetName("frame1");
  TGraphErrors* g = (TGraphErrors*)c->GetPrimitive("chi2");
  g->SetName("chi2_1");
  //g->SetMarkerSize(0.8);
  fr->Draw("");
  
  pad1.Draw();
  pad1.cd();
  pad1.SetLeftMargin(0.21);
  pad1.SetBottomMargin(0.21);
  pad1.SetTopMargin(0.04);
  pad1.SetRightMargin(0.04);
  TH1D* fr1 = (TH1D*)c1->GetPrimitive("frame");
  TGraphErrors* g1 = (TGraphErrors*)c1->GetPrimitive("chi2");
  fr1->GetYaxis()->SetTitleFont(44);
  fr1->GetYaxis()->SetTitleSize(20);
  fr1->GetYaxis()->SetLabelFont(44);
  fr1->GetYaxis()->SetLabelSize(15);
  fr1->GetXaxis()->SetTitleFont(44);
  fr1->GetXaxis()->SetTitleSize(20);
  fr1->GetXaxis()->SetLabelFont(44);
  fr1->GetXaxis()->SetLabelSize(15);
  fr1->GetYaxis()->SetTitleOffset(1.);
  fr1->GetXaxis()->SetTitleOffset(1.);
  fr1->GetXaxis()->SetNdivisions(505);
  fr1->GetYaxis()->SetNdivisions(505);
  fr1->Draw("");
  //g1->SetMarkerSize(0.8);
  g1->Draw("psame");

  pad0.cd();

  g->Draw("psame");

  TLatex t;
  t.SetTextFont(44);
  t.SetTextSize(20);
  t.SetNDC();
  t.DrawLatex(0.38, 0.87, "ALICE, Pb#minusPb, #sqrt{#it{s}_{NN}} = 5.02 TeV");

  TF1 *profile = (TF1*)g->GetFunction("profile");
  double minX = profile->GetMinimumX(1., 4., 1.e-4, 1000000.);
  double min = profile->GetMinimum(1., 4., 1.e-4, 1000000.);
  double left = profile->GetX(min + 1, 1., minX, 1.e-4, 1000000.);
  double right = profile->GetX(min + 1, minX, 4., 1.e-4, 1000000.);
  t.DrawLatex(0.47, 0.8, Form("#it{V}_{c} = %.2f #pm %.2f d#it{V}/d#it{y}", minX, 0.5 * (right - left)));

  TLegend leg(0.5, 0.65, 0.8, 0.77);
  //t.SetNDC(false);
  leg.SetTextFont(44);
  leg.SetTextSize(20);
  leg.AddEntry(g, "Data-model #chi^{2}", "p");
  leg.AddEntry(g->GetFunction("profile"), "Fit", "l");
  leg.Draw("same");

  fout.cd();

  fout.cd();
  c0.Write();
  c0.Print("cChi2Inset.pdf");
}
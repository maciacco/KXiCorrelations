void MCwwoTruth(){
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  TFile f1("test_mc20e3_mcTruth_1.root");
  TFile f2("test_mc20e3_nomcTruth.root");
  TH1D *gen = (TH1D*)f1.Get("hNetCorrCentEffCorrCBWC_Gen");
  gen->GetFunction("pol0")->Delete();
  TH1D *rec_truth = (TH1D*)f1.Get("hNetCorrCentEffCorrCBWC_Corr");
  rec_truth->SetName("rec_truth");
  TH1D *rec_notruth = (TH1D*)f2.Get("hNetCorrCentEffCorrCBWC_Corr");
  rec_notruth->SetName("rec_no_truth");

  gen->GetYaxis()->SetTitle("#it{#rho}_{#DeltaK#Delta#Xi}");
  gen->GetXaxis()->SetTitle("Centrality (%)");
  gen->SetLineWidth(2);
  gen->SetMarkerStyle(20);
  gen->SetMarkerSize(0.8);
  gen->SetMarkerColor(kRed);
  gen->SetLineColor(kRed);

  rec_truth->SetLineWidth(2);
  rec_truth->SetMarkerStyle(20);
  rec_truth->SetMarkerSize(0.8);
  rec_truth->SetMarkerColor(kBlue);
  rec_truth->SetLineColor(kBlue);
  rec_truth->GetFunction("pol0")->SetLineStyle(7);
  rec_truth->GetFunction("pol0")->SetLineColor(kBlue);

  rec_notruth->SetLineWidth(2);
  rec_notruth->SetMarkerStyle(20);
  rec_notruth->SetMarkerSize(0.8);
  rec_notruth->SetMarkerColor(kGreen + 2);
  rec_notruth->SetLineColor(kGreen + 2);
  rec_notruth->GetFunction("pol0")->SetLineStyle(7);
  rec_notruth->GetFunction("pol0")->SetLineColor(kGreen + 2);

  TCanvas c("c", "c", 500, 500);
  c.SetTopMargin(0.05);
  c.SetRightMargin(0.05);
  c.cd();
  gen->Draw();
  gen->GetYaxis()->SetRangeUser(-0.14, 0.14);
  rec_truth->Draw("same");
  rec_notruth->Draw("same");

  TLegend l(0.45, 0.78, 0.8, 0.92);
  l.SetTextFont(44);
  l.SetTextSize(15);
  l.AddEntry(gen, "gen.", "pe");
  l.AddEntry(rec_truth, "rec. (w/ MC truth)", "pe");
  l.AddEntry(rec_notruth, "rec. (w/o MC truth)", "pe");
  l.Draw("same");

  double pol0_1 = rec_truth->GetFunction("pol0")->GetParameter(0);
  double pol0_1_err = rec_truth->GetFunction("pol0")->GetParError(0);
  double pol0_2 = rec_notruth->GetFunction("pol0")->GetParameter(0);
  double pol0_2_err = rec_notruth->GetFunction("pol0")->GetParError(0);
  TLegend l2(0.15, 0.15, 0.5, 0.25);
  l2.SetTextFont(44);
  l2.SetTextSize(15);
  l2.AddEntry(rec_truth->GetFunction("pol0"), Form("p_{0} = %.4f #pm %.4f", rec_truth->GetFunction("pol0")->GetParameter(0), rec_truth->GetFunction("pol0")->GetParError(0)), "l");
  l2.AddEntry(rec_notruth->GetFunction("pol0"), Form("p_{0} = %.4f #pm %.4f", rec_notruth->GetFunction("pol0")->GetParameter(0), rec_notruth->GetFunction("pol0")->GetParError(0)), "l");
  l2.Draw("same");

  TLatex t;
  t.SetTextFont(44);
  t.SetTextSize(15);
  t.DrawLatex(50, -0.125, Form("Barlow check = %.3f", (pol0_2 - pol0_1) / sqrt(abs(pol0_1_err * pol0_1_err - pol0_2_err * pol0_2_err))));

  c.Print("cMcwwotruth.pdf");
  f1.Close();
  f2.Close();
}
void scale_contamination(){
  double avg_purity_k = 0.95;
  double avg_purity_xi = 0.95;
  TFile fXi("Analysis/sidebands_test_PbPb_2018.root");
  TFile fK("Analysis/test_sidebands_PbPb_2018_noK.root");
  TFile fMult("Analysis/out_18qr_postCalib.root");
  auto hXi = (TH1D*)fXi.Get("hNetCorrCentEffCorrCBWC_Corr");
  auto hK = (TH1D*)fK.Get("hNetCorrCentEffCorrCBWC_Corr");
  auto hKMult = (TH1D*)fMult.Get("hNetAvgKCentEffCorrCBWC_Corr");
  auto hXiMult = (TH1D*)fMult.Get("hNetAvgKCentEffCorrCBWC_Corr");
  TH1D hRhoCont(*hK);
  for (int i{1}; i < hRhoCont.GetNbinsX() + 1; ++i){
    double kMult = hKMult->GetBinContent(i);
    double xiMult = hXiMult->GetBinContent(i);
    double kRho = hK->GetBinContent(i);
    double kRho_err = hK->GetBinError(i);
    double xiRho = hXi->GetBinContent(i);
    double xiRho_err = hXi->GetBinError(i);
    hRhoCont.SetBinContent(i, (kMult * (1. - avg_purity_k) * kRho + xiMult * (1. - avg_purity_xi) * xiRho) / (kMult + xiMult));
    hRhoCont.SetBinError(i, sqrt((kMult * (1. - avg_purity_k) * kRho_err * kMult * (1. - avg_purity_k) * kRho_err + xiMult * (1. - avg_purity_xi) * xiRho_err * xiMult * (1. - avg_purity_xi) * xiRho_err) / (kMult + xiMult) / (kMult + xiMult)) );
  }
  hRhoCont.Fit("pol0");
  TFile fo("scale_contamination_PbPb.root", "recreate");
  fo.cd();
  hRhoCont.Write();
  fo.Close();
}
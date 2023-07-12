void average_contamination(){
  TFile fo("average_contamination.root", "recreate");

  TFile f18("Analysis/sidebands_test_PbPb_2018.root");
  TFile f15("Analysis/sidebands_test_PbPb_2015.root");

  TH1D* h18 = (TH1D*)f18.Get("hNetCorrCentEffCorrCBWC_Corr");
  TH1D* h15 = (TH1D*)f15.Get("hNetCorrCentEffCorrCBWC_Corr");
  TH1D* htot = new TH1D(*h18);

  for (int i{1}; i < h18->GetNbinsX() + 1; ++i){
    double x = h18->GetBinContent(i);
    double y = h15->GetBinContent(i);
    double errx = h18->GetBinError(i);
    double erry = h15->GetBinError(i);
    double avg = ( x / errx + y / erry ) / (1. / errx + 1. / erry);
    double avg_err = sqrt(1. / (1. / errx / errx + 1. / erry / erry));
    htot->SetBinContent(i, avg);
    htot->SetBinError(i, avg_err);
  }

  htot->Fit("pol0");

  fo.cd();
  htot->Write();

  f18.Close();
  f15.Close();

  fo.Close();
}
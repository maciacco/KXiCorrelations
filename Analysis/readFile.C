void readFile(const char *fname = ""){
  TFile in(fname);
  TH1D* h = (TH1D*)in.Get("hrho_net");
  for (int i{1}; i <= h->GetNbinsX(); ++i){
    std::cout << "{" << h->GetBinContent(i) << ", " << h->GetBinError(i) << "}, ";
  }
  std::cout << " --- " << std::endl;
  TH1D* hh = (TH1D*)in.Get("hmult");
  for (int i{1}; i <= hh->GetNbinsX(); ++i){
    std::cout << hh->GetBinContent(i) << ", ";
  }
  in.Close();
}
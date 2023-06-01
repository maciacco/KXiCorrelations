#include "../utils/Config.h"

void BDTEffMap(const char* inFileName = "results/oo_limit_lhc22l5_postCalib_1", const char* outFileName = "bdt_eff_map_lhc22l5_postCalib"){
  TFile f(Form("%s.root", inFileName));
  auto h = (TH3D*)f.Get("subsample_1_var_7/hABDTOutXi");
  TFile o(Form("%s.root", outFileName), "recreate");

  TH1D *hEffBDT[8];
  for (int iP = 1; iP < 8; ++iP){
    TH1D* hh = h->ProjectionZ("projBDT", 1, 10, iP, iP);
    hEffBDT[iP] = new TH1D(Form("hEffBDT_%d", iP), Form("hEffBDT_%d", iP), 1000, 0, 1);

    double integral_tot = hh->Integral(hh->FindBin(0.), hh->FindBin(1.));
    for (int iB = 1; iB < hEffBDT[iP]->GetNbinsX() + 1; iB++){
      double integral = hh->Integral(iB, hh->FindBin(1.));
      //std::cout << "integral (0.98) = " << integral << " -> BDT efficiency = " << integral / integral_tot << std::endl;
      hEffBDT[iP]->SetBinContent(iB, integral / integral_tot);
    }

    o.cd();
    hEffBDT[iP]->Write();
  }

  f.Close();
  o.Close();
}
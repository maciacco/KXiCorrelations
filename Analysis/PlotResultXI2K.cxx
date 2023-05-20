#include "../utils/Config.h"

void PlotResultXI2K(){
  gStyle->SetOptStat(0);

  TFile fpp("out_pp_afterCalib.root"); // 17pq
  TFile f("out_15o_postCalib.root"); // 15o
  TFile f2("out_18qr_postCalib.root"); // 18qr
  auto h_c1a_xi = (TH1D*)f.Get("hMAvgXiCentEffCorrCBWC_Corr");
  auto h_c1m_xi = (TH1D*)f.Get("hAAvgXiCentEffCorrCBWC_Corr");
  auto h2_c1a_xi = (TH1D*)f2.Get("hMAvgXiCentEffCorrCBWC_Corr");
  auto h2_c1m_xi = (TH1D*)f2.Get("hAAvgXiCentEffCorrCBWC_Corr");
  auto hpp_c1a_xi = (TH1D*)fpp.Get("hMAvgXiCentEffCorrCBWC_Corr");
  auto hpp_c1m_xi = (TH1D*)fpp.Get("hAAvgXiCentEffCorrCBWC_Corr");
  auto h_c1a_kaon = (TH1D*)f.Get("hAAvgKCentEffCorrCBWC_Corr");
  auto h_c1m_kaon = (TH1D*)f.Get("hMAvgKCentEffCorrCBWC_Corr");
  auto h2_c1a_kaon = (TH1D*)f2.Get("hAAvgKCentEffCorrCBWC_Corr");
  auto h2_c1m_kaon = (TH1D*)f2.Get("hMAvgKCentEffCorrCBWC_Corr");
  auto hpp_c1a_kaon = (TH1D*)fpp.Get("hAAvgKCentEffCorrCBWC_Corr");
  auto hpp_c1m_kaon = (TH1D*)fpp.Get("hMAvgKCentEffCorrCBWC_Corr");
  TCanvas cResult("cResult", "cResult", 800, 800);
  TH2D frame("frame", ";#LTd#it{N}_{ch}/d#eta#GT;C_{1}(#Xi^{-})/C_{1}(K^{-})", 1, 1, 2500, 1, 0., .1);
  TLegend leg(0.16395348837209303, 0.650399548902823, 0.7395348837209302, 0.9167587400421383);

  TGraphErrors gData;
  TGraphErrors gData_pp;
  TGraphErrors gSHM;
  TGraphErrors gPYTHIA;
  TGraphErrors gPYTHIA_CRMPI_ROPOFF;
  TGraphErrors gPYTHIA_CRMPI_ROPON;
  TGraphErrors gPYTHIA_CRQCD;

  h_c1a_xi->Divide(h_c1m_kaon);
  h2_c1a_xi->Divide(h2_c1m_kaon);
  hpp_c1a_xi->Divide(hpp_c1m_kaon);

  // canvas
  cResult.SetLogx();
  frame.GetYaxis()->SetNdivisions(210);
  frame.GetYaxis()->SetTitleOffset(1.2);
  cResult.SetLeftMargin(0.13);
  cResult.SetRightMargin(0.02);
  cResult.SetTopMargin(0.05);

  for (int iB = 1; iB < h2_c1a_xi->GetNbinsX(); ++iB){
    double x = h_c1a_xi->GetBinContent(iB);
    double y = h2_c1a_xi->GetBinContent(iB);
    double errx = h_c1a_xi->GetBinError(iB);
    double erry = h2_c1a_xi->GetBinError(iB);
    gData.AddPoint(mult_shm_PbPb[iB - 1], (x/pow(errx, 2) + y/pow(erry, 2)) / (1./pow(errx, 2) + 1./pow(erry, 2)));
    gData.SetPointError(iB - 1, 0, 1. / sqrt((1./pow(errx, 2) + 1./pow(erry, 2))));
  }

  for (int iB = 1; iB < hpp_c1a_xi->GetNbinsX() + 1; ++iB){
    double y = hpp_c1a_xi->GetBinContent(iB);
    double erry = hpp_c1a_xi->GetBinError(iB);
    gData_pp.AddPoint(mult_pp[iB - 1], y);
    gData_pp.SetPointError(iB - 1, 0, erry);
  }

  // for (int iP = 0; iP < kNPointsPbPb + kNPointspp - 1; ++iP){
  //   int index = kNPointsPbPb - iP - 2 + kNPointspp;
  //   gSHM.AddPoint(iP < kNPointspp - 1 ? mult_shm_pp[iP] : mult_shm_PbPb[index], iP < kNPointspp - 1 ? shm_pp_c2c1[iP][0] : shm_PbPb_c2c1[index][0]);
  //   gSHM.SetPointError(iP, 0, iP < kNPointspp - 1 ? shm_pp_c2c1[iP][1] : shm_PbPb_c2c1[index][1]);
  // }
  // for (int iP = 0; iP < kNPointspp - 2; ++iP){
  //   gPYTHIA.AddPoint(mult_pythia_c2c1[iP], pythia_c2c1[iP][0]);
  //   gPYTHIA.SetPointError(iP, 0, pythia_c2c1[iP][1]);
  // }

  // for (int iP = 0; iP < kNPointspp - 2; ++iP){
  //   gPYTHIA_CRQCD.AddPoint(mult_pythia_c2c1[iP], pythia_crqcd_c2c1[iP][0]);
  //   gPYTHIA_CRQCD.SetPointError(iP, 0, pythia_crqcd_c2c1[iP][1]);
  // }

  // for (int iP = 0; iP < kNPointspp - 2; ++iP){
  //   gPYTHIA_CRMPI_ROPON.AddPoint(mult_pythia_c2c1[iP], pythia_crmpi_ropon_c2c1[iP][0]);
  //   gPYTHIA_CRMPI_ROPON.SetPointError(iP, 0, pythia_crmpi_ropon_c2c1[iP][1]);
  // }

  // for (int iP = 0; iP < kNPointspp - 2; ++iP){
  //   gPYTHIA_CRMPI_ROPOFF.AddPoint(mult_pythia_c2c1[iP], pythia_crmpi_ropoff_c2c1[iP][0]);
  //   gPYTHIA_CRMPI_ROPOFF.SetPointError(iP, 0, pythia_crmpi_ropoff_c2c1[iP][1]);
  // }

  // set colors and style
  gData.SetLineWidth(1);
  gData.SetMarkerStyle(20);
  gData.SetMarkerSize(1.2);
  gData.SetLineColor(kRed);
  gData.SetMarkerColor(kRed);

  gData_pp.SetLineWidth(1);
  gData_pp.SetMarkerStyle(21);
  gData_pp.SetMarkerSize(1.2);
  gData_pp.SetLineColor(kRed);
  gData_pp.SetMarkerColor(kRed);

  gSHM.SetLineWidth(1);
  gSHM.SetLineColor(kBlue + 1);
  gSHM.SetFillColor(kBlue + 1);
  gSHM.SetFillStyle(3254);

  gPYTHIA.SetLineWidth(2);
  gPYTHIA.SetLineColor(kOrange);
  gPYTHIA.SetFillColor(kOrange);
  gPYTHIA.SetFillStyle(3254);
  //gPYTHIA.SetLineStyle(kDashed);

  gPYTHIA_CRQCD.SetLineWidth(2);
  gPYTHIA_CRQCD.SetLineColor(kOrange+1);
  gPYTHIA_CRQCD.SetFillColor(kOrange+1);
  gPYTHIA_CRQCD.SetFillStyle(3254);

  gPYTHIA_CRMPI_ROPOFF.SetLineWidth(2);
  gPYTHIA_CRMPI_ROPOFF.SetLineColor(kOrange+2);
  gPYTHIA_CRMPI_ROPOFF.SetFillColor(kOrange+2);
  gPYTHIA_CRMPI_ROPOFF.SetFillStyle(3254);

  gPYTHIA_CRMPI_ROPON.SetLineWidth(2);
  gPYTHIA_CRMPI_ROPON.SetLineColor(kOrange+3);
  gPYTHIA_CRMPI_ROPON.SetFillColor(kOrange+3);
  gPYTHIA_CRMPI_ROPON.SetFillStyle(3254);

  // legend
  leg.SetTextFont(44);
  leg.SetTextSize(25);
  //leg.SetNColumns(2);
  leg.AddEntry(&gData, "ALICE Work in Progress, Pb-Pb #sqrt{#it{s}_{NN}}=5.02 TeV", "pe");
  leg.AddEntry(&gData_pp, "ALICE Work in Progress, pp #sqrt{#it{s}}=5.02 TeV", "pe");
  // leg.AddEntry(&gSHM, "Thermal-FIST: #gamma_{s} CSM, 3 d#it{V}/d#it{y}", "f");
  // leg.AddEntry(&gPYTHIA, "PYTHIA, pp #sqrt{#it{s}_{NN}}=13 TeV");

  cResult.cd();
  frame.Draw();
  gData.Draw("pesame");
  gData_pp.Draw("pesame");
  // gSHM.Draw("samee3l");
  // gPYTHIA.Draw("samee3l");
  // gPYTHIA_CRQCD.Draw("samee3l");
  // gPYTHIA_CRMPI_ROPON.Draw("samee3l");
  // gPYTHIA_CRMPI_ROPOFF.Draw("samee3l");
  leg.Draw("same");
  TFile o("out_merge_parallel_avg_y.root", "recreate");
  o.cd();
  gData.Write();
  cResult.Write();
  cResult.Print("c1.pdf");
  o.Close();
  f.Close();
  f2.Close();
}
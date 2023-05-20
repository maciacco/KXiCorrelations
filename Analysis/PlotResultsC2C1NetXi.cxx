#include "../utils/Config.h"

void PlotResultsC2C1NetXi(){
  gStyle->SetOptStat(0);

  TFile fpp("out_pp_afterCalib.root"); // 17pq
  TFile f("out_15o_postCalib.root"); // 15o
  TFile f2("out_18qr_postCalib.root"); // 18qr
  auto h = (TH1D*)f.Get("hNetVarXiCentEffCorrCBWC_Corr");
  auto h_c1a = (TH1D*)f.Get("hAAvgXiCentEffCorrCBWC_Corr");
  auto h_c1m = (TH1D*)f.Get("hMAvgXiCentEffCorrCBWC_Corr");
  auto h2 = (TH1D*)f2.Get("hNetVarXiCentEffCorrCBWC_Corr");
  auto h2_c1a = (TH1D*)f2.Get("hAAvgXiCentEffCorrCBWC_Corr");
  auto h2_c1m = (TH1D*)f2.Get("hMAvgXiCentEffCorrCBWC_Corr");
  auto hpp = (TH1D*)fpp.Get("hNetVarXiCentEffCorrCBWC_Corr");
  auto hpp_c1a = (TH1D*)fpp.Get("hAAvgXiCentEffCorrCBWC_Corr");
  auto hpp_c1m = (TH1D*)fpp.Get("hMAvgXiCentEffCorrCBWC_Corr");
  TCanvas cResult("cResult", "cResult", 800, 800);
  TH2D frame("frame", ";#LTd#it{N}_{ch}/d#eta#GT;C_{2}(#Delta#Xi)/(C_{1}(#Xi^{+})+C_{1}(#Xi^{-}))", 1, 1, 2500, 1, 0.72, 1.06);
  //TLegend leg(0.16395348837209303, 0.60399548902823, 0.7395348837209302, 0.9167587400421383);
  TLegend leg(0.17115054378935318, 0.16399002075195312, 0.546994848311391, 0.57681724588284313);

  TGraphErrors gData;
  TGraphErrors gData_pp;
  TGraphErrors gSHM;
  TGraphErrors gPYTHIA;
  TGraphErrors gPYTHIA_CRMPI_ROPOFF;
  TGraphErrors gPYTHIA_CRMPI_ROPON;
  TGraphErrors gPYTHIA_CRQCD;
  TGraphErrors gPYTHIA_ANGANTYR;
  TGraphErrors gHIJING;

  h_c1a->Add(h_c1m);
  h->Divide(h_c1a);
  h2_c1a->Add(h2_c1m);
  h2->Divide(h2_c1a);
  hpp_c1a->Add(hpp_c1m);
  hpp->Divide(hpp_c1a);

  // canvas
  cResult.SetLogx();
  frame.GetYaxis()->SetNdivisions(210);
  frame.GetYaxis()->SetTitleOffset(1.2);
  cResult.SetLeftMargin(0.13);
  cResult.SetRightMargin(0.02);
  cResult.SetTopMargin(0.05);

  for (int iB = 1; iB < h2->GetNbinsX(); ++iB){
    double x = h->GetBinContent(iB);
    double y = h2->GetBinContent(iB);
    double errx = h->GetBinError(iB);
    double erry = h2->GetBinError(iB);
    gData.AddPoint(mult_shm_PbPb[iB - 1], (/* x/pow(errx, 2) +  */y/pow(erry, 2)) / (/* 1./pow(errx, 2) + */ 1./pow(erry, 2)));
    gData.SetPointError(iB - 1, 0, 1. / sqrt((/* 1./pow(errx, 2) + */ 1./pow(erry, 2))));
  }

  for (int iB = 1; iB < hpp->GetNbinsX() + 1; ++iB){
    double y = hpp->GetBinContent(iB);
    double erry = hpp->GetBinError(iB);
    gData_pp.AddPoint(mult_pp[iB - 1], y);
    gData_pp.SetPointError(iB - 1, 0, erry);
  }

  for (int iP = 0; iP < kNPointsPbPb + kNPointspp - 1; ++iP){
    int index = kNPointsPbPb - iP - 2 + kNPointspp;
    gSHM.AddPoint(iP < kNPointspp - 1 ? mult_shm_pp[iP] : mult_shm_PbPb[index], iP < kNPointspp - 1 ? shm_pp_c2c1[iP][0] : shm_PbPb_c2c1[index][0]);
    gSHM.SetPointError(iP, 0, iP < kNPointspp - 1 ? shm_pp_c2c1[iP][1] : shm_PbPb_c2c1[index][1]);
  }

  for (int iP = 0; iP < kNPointsPbPb; ++iP){
    gHIJING.AddPoint(mult_shm_PbPb[iP], hijing_c2c1[iP][0]);
    gHIJING.SetPointError(iP, 0, hijing_c2c1[iP][1]);
  }

  // for (int iP = 0; iP < kNPointspp - 2; ++iP){
  //   gPYTHIA.AddPoint(mult_pythia_c2c1[iP], pythia_c2c1[iP][0]);
  //   gPYTHIA.SetPointError(iP, 0, pythia_c2c1[iP][1]);
  // }

  for (int iP = 0; iP < kNPointspp - 2; ++iP){
    gPYTHIA_CRQCD.AddPoint(mult_shm_pp_[iP], pythia_crqcd_c2c1[iP][0]);
    gPYTHIA_CRQCD.SetPointError(iP, 0, pythia_crqcd_c2c1[iP][1]);
  }

  for (int iP = 0; iP < kNPointspp - 2; ++iP){
    gPYTHIA_CRMPI_ROPON.AddPoint(mult_shm_pp_[iP], pythia_crmpi_ropon_c2c1[iP][0]);
    gPYTHIA_CRMPI_ROPON.SetPointError(iP, 0, pythia_crmpi_ropon_c2c1[iP][1]);
  }

  for (int iP = 0; iP < kNPointspp - 2; ++iP){
    gPYTHIA_CRMPI_ROPOFF.AddPoint(mult_shm_pp_[iP], pythia_crmpi_ropoff_c2c1[iP][0]);
    gPYTHIA_CRMPI_ROPOFF.SetPointError(iP, 0, pythia_crmpi_ropoff_c2c1[iP][1]);
  }

  for (int iP = 0; iP < 4; ++iP){
    gPYTHIA_ANGANTYR.AddPoint(mult_pythia_angantyr[iP], pythia_angantyr_c2c1[iP][0]);
    gPYTHIA_ANGANTYR.SetPointError(iP, 0, pythia_angantyr_c2c1[iP][1]);
  }

  // set colors and style
  gData.SetLineWidth(2);
  gData.SetMarkerStyle(20);
  gData.SetMarkerSize(1.2);
  gData.SetLineColor(kRed);
  gData.SetMarkerColor(kRed);

  gData_pp.SetLineWidth(2);
  gData_pp.SetMarkerStyle(21);
  gData_pp.SetMarkerSize(1.2);
  gData_pp.SetLineColor(kRed);
  gData_pp.SetMarkerColor(kRed);

  gSHM.SetLineWidth(2);
  gSHM.SetLineColor(kBlue + 1);
  gSHM.SetFillColor(kBlue + 1);
  gSHM.SetFillStyle(3254);

  gHIJING.SetLineWidth(2);
  gHIJING.SetLineColor(kMagenta);
  gHIJING.SetFillColor(kMagenta);
  gHIJING.SetFillStyle(3254);

  gPYTHIA_ANGANTYR.SetLineWidth(2);
  gPYTHIA_ANGANTYR.SetLineColor(kOrange);
  gPYTHIA_ANGANTYR.SetFillColor(kOrange);
  gPYTHIA_ANGANTYR.SetFillStyle(3254);
  //gPYTHIA.SetLineStyle(kDashed);

  gPYTHIA_CRQCD.SetLineWidth(2);
  gPYTHIA_CRQCD.SetLineColor(kGreen+2);
  gPYTHIA_CRQCD.SetFillColor(kGreen+2);
  gPYTHIA_CRQCD.SetFillStyle(3254);

  gPYTHIA_CRMPI_ROPOFF.SetLineWidth(2);
  gPYTHIA_CRMPI_ROPOFF.SetLineColor(kOrange+7);
  gPYTHIA_CRMPI_ROPOFF.SetFillColor(kOrange+7);
  gPYTHIA_CRMPI_ROPOFF.SetFillStyle(3254);

  gPYTHIA_CRMPI_ROPON.SetLineWidth(2);
  gPYTHIA_CRMPI_ROPON.SetLineColor(kAzure+2);
  gPYTHIA_CRMPI_ROPON.SetFillColor(kAzure+2);
  gPYTHIA_CRMPI_ROPON.SetFillStyle(3254);

  // legend
  leg.SetTextFont(44);
  leg.SetTextSize(25);
  //leg.SetNColumns(2);
  leg.AddEntry(&gData, "ALICE WIP, Pb-Pb #sqrt{#it{s}_{NN}}=5.02 TeV", "pe");
  leg.AddEntry(&gData_pp, "ALICE WIP, pp #sqrt{#it{s}}=5.02 TeV", "pe");
  leg.AddEntry(&gSHM, "Thermal-FIST: #gamma_{s} CSM, 3 d#it{V}/d#it{y}", "f");
  leg.AddEntry(&gHIJING, "HIJING, Pb-Pb #sqrt{#it{s}_{NN}}=5 TeV", "f");
  leg.AddEntry(&gPYTHIA_ANGANTYR, "PYTHIA Angantyr, Pb-Pb #sqrt{#it{s}_{NN}}=5 TeV", "f");
  leg.AddEntry(&gPYTHIA_CRQCD, "PYTHIA, CRQCD, pp #sqrt{#it{s}_{NN}}=5 TeV", "f");
  leg.AddEntry(&gPYTHIA_CRMPI_ROPOFF, "PYTHIA, CRMPI, rope off, pp #sqrt{#it{s}_{NN}}=5 TeV", "f");
  leg.AddEntry(&gPYTHIA_CRMPI_ROPON, "PYTHIA, CRMPI, rope on, pp #sqrt{#it{s}_{NN}}=5 TeV", "f");

  cResult.cd();
  frame.Draw();
  gSHM.Draw("samee3l");
  // gPYTHIA.Draw("samee3l");
  gHIJING.Draw("samee3l");
  gPYTHIA_ANGANTYR.Draw("samee3l");
  gPYTHIA_CRQCD.Draw("samee3l");
  gPYTHIA_CRMPI_ROPON.Draw("samee3l");
  gPYTHIA_CRMPI_ROPOFF.Draw("samee3l");
  gData.Draw("pesame");
  gData_pp.Draw("pesame");
  leg.Draw("same");
  TFile o("out_merge_parallel_avg_2.root", "recreate");
  o.cd();
  gData.Write();
  cResult.Write();
  cResult.Print("c.pdf");
  o.Close();
  f.Close();
  f2.Close();
}
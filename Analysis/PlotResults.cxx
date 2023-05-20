#include "../utils/Config.h"

void PlotResults(){
  gStyle->SetOptStat(0);

  TFile fpp("out_pp_afterCalib.root"); // 17pq
  TFile f("out_15o_postCalib.root"); // 15o
  TFile f2("out_18qr_postCalib.root"); // 18qr
  auto h = (TH1D*)f.Get("hNetCorrCentEffCorrCBWC_Corr");
  auto h2 = (TH1D*)f2.Get("hNetCorrCentEffCorrCBWC_Corr");
  auto hpp = (TH1D*)fpp.Get("hNetCorrCentEffCorrCBWC_Corr");
  TCanvas cResult("cResult", "cResult", 800, 800);
  TH2D frame("frame", ";#LTd#it{N}_{ch}/d#eta#GT;#rho_{#Delta#Xi #DeltaK}", 1, 1, 2500, 1, -0.05, 0.025);
  TLegend leg(0.16395348837209303, 0.650399548902823, 0.7395348837209302, 0.9167587400421383);
  // TGraphErrors gSHM_PbPb;
  // TGraphErrors gSHM_pp;
  TGraphErrors gSHM;
  TGraph gPYTHIA;
  TGraphErrors gHIJING;
  TGraphErrors gData;
  TGraphErrors gData_pp;

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
    gData.AddPoint(mult_shm_PbPb[iB - 1], (x/pow(errx, 2) + y/pow(erry, 2)) / (1./pow(errx, 2) + 1./pow(erry, 2)));
    gData.SetPointError(iB - 1, 0, 1. / sqrt((1./pow(errx, 2) + 1./pow(erry, 2))));
  }
  gData.Fit("pol0", "R", "", 50, 2000);

  for (int iB = 1; iB < hpp->GetNbinsX() + 1; ++iB){
    double y = hpp->GetBinContent(iB);
    double erry = hpp->GetBinError(iB);
    gData_pp.AddPoint(mult_pp[iB - 1], y);
    gData_pp.SetPointError(iB - 1, 0, erry);
  }

  for (int iP = 0; iP < kNPointsPbPb + kNPointspp - 1; ++iP){
    int index = kNPointsPbPb - iP - 2 + kNPointspp;
    gSHM.AddPoint(iP < kNPointspp - 1 ? mult_shm_pp[iP] : mult_shm_PbPb[index], iP < kNPointspp - 1 ? shm_pp[iP][0] : shm_PbPb[index][0]);
    gSHM.SetPointError(iP, 0, iP < kNPointspp - 1 ? shm_pp[iP][1] : shm_PbPb[index][1]);
  }
  for (int iP = 0; iP < kNPointsPbPb; ++iP){
    // gSHM_PbPb.AddPoint(mult_shm_PbPb[iP], shm_PbPb[iP][0]);
    // gSHM_PbPb.SetPointError(iP, 0, shm_PbPb[iP][1]);
    gHIJING.AddPoint(mult_shm_PbPb[iP], hijing[iP][0]);
    gHIJING.SetPointError(iP, 0, hijing[iP][1]);
  }
  for (int iP = 0; iP < kNPointspp; ++iP){
    // gSHM_pp.AddPoint(mult_shm_pp[iP], shm_pp[iP][0]);
    // gSHM_pp.SetPointError(iP, 0, shm_pp[iP][1]);
    gPYTHIA.AddPoint(mult_pythia[iP], pythia[iP]);
  }

  for (int iP = 0; iP < kNPointspp - 2; ++iP){
    gPYTHIA_CRQCD.AddPoint(mult_pythia_netCorr[iP], pythia_crqcd_netCorr[iP][0]);
    gPYTHIA_CRQCD.SetPointError(iP, 0, pythia_crqcd_netCorr[iP][1]);
  }

  for (int iP = 0; iP < kNPointspp - 2; ++iP){
    gPYTHIA_CRMPI_ROPON.AddPoint(mult_pythia_netCorr[iP], pythia_crmpi_ropon_netCorr[iP][0]);
    gPYTHIA_CRMPI_ROPON.SetPointError(iP, 0, pythia_crmpi_ropon_netCorr[iP][1]);
  }

  for (int iP = 0; iP < kNPointspp - 2; ++iP){
    gPYTHIA_CRMPI_ROPOFF.AddPoint(mult_pythia_netCorr[iP], pythia_crmpi_ropoff_netCorr[iP][0]);
    gPYTHIA_CRMPI_ROPOFF.SetPointError(iP, 0, pythia_crmpi_ropoff_netCorr[iP][1]);
  }

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

  // gSHM_PbPb.SetLineWidth(1);
  // gSHM_PbPb.SetLineColor(kBlue + 1);
  // gSHM_PbPb.SetFillColor(kBlue + 1);
  // gSHM_PbPb.SetFillStyle(3254);

  // gSHM_pp.SetLineWidth(1);
  // gSHM_pp.SetFillColor(kAzure + 7);
  // gSHM_pp.SetFillStyle(3245);
  // gSHM_pp.SetLineColor(kAzure + 7);
  //gSHM_pp.SetLineStyle(kDashed);

  gHIJING.SetLineWidth(1);
  gHIJING.SetFillColor(kOrange+7);
  gHIJING.SetLineColor(kOrange+7);
  gHIJING.SetFillStyle(3254);

  gPYTHIA.SetLineWidth(2);
  gPYTHIA.SetLineColor(kOrange);
  //gPYTHIA.SetLineStyle(kDashed);

  // legend
  leg.SetTextFont(44);
  leg.SetTextSize(25);
  //leg.SetNColumns(2);
  leg.AddEntry(&gData, "ALICE Work in Progress, Pb-Pb #sqrt{#it{s}_{NN}}=5.02 TeV", "pe");
  leg.AddEntry(&gData_pp, "ALICE Work in Progress, pp #sqrt{#it{s}}=5.02 TeV", "pe");
  // leg.AddEntry(&gSHM_PbPb, "Thermal-FIST, 3 d#it{V}/d#it{y}, Pb-Pb #sqrt{#it{s}_{NN}}=5.02 TeV", "f");
  // leg.AddEntry(&gSHM_pp, "Thermal-FIST, 3 d#it{V}/d#it{y}, pp #sqrt{#it{s}_{NN}}=13 TeV", "f");
  leg.AddEntry(&gSHM, "Thermal-FIST: #gamma_{s} CSM, 3 d#it{V}/d#it{y}", "f");
  leg.AddEntry(&gHIJING, "HIJING, Pb-Pb #sqrt{#it{s}_{NN}}=5.02 TeV", "f");
  leg.AddEntry(&gPYTHIA, "PYTHIA, pp #sqrt{#it{s}_{NN}}=13 TeV");

  cResult.cd();
  frame.Draw();
  // gSHM_PbPb.Draw("samee3l");
  // gSHM_pp.Draw("samee3l");
  gSHM.Draw("samee3l");
  gPYTHIA.Draw("same");
  gHIJING.Draw("samee3l");
  gData.Draw("pesame");
  gData_pp.Draw("pesame");
  leg.Draw("same");
  TFile o("out_merge_parallel_avg.root", "recreate");
  o.cd();
  gData.Write();
  // gSHM_PbPb.Write();
  // gSHM_pp.Write();
  gPYTHIA.Write();
  cResult.Write();
  cResult.Print("c.pdf");
  o.Close();
  f.Close();
  f2.Close();
}
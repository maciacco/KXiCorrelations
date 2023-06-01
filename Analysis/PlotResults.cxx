#include "../utils/Config.h"

void PlotResults(){
  gStyle->SetOptStat(0);

  TFile fpp("out_prova_sys_17_new.root"); // 17pq
  TFile f("out_sys_PbPb_new.root"); // 15o+18qr
  auto gpp_stat = (TGraphErrors*)fpp.Get("gRho");
  auto gpp_sys = (TGraphErrors*)fpp.Get("gRhoSys");
  gpp_stat->SetName("gRhoStat_pp");
  gpp_sys->SetName("gRhoSys_pp");
  auto gPbPb_stat = (TGraphErrors*)f.Get("gRho");
  auto gPbPb_sys = (TGraphErrors*)f.Get("gRhoSys");
  gPbPb_stat->SetName("gRhoStat_PbPb");
  gPbPb_sys->SetName("gRhoSys_PbPb");

  TCanvas cResult("cResult", "cResult", 800, 800);
  TH2D frame("frame", ";#LTd#it{N}_{ch}/d#eta#GT;#rho_{#Delta#Xi #DeltaK}", 1, 1, 2500, 1, -0.07, 0.002);
  TLegend leg(0.17245327102803738, 0.15032415158745518, 0.495, 0.4962954684062296);
  //(0.16395348837209303, 0.650399548902823, 0.7395348837209302, 0.9167587400421383);

  TGraphErrors gSHM;
  TGraph gPYTHIA;
  TGraphErrors gPYTHIA_CRMPI_ROPOFF;
  TGraphErrors gPYTHIA_CRMPI_ROPON;
  TGraphErrors gPYTHIA_CRQCD;
  TGraphErrors gPYTHIA_ANGANTYR;
  TGraphErrors gHIJING;
  TGraphErrors gData;
  TGraphErrors gData_pp;

  // canvas
  cResult.SetLogx();
  frame.GetYaxis()->SetNdivisions(210);
  frame.GetYaxis()->SetTitleOffset(1.2);
  cResult.SetLeftMargin(0.13);
  cResult.SetRightMargin(0.02);
  cResult.SetTopMargin(0.03);

  for (int iB = 0; iB < 8; ++iB){
    gPbPb_sys->SetPointError(iB, mult_shm_PbPb[iB] * 5.e-2, gPbPb_sys->GetErrorY(iB));
    gPbPb_stat->SetPointX(iB, mult_shm_PbPb[iB]);
    gPbPb_sys->SetPointX(iB, mult_shm_PbPb[iB]);
  }
  //gPbPb_stat->Fit("pol0", "R", "", 40, 2000);

  for (int iB = 0; iB < 7; ++iB){
    gpp_sys->SetPointError(iB, mult_pp[iB] * 5.e-2, gpp_sys->GetErrorY(iB));
    gpp_stat->SetPointX(iB, mult_pp[iB]);
    gpp_sys->SetPointX(iB, mult_pp[iB]);
  }

  // for (int iB = 1; iB < h2->GetNbinsX(); ++iB){
  //   double x = h->GetBinContent(iB);
  //   double y = h2->GetBinContent(iB);
  //   double errx = h->GetBinError(iB);
  //   double erry = h2->GetBinError(iB);
  //   gData.AddPoint(mult_shm_PbPb[iB - 1], (x/pow(errx, 2) + y/pow(erry, 2)) / (1./pow(errx, 2) + 1./pow(erry, 2)));
  //   gData.SetPointError(iB - 1, 0, 1. / sqrt((1./pow(errx, 2) + 1./pow(erry, 2))));
  // }

  // for (int iB = 1; iB < hpp->GetNbinsX() + 1; ++iB){
  //   double y = hpp->GetBinContent(iB);
  //   double erry = hpp->GetBinError(iB);
  //   gData_pp.AddPoint(mult_pp[iB - 1], y);
  //   gData_pp.SetPointError(iB - 1, 0, erry);
  // }

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
    gPYTHIA_CRQCD.AddPoint(mult_pythia_[iP], pythia_crqcd_netCorr[iP][0]);
    gPYTHIA_CRQCD.SetPointError(iP, 0, pythia_crqcd_netCorr[iP][1]);
  }

  for (int iP = 0; iP < kNPointspp - 2; ++iP){
    gPYTHIA_CRMPI_ROPON.AddPoint(mult_pythia_[iP], pythia_crmpi_ropon_netCorr[iP][0]);
    gPYTHIA_CRMPI_ROPON.SetPointError(iP, 0, pythia_crmpi_ropon_netCorr[iP][1]);
  }

  for (int iP = 0; iP < kNPointspp - 2; ++iP){
    gPYTHIA_CRMPI_ROPOFF.AddPoint(mult_pythia_[iP], pythia_crmpi_ropoff_netCorr[iP][0]);
    gPYTHIA_CRMPI_ROPOFF.SetPointError(iP, 0, pythia_crmpi_ropoff_netCorr[iP][1]);
  }

  for (int iP = 0; iP < 4; ++iP){
    gPYTHIA_ANGANTYR.AddPoint(mult_pythia_angantyr[iP], pythia_angantyr_netCorr[iP][0]);
    gPYTHIA_ANGANTYR.SetPointError(iP, 0, pythia_angantyr_netCorr[iP][1]);
  }

  // set colors and style
  gPbPb_stat->SetLineWidth(2);
  gPbPb_stat->SetMarkerStyle(20);
  gPbPb_stat->SetMarkerSize(1.2);
  gPbPb_stat->SetLineColor(kRed);
  gPbPb_stat->SetMarkerColor(kRed);
  gPbPb_sys->SetLineWidth(2);
  gPbPb_sys->SetFillColor(41);
  gPbPb_sys->SetFillStyle(3010);
  gPbPb_sys->SetMarkerStyle(20);
  gPbPb_sys->SetMarkerSize(1.2);
  gPbPb_sys->SetLineColor(kRed);
  gPbPb_sys->SetMarkerColor(kRed);

  gpp_stat->SetLineWidth(2);
  gpp_stat->SetMarkerStyle(21);
  gpp_stat->SetMarkerSize(1.2);
  gpp_stat->SetLineColor(kRed);
  gpp_stat->SetMarkerColor(kRed);
  gpp_sys->SetLineWidth(2);
  gpp_sys->SetFillColor(41);
  gpp_sys->SetFillStyle(3010);
  gpp_sys->SetMarkerStyle(21);
  gpp_sys->SetMarkerSize(1.2);
  gpp_sys->SetLineColor(kRed);
  gpp_sys->SetMarkerColor(kRed);

  // gData_pp.SetLineWidth(1);
  // gData_pp.SetMarkerStyle(21);
  // gData_pp.SetMarkerSize(1.2);
  // gData_pp.SetLineColor(kRed);
  // gData_pp.SetMarkerColor(kRed);

  gSHM.SetLineWidth(2);
  gSHM.SetLineColor(kBlue + 1);
  gSHM.SetFillColor(kBlue + 1);
  gSHM.SetFillStyle(3015);

  // gSHM_PbPb.SetLineWidth(1);
  // gSHM_PbPb.SetLineColor(kBlue + 1);
  // gSHM_PbPb.SetFillColor(kBlue + 1);
  // gSHM_PbPb.SetFillStyle(3015);

  // gSHM_pp.SetLineWidth(1);
  // gSHM_pp.SetFillColor(kAzure + 7);
  // gSHM_pp.SetFillStyle(3245);
  // gSHM_pp.SetLineColor(kAzure + 7);
  //gSHM_pp.SetLineStyle(kDashed);

  gHIJING.SetLineWidth(2);
  gHIJING.SetFillColor(kMagenta);
  gHIJING.SetLineColor(kMagenta);
  gHIJING.SetFillStyle(3015);

  gPYTHIA_ANGANTYR.SetLineWidth(2);
  gPYTHIA_ANGANTYR.SetLineColor(kOrange);
  gPYTHIA_ANGANTYR.SetFillColor(kOrange);
  gPYTHIA_ANGANTYR.SetFillStyle(3015);
  //gPYTHIA.SetLineStyle(kDashed);

  gPYTHIA_CRQCD.SetLineWidth(2);
  gPYTHIA_CRQCD.SetLineColor(kGreen+2);
  gPYTHIA_CRQCD.SetFillColor(kGreen+2);
  gPYTHIA_CRQCD.SetFillStyle(3015);

  gPYTHIA_CRMPI_ROPOFF.SetLineWidth(2);
  gPYTHIA_CRMPI_ROPOFF.SetLineColor(kOrange+7);
  gPYTHIA_CRMPI_ROPOFF.SetFillColor(kOrange+7);
  gPYTHIA_CRMPI_ROPOFF.SetFillStyle(3015);

  gPYTHIA_CRMPI_ROPON.SetLineWidth(2);
  gPYTHIA_CRMPI_ROPON.SetLineColor(kAzure+2);
  gPYTHIA_CRMPI_ROPON.SetFillColor(kAzure+2);
  gPYTHIA_CRMPI_ROPON.SetFillStyle(3015);

  gPYTHIA.SetLineWidth(2);
  gPYTHIA.SetLineColor(kOrange);
  //gPYTHIA.SetLineStyle(kDashed);

  // legend
  leg.SetTextFont(44);
  leg.SetTextSize(25);
  //leg.SetNColumns(2);
  leg.AddEntry(gPbPb_stat, "ALICE Work in Progress, Pb-Pb #sqrt{#it{s}_{NN}}=5.02 TeV", "pe");
  leg.AddEntry(gpp_stat, "ALICE Work in Progress, pp #sqrt{#it{s}}=5.02 TeV", "pe");
  // leg.AddEntry(&gSHM_PbPb, "Thermal-FIST, 3 d#it{V}/d#it{y}, Pb-Pb #sqrt{#it{s}_{NN}}=5.02 TeV", "f");
  // leg.AddEntry(&gSHM_pp, "Thermal-FIST, 3 d#it{V}/d#it{y}, pp #sqrt{#it{s}_{NN}}=13 TeV", "f");
  leg.AddEntry(&gSHM, "Thermal-FIST: #gamma_{s} CSM, 3 d#it{V}/d#it{y}", "f");
  leg.AddEntry(&gHIJING, "HIJING, Pb-Pb #sqrt{#it{s}_{NN}}=5.02 TeV", "f");
  //leg.AddEntry(&gPYTHIA_ANGANTYR, "PYTHIA Angantyr, Pb-Pb #sqrt{#it{s}_{NN}}=5 TeV", "f");
  leg.AddEntry(&gPYTHIA_CRQCD, "PYTHIA, CRQCD, Rope Had., pp #sqrt{#it{s}_{NN}}=5 TeV", "f");
  leg.AddEntry(&gPYTHIA_CRMPI_ROPOFF, "PYTHIA, CRMPI, pp #sqrt{#it{s}_{NN}}=5 TeV", "f");
  leg.AddEntry(&gPYTHIA_CRMPI_ROPON, "PYTHIA, CRMPI, Rope Had., pp #sqrt{#it{s}_{NN}}=5 TeV", "f");
  //leg.AddEntry(&gPYTHIA, "PYTHIA, pp #sqrt{#it{s}_{NN}}=13 TeV");

  cResult.cd();
  frame.Draw();
  // gSHM_PbPb.Draw("samee3l");
  // gSHM_pp.Draw("samee3l");
  gSHM.Draw("samee3l");
  // gPYTHIA.Draw("same");
  gHIJING.Draw("samee3l");
  //gPYTHIA_ANGANTYR.Draw("samee3l");
  gPYTHIA_CRQCD.Draw("samee3l");
  gPYTHIA_CRMPI_ROPON.Draw("samee3l");
  gPYTHIA_CRMPI_ROPOFF.Draw("samee3l");

  gPbPb_sys->Draw("e5same");
  gpp_sys->Draw("e5same");
  gPbPb_stat->Draw("pesame");
  gpp_stat->Draw("pesame");

  leg.Draw("same");
  TFile o("out_merge_parallel_avg.root", "recreate");
  o.cd();

  gPYTHIA.Write();
  cResult.Write();
  cResult.Print("c.pdf");
  o.Close();
  f.Close();
  f.Close();
  fpp.Close();
}
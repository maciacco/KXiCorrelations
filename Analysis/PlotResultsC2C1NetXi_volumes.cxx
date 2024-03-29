#include "../utils/Config.h"
#include "../utils/Utils.h"

void PlotResultsC2C1NetXi_volumes(){
  gStyle->SetOptStat(0);

  TFile fpp("out_sys_17_finalBinning_c2c1.root"); // 17pq
  TFile fpPb("out_sys_16_finalBinning_c2c1.root"); // 17pq
  TFile f("out_sys_PbPb_finalBinning_c2c1.root"); // 15o+18qr
  // TFile fpp("out_pp_afterCalib.root"); // 17pq
  // TFile f("out_15o_postCalib.root"); // 15o
  // TFile f2("out_18qr_postCalib.root"); // 18qr
  auto gpp_stat = (TGraphErrors*)fpp.Get("gRho");
  auto gpp_sys = (TGraphErrors*)fpp.Get("gRhoSys");
  gpp_stat->SetName("gRhoStat_pp");
  gpp_sys->SetName("gRhoSys_pp");
  auto gpPb_stat = (TGraphErrors*)fpPb.Get("gRho");
  auto gpPb_sys = (TGraphErrors*)fpPb.Get("gRhoSys");
  gpPb_stat->SetName("gRhoStat_pPb");
  gpPb_sys->SetName("gRhoSys_pPb");
  auto gPbPb_stat = (TGraphErrors*)f.Get("gRho");
  auto gPbPb_sys = (TGraphErrors*)f.Get("gRhoSys");
  gPbPb_stat->SetName("gRhoStat_PbPb");
  gPbPb_sys->SetName("gRhoSys_PbPb");

  TFile fPythia("models/PYTHIA_5TEV_CRQCD_RopeOn.root");
  TFile fSHM_BS_1("models/Output_Final_BS_vc1.0_CE.root");
  TFile fSHM_BS_2("models/Output_Final_BS_vc2.0_CE.root");
  //TFile fSHM_BS_3("models/Output_Final_BS_vc3.0_CE.root");
  TFile fSHM_BS_4("models/Output_Final_BS_vc4.0_CE.root");
  TFile fSHM_BS_1_5("models/Output_Final_BS_vc1.5_CE.root");
  TFile fSHM_BS_2_5("models/Output_Final_BS_vc2.5_CE.root");
  TFile fSHM_BS_3_5("models/Output_Final_BS_vc3.5_CE.root");
  TFile fSHM_BS_2_6("models/Output_Final_BS_vc2.6_CE.root");
  TFile fSHM_BS_2_8("models/out_analysis_28.root");
  TFile fSHM_BS_2_9("models/out_analysis_29.root");
  TFile fSHM_BS_3("models/out_analysis_30.root");
  TFile fSHM_BS_3_1("models/out_analysis_31.root");
  TFile fSHM_BS_3_2("models/out_analysis_32.root");
  TFile fSHM_BS_3_3("models/out_analysis_33.root");
  TFile fSHM_BS_3_4("models/out_analysis_34.root");

  TCanvas cResult("cResult", "cResult", 800, 800);
  TH2D frame("frame", ";#LTd#it{N}_{ch}/d#it{#eta}#GT;#it{#kappa}_{2}(#bar{#Xi}^{+} - #Xi^{#minus})/#it{#kappa}_{1}(#bar{#Xi}^{+} + #Xi^{#minus})", 1, 30, 2500, 1, 0.94, 1.03);
  TLegend leg(0.18, 0.6925, 0.7, 0.88);
  //TLegend leg2(0.162, 0.8, 0.7, 0.85);
  TLegend leg2(0.18, 0.89, 0.7, 0.94);

  TGraphErrors gData;
  TGraphErrors gData_pp;
  TGraphErrors gSHM;
  TGraphErrors *gSHM_BS[13];
  TGraphErrors *gSHM_B;
  TGraphErrors *gSHM_S;
  TGraphErrors gPYTHIA;
  TGraphErrors gPYTHIA_CRMPI_ROPOFF;
  TGraphErrors gPYTHIA_CRMPI_ROPON;
  TGraphErrors *gPYTHIA_CRQCD;
  TGraphErrors gPYTHIA_ANGANTYR;
  TGraphErrors gPYTHIA_ANGANTYR_PPB;
  TGraphErrors gEPOS_pPb;
  TGraphErrors gHIJING;

  // h_c1a->Add(h_c1m);
  // h->Divide(h_c1a);
  // h2_c1a->Add(h2_c1m);
  // h2->Divide(h2_c1a);
  // hpp_c1a->Add(hpp_c1m);
  // hpp->Divide(hpp_c1a);

  // canvas
  cResult.SetLogx();
  frame.GetYaxis()->SetNdivisions(210);
  frame.GetYaxis()->SetTitleOffset(1.45);
  cResult.SetLeftMargin(0.15);
  cResult.SetRightMargin(0.02);
  cResult.SetTopMargin(0.03);

  for (int iB = 0; iB < 6; ++iB){
    TH1D* hsys = (TH1D*)f.Get(Form("hSys_%d", iB));
    gPbPb_sys->SetPointY(iB, hsys->GetMean());
    gPbPb_stat->SetPointY(iB, hsys->GetMean());
    gPbPb_sys->SetPointError(iB, mult_shm_PbPb_[iB] * 7.e-2, gPbPb_sys->GetErrorY(iB));
    gPbPb_stat->SetPointX(iB, mult_shm_PbPb_[iB]);
    gPbPb_sys->SetPointX(iB, mult_shm_PbPb_[iB]);
  }
  //gPbPb_stat->Fit("pol0", "R", "", 40, 2000);

  for (int iB = 0; iB < 6; ++iB){
    TH1D* hsys = (TH1D*)fpPb.Get(Form("hSys_%d", iB));
    gpPb_sys->SetPointY(iB, hsys->GetMean());
    gpPb_stat->SetPointY(iB, hsys->GetMean());
    gpPb_sys->SetPointError(iB, mult_pPb[iB] * 7.e-2, gpPb_sys->GetErrorY(iB));
    gpPb_stat->SetPointX(iB, mult_pPb[iB]);
    gpPb_sys->SetPointX(iB, mult_pPb[iB]);
  }

  for (int iB = 0; iB < 8; ++iB){
    TH1D* hsys = (TH1D*)fpp.Get(Form("hSys_%d", iB));
    gpp_sys->SetPointY(iB, hsys->GetMean());
    gpp_stat->SetPointY(iB, hsys->GetMean());
    gpp_sys->SetPointError(iB, mult_pp[iB] * 7.e-2, gpp_sys->GetErrorY(iB));
    gpp_stat->SetPointX(iB, mult_pp[iB]);
    gpp_sys->SetPointX(iB, mult_pp[iB]);
  }

  // for (int iB = 1; iB < h2->GetNbinsX(); ++iB){
  //   double x = h->GetBinContent(iB);
  //   double y = h2->GetBinContent(iB);
  //   double errx = h->GetBinError(iB);
  //   double erry = h2->GetBinError(iB);
  //   gData.AddPoint(mult_shm_PbPb[iB - 1], (/* x/pow(errx, 2) +  */y/pow(erry, 2)) / (/* 1./pow(errx, 2) + */ 1./pow(erry, 2)));
  //   gData.SetPointError(iB - 1, 0, 1. / sqrt((/* 1./pow(errx, 2) + */ 1./pow(erry, 2))));
  // }

  // for (int iB = 1; iB < hpp->GetNbinsX() + 1; ++iB){
  //   double y = hpp->GetBinContent(iB);
  //   double erry = hpp->GetBinError(iB);
  //   gData_pp.AddPoint(mult_pp[iB - 1], y);
  //   gData_pp.SetPointError(iB - 1, 0, erry);
  // }

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

  for (int iP = 0; iP < 7; ++iP){
    gEPOS_pPb.AddPoint(mult_pPb_model[iP], epos_pPb_c2c1[iP][0]);
    gEPOS_pPb.SetPointError(iP, 0, epos_pPb_c2c1[iP][1]);
  }

  // for (int iP = 0; iP < kNPointspp - 2; ++iP){
  //   gPYTHIA_CRQCD.AddPoint(mult_shm_pp_[iP], pythia_crqcd_c2c1[iP][0]);
  //   gPYTHIA_CRQCD.SetPointError(iP, 0, pythia_crqcd_c2c1[iP][1]);
  // }

  gPYTHIA_CRQCD = (TGraphErrors*)fPythia.Get("Grc2byc1xi_net");

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

  for (int iP = 0; iP < 5; ++iP){
    gPYTHIA_ANGANTYR_PPB.AddPoint(pythia_angantyr_pPb_mult[iP], pythia_angantyr_pPb_c2c1[iP][0]);
    gPYTHIA_ANGANTYR_PPB.SetPointError(iP, 0, pythia_angantyr_pPb_c2c1[iP][1]);
  }

  gSHM_BS[0] = (TGraphErrors*)fSHM_BS_1.Get("Grc2byc1xi");

  gSHM_BS[1] = (TGraphErrors*)fSHM_BS_1_5.Get("Grc2byc1xi");

  gSHM_BS[2] = (TGraphErrors*)fSHM_BS_2.Get("Grc2byc1xi");

  gSHM_BS[3] = (TGraphErrors*)fSHM_BS_2_5.Get("Grc2byc1xi");

  //gSHM_BS[4] = (TGraphErrors*)fSHM_BS_2_6.Get("Grc2byc1xi");

  gSHM_BS[4] = (TGraphErrors*)fSHM_BS_2_8.Get("Grc2byc1xi");

  gSHM_BS[5] = (TGraphErrors*)fSHM_BS_2_9.Get("Grc2byc1xi");

  gSHM_BS[6] = (TGraphErrors*)fSHM_BS_3.Get("Grc2byc1xi");

  gSHM_BS[7] = (TGraphErrors*)fSHM_BS_3_1.Get("Grc2byc1xi");

  gSHM_BS[8] = (TGraphErrors*)fSHM_BS_3_2.Get("Grc2byc1xi");

  gSHM_BS[9] = (TGraphErrors*)fSHM_BS_3_3.Get("Grc2byc1xi");

  gSHM_BS[10] = (TGraphErrors*)fSHM_BS_3_4.Get("Grc2byc1xi");

  gSHM_BS[11] = (TGraphErrors*)fSHM_BS_3_5.Get("Grc2byc1xi");

  gSHM_BS[12] = (TGraphErrors*)fSHM_BS_4.Get("Grc2byc1xi");

  // // set colors and style
  // gData.SetLineWidth(2);
  // gData.SetMarkerStyle(20);
  // gData.SetMarkerSize(1.2);
  // gData.SetLineColor(kRed);
  // gData.SetMarkerColor(kRed);

  // gData_pp.SetLineWidth(2);
  // gData_pp.SetMarkerStyle(21);
  // gData_pp.SetMarkerSize(1.2);
  // gData_pp.SetLineColor(kRed);
  // gData_pp.SetMarkerColor(kRed);


  // set colors and style
  gPbPb_stat->SetLineWidth(2);
  gPbPb_stat->SetMarkerStyle(20);
  gPbPb_stat->SetMarkerSize(1.);
  gPbPb_stat->SetLineColor(kBlue);
  gPbPb_stat->SetMarkerColor(kBlue);
  gPbPb_sys->SetLineWidth(2);
  gPbPb_sys->SetFillStyle(0);
  gPbPb_sys->SetMarkerStyle(20);
  gPbPb_sys->SetMarkerSize(1.);
  gPbPb_sys->SetLineColor(kBlue);
  gPbPb_sys->SetMarkerColor(kBlue);

  gpPb_stat->SetLineWidth(2);
  gpPb_stat->SetMarkerStyle(33);
  gpPb_stat->SetMarkerSize(1.5);
  gpPb_stat->SetLineColor(kGreen + 3);
  gpPb_stat->SetMarkerColor(kGreen + 3);
  gpPb_sys->SetLineWidth(2);
  gpPb_sys->SetFillStyle(0);
  gpPb_sys->SetMarkerStyle(33);
  gpPb_sys->SetMarkerSize(1.5);
  gpPb_sys->SetLineColor(kGreen + 3);
  gpPb_sys->SetMarkerColor(kGreen + 3);

  gpp_stat->SetLineWidth(2);
  gpp_stat->SetMarkerStyle(21);
  gpp_stat->SetMarkerSize(1.);
  gpp_stat->SetLineColor(kRed);
  gpp_stat->SetMarkerColor(kRed);
  gpp_sys->SetLineWidth(2);
  gpp_sys->SetFillStyle(0);
  gpp_sys->SetMarkerStyle(21);
  gpp_sys->SetMarkerSize(1.);
  gpp_sys->SetLineColor(kRed);
  gpp_sys->SetMarkerColor(kRed);

  gSHM.SetLineWidth(2);
  gSHM.SetLineColor(kOrange - 3);
  gSHM.SetFillColor(kOrange - 3);
  gSHM.SetFillStyle(3002);

  gSHM_BS[0]->SetLineWidth(2);
  gSHM_BS[0]->SetLineColor(kBlue); // kViolet
  gSHM_BS[0]->SetFillColor(kBlue);
  gSHM_BS[0]->SetFillStyle(3002);

  gSHM_BS[1]->SetLineWidth(2);
  gSHM_BS[1]->SetLineColor(kAzure + 7); // kBlue
  gSHM_BS[1]->SetFillColor(kAzure + 7);
  gSHM_BS[1]->SetFillStyle(3002);

  gSHM_BS[2]->SetLineWidth(2);
  gSHM_BS[2]->SetLineColor(kTeal - 7); // kCyan
  gSHM_BS[2]->SetFillColor(kTeal - 7);
  gSHM_BS[2]->SetFillStyle(3002);

  gSHM_BS[3]->SetLineWidth(2);
  gSHM_BS[3]->SetLineColor(kGreen + 1); // kAzure + 7
  gSHM_BS[3]->SetFillColor(kGreen + 1);
  gSHM_BS[3]->SetFillStyle(3002);

  // gSHM_BS[4]->SetLineWidth(2);
  // gSHM_BS[4]->SetLineColor(kTeal - 7);
  // gSHM_BS[4]->SetFillColor(kTeal - 7);
  // gSHM_BS[4]->SetFillStyle(3002);

  // gSHM_BS[5]->SetLineWidth(2);
  // gSHM_BS[5]->SetLineColor(kGreen + 2);
  // gSHM_BS[5]->SetFillColor(kGreen + 2);
  // gSHM_BS[5]->SetFillStyle(3002);

  gSHM_BS[6]->SetLineWidth(2);
  gSHM_BS[6]->SetLineColor(kOrange - 2); // kGreen + 1
  gSHM_BS[6]->SetFillColor(kOrange - 2);
  gSHM_BS[6]->SetFillStyle(3002);

  // gSHM_BS[7]->SetLineWidth(2);
  // gSHM_BS[7]->SetLineColor(kOrange - 2);
  // gSHM_BS[7]->SetFillColor(kOrange - 2);
  // gSHM_BS[7]->SetFillStyle(3002);

  gSHM_BS[11]->SetLineWidth(2);
  gSHM_BS[11]->SetLineColor(kOrange + 7); // kOrange + 7
  gSHM_BS[11]->SetFillColor(kOrange + 7);
  gSHM_BS[11]->SetFillStyle(3002);

  gSHM_BS[12]->SetLineWidth(2);
  gSHM_BS[12]->SetLineColor(kRed); // kRed
  gSHM_BS[12]->SetFillColor(kRed);
  gSHM_BS[12]->SetFillStyle(3002);

  // gPYTHIA_ANGANTYR.SetLineWidth(2);
  // gPYTHIA_ANGANTYR.SetLineColor(kOrange);
  // gPYTHIA_ANGANTYR.SetFillColor(kOrange);
  // gPYTHIA_ANGANTYR.SetFillStyle(3254);
  //gPYTHIA.SetLineStyle(kDashed);

  gHIJING.SetLineWidth(2);
  gHIJING.SetFillColor(kMagenta);
  gHIJING.SetLineColor(kMagenta);
  gHIJING.SetFillStyle(3002);

  // gEPOS_pPb.SetLineWidth(2);
  // gEPOS_pPb.SetLineColor(kViolet+1);
  // gEPOS_pPb.SetFillColor(kViolet+1);
  // gEPOS_pPb.SetFillStyle(3002);
  //gPYTHIA.SetLineStyle(kDashed);

  gPYTHIA_CRQCD->SetLineWidth(2);
  gPYTHIA_CRQCD->SetLineColor(kCyan - 2);
  gPYTHIA_CRQCD->SetFillColor(kCyan - 2);
  gPYTHIA_CRQCD->SetFillStyle(3002);

  gPYTHIA_CRMPI_ROPOFF.SetLineWidth(2);
  gPYTHIA_CRMPI_ROPOFF.SetLineColor(kOrange + 7);
  gPYTHIA_CRMPI_ROPOFF.SetFillColor(kOrange + 7);
  gPYTHIA_CRMPI_ROPOFF.SetFillStyle(3002);

  gPYTHIA_CRMPI_ROPON.SetLineWidth(2);
  gPYTHIA_CRMPI_ROPON.SetLineColor(kViolet + 6);
  gPYTHIA_CRMPI_ROPON.SetFillColor(kViolet + 6);
  gPYTHIA_CRMPI_ROPON.SetFillStyle(3002);

  gPYTHIA_ANGANTYR_PPB.SetLineWidth(2);
  gPYTHIA_ANGANTYR_PPB.SetLineColor(kAzure - 7);
  gPYTHIA_ANGANTYR_PPB.SetFillColor(kAzure - 7);
  gPYTHIA_ANGANTYR_PPB.SetFillStyle(3002);

  // legend
  leg.SetTextFont(44);
  leg.SetTextSize(23);
  leg2.SetTextFont(44);
  leg2.SetTextSize(30);
  leg.SetNColumns(2);
  leg2.SetNColumns(3);
  //leg.SetColumnSeparation(0.4);
  leg2.SetColumnSeparation(0.2);
  // leg.AddEntry(&gSHM_PbPb, "Thermal-FIST, 3 d#it{V}/d#it{y}, Pb-Pb #sqrt{#it{s}_{NN}}=5.02 TeV", "f");
  // leg.AddEntry(&gSHM_pp, "Thermal-FIST, 3 d#it{V}/d#it{y}, pp #sqrt{#it{s}_{NN}}=13 TeV", "f");
  //leg.AddEntry(&gHIJING, "HIJING Pb-Pb", "f");
  //leg.AddEntry(&gSHM, "TheFIST #gamma_{s} CSM, #it{V}_{C}=3d#it{V}/d#it{y}", "f");
  // leg.AddEntry(&gPYTHIA_CRMPI_ROPOFF, "PYTHIA MPI, pp", "f");
  // leg.AddEntry(&gPYTHIA_CRMPI_ROPON, "PYTHIA MPI + Rope, pp", "f");
  //leg.AddEntry(&gEPOS_pPb, "EPOS, p-Pb", "f");
  // leg.AddEntry(gPYTHIA_CRQCD, "PYTHIA QCD + Rope, pp", "f");
  // leg.AddEntry(&gPYTHIA_ANGANTYR_PPB, "PYTHIA Angantyr, p-Pb", "f");
  leg.SetHeader("TheFIST CSM, #it{T}_{chem} = 155 MeV, B + S conservation");
  leg.AddEntry(gSHM_BS[0], "#it{V}_{c} = 1.0 d#it{V}/d#it{y}", "lf");
  leg.AddEntry(gSHM_BS[1], "#it{V}_{c} = 1.5 d#it{V}/d#it{y}", "lf");
  leg.AddEntry(gSHM_BS[2], "#it{V}_{c} = 2.0 d#it{V}/d#it{y}", "lf");
  leg.AddEntry(gSHM_BS[3], "#it{V}_{c} = 2.5 d#it{V}/d#it{y}", "lf");
  // leg.AddEntry(gSHM_BS[4], "#it{V}_{c} = 2.6 d#it{V}/d#it{y}", "lf");
  // leg.AddEntry(gSHM_BS[5], "#it{V}_{c} = 2.8 d#it{V}/d#it{y}", "lf");
  leg.AddEntry(gSHM_BS[6], "#it{V}_{c} = 3.0 d#it{V}/d#it{y}", "lf");
  // leg.AddEntry(gSHM_BS[7], "#it{V}_{c} = 3.2 d#it{V}/d#it{y}", "lf");
  leg.AddEntry(gSHM_BS[11], "#it{V}_{c} = 3.5 d#it{V}/d#it{y}", "lf");
  leg.AddEntry(gSHM_BS[12], "#it{V}_{c} = 4.0 d#it{V}/d#it{y}", "lf");
  // leg2.AddEntry(gpp_stat, "ALICE, pp", "pe");
  // leg2.AddEntry(gpPb_stat, "ALICE, p-Pb", "pe");
  //leg2.SetHeader("ALICE");
  leg2.AddEntry(gPbPb_stat, "ALICE, Pb#minusPb, #sqrt{#it{s}_{NN}} = 5.02 TeV", "pe");
  //leg.AddEntry(&gPYTHIA, "PYTHIA, pp #sqrt{#it{s}_{NN}}=13 TeV");

  // line
  TLine l(30, 1, 2500, 1);
  l.SetLineWidth(2);
  l.SetLineStyle(kDashed);

  cResult.cd();
  frame.Draw();
  //gSHM.Draw("samee3l");
  // gPYTHIA.Draw("samee3l");
  // gHIJING.Draw("samee3l");
  //gPYTHIA_ANGANTYR.Draw("samee3l");
  //gEPOS_pPb.Draw("samee3l");
  // gPYTHIA_ANGANTYR_PPB.Draw("samee3l");
  // gPYTHIA_CRQCD->Draw("samee3l");
  // gPYTHIA_CRMPI_ROPON.Draw("samee3l");
  // gPYTHIA_CRMPI_ROPOFF.Draw("samee3l");

  gSHM_BS[0]->Draw("samee3l");
  gSHM_BS[1]->Draw("samee3l");
  gSHM_BS[2]->Draw("samee3l");
  gSHM_BS[3]->Draw("samee3l");
  // gSHM_BS[4]->Draw("samee3l");
  // gSHM_BS[5]->Draw("samee3l");
  gSHM_BS[6]->Draw("samee3l");
  // gSHM_BS[7]->Draw("samee3l");
  // gSHM_BS[8]->Draw("samee3l");
  // gSHM_BS[9]->Draw("samee3l");
  // gSHM_BS[10]->Draw("samee3l");
  gSHM_BS[11]->Draw("samee3l");
  gSHM_BS[12]->Draw("samee3l");

  leg.Draw("same");
  leg2.Draw("same");

  gPbPb_sys->Draw("e5same");
  gPbPb_stat->Draw("pesame");
  // gpp_sys->Draw("e5same");
  // gpp_stat->Draw("pesame");
  // gpPb_sys->Draw("e5same");
  // gpPb_stat->Draw("pesame");
  l.Draw("same");

  TLatex t;
  t.SetTextFont(44);
  t.SetTextSize(30);
  t.DrawLatex(37, 0.9515, "|#it{#eta}| < 0.8");

  //t.DrawLatex(35, 0.945, "0.2 < #it{p}_{T} (K) < 1.0 GeV/#it{c}");
  t.DrawLatex(37, 0.945, "1.0 < #it{p}_{T}(#Xi) < 3.0 GeV/#it{c}");

  TFile o("final_plot_c2c1_volumes_nosys.root", "recreate");
  o.cd();
  gData.Write();
  cResult.Write();
  cResult.Print("cC2C1_volumes.pdf"/* .pdf" */);

  // chi2
  for (int i = 0; i < 13; ++i){
    utils::MergeGraphPoints(gSHM_BS[i], 1, 2);
    utils::MergeGraphPoints(gSHM_BS[i], 4, 5);
  }

  std::cout << "chi2 = " << utils::chi2(gPbPb_stat, gPbPb_sys, gSHM_BS[2]) << std::endl;

  TCanvas cchi2("cchi2", "cchi2");
  TGraphErrors gchi2;
  double vols[] = {1., 1.5, 2., 2.5, 2.8, 2.9, 3., 3.1, 3.2, 3.3, 3.4, 3.5, 4.};
  for(int i{0}; i < 13; ++i){
    //if (i == 4 || i == 5 || i == 7) continue;
    gchi2.AddPoint(vols[i], utils::chi2(gPbPb_stat, gPbPb_sys, gSHM_BS[i]));
    gchi2.SetPointError(gchi2.GetN() - 1, 0., utils::chi2err(gPbPb_stat, gPbPb_sys, gSHM_BS[i]));
  }
  cchi2.cd();
  gchi2.Draw();
  cchi2.Write();
  gchi2.Write();

  o.Close();
  f.Close();
  //f2.Close();
}
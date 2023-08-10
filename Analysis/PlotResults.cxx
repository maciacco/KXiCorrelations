#include "../utils/Config.h"

void PlotResults(){
  gStyle->SetOptStat(0);

  TFile fpp("out_sys_17_finalBinning.root"); // 17pq
  TFile fpPb("out_sys_16_finalBinning.root"); // 16qt
  TFile f("out_sys_PbPb_finalBinning.root" /* "out_sys_PbPb_new_3.root" */); // 15o+18qr
  auto gpp_stat = (TGraphErrors*)fpp.Get("gRho");
  auto gpp_sys = (TGraphErrors*)fpp.Get("gRhoSys");
  gpp_stat->SetName("gRhoStat_pp");
  gpp_sys->SetName("gRhoSys_pp");
  auto gPbPb_stat = (TGraphErrors*)f.Get("gRho");
  auto gPbPb_sys = (TGraphErrors*)f.Get("gRhoSys");
  gPbPb_stat->SetName("gRhoStat_PbPb");
  gPbPb_sys->SetName("gRhoSys_PbPb");
  auto gpPb_stat = (TGraphErrors*)fpPb.Get("gRho");
  auto gpPb_sys = (TGraphErrors*)fpPb.Get("gRhoSys");
  gpPb_stat->SetName("gRhoStat_pPb");
  gpPb_sys->SetName("gRhoSys_pPb");

  TFile fPythia("models/PYTHIA_5TEV_CRQCD_RopeOn.root");

  TCanvas cResult("cResult", "cResult", 800, 800);
  TH2D frame("frame", ";#LTd#it{N}_{ch}/d#it{#eta}#GT;#it{#rho}_{#Delta#Xi #DeltaK}", 1, 1.5, 2500, 1, -0.056, 0.005);
  TLegend leg(0.16, 0.15, 0.7, 0.3);
  TLegend leg2(0.17, 0.3, 0.5, 0.4);
  //(0.16395348837209303, 0.650399548902823, 0.7395348837209302, 0.9167587400421383);

  TGraphErrors gSHM;
  TGraph gPYTHIA;
  TGraphErrors gPYTHIA_CRMPI_ROPOFF;
  TGraphErrors gPYTHIA_CRMPI_ROPON;
  TGraphErrors *gPYTHIA_CRQCD;
  TGraphErrors gPYTHIA_ANGANTYR;
  TGraphErrors gPYTHIA_ANGANTYR_PPB;
  TGraphErrors gPYTHIA_ANGANTYR_PPB_CRQCD_ROPE;
  TGraphErrors gHIJING;
  TGraphErrors gEPOS_pPb;
  TGraphErrors gData;
  TGraphErrors gData_pp;
  TGraphErrors gSHM_PPB_BOOST;
  TGraphErrors gSHM_PPB_WOBOOST;

  // canvas
  cResult.SetLogx();
  frame.GetYaxis()->SetNdivisions(210);
  frame.GetYaxis()->SetTitleOffset(1.2);
  cResult.SetLeftMargin(0.13);
  cResult.SetRightMargin(0.02);
  cResult.SetTopMargin(0.03);

  for (int iB = 0; iB < 6; ++iB){
    gPbPb_sys->SetPointError(iB, mult_shm_PbPb_[iB] * 7.e-2, gPbPb_sys->GetErrorY(iB));
    gPbPb_stat->SetPointX(iB, mult_shm_PbPb_[iB]);
    gPbPb_sys->SetPointX(iB, mult_shm_PbPb_[iB]);
  }

  for (int iB = 0; iB < 6; ++iB){
    gpPb_sys->SetPointError(iB, mult_pPb[iB] * 7.e-2, gpPb_sys->GetErrorY(iB));
    gpPb_stat->SetPointX(iB, mult_pPb[iB]);
    gpPb_sys->SetPointX(iB, mult_pPb[iB]);
  }
  //gPbPb_stat->Fit("pol0", "R", "", 40, 2000);

  for (int iB = 0; iB < 8; ++iB){
    gpp_sys->SetPointError(iB, mult_pp[iB] * 7.e-2, gpp_sys->GetErrorY(iB));
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

  TFile fSHM_BS_("models/PbPb_gammaS_SHM_baryonplusstrange.root");
  TGraphErrors* gSHM_BS_ = (TGraphErrors*)fSHM_BS_.Get("Grrho");

  for (int iP = 0; iP < kNPointsPbPb + kNPointspp - 1; ++iP){
    int index = kNPointsPbPb - iP - 2 + kNPointspp;
    gSHM.AddPoint(iP < kNPointspp - 1 ? mult_shm_pp[iP] : gSHM_BS_->GetPointX(iP - kNPointspp + 1), iP < kNPointspp - 1 ? shm_pp[iP][0] : gSHM_BS_->GetPointY(iP - kNPointspp + 1));
    gSHM.SetPointError(iP, 0, iP < kNPointspp - 1 ? shm_pp[iP][1] : gSHM_BS_->GetErrorY(iP - kNPointspp + 1));
  }

  // for (int iP = 0; iP < kNPointsPbPb + kNPointspp - 1; ++iP){
  //   int index = kNPointsPbPb - iP - 2 + kNPointspp;
  //   gSHM.AddPoint(iP < kNPointspp - 1 ? mult_shm_pp[iP] : mult_shm_PbPb[index], iP < kNPointspp - 1 ? shm_pp[iP][0] : shm_PbPb[index][0]);
  //   gSHM.SetPointError(iP, 0, iP < kNPointspp - 1 ? shm_pp[iP][1] : shm_PbPb[index][1]);
  // }

  for (int iP = 0; iP < kNPointsPbPb; ++iP){
    // gSHM_PbPb.AddPoint(mult_shm_PbPb[iP], shm_PbPb[iP][0]);
    // gSHM_PbPb.SetPointError(iP, 0, shm_PbPb[iP][1]);
    gHIJING.AddPoint(mult_shm_PbPb[iP], hijing[iP][0]);
    gHIJING.SetPointError(iP, 0, hijing[iP][1]);
  }

  for (int iP = 0; iP < 7; ++iP){
    gEPOS_pPb.AddPoint(mult_pPb_model[iP], epos_pPb[iP][0]);
    gEPOS_pPb.SetPointError(iP, 0, epos_pPb[iP][1]);
  }

  for (int iP = 0; iP < kNPointspp; ++iP){
    // gSHM_pp.AddPoint(mult_shm_pp[iP], shm_pp[iP][0]);
    // gSHM_pp.SetPointError(iP, 0, shm_pp[iP][1]);
    gPYTHIA.AddPoint(mult_pythia[iP], pythia[iP]);
  }

  // for (int iP = 0; iP < kNPointspp - 2; ++iP){
  //   gPYTHIA_CRQCD.AddPoint(mult_pythia_[iP], pythia_crqcd_netCorr[iP][0]);
  //   gPYTHIA_CRQCD.SetPointError(iP, 0, pythia_crqcd_netCorr[iP][1]);
  // }

  gPYTHIA_CRQCD = (TGraphErrors*)fPythia.Get("Grrho_net");

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

  for (int iP = 0; iP < 5; ++iP){
    gPYTHIA_ANGANTYR_PPB_CRQCD_ROPE.AddPoint(pythia_angantyr_pPb_crqcd_rope_mult[iP], pythia_angantyr_pPb_crqcd_rope[iP][0]);
    gPYTHIA_ANGANTYR_PPB_CRQCD_ROPE.SetPointError(iP, 0, pythia_angantyr_pPb_crqcd_rope[iP][1]);
  }

  for (int iP = 0; iP < 5; ++iP){
    gPYTHIA_ANGANTYR_PPB.AddPoint(pythia_angantyr_pPb_mult[iP], pythia_angantyr_pPb[iP][0]);
    gPYTHIA_ANGANTYR_PPB.SetPointError(iP, 0, pythia_angantyr_pPb[iP][1]);
  }

  for (int iP = 0; iP < 7; ++iP){
    gSHM_PPB_BOOST.AddPoint(SHM_pPb_boost_mult[iP], SHM_pPb_boost[iP][0]);
    gSHM_PPB_BOOST.SetPointError(iP, 0, SHM_pPb_boost[iP][1]);
  }

  for (int iP = 0; iP < 7; ++iP){
    gSHM_PPB_WOBOOST.AddPoint(SHM_pPb_woboost_mult[iP], SHM_pPb_woboost[iP][0]);
    gSHM_PPB_WOBOOST.SetPointError(iP, 0, SHM_pPb_woboost[iP][1]);
  }

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

  // gData_pp.SetLineWidth(1);
  // gData_pp.SetMarkerStyle(21);
  // gData_pp.SetMarkerSize(1.2);
  // gData_pp.SetLineColor(kRed);
  // gData_pp.SetMarkerColor(kRed);

  gSHM.SetLineWidth(2);
  gSHM.SetLineColor(kOrange - 3);
  gSHM.SetFillColor(kOrange - 3);
  gSHM.SetFillStyle(3002);

  gPYTHIA_ANGANTYR.SetLineWidth(2);
  gPYTHIA_ANGANTYR.SetLineColor(kAzure + 1);
  gPYTHIA_ANGANTYR.SetFillColor(kAzure + 1);
  gPYTHIA_ANGANTYR.SetFillStyle(3002);

  gPYTHIA_ANGANTYR_PPB.SetLineWidth(2);
  gPYTHIA_ANGANTYR_PPB.SetLineColor(kAzure - 7);
  gPYTHIA_ANGANTYR_PPB.SetFillColor(kAzure - 7);
  gPYTHIA_ANGANTYR_PPB.SetFillStyle(3002);

  // gPYTHIA_ANGANTYR_PPB_CRQCD_ROPE.SetLineWidth(2);
  // gPYTHIA_ANGANTYR_PPB_CRQCD_ROPE.SetLineColor(kGreen);
  // gPYTHIA_ANGANTYR_PPB_CRQCD_ROPE.SetFillColor(kGreen);
  // gPYTHIA_ANGANTYR_PPB_CRQCD_ROPE.SetFillStyle(3002);

  gSHM_PPB_BOOST.SetLineWidth(2);
  gSHM_PPB_BOOST.SetLineColor(kBlue - 3);
  gSHM_PPB_BOOST.SetFillColor(kBlue - 3);
  gSHM_PPB_BOOST.SetFillStyle(3002);

  // gSHM_PPB_WOBOOST.SetLineWidth(2);
  // gSHM_PPB_WOBOOST.SetLineColor(kBlue);
  // gSHM_PPB_WOBOOST.SetFillColor(kBlue);
  // gSHM_PPB_WOBOOST.SetFillStyle(3002);

  // gSHM_PbPb.SetLineWidth(1);
  // gSHM_PbPb.SetLineColor(kBlue + 1);
  // gSHM_PbPb.SetFillColor(kBlue + 1);
  // gSHM_PbPb.SetFillStyle(3002);

  // gSHM_pp.SetLineWidth(1);
  // gSHM_pp.SetFillColor(kAzure + 7);
  // gSHM_pp.SetFillStyle(3245);
  // gSHM_pp.SetLineColor(kAzure + 7);
  //gSHM_pp.SetLineStyle(kDashed);

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
  gPYTHIA_CRQCD->SetLineColor(kGreen + 2);
  gPYTHIA_CRQCD->SetFillColor(kGreen + 2);
  gPYTHIA_CRQCD->SetFillStyle(3002);

  gPYTHIA_CRMPI_ROPOFF.SetLineWidth(2);
  gPYTHIA_CRMPI_ROPOFF.SetLineColor(kOrange + 7);
  gPYTHIA_CRMPI_ROPOFF.SetFillColor(kOrange + 7);
  gPYTHIA_CRMPI_ROPOFF.SetFillStyle(3002);

  gPYTHIA_CRMPI_ROPON.SetLineWidth(2);
  gPYTHIA_CRMPI_ROPON.SetLineColor(kViolet + 6);
  gPYTHIA_CRMPI_ROPON.SetFillColor(kViolet + 6);
  gPYTHIA_CRMPI_ROPON.SetFillStyle(3002);

  gPYTHIA.SetLineWidth(2);
  gPYTHIA.SetLineColor(kOrange);
  //gPYTHIA.SetLineStyle(kDashed);

  // legend
  leg.SetTextFont(44);
  leg.SetTextSize(23);
  leg2.SetTextFont(44);
  leg2.SetTextSize(23);
  leg.SetNColumns(2);
  leg2.SetNColumns(3);
  leg.SetColumnSeparation(0.4);
  leg2.SetHeader("ALICE Preliminary");
  leg2.SetColumnSeparation(0.1); // 0.2
  // leg.AddEntry(&gSHM_PbPb, "Thermal-FIST, 3 d#it{V}/d#it{y}, Pb-Pb #sqrt{#it{s}_{NN}}=5.02 TeV", "f");
  // leg.AddEntry(&gSHM_pp, "Thermal-FIST, 3 d#it{V}/d#it{y}, pp #sqrt{#it{s}_{NN}}=13 TeV", "f");
  leg.AddEntry(&gPYTHIA_ANGANTYR_PPB, "PYTHIA Angantyr, p#minusPb", "lf");
  leg.AddEntry(&gHIJING, "HIJING Pb#minusPb", "lf");
  leg.AddEntry(&gPYTHIA_CRMPI_ROPOFF, "PYTHIA Monash, pp", "lf");
  //leg.AddEntry(&gPYTHIA_CRMPI_ROPON, "PYTHIA MPI + Rope, pp", "f");
  leg.AddEntry(&gPYTHIA_ANGANTYR, "PYTHIA Angantyr, Pb#minusPb", "lf");
  leg.AddEntry(gPYTHIA_CRQCD, "PYTHIA QCD + Rope, pp", "lf");
  leg.AddEntry(&gSHM, "TheFIST #gamma_{s} CSM, #it{V}_{C} = 3d#it{V}/d#it{y}", "lf");
  // leg.AddEntry(&gEPOS_pPb, "EPOS, p-Pb", "f");
  // leg.AddEntry(&gSHM_PPB_BOOST, "TheFIST p#minusPb w/ rapidity boost", "f");
  leg2.AddEntry(gpp_stat, "pp", "pe");
  leg2.AddEntry(gpPb_stat, "p#minusPb", "pe");
  leg2.AddEntry(gPbPb_stat, "Pb#minusPb", "pe");
  //leg.AddEntry(&gPYTHIA, "PYTHIA, pp #sqrt{#it{s}_{NN}}=13 TeV");

  // line
  TLine l(1.5, 0, 2500, 0);
  l.SetLineWidth(2);
  l.SetLineStyle(kDashed);

  cResult.cd();
  frame.Draw();
  l.Draw("same");
  leg.Draw("same");
  leg2.Draw("same");
  // gSHM_PbPb.Draw("samee3l");
  // gSHM_pp.Draw("samee3l");
  gSHM.Draw("samee3l");
  gPYTHIA_ANGANTYR.Draw("samee3l");
 //gPYTHIA_ANGANTYR_PPB_CRQCD_ROPE.Draw("samee3l");
  gPYTHIA_ANGANTYR_PPB.Draw("samee3l");
  gHIJING.Draw("samee3l");
  //gEPOS_pPb.Draw("samee3l");
  gPYTHIA_CRQCD->Draw("samee3l");
  //gPYTHIA_CRMPI_ROPON.Draw("samee3l");
  gPYTHIA_CRMPI_ROPOFF.Draw("samee3l");
  //gSHM_PPB_BOOST.Draw("same3l");
  //gSHM_PPB_WOBOOST.Draw("same3l");

  gPbPb_sys->Draw("e5same");
  gpp_sys->Draw("e5same");
  gPbPb_stat->Draw("pesame");
  gpp_stat->Draw("pesame");
  gpPb_sys->Draw("e5same");
  gpPb_stat->Draw("pesame");

  TLatex t;
  t.SetTextFont(44);
  t.SetTextSize(23);
  t.DrawLatex(2.1, -0.004, "#sqrt{#it{s}_{NN}} = 5.02 TeV, |#it{#eta}| < 0.8");
  t.DrawLatex(70, -0.004, "0.2 #leq #it{p}_{T} (K) < 1.0 GeV/#it{c}");
  t.DrawLatex(70, -0.007, "1.0 #leq #it{p}_{T} (#Xi) < 3.0 GeV/#it{c}");

  TFile o("final_plot_rho_finalBinning.root", "recreate");
  o.cd();

  gPYTHIA.Write();
  cResult.Write();
  cResult.Print("cRho.pdf");
  o.Close();
  f.Close();
  f.Close();
  fpp.Close();
}
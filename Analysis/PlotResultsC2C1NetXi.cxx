#include "../utils/Config.h"
#include "../utils/Utils.h"

void PlotResultsC2C1NetXi(){
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
  TFile fEPOSPbPb("models/out_analysis_repro.root");

  TCanvas cResult("cResult", "cResult", 800, 800);
  TH2D frame("frame", ";#LTd#it{N}_{ch}/d#it{#eta}#GT;#it{#kappa}_{2}(#bar{#Xi}^{+} - #Xi^{#minus})/#it{#kappa}_{1}(#bar{#Xi}^{+} + #Xi^{#minus})", 1, 1.5, 2500, 1, 0.90, 1.08);
  TLegend leg(0.16, /* 0.65 */0.61, 0.7, 0.81); // 0.85
  //TLegend leg2(0.162, 0.8, 0.7, 0.85);
  TLegend leg2(0.18, 0.82, 0.5, 0.95);
  TLegend leg3(0.5275, 0.84, 0.7975, 0.89);

  TGraphErrors gData;
  TGraphErrors gData_pp;
  TGraphErrors gSHM;
  TGraphErrors gPYTHIA;
  TGraphErrors gPYTHIA_CRMPI_ROPOFF;
  TGraphErrors gPYTHIA_CRMPI_ROPON;
  TGraphErrors *gPYTHIA_CRQCD;
  TGraphErrors gPYTHIA_ANGANTYR;
  TGraphErrors gPYTHIA_ANGANTYR_PPB;
  TGraphErrors gEPOS_pPb;
  TGraphErrors *gEPOS_PbPb;
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

  TFile fSHM_BS_("models/PbPb_gammaS_SHM_baryonplusstrange.root");
  TGraphErrors* gSHM_BS_ = (TGraphErrors*)fSHM_BS_.Get("Grc2byc1xi");

  for (int iP = 0; iP < kNPointsPbPb + kNPointspp - 1; ++iP){
    int index = kNPointsPbPb - iP - 2 + kNPointspp;
    gSHM.AddPoint(iP < kNPointspp - 1 ? mult_shm_pp[iP] : gSHM_BS_->GetPointX(iP - kNPointspp + 1), iP < kNPointspp - 1 ? shm_pp_c2c1[iP][0] : gSHM_BS_->GetPointY(iP - kNPointspp + 1));
    gSHM.SetPointError(iP, 0, iP < kNPointspp - 1 ? shm_pp_c2c1[iP][1] : gSHM_BS_->GetErrorY(iP - kNPointspp + 1));
  }

  // for (int iP = 0; iP < kNPointsPbPb + kNPointspp - 1; ++iP){
  //   int index = kNPointsPbPb - iP - 2 + kNPointspp;
  //   gSHM.AddPoint(iP < kNPointspp - 1 ? mult_shm_pp[iP] : mult_shm_PbPb[index], iP < kNPointspp - 1 ? shm_pp_c2c1[iP][0] : shm_PbPb_c2c1[index][0]);
  //   gSHM.SetPointError(iP, 0, iP < kNPointspp - 1 ? shm_pp_c2c1[iP][1] : shm_PbPb_c2c1[index][1]);
  // }

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
  gEPOS_PbPb = (TGraphErrors*)fEPOSPbPb.Get("Grc2byc1xi");

  TFile fSHM_4_("models/Output_Final_BS_vc4.0_CE.root");
  TGraphErrors* gSHM_4 = (TGraphErrors*)fSHM_4_.Get("Grc2byc1xi");

  TFile fSHM_1_("models/Output_Final_BS_vc1.0_CE.root");
  TGraphErrors* gSHM_1 = (TGraphErrors*)fSHM_1_.Get("Grc2byc1xi");

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

  TFile fSHM_Vanilla_("models/out_analysis_30_low_mult_vanilla_refitted_extend.root");
  TGraphErrors* gSHM_Vanilla_ = (TGraphErrors*)fSHM_Vanilla_.Get("Grc2byc1xi");

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

  // varied volume
  gSHM_4->SetLineWidth(2);
  gSHM_4->SetLineColor(kRed);
  gSHM_4->SetFillColor(kRed);
  gSHM_4->SetFillStyle(3002);

  gSHM_1->SetLineWidth(2);
  gSHM_1->SetLineColor(kGreen + 2);
  gSHM_1->SetFillColor(kGreen + 2);
  gSHM_1->SetFillStyle(3002);

  gPYTHIA_ANGANTYR.SetLineWidth(2);
  gPYTHIA_ANGANTYR.SetLineColor(kAzure + 1);
  gPYTHIA_ANGANTYR.SetFillColor(kAzure + 1);
  gPYTHIA_ANGANTYR.SetFillStyle(3002);

  gHIJING.SetLineWidth(2);
  gHIJING.SetFillColor(kMagenta - 7);
  gHIJING.SetLineColor(kMagenta - 7);
  gHIJING.SetFillStyle(3002);

  gEPOS_PbPb->SetLineWidth(2);
  gEPOS_PbPb->SetLineColor(kViolet+1);
  gEPOS_PbPb->SetFillColor(kViolet+1);
  gEPOS_PbPb->SetFillStyle(3002);

  gEPOS_pPb.SetLineWidth(2);
  gEPOS_pPb.SetLineColor(kViolet+1);
  gEPOS_pPb.SetFillColor(kViolet+1);
  gEPOS_pPb.SetFillStyle(3002);
  //gPYTHIA.SetLineStyle(kDashed);

  gPYTHIA_CRQCD->SetLineWidth(2);
  gPYTHIA_CRQCD->SetLineColor(kGreen + 2);
  gPYTHIA_CRQCD->SetFillColor(kGreen + 2);
  gPYTHIA_CRQCD->SetFillStyle(3002);

  gPYTHIA_CRMPI_ROPOFF.SetLineWidth(2);
  gPYTHIA_CRMPI_ROPOFF.SetLineColor(kRed - 4);
  gPYTHIA_CRMPI_ROPOFF.SetFillColor(kRed - 4);
  gPYTHIA_CRMPI_ROPOFF.SetFillStyle(3002);

  gPYTHIA_CRMPI_ROPON.SetLineWidth(2);
  gPYTHIA_CRMPI_ROPON.SetLineColor(kViolet + 6);
  gPYTHIA_CRMPI_ROPON.SetFillColor(kViolet + 6);
  gPYTHIA_CRMPI_ROPON.SetFillStyle(3002);

  gPYTHIA_ANGANTYR_PPB.SetLineWidth(2);
  gPYTHIA_ANGANTYR_PPB.SetLineColor(kAzure - 7);
  gPYTHIA_ANGANTYR_PPB.SetFillColor(kAzure - 7);
  gPYTHIA_ANGANTYR_PPB.SetFillStyle(3002);



  gSHM_Vanilla_->SetLineWidth(2);
  gSHM_Vanilla_->SetLineColor(kViolet + 1);
  gSHM_Vanilla_->SetFillColor(kViolet + 1);
  gSHM_Vanilla_->SetFillStyle(3002);

  // legend
  leg.SetTextFont(44);
  leg.SetTextSize(23);
  leg2.SetTextFont(44);
  leg2.SetTextSize(30);
  leg3.SetTextFont(44);
  leg3.SetTextSize(23);
  leg.SetNColumns(2);
  leg2.SetNColumns(3);
  leg.SetColumnSeparation(0.4);
  leg2.SetColumnSeparation(0.1);
  leg2.SetHeader("ALICE, #sqrt{#it{s}_{NN}} = 5.02 TeV, |#it{#eta}| < 0.8");
  // leg.AddEntry(&gSHM_PbPb, "Thermal-FIST, 3 d#it{V}/d#it{y}, Pb-Pb #sqrt{#it{s}_{NN}}=5.02 TeV", "f");
  // leg.AddEntry(&gSHM_pp, "Thermal-FIST, 3 d#it{V}/d#it{y}, pp #sqrt{#it{s}_{NN}}=13 TeV", "f");
  TH1F hEmpty;
  leg.AddEntry(&gPYTHIA_CRMPI_ROPOFF, "PYTHIA Monash, pp", "lf");
  leg.AddEntry(&hEmpty, "", "");
  //leg.AddEntry(&gEPOS_pPb, "EPOS 3, p#minusPb", "lf");
  leg.AddEntry(gPYTHIA_CRQCD, "PYTHIA QCD + Rope, pp", "lf");
  leg.AddEntry(&gHIJING, "HIJING Pb#minusPb", "lf");
  //leg.AddEntry(&gPYTHIA_CRMPI_ROPON, "PYTHIA MPI + Rope, pp", "f");
  leg.AddEntry(&gPYTHIA_ANGANTYR_PPB, "PYTHIA Angantyr, p#minusPb", "lf");
  leg.AddEntry(&gPYTHIA_ANGANTYR, "PYTHIA Angantyr, Pb#minusPb", "lf");
  leg.AddEntry(&gSHM, "TheFIST CE SHM, #it{V}_{c} = 3 d#it{V}/d#it{y}", "lf");
  leg.AddEntry(&hEmpty, "", "");
  leg.AddEntry(&hEmpty, "#it{T}_{chem}, d#it{V}/d#it{y}, and #gamma_{s} from Phys. Rev. C 100 (2019) 054906", "");
 // leg.AddEntry(gSHM_4, "TheFIST CSM, #it{V}_{C} = 4d#it{V}/d#it{y}", "f");
  leg2.AddEntry(gpp_stat, "pp", "pe");
  leg2.AddEntry(gpPb_stat, "p#minusPb", "pe");
  leg2.AddEntry(gPbPb_stat, "Pb#minusPb", "pe");
  //leg.AddEntry(&gPYTHIA, "PYTHIA, pp #sqrt{#it{s}_{NN}}=13 TeV");

  //leg3.AddEntry(&gEPOS_pPb, "EPOS LHC, p#minusPb", "lf");

  // line
  TLine l(1.5/* 30 */, 1, 2500, 1);
  l.SetLineWidth(2);
  l.SetLineStyle(kDashed);

  cResult.cd();
  frame.Draw();
  gSHM.Draw("samee3l");
  // gPYTHIA.Draw("samee3l");
  gHIJING.Draw("samee3l");
  gPYTHIA_ANGANTYR.Draw("samee3l");

  //gEPOS_pPb.Draw("samee3l");
  //gEPOS_PbPb->Draw("samee3l");
  gPYTHIA_ANGANTYR_PPB.Draw("samee3l");
  gPYTHIA_CRQCD->Draw("samee3l");

  //gPYTHIA_CRMPI_ROPON.Draw("samee3l");
  gPYTHIA_CRMPI_ROPOFF.Draw("samee3l");

  //gSHM_Vanilla_->Draw("samee3l");

  // gSHM_4->Draw("samee3l");
  // gSHM_1->Draw("samee3l");

  leg.Draw("same");
  leg2.Draw("same");
  //leg3.Draw("same");

  gPbPb_sys->Draw("e5same");
  gPbPb_stat->Draw("pesame");
  gpp_sys->Draw("e5same");
  gpp_stat->Draw("pesame");
  gpPb_sys->Draw("e5same");
  gpPb_stat->Draw("pesame");
  l.Draw("same");

  TLatex t;
  t.SetTextFont(44);
  t.SetTextSize(30);
  //t.DrawLatex(/* 35 */2.1, 0.92, "#sqrt{#it{s}_{NN}} = 5.02 TeV, |#it{#eta}| < 0.8");

  //t.DrawLatex(2.5, 0.940, "0.2 < #it{p}_{T} (K) < 1.0 GeV/#it{c}");
  t.DrawLatex(2.1, 0.91 /* 0.935 */, "1.0 < #it{p}_{T}(#Xi) < 3.0 GeV/#it{c}");

  std::cout << utils::chi2interp(gpp_stat, gpp_sys, gPYTHIA_CRQCD) << "\n";
  std::cout << utils::chi2interpMatrices(gpp_stat, gpp_sys, gPYTHIA_CRQCD) << "\n";

  TFile o("final_plot_c2c1.root", "recreate");
  o.cd();
  gData.Write();
  cResult.Write();
  cResult.Print("cC2C1.pdf"/*.pdf"*/);
  o.Close();
  f.Close();
  //f2.Close();
}
#include "../utils/Config.h"
const int kKcut = 68;
const int kXiCut = 8;
const int kNCentBinsAnalysis = 7;


double kCentClasses[] = {0., 10., 30., 40., 50., 70., 80.};

void remove_outlier(TH1D* h, double reject_level = 3.){
  double mean = h->GetMean();
  double std_dev = h->GetStdDev();
  for (int iB{1}; iB <= h->GetNbinsX(); ++iB){
    if (std::abs(h->GetBinCenter(iB) - mean) > reject_level * std_dev){
      h->SetBinContent(iB, 0.);
    }
  }
}

const bool kRho = false;
const bool kC2c1 = true;

void AnalysisSysPbPbCombined_finalBinning_Xi()
{
  gStyle->SetOptStat(0);
  TFile *fin = new TFile(kC2c1 ? "out_sys_15o_finalBinning__xi_p.root" : "out_sys_15o_finalBinning_.root");
  TFile *fin2 = new TFile(kC2c1 ? "out_sys_18qr_finalBinning__xi_p.root" : "out_sys_18qr_finalBinning_.root");
  TFile f(kC2c1 ? "out_sys_PbPb_finalBinning_xi_p.root" : "out_sys_PbPb_finalBinning.root", "recreate");
  TH1D *hSys[kNCentBinsAnalysis];
  TGraphErrors gRho;
  gRho.SetName("gRho");
  gRho.SetTitle(";Centrality (%);#rho_{#Delta#Xi#DeltaK}");
  TGraphErrors gRhoSys;
  TCanvas c("c", "c");
  c.cd();
  TH2D hFrame("hFrame", ";Centrality (%);#rho_{#Delta#Xi#DeltaK}", 1, 0, 90, 1, (kC2c1 ? 0. : -0.045), (kC2c1 ? 100.0 : -0.02));
  hFrame.SetStats(0);
  hFrame.Draw();
  //TH2D hFrame("hFrame", ";Centrality (%);C_{2}/C_{1}", 1, 0, 90, 1, 0., 1.5);
  gRhoSys.SetName("gRhoSys");
  TCanvas cSys("cSys", "cSys", 600, 400);
  cSys.Divide(3, 2);
  for (int i{0}; i < kNCentBinsAnalysis; ++i){
    if (kRho) hSys[i] = new TH1D(Form("hSys_%d", i), ";#rho;Entries", 45, -1, -1/* 160, -0.05, -0.01 */);
    if (kC2c1) hSys[i] = new TH1D(Form("hSys_%d", i), ";C_{2}/C_{1};Entries", 20, -1, -1/* 1000, 0.8, 1.0 */);
  }

  TGraphErrors *g[60];
  for(int iVar = 0; iVar < 1800; ++iVar)
  {
    //if (iVar == 26) continue;
    if (kRho){
      //if ((iVar/3/5)%2!=1) continue;
      //if ((iVar/3/5/2/2/3)%11>9) continue;
    }
    //if ((iVar/3/5/2)%3==0) continue;
    if (((iVar%60)/60.) > 0.001 && kC2c1) continue;

    TGraphErrors *g1 = (TGraphErrors*)fin->Get(Form("g_%d", iVar));
    g1->SetName(Form("g_%d_1", iVar));
    TGraphErrors *g2 = (TGraphErrors*)fin2->Get(Form("g_%d", iVar));
    g2->SetName(Form("g_%d_2", iVar));
    
    g[iVar] = new TGraphErrors();
    g[iVar]->SetName(Form("g_%d", iVar));

    for(int i = 1; i < kNCentBinsAnalysis; i++)
    {
      double x = g1->GetPointY(i - 1);
      double errx = g1->GetErrorY(i - 1);
      double y = g2->GetPointY(i - 1);
      double erry = g2->GetErrorY(i - 1);

      double rhomean = (x/pow(errx, 2) + y/pow(erry, 2)) / (1./pow(errx, 2) + 1./pow(erry, 2));
      double rhorms = 1. / sqrt((1./pow(errx, 2) + 1./pow(erry, 2)));
      g[iVar]->AddPoint(0.5 * (kCentClasses[i - 1] + kCentClasses[i]), rhomean);
      g[iVar]->SetPointError(i - 1, 0, rhorms);

      // cout << TMath::Sqrt(rhorms / (( kNSample - nSkip) * (( kNSample - nSkip) - 1))) << "\n";

      hSys[i - 1]->Fill(rhomean);
      if (iVar == (kC2c1 ? 960 : 1012)){
        gRho.AddPoint(0.5 * (kCentClasses[i - 1] + kCentClasses[i]), rhomean);
        gRhoSys.AddPoint(0.5 * (kCentClasses[i - 1] + kCentClasses[i]), rhomean);
        gRho.SetPointError(i - 1, 0, rhorms);
      }
    }

    //TCanvas c(Form("c_%d", iVar), Form("c_%d", iVar));
    c.cd();
    g[iVar]->SetLineWidth(2);
    g[iVar]->SetLineColor(kAzure + 2);
    g[iVar]->Draw("samepl");
    f.cd();
    g[iVar]->Write();
   // c.Write();
  }
  TLine *line[kNCentBinsAnalysis];
  TH1D* hSysFrame[kNCentBinsAnalysis];
  for (int i{0}; i < kNCentBinsAnalysis - 1; ++i){
    line[i] = new TLine(gRho.GetPointY(i), 0, gRho.GetPointY(i), hSys[i]->GetBinContent(hSys[i]->GetMaximumBin()));
    hSysFrame[i] = new TH1D(Form("hSysFrame_%d", i), kC2c1 ? ";#it{C}_{2}/#it{C}_{1};Entries" : ";#it{#rho};Entries", 10000000, -10, 200);
    //remove_outlier(hSys[i]);
    //hSys[i]->SetStats(0);
    hSys[i]->SetFillStyle(3004);
    hSys[i]->SetLineWidth(2);
    hSys[i]->SetLineColor(kBlue);
    hSys[i]->SetFillColor(kBlue);
    hSysFrame[i]->SetMaximum(hSys[i]->GetBinContent(hSys[i]->GetMaximumBin()) + hSys[i]->GetBinContent(hSys[i]->GetMaximumBin()) * 0.05);
    if (kRho) hSysFrame[i]->GetXaxis()->SetRangeUser(hSys[i]->GetMean() - 5.*hSys[i]->GetStdDev(), hSys[i]->GetMean() + 5.*hSys[i]->GetStdDev());
    if (kC2c1) hSysFrame[i]->GetXaxis()->SetRangeUser(hSys[i]->GetMean() - 7.*hSys[i]->GetStdDev(), hSys[i]->GetMean() + 7.*hSys[i]->GetStdDev());
    hSys[i]->Write();
    gRhoSys.SetPointError(i, 2, hSys[i]->GetStdDev());
    hSysFrame[i]->SetTitle(Form("%.0f-%.0f%%",  kCentClasses[i], kCentClasses[i + 1]));
    cSys.cd(i + 1);
    hSysFrame[i]->Draw("");
    hSys[i]->Draw("histosame");
    cSys.Update();
    //hSys[i]->Fit("gaus", "LM+");
    // TPaveStats *ps = (TPaveStats*)hSys[i]->FindObject("stats");
    // ps->SetTextFont(44);
    // ps->SetTextSize(10);
    // ps->SetX1NDC(0.6515426497277677);
    // ps->SetY1NDC(0.5675675675675675);
    // ps->SetX2NDC(0.9818511796733213);
    // ps->SetY2NDC(0.933933933933934);
    TLatex t;
    t.SetTextFont(44);
    t.SetTextSize(8);
    t.SetNDC();
    t.DrawLatex(0.7, 0.8, Form("#mu = %.5f", hSys[i]->GetMean()));
    t.DrawLatex(0.7, 0.75, Form("#sigma = %.5f", hSys[i]->GetRMS()));
    line[i]->SetLineWidth(2);
    line[i]->SetLineStyle(7);
    line[i]->SetLineColor(kRed);
    line[i]->Draw("same");
  }
  gRho.Write();
  gRhoSys.Write();
  gRhoSys.SetFillStyle(0);
  gRhoSys.SetLineColor(kRed);
  gRhoSys.SetLineWidth(2);
  gRho.SetLineColor(kRed);
  gRho.SetLineWidth(2);
  gRho.SetMarkerColor(kRed);
  gRho.SetMarkerStyle(20);
  gRho.SetMarkerSize(1.5);
  // c.cd();
  // hFrame.SetStats(0);
  // hFrame.Draw();
  gRhoSys.GetXaxis()->SetTitle("Centrality (%)");
  gRhoSys.GetYaxis()->SetTitle("#rho(#Delta#Xi#DeltaK)");
  //gRhoSys.GetYaxis()->SetTitle("C2/C1");
  c.cd();
  gRhoSys.Draw("pe5same");
  gRho.Draw("pesame");
  c.Write();
  cSys.Write();
  cSys.Print(kC2c1 ? "cSys_xi_p.pdf" : "cSys.pdf");
  f.Close();
}

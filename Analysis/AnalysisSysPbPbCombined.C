#include "../utils/Config.h"
const int kKcut = 68;
const int kXiCut = 8;
const int kNCentBinsAnalysis = 9;

void AnalysisSysPbPbCombined()
{
  TFile *fin = new TFile("out_prova_sys_15.root");
  TFile *fin2 = new TFile("out_prova_sys_18.root");
  TFile f("out_sys_PbPb.root", "recreate");
  TH1D *hSys[kNCentBinsAnalysis];
  TGraphErrors gRho;
  gRho.SetName("gRho");
  gRho.SetTitle(";Centrality (%);#rho_{#Delta#Xi#DeltaK}");
  TGraphErrors gRhoSys;
  TH2D hFrame("hFrame", ";Centrality (%);#rho_{#Delta#Xi#DeltaK}", 1, 0, 90, 1, -0.055, -0.01);
  gRhoSys.SetName("gRhoSys");
  TCanvas cSys("cSys", "cSys");
  cSys.Divide(3, 3);
  for (int i{0}; i < kNCentBinsAnalysis; ++i){
    hSys[i] = new TH1D(Form("hSys_%d", i), ";#rho;Entries", 120, -0.06, 0.);
  }
  for(int iVar = 0; iVar < 2025; ++iVar)
  {
    TGraphErrors *g1 = (TGraphErrors*)fin->Get(Form("g_%d", iVar));
    g1->SetName(Form("g_%d_1", iVar));
    TGraphErrors *g2 = (TGraphErrors*)fin2->Get(Form("g_%d", iVar));
    g2->SetName(Form("g_%d_2", iVar));
    
    TGraphErrors g;
    g.SetName(Form("g_%d", iVar));

    for(int i = 1; i < kNCentBinsAnalysis; i++)
    {
      double x = g1->GetPointY(i - 1);
      double errx = g1->GetErrorY(i - 1);
      double y = g2->GetPointY(i - 1);
      double erry = g2->GetErrorY(i - 1);

      double rhomean = (x/pow(errx, 2) + y/pow(erry, 2)) / (1./pow(errx, 2) + 1./pow(erry, 2));
      double rhorms = 1. / sqrt((1./pow(errx, 2) + 1./pow(erry, 2)));
      g.AddPoint(i * 10. - 5, rhomean);
      g.SetPointError(i - 1, 0, rhorms);

      // cout << TMath::Sqrt(rhorms / (( kNSample - nSkip) * (( kNSample - nSkip) - 1))) << "\n";

      hSys[i - 1]->Fill(rhomean);
      if (iVar == 1012){
        gRho.AddPoint(i * 10. - 5., rhomean);
        gRhoSys.AddPoint(i * 10. - 5., rhomean);
        gRho.SetPointError(i - 1, 0, rhorms);
      }
    }

    TCanvas c(Form("c_%d", iVar), Form("c_%d", iVar));
    c.cd();
    //g.Draw();
    f.cd();
    //g.Write();
   // c.Write();
  }
  for (int i{0}; i < 8; ++i){
    hSys[i]->Write();
    gRhoSys.SetPointError(i, 2, hSys[i]->GetStdDev());
    hSys[i]->SetTitle(Form("%d-%d", i * 10, (i + 1) * 10));
    cSys.cd(i + 1);
    hSys[i]->Fit("gaus", "LM+");
    hSys[i]->Draw("pe");
  }
  gRho.Write();
  gRhoSys.Write();
  TCanvas c("c", "c");
  gRhoSys.SetLineColor(kRed);
  gRhoSys.SetLineWidth(2);
  gRho.SetLineColor(kRed);
  gRho.SetLineWidth(2);
  gRho.SetMarkerColor(kRed);
  gRho.SetMarkerStyle(20);
  gRho.SetMarkerSize(1.5);
  c.cd();
  hFrame.SetStats(0);
  hFrame.Draw();
  gRhoSys.Draw("pe5same");
  gRho.Draw("pesame");
  c.Write();
  cSys.Write();
  f.Close();
}

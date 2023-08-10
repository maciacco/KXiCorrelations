#include "../utils/Config.h"
const int kNSample = 30;
const int kKcut = 67;
const int kXiCut = 7;
const int kNCentBinsAnalysis = 9;
const int kMultClasses[][2] = {{0, 5}, {5, 10}, {10, 20}, {20, 30}, {30, 40}, {40, 50}, {50, 70}, {70, 100}};

const bool kRho = false;
const bool kC2c1 = true;

void remove_outlier(TH1D* h, double reject_level = 3.){
  double mean = h->GetMean();
  double std_dev = h->GetStdDev();
  for (int iB{1}; iB <= h->GetNbinsX(); ++iB){
    if (std::abs(h->GetBinCenter(iB) - mean) > reject_level * std_dev){
      h->SetBinContent(iB, 0.);
    }
  }
}

void Analysis_pp_finalBinning_c2c1K()
{
  gStyle->SetOptStat(0);
  TFile f(kC2c1 ? "out_sys_17_finalBinning_c2c1_k.root" : "out_sys_17_finalBinning.root", "recreate");
  TH1D *hSys[kNCentBinsAnalysis];
  TH1D *hStat[kNCentBinsAnalysis];
  TGraphErrors gRho;
  TGraphErrors gRelativeStat;
  gRelativeStat.SetName("gRelativeStat");
  gRelativeStat.SetTitle(";Centrality (%);relative stat");
  gRho.SetName("gRho");
  gRho.SetTitle(";Centrality (%);#rho_{#Delta#XiDeltaK}");
  TGraphErrors gRhoSys;
  gRhoSys.SetName("gRhoSys");
  TCanvas cSys("cSys", "cSys", 600, 600);
  cSys.Divide(3, 3);
  for (int i{0}; i < kNCentBinsAnalysis; ++i){
    if (kRho) hSys[i] = new TH1D(Form("hSys_%d", i), ";#rho;Entries", 45, -1, -1/* 250, -0.05, 0. *//* 100000, -1.0, 1.0 */);
    else if (kC2c1) hSys[i] = new TH1D(Form("hSys_%d", i), ";C_{2}/C_{1};Entries", /* 10, -1, -1 */i == 7 ? 4000 : 3000, 0.5, 1.5);
  }
  for (int i{0}; i < kNCentBinsAnalysis; ++i){
    if (kRho) hStat[i] = new TH1D(Form("hStat_%d", i), ";#rho;Entries", 500, -0.5, 0.5);
    if (kC2c1) hStat[i] = new TH1D(Form("hStat_%d", i), ";C_{2}/C_{1};Entries", 500, -1., 3.);
  }
  for(int iVar = 0; iVar < 60; ++iVar)
  {
    //if ((iVar/3/5/2/2/3)%11>8) continue;
    // if ( (((iVar%60)/60.) > 0.001)  && kC2c1 ) continue;
    // if (((iVar/15)%3)==0) continue;
    //if ((iVar/15)%2==1) continue;
   // if ((iVar/135)%3 == 2) continue;
    //if ((iVar/90)%3 == 0) continue;
   // if (((iVar%45)/45.) > 0.01) continue;
    //if ((iVar/135/3)%5 > 2) continue;
    double iB0 = 0.;
    double iB1 = 0.;
    double nSkip = 0;
    double c11_pp[kNSample][10], c11_pn[kNSample][10], c11_np[kNSample][10], c11_nn[kNSample][10], c11_pn_kk[kNSample][10], c11_pn_xixi[kNSample][10], c1xi_p[kNSample][10], c1xi_n[kNSample][10], c2xi_p[kNSample][10], c2xi_n[kNSample][10], c1k_p[kNSample][10], c1k_n[kNSample][10], c2k_p[kNSample][10], c2k_n[kNSample][10], c2k_pn[kNSample][10], c2xi_pn[kNSample][10];
    for(int sample = 0; sample < kNSample; sample++)
    {
      TFile *fin = new TFile(Form("%s/output_sys_dir/17pq_3/output_sys_17pq_3_%d_%d.root", kResDir, sample, iVar));
      if (!fin) {std::cout << "skip = " << iVar << std::endl; nSkip++; fin->Close(); delete fin; continue;}
      //TFile *fin_same=new TFile(Form("o%d.root",sample));

      TProfile *q1xikaon_pp = (TProfile*)fin->Get(Form("var_%d/q1_xi_kaon_pp", iVar));
      TProfile *q1xikaon_pn = (TProfile*)fin->Get(Form("var_%d/q1_xi_kaon_pn", iVar));
      TProfile *q1xikaon_np = (TProfile*)fin->Get(Form("var_%d/q1_xi_kaon_np", iVar));
      TProfile *q1xikaon_nn = (TProfile*)fin->Get(Form("var_%d/q1_xi_kaon_nn", iVar));
      TProfile *q1xixi_pn = (TProfile*)fin->Get(Form("var_%d/q1_xi_xi_pn", iVar));
      TProfile *q1kk_pn = (TProfile*)fin->Get(Form("var_%d/q1_kaon_kaon_pn", iVar));
      TProfile *q1xi_p = (TProfile*)fin->Get(Form("var_%d/q1_xi_p", iVar));
      TProfile *q2xi_p = (TProfile*)fin->Get(Form("var_%d/q2_xi_p", iVar));
      TProfile *q1xi_n = (TProfile*)fin->Get(Form("var_%d/q1_xi_n", iVar));
      TProfile *q2xi_n = (TProfile*)fin->Get(Form("var_%d/q2_xi_n", iVar));
      TProfile *q1squarexi_p = (TProfile*)fin->Get(Form("var_%d/q1square_xi_p", iVar));
      TProfile *q1squarexi_n = (TProfile*)fin->Get(Form("var_%d/q1square_xi_n", iVar));
      TProfile *q1kaon_p = (TProfile*)fin->Get(Form("var_%d/q1_kaon_p", iVar));
      TProfile *q2kaon_p = (TProfile*)fin->Get(Form("var_%d/q2_kaon_p", iVar));
      TProfile *q1kaon_n = (TProfile*)fin->Get(Form("var_%d/q1_kaon_n", iVar));
      TProfile *q2kaon_n = (TProfile*)fin->Get(Form("var_%d/q2_kaon_n", iVar));
      TProfile *q1squarekaon_p = (TProfile*)fin->Get(Form("var_%d/q1square_kaon_p", iVar));
      TProfile *q1squarekaon_n = (TProfile*)fin->Get(Form("var_%d/q1square_kaon_n", iVar));

      double xbins[] = {0., 5., 10., 20., 30., 40., 50., 70., 100.};

      if (!q1xikaon_pp || !q1xikaon_pn || !q1xikaon_np || !q1xikaon_nn || !q1xixi_pn || !q1kk_pn || !q1xi_n || !q1xi_p || !q2xi_n || !q2xi_p || !q1squarexi_n || !q1squarexi_p || !q1kaon_n || !q1kaon_p || !q2kaon_n || !q2kaon_p || !q1squarekaon_n || !q1squarekaon_p){
        std::cout << "skip = " << iVar << std::endl; nSkip++; fin->Close(); delete fin; continue;
      }

      // iB0 += q1xikaon_pp->GetBinEntries(1);
      // iB1 += q1xikaon_pp->GetBinEntries(2);

      for(int i=1;i<=kNCentBinsAnalysis;i++)
      {
        c11_pp[sample][i-1] = q1xikaon_pp->GetBinContent(i) - q1xi_p->GetBinContent(i) * q1kaon_p->GetBinContent(i);
        c11_pn[sample][i-1] = q1xikaon_pn->GetBinContent(i) - q1xi_p->GetBinContent(i) * q1kaon_n->GetBinContent(i);
        c11_np[sample][i-1] = q1xikaon_np->GetBinContent(i) - q1xi_n->GetBinContent(i) * q1kaon_p->GetBinContent(i);
        c11_nn[sample][i-1] = q1xikaon_nn->GetBinContent(i) - q1xi_n->GetBinContent(i) * q1kaon_n->GetBinContent(i);
        c11_pn_xixi[sample][i-1] = q1xixi_pn->GetBinContent(i) - q1xi_p->GetBinContent(i) * q1xi_n->GetBinContent(i);
        c11_pn_kk[sample][i-1] = q1kk_pn->GetBinContent(i) - q1kaon_p->GetBinContent(i) * q1kaon_n->GetBinContent(i);
        c1xi_p[sample][i-1] = q1xi_p->GetBinContent(i);
        c1k_p[sample][i-1] = q1kaon_p->GetBinContent(i);
        c1xi_n[sample][i-1] = q1xi_n->GetBinContent(i);
        c1k_n[sample][i-1] = q1kaon_n->GetBinContent(i);
        c2xi_p[sample][i-1] = q1squarexi_p->GetBinContent(i) - TMath::Power(q1xi_p->GetBinContent(i), 2.0) + q1xi_p->GetBinContent(i) - q2xi_p->GetBinContent(i);
        c2k_p[sample][i-1] = q1squarekaon_p->GetBinContent(i) - TMath::Power(q1kaon_p->GetBinContent(i), 2.0) + q1kaon_p->GetBinContent(i) - q2kaon_p->GetBinContent(i);
        c2xi_n[sample][i-1] = q1squarexi_n->GetBinContent(i) - TMath::Power(q1xi_n->GetBinContent(i), 2.0) + q1xi_n->GetBinContent(i) - q2xi_n->GetBinContent(i);
        c2k_n[sample][i-1] = q1squarekaon_n->GetBinContent(i) - TMath::Power(q1kaon_n->GetBinContent(i), 2.0) + q1kaon_n->GetBinContent(i) - q2kaon_n->GetBinContent(i);
        c2xi_pn[sample][i-1] = c2xi_p[sample][i-1] + c2xi_n[sample][i-1] - 2 * c11_pn_xixi[sample][i-1];
        c2k_pn[sample][i-1] = c2k_p[sample][i-1] + c2k_n[sample][i-1] - 2 * c11_pn_kk[sample][i-1];
        //cout<<c1xi[sample][i-1]<<"\t"<<c2xi[sample][i-1]<<"\t"<<c2k[sample][i-1]<<"\t";
        //cout<<c11[sample][i-1]<<"\n";
      }

      fin->Close();
      delete fin;
    }
    
    TGraphErrors g;
    g.SetName(Form("g_%d", iVar));
    for(int i = 1; i < kNCentBinsAnalysis; i++)
    {
      double rhomean = 0.0;
      double rhorms = 0.0;
      for(int sample = 0; sample < kNSample; sample++)
      {
        if (kRho) rhomean = rhomean + ( /* (c1xi_p[sample][i-1] + c1xi_n[sample][i-1]) / (c1k_p[sample][i-1] + c1k_n[sample][i-1]) ) */( (c11_pp[sample][i-1] + c11_nn[sample][i-1] - c11_pn[sample][i-1] - c11_np[sample][i-1]) / sqrt(c2xi_pn[sample][i-1] * c2k_pn[sample][i-1]) ));
        else if (kC2c1) rhomean = rhomean + ( ( c2k_pn[sample][i-1] ) / ( c1k_n[sample][i-1] + c1k_p[sample][i-1] ));
        //if (iVar == 712) hStat[i - 1]->Fill((c11_pp[sample][i-1] + c11_nn[sample][i-1] - c11_pn[sample][i-1] - c11_np[sample][i-1]) / sqrt(c2xi_pn[sample][i-1] * c2k_pn[sample][i-1]));
        if (iVar == (kC2c1 ? 52 : 1012)) hStat[i - 1]->Fill(( c2k_pn[sample][i-1] ) / ( c1k_n[sample][i-1] + c1k_p[sample][i-1] ));
      }
      rhomean = rhomean / ( kNSample - nSkip);

      // if (i == 5 && rhomean < -0.012){
      //   std::cout << "var = " << iVar << std::endl;
      // }

      // cout << rhomean << "\t";
      for(int sample = 0; sample < kNSample; sample++)
      {
        if (kRho) rhorms = rhorms + TMath::Power(rhomean - ( /* (c1xi_p[sample][i-1] + c1xi_n[sample][i-1]) / (c1k_p[sample][i-1] + c1k_n[sample][i-1]) ) */( (c11_pp[sample][i-1] + c11_nn[sample][i-1] - c11_pn[sample][i-1] - c11_np[sample][i-1]) / sqrt(c2xi_pn[sample][i-1] * c2k_pn[sample][i-1]) )), 2.0);
        else if (kC2c1) rhorms = rhorms + TMath::Power(rhomean - ( ( c2k_pn[sample][i-1] ) / ( c1k_n[sample][i-1] + c1k_p[sample][i-1] )), 2.0);
      }
      // cout << TMath::Sqrt(rhorms / (( kNSample - nSkip) * (( kNSample - nSkip) - 1))) << "\n";

      // if (i == 2){
      //   rhomean = (rhomean / iB1 + g.GetPointY(0) / iB0) / (1. / iB0 + 1. / iB1);
      // }
      // if (i == 2){
      //   rhorms = 1. / (1. / iB0 + 1. / iB1) / (1. / iB0 + 1. / iB1) * ( rhorms * (1. / iB1) * (1. / iB1) + (g.GetErrorY(0) * g.GetErrorY(0)) * ( kNSample - nSkip) * (( kNSample - nSkip) - 1) * (1. / iB0) * (1. / iB0));
      // }

      // if ( !(kC2c1 && ( (i == 8 && (iVar/60)%3 > 1)) )){
      //   if (!(kC2c1 && rhomean > 1.)){
      //     //std::cout << "var = " << (iVar/60)%3 << " " << (iVar/60/3)%11 << std::endl;
      //     g.AddPoint(0.5 * (kMultClasses[i - 1][1] + kMultClasses[i - 1][0]), rhomean);
      //     hSys[i - 1]->Fill(rhomean);
      //   }
      // }
      
      g.AddPoint(0.5 * (kMultClasses[i - 1][1] + kMultClasses[i - 1][0]), rhomean);
      hSys[i - 1]->Fill(rhomean);
      
      //if (rhomean < -0.023 && i == 2) cout << iVar << std::endl;
      g.SetPointError(i - 1, 0, i == 2 ? TMath::Sqrt(rhorms / (( kNSample - nSkip) * ( ( kNSample - nSkip) - 1))) : TMath::Sqrt(rhorms / (( kNSample - nSkip) * (( kNSample - nSkip) - 1))));
      if (iVar == (kC2c1 ? 52 : 1012)){
        gRho.AddPoint(0.5 * (kMultClasses[i - 1][1] + kMultClasses[i - 1][0]), rhomean);
        gRhoSys.AddPoint(0.5 * (kMultClasses[i - 1][1] + kMultClasses[i - 1][0]), rhomean);
        gRho.SetPointError(i - 1, 0, i == 2 ? TMath::Sqrt(rhorms / ( ( kNSample - nSkip) * (( kNSample - nSkip) - 1))) : TMath::Sqrt(rhorms / (( kNSample - nSkip) * (( kNSample - nSkip) - 1))));
        gRelativeStat.AddPoint(i, i == 2 ? TMath::Sqrt(rhorms / (( kNSample - nSkip) * (( kNSample - nSkip) - 1))) / std::abs(rhomean) : TMath::Sqrt(rhorms / (( kNSample - nSkip) * (( kNSample - nSkip) - 1))) / std::abs(rhomean));
      }
    }

    TCanvas c(Form("c_%d", iVar), Form("c_%d", iVar));
    c.cd();
    //g.Draw();
    f.cd();
    g.Write();
    
    // g.RemovePoint(0);
    // if (iVar == (kC2c1 ? 210 : 1012)){
    //   gRho.RemovePoint(0);
    //   gRhoSys.RemovePoint(0);
    //   gRelativeStat.RemovePoint(0);
    // }
   // c.Write();
  }

  TLine *line[kNCentBinsAnalysis];
  TH1D* hSysFrame[kNCentBinsAnalysis];
  for (int i{0}; i < kNCentBinsAnalysis - 1; ++i){
    line[i] = new TLine(gRho.GetPointY(i), 0, gRho.GetPointY(i), hSys[i]->GetBinContent(hSys[i]->GetMaximumBin()));
    hSysFrame[i] = new TH1D(Form("hSysFrame_%d", i), kC2c1 ? ";#it{C}_{2}/#it{C}_{1};Entries" : ";#it{#rho};Entries", 1000000, -0.1, 1.1);
    remove_outlier(hSys[i]);
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
    hSysFrame[i]->SetTitle(Form("%d-%d%%",  kMultClasses[i][0], kMultClasses[i][1]));
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
    t.DrawLatex(0.7, 0.7, Form("#sigma = %.5f", hSys[i]->GetStdDev()));
    line[i]->SetLineWidth(2);
    line[i]->SetLineStyle(7);
    line[i]->SetLineColor(kRed);
    line[i]->Draw("same");
  }
  
  gRho.Write();
  gRhoSys.Write();
  gRelativeStat.Write();
  TCanvas c("c", "c");
  c.cd();
  gRhoSys.SetLineWidth(2);
  gRho.SetLineWidth(2);
  gRho.SetMarkerStyle(20);
  gRho.SetMarkerSize(1.1);
  gRhoSys.SetLineColor(kRed);
  gRho.SetMarkerColor(kRed);
  gRho.SetLineColor(kRed);
  gRhoSys.Draw("ape5");
  gRhoSys.GetXaxis()->SetTitle("Multiplicity (%)");
  if (kC2c1) gRhoSys.GetYaxis()->SetTitle("C_{2}(#Xi^{-}-#Xi^{+})/C_{1}(#Xi^{-}+#Xi^{+})");
  else if (kRho) gRhoSys.GetYaxis()->SetTitle("#rho(#Delta#Xi#DeltaK)");
  gRho.Draw("pesame");
  c.Print("c1xin_pp.pdf");
  c.Write();
  cSys.Write();
  cSys.Print(kC2c1 ? "cSys_pp_c2c1.pdf" : "cSys_pp.pdf");
  f.Close();
}

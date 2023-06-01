#include "../utils/Config.h"
const int kNSample = 30;
const int kKcut = 67;
const int kXiCut = 7;
const int kNCentBinsAnalysis = 8;

void remove_outlier(TH1D* h, double reject_level = 3.){
  double mean = h->GetMean();
  double std_dev = h->GetStdDev();
  for (int iB{1}; iB <= h->GetNbinsX(); ++iB){
    if (std::abs(h->GetBinCenter(iB) - mean) > reject_level * std_dev){
      h->SetBinContent(iB, 0.);
    }
  }
}

void Analysis_pp()
{
  TFile f("out_prova_sys_17_new_2.root", "recreate");
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
  TCanvas cSys("cSys", "cSys");
  cSys.Divide(3, 3);
  for (int i{0}; i < kNCentBinsAnalysis; ++i){
    hSys[i] = new TH1D(Form("hSys_%d", i), ";#rho;Entries", 100, -0.04, 0.0);
  }
  for (int i{0}; i < kNCentBinsAnalysis; ++i){
    hStat[i] = new TH1D(Form("hStat_%d", i), ";#rho;Entries", 500, -0.5, 0.5);
  }
  for(int iVar = 0; iVar < 2025; ++iVar)
  {
    //if ((iVar%135)/15/3 == 2) continue;
    double iB0 = 0.;
    double iB1 = 0.;
    double nSkip = 0;
    double c11_pp[kNSample][10], c11_pn[kNSample][10], c11_np[kNSample][10], c11_nn[kNSample][10], c11_pn_kk[kNSample][10], c11_pn_xixi[kNSample][10], c1xi_p[kNSample][10], c1xi_n[kNSample][10], c2xi_p[kNSample][10], c2xi_n[kNSample][10], c1k_p[kNSample][10], c1k_n[kNSample][10], c2k_p[kNSample][10], c2k_n[kNSample][10], c2k_pn[kNSample][10], c2xi_pn[kNSample][10];
    for(int sample = 0; sample < kNSample; sample++)
    {
      TFile *fin = new TFile(Form("%s/output_sys_dir/17pq_new/output_sys_17pq_new_%d_%d.root", kResDir, sample, iVar));
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

      double xbins[] = {0., 10., 20., 30., 40., 50., 70., 100.};
      q1xikaon_pp = (TProfile*)q1xikaon_pp->Rebin(7, Form("var_%d/q1_xi_kaon_pp", iVar), xbins);
      q1xikaon_pn = (TProfile*)q1xikaon_pn->Rebin(7, Form("var_%d/q1_xi_kaon_pn", iVar), xbins);
      q1xikaon_np = (TProfile*)q1xikaon_np->Rebin(7, Form("var_%d/q1_xi_kaon_np", iVar), xbins);
      q1xikaon_nn = (TProfile*)q1xikaon_nn->Rebin(7, Form("var_%d/q1_xi_kaon_nn", iVar), xbins);
      q1xixi_pn = (TProfile*)q1xixi_pn->Rebin(7, Form("var_%d/q1_xi_xi_pn", iVar), xbins);
      q1kk_pn = (TProfile*)q1kk_pn->Rebin(7, Form("var_%d/q1_kaon_kaon_pn", iVar), xbins);
      q1xi_p = (TProfile*)q1xi_p->Rebin(7, Form("var_%d/q1_xi_p", iVar), xbins);
      q2xi_p = (TProfile*)q2xi_p->Rebin(7, Form("var_%d/q2_xi_p", iVar), xbins);
      q1xi_n = (TProfile*)q1xi_n->Rebin(7, Form("var_%d/q1_xi_n", iVar), xbins);
      q2xi_n = (TProfile*)q2xi_n->Rebin(7, Form("var_%d/q2_xi_n", iVar), xbins);
      q1squarexi_p = (TProfile*)q1squarexi_p->Rebin(7, Form("var_%d/q1square_xi_p", iVar), xbins);
      q1squarexi_n = (TProfile*)q1squarexi_n->Rebin(7, Form("var_%d/q1square_xi_n", iVar), xbins);
      q1kaon_p = (TProfile*)q1kaon_p->Rebin(7, Form("var_%d/q1_kaon_p", iVar), xbins);
      q2kaon_p = (TProfile*)q2kaon_p->Rebin(7, Form("var_%d/q2_kaon_p", iVar), xbins);
      q1kaon_n = (TProfile*)q1kaon_n->Rebin(7, Form("var_%d/q1_kaon_n", iVar), xbins);
      q2kaon_n = (TProfile*)q2kaon_n->Rebin(7, Form("var_%d/q2_kaon_n", iVar), xbins);
      q1squarekaon_p = (TProfile*)q1squarekaon_p->Rebin(7, Form("var_%d/q1square_kaon_p", iVar), xbins);
      q1squarekaon_n = (TProfile*)q1squarekaon_n->Rebin(7, Form("var_%d/q1square_kaon_n", iVar), xbins);

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
        rhomean = rhomean + ( (c11_pp[sample][i-1] + c11_nn[sample][i-1] - c11_pn[sample][i-1] - c11_np[sample][i-1]) / sqrt(c2xi_pn[sample][i-1] * c2k_pn[sample][i-1]) );
        if (iVar == 1012) hStat[i - 1]->Fill((c11_pp[sample][i-1] + c11_nn[sample][i-1] - c11_pn[sample][i-1] - c11_np[sample][i-1]) / sqrt(c2xi_pn[sample][i-1] * c2k_pn[sample][i-1]));
      }
      rhomean = rhomean / ( kNSample - nSkip);

      // cout << rhomean << "\t";
      for(int sample = 0; sample < kNSample; sample++)
      {
        rhorms = rhorms + TMath::Power(rhomean - ( (c11_pp[sample][i-1] + c11_nn[sample][i-1] - c11_pn[sample][i-1] - c11_np[sample][i-1]) / sqrt(c2xi_pn[sample][i-1] * c2k_pn[sample][i-1]) ), 2.0);
      }
      // cout << TMath::Sqrt(rhorms / (( kNSample - nSkip) * (( kNSample - nSkip) - 1))) << "\n";

      // if (i == 2){
      //   rhomean = (rhomean / iB1 + g.GetPointY(0) / iB0) / (1. / iB0 + 1. / iB1);
      // }
      // if (i == 2){
      //   rhorms = 1. / (1. / iB0 + 1. / iB1) / (1. / iB0 + 1. / iB1) * ( rhorms * (1. / iB1) * (1. / iB1) + (g.GetErrorY(0) * g.GetErrorY(0)) * ( kNSample - nSkip) * (( kNSample - nSkip) - 1) * (1. / iB0) * (1. / iB0));
      // }

      g.AddPoint(i * 10. - 5., rhomean);
      hSys[i - 1]->Fill(rhomean);
      //if (rhomean < -0.01 && i == 6) cout << iVar << std::endl;
      g.SetPointError(i - 1, 0, i == 2 ? TMath::Sqrt(rhorms / (( kNSample - nSkip) * ( ( kNSample - nSkip) - 1))) : TMath::Sqrt(rhorms / (( kNSample - nSkip) * (( kNSample - nSkip) - 1))));
      if (iVar == 1012){
        gRho.AddPoint(i * 10. - 5., rhomean);
        gRhoSys.AddPoint(i * 10. - 5., rhomean);
        gRho.SetPointError(i - 1, 0, i == 2 ? TMath::Sqrt(rhorms / ( ( kNSample - nSkip) * (( kNSample - nSkip) - 1))) : TMath::Sqrt(rhorms / (( kNSample - nSkip) * (( kNSample - nSkip) - 1))));
        gRelativeStat.AddPoint(i, i == 2 ? TMath::Sqrt(rhorms / (( kNSample - nSkip) * (( kNSample - nSkip) - 1))) / std::abs(rhomean) : TMath::Sqrt(rhorms / (( kNSample - nSkip) * (( kNSample - nSkip) - 1))) / std::abs(rhomean));
      }
    }

    TCanvas c(Form("c_%d", iVar), Form("c_%d", iVar));
    c.cd();
    //g.Draw();
    f.cd();
    // g.Write();
    
    // g.RemovePoint(0);
    // if (iVar == 1012){
    //   gRho.RemovePoint(0);
    //   gRhoSys.RemovePoint(0);
    //   gRelativeStat.RemovePoint(0);
    // }
   // c.Write();
  }

  for (int i{0}; i < 7; ++i){
    remove_outlier(hSys[i]);
    hSys[i]->Write();
    gRhoSys.SetPointError(i, 2, hSys[i]->GetStdDev());
    hSys[i]->SetTitle(Form("mult. class %d", i));
    cSys.cd(i + 1);
    //hSys[i]->Fit("gaus", "LM+");
    hSys[i]->Draw("");
    hStat[i]->Write();
  }

  
  gRho.Write();
  gRhoSys.Write();
  gRelativeStat.Write();
  TCanvas c("c", "c");
  c.cd();
  gRhoSys.Draw("ape5");
  gRho.Draw("pesame");
  c.Write();
  cSys.Write();
  f.Close();
}

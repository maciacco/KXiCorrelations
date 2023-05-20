#include "../utils/Config.h"
const int kNSample = 10;
const int kKcut = 68;
const int kXiCut = 8;

void Analysis()
{
  TFile f("out_prova_sys.root", "recreate");
  TH1D *hSys[9];
  TGraphErrors gRho;
  gRho.SetName("gRho");
  gRho.SetTitle(";Centrality (%);#rho_{#Delta#XiDeltaK}");
  TGraphErrors gRhoSys;
  gRhoSys.SetName("gRhoSys");
  for (int i{0}; i < 9; ++i){
    hSys[i] = new TH1D(Form("hSys_%d", i), ";#rho;Entries", 1000, -0.1, 0.1);
  }
  for(int iVar = 0; iVar < 2025; ++iVar)
  {
    double c11_pp[32][10], c11_pn[32][10], c11_np[32][10], c11_nn[32][10], c11_pn_kk[32][10], c11_pn_xixi[32][10], c1xi_p[32][10], c1xi_n[32][10], c2xi_p[32][10], c2xi_n[32][10], c1k_p[32][10], c1k_n[32][10], c2k_p[32][10], c2k_n[32][10], c2k_pn[32][10], c2xi_pn[32][10];
    for(int sample = 0; sample < kNSample; sample++)
    {
      TFile *fin = new TFile(Form("%s/output_sys_dir/output_sys_15o_%d_%d.root", kResDir, sample, iVar));
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

      for(int i=1;i<=9;i++)
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
    }
    
    TGraphErrors g;
    g.SetName(Form("g_%d", iVar));
    for(int i = 1; i < 9; i++)
    {
      double rhomean = 0.0;
      double rhorms = 0.0;
      for(int sample = 0; sample < kNSample; sample++)
      {
        rhomean = rhomean + ( (c11_pp[sample][i-1] + c11_nn[sample][i-1] - c11_pn[sample][i-1] - c11_np[sample][i-1]) / sqrt(c2xi_pn[sample][i-1] * c2k_pn[sample][i-1]) );
      }
      rhomean = rhomean / kNSample;
      cout << rhomean << "\t";
      for(int sample = 0; sample < kNSample; sample++)
      {
        rhorms = rhorms + TMath::Power(rhomean - ( (c11_pp[sample][i-1] + c11_nn[sample][i-1] - c11_pn[sample][i-1] - c11_np[sample][i-1]) / sqrt(c2xi_pn[sample][i-1] * c2k_pn[sample][i-1]) ), 2.0);
      }
      cout << TMath::Sqrt(rhorms / (kNSample * (kNSample - 1))) << "\n";

      g.AddPoint(i * 10. - 5., rhomean);
      hSys[i - 1]->Fill(rhomean);
      g.SetPointError(i - 1, 0, TMath::Sqrt(rhorms / (kNSample * (kNSample - 1))));
      if (iVar == 1012){
        gRho.AddPoint(i * 10. - 5., rhomean);
        gRhoSys.AddPoint(i * 10. - 5., rhomean);
        gRho.SetPointError(i - 1, 0, TMath::Sqrt(rhorms / (kNSample * (kNSample - 1))));
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
  }
  gRho.Write();
  gRhoSys.Write();
  TCanvas c("c", "c");
  c.cd();
  gRhoSys.Draw("ape5");
  gRho.Draw("pesame");
  c.Write();
  f.Close();
}
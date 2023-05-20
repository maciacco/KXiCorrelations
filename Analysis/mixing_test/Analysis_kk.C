void Analysis_kk()
{
  double c11[32][10],c1xi[32][10],c2xi[32][10],c1k[32][10],c2k[32][10],c2net[32][10];
  for(int sample=0;sample<32;sample++)
    {
      if (sample == 28) continue;
      TFile *fin=new TFile(Form("output_mixd_kk2_sample%d.root",sample));
      //TFile *fin_same=new TFile(Form("o%d.root",sample));
      TProfile *q1xikaon=(TProfile*)fin->Get("q1_kaon_kaon");
      TProfile *q1xi=(TProfile*)fin->Get("q1_kaon_neg");
      TProfile *q1kaon=(TProfile*)fin->Get("q1_kaon_pos");
      TProfile *q2xi=(TProfile*)fin->Get("q2_kaon_neg");
      TProfile *q2kaon=(TProfile*)fin->Get("q2_kaon_pos");
      TProfile *q1squarexi=(TProfile*)fin->Get("q1square_kaon_neg");
      TProfile *q1squarekaon=(TProfile*)fin->Get("q1square_kaon_pos");
      for(int i=1;i<=9;i++)
	{
	  c11[sample][i-1]=q1xikaon->GetBinContent(i)-q1xi->GetBinContent(i)*q1kaon->GetBinContent(i);
	  c1xi[sample][i-1]=q1xi->GetBinContent(i);
	  c1k[sample][i-1]=q1kaon->GetBinContent(i);
	  c2xi[sample][i-1]=q1squarexi->GetBinContent(i)-TMath::Power(q1xi->GetBinContent(i),2.0)+q1xi->GetBinContent(i)-q2xi->GetBinContent(i);
	  c2k[sample][i-1]=q1squarekaon->GetBinContent(i)-TMath::Power(q1kaon->GetBinContent(i),2.0)+q1kaon->GetBinContent(i)-q2kaon->GetBinContent(i);
    c2net[sample][i-1]=c2k[sample][i-1]+c2xi[sample][i-1]-2*c11[sample][i-1];
	  //cout<<c1xi[sample][i-1]<<"\t"<<c2xi[sample][i-1]<<"\t"<<c2k[sample][i-1]<<"\t";
    //cout<<c11[sample][i-1]<<"\n";
	}
    }
  
  
  TGraphErrors g;
  for(int i=1;i<9;i++)
    {
      double rhomean=0.0;
      double rhorms=0.0;
      for(int sample=0;sample<32;sample++)
	{
    if (sample==28)continue;
    //rhomean=rhomean+(c2net[sample][i-1]/(c1xi[sample][i-1]+c1k[sample][i-1]));
	  rhomean=rhomean+(c11[sample][i-1]/(c1xi[sample][i-1]*c1k[sample][i-1]));
    //rhomean=rhomean+((c1k[sample][i-1]*c2xi[sample][i-1]/c1xi[sample][i-1] + c1xi[sample][i-1]*c2k[sample][i-1]/c1k[sample][i-1] - 2*c11[sample][i-1])/(c1k[sample][i-1] + c1xi[sample][i-1]));
	}
      rhomean=rhomean/31.0;
      cout<<rhomean<<"\t";
      for(int sample=0;sample<32;sample++)
	{

    if (sample==28)continue;
    //rhorms=rhorms+TMath::Power(rhomean-(c2net[sample][i-1]/(c1xi[sample][i-1]+c1k[sample][i-1])),2.0);
	  rhorms=rhorms+TMath::Power(rhomean-(c11[sample][i-1]/(c1xi[sample][i-1]*c1k[sample][i-1])),2.0);
    //rhorms=rhorms+TMath::Power(rhomean-((c1k[sample][i-1]*c2xi[sample][i-1]/c1xi[sample][i-1] + c1xi[sample][i-1]*c2k[sample][i-1]/c1k[sample][i-1] - 2*c11[sample][i-1])/(c1k[sample][i-1] + c1xi[sample][i-1])),2.0);
	}
      cout<<TMath::Sqrt(rhorms/(31.*30.0))<<"\n";

      g.AddPoint(i*10.-5., rhomean);
      g.SetPointError(i - 1, 0, TMath::Sqrt(rhorms/(31.0*30.0)));
    }

  TCanvas c("c", "c");
  c.cd();
  g.GetYaxis()->SetTitle("C_{11}(K^{+}#Xi^{-})/[C_{1}(K^{+}C_{1}(#Xi^{-}))]");
  //g.GetYaxis()->SetTitle("#Sigma[K^{+}#Xi^{-}]");
  g.GetXaxis()->SetTitle("Centrality (%)");
  g.Fit("pol0", "R", "", 0, 90);
  g.Draw();
  TFile f("out_mixedev_smallBin_test_mixSame.root", "recreate");
  g.Write();
  c.Write();
  f.Close();

}

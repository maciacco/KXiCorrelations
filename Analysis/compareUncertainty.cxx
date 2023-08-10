void compareUncertainty(){
  gStyle->SetOptStat(0);

  TFile fSHM_BS_1("models/Output_Final_BS_vc1.0_CE.root");
  TFile fSHM_BS_2("models/Output_Final_BS_vc2.0_CE.root");
  TFile fSHM_BS_3("models/Output_Final_BS_vc3.0_CE.root");
  TFile fSHM_BS_4("models/Output_Final_BS_vc4.0_CE.root");
  TFile fSHM_BS_1_5("models/Output_Final_BS_vc1.5_CE.root");
  TFile fSHM_BS_2_5("models/Output_Final_BS_vc2.5_CE.root");
  TFile fSHM_BS_3_5("models/Output_Final_BS_vc3.5_CE.root");

  TGraphErrors *gSHM_BS[7];

  gSHM_BS[0] = (TGraphErrors*)fSHM_BS_1.Get("Grrho");
  gSHM_BS[1] = (TGraphErrors*)fSHM_BS_1_5.Get("Grrho");
  gSHM_BS[2] = (TGraphErrors*)fSHM_BS_2.Get("Grrho");
  gSHM_BS[3] = (TGraphErrors*)fSHM_BS_2_5.Get("Grrho");
  gSHM_BS[4] = (TGraphErrors*)fSHM_BS_3.Get("Grrho");
  gSHM_BS[5] = (TGraphErrors*)fSHM_BS_3_5.Get("Grrho");
  gSHM_BS[6] = (TGraphErrors*)fSHM_BS_4.Get("Grrho");

  gSHM_BS[0]->SetLineWidth(2);
  gSHM_BS[0]->SetLineColor(kBlue);

  gSHM_BS[1]->SetLineWidth(2);
  gSHM_BS[1]->SetLineColor(kAzure + 7);

  gSHM_BS[2]->SetLineWidth(2);
  gSHM_BS[2]->SetLineColor(kTeal - 7);

  gSHM_BS[3]->SetLineWidth(2);
  gSHM_BS[3]->SetLineColor(kGreen + 1);

  gSHM_BS[4]->SetLineWidth(2);
  gSHM_BS[4]->SetLineColor(kOrange - 2);

  gSHM_BS[5]->SetLineWidth(2);
  gSHM_BS[5]->SetLineColor(kOrange + 7);

  gSHM_BS[6]->SetLineWidth(2);
  gSHM_BS[6]->SetLineColor(kRed);

  TCanvas c;
  TH2D h("h", ";#LTd#it{N}_{ch}/d#it{y}#GT;Stat.", 1, 0, 1900, 1, 0., 0.002);
  c.cd();
  h.Draw("same");
  for (int i{0}; i < 7; ++i){
    for (int j{0}; j < gSHM_BS[i]->GetN(); ++j){
      gSHM_BS[i]->SetPointY(j, gSHM_BS[i]->GetErrorY(j));
      gSHM_BS[i]->SetPointError(j, 0., 0.);
    }
    gSHM_BS[i]->Draw("same");
  }
  c.Print("cSHM.png");
}
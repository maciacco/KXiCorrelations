#include "../utils/Config.h"

//#define CLOSURE_TEST

void get_subsample_average(const int iCent, const TH1* distr, TH1* avg);
void compute_average_variance(double res[2], const double Q1[2], const double Q1Sq[2], const double Q2[2]);
double compute_correlation(const int iC, const double kaonQ1[2], const double kaonQ1Sq[2], const double kaonQ2[2], const double xiQ1[2], const double xiQ1Sq[2], const double xiQ2[2], const double sameKaonXiQ11[2], const double oppKaonXiQ11[2]);

void Correlation (const char* inFileName = "o", const char* outFileName = "out"){
  TFile inFile(Form("%s.root", inFileName));
  TFile outFile(Form("%s.root", outFileName), "recreate");

  TH1D *hCorrCent[2];
  TH1D *hAvgVarKCent[2];
  TH1D *hAvgVarXiCent[2];
  TH1D *hCorrDistr[2][kNCentBins];
  TH1D *hAvgVarKDistr[2][kNCentBins];
  TH1D *hAvgVarXiDistr[2][kNCentBins];
  #ifdef CLOSURE_TEST
    TH1D *hCorrCent_Gen[2];
    TH1D *hAvgVarKCent_Gen[2];
    TH1D *hAvgVarXiCent_Gen[2];
    TH1D *hCorrDistr_Gen[2][kNCentBins];
    TH1D *hAvgVarKDistr_Gen[2][kNCentBins];
    TH1D *hAvgVarXiDistr_Gen[2][kNCentBins];
  #endif // CLOSURE_TEST
  for (int iC = 0; iC < 2; ++iC){
    hCorrCent[iC] = new TH1D(Form("h%sCorrCent", kChargeLabel[iC]), ";Centrality (%);#rho_{#Xi K}", kNCentBins, kCentBins);
    hAvgVarKCent[iC] = new TH1D(Form("h%sKCent", kAvgVarLabel[iC]), Form(";Centrality (%%);C_{%d}^{K}", iC + 1), kNCentBins, kCentBins);
    hAvgVarXiCent[iC] = new TH1D(Form("h%sXiCent", kAvgVarLabel[iC]), Form(";Centrality (%%);C_{%d}^{#Xi}", iC + 1), kNCentBins, kCentBins);
    #ifdef CLOSURE_TEST
      hCorrCent_Gen[iC] = new TH1D(Form("h%sCorrCent_Gen", kChargeLabel[iC]), ";Centrality (%);#rho_{#Xi K}", kNCentBins, kCentBins);
      hAvgVarKCent_Gen[iC] = new TH1D(Form("h%sKCent_Gen", kAvgVarLabel[iC]), Form(";Centrality (%%);C_{%d}^{K}", iC + 1), kNCentBins, kCentBins);
      hAvgVarXiCent_Gen[iC] = new TH1D(Form("h%sXiCent_Gen", kAvgVarLabel[iC]), Form(";Centrality (%%);C_{%d}^{#Xi}", iC + 1), kNCentBins, kCentBins);
    #endif // CLOSUrE_TEST
    for (int iCent = 0; iCent < kNCentBins; ++iCent){
      hCorrDistr[iC][iCent] = new TH1D(Form("h%sCorrDistr_%d", kChargeLabel[iC], iCent), ";#rho_{#Xi K};Entries", 20000, -2., 2.);
      hAvgVarKDistr[iC][iCent] = new TH1D(Form("h%sKDistr_%d", kAvgVarLabel[iC], iCent), Form(";C_{%d}^{K};Entries", iC + 1), 20000, -10000., 10000.);
      hAvgVarXiDistr[iC][iCent] = new TH1D(Form("h%sXiDistr_%d", kAvgVarLabel[iC], iCent), Form(";C_{%d}^{#Xi};Entries", iC + 1), 2000, -1000., 1000.);
      #ifdef CLOSURE_TEST
        hCorrDistr_Gen[iC][iCent] = new TH1D(Form("h%sCorrDistr_Gen_%d", kChargeLabel[iC], iCent), ";#rho_{#Xi K};Entries", 20000, -2., 2.);
        hAvgVarKDistr_Gen[iC][iCent] = new TH1D(Form("h%sKDistr_Gen_%d", kAvgVarLabel[iC], iCent), Form(";C_{%d}^{K};Entries", iC + 1), 20000, -10000., 10000.);
        hAvgVarXiDistr_Gen[iC][iCent] = new TH1D(Form("h%sXiDistr_Gen_%d", kAvgVarLabel[iC], iCent), Form(";C_{%d}^{#Xi};Entries", iC + 1), 2000, -1000., 1000.);
      #endif // CLOSURE_TEST
      for (int iS = 0; iS < N_SAMPLE; ++iS){
        TH3F *hNKaonXi = (TH3F*)inFile.Get(Form("subsample_%d/h%sNKaonXi_%d", iS, kChargeLabel[iC], iS)); // TO BE IMPROVED TO REDUCE NUMBER OF FILE ACCESSES
        double avg_corr = 0.;
        double avg_avg_var_k = 0.;
        double avg_avg_var_xi = 0.;
        double n_ev_tot = 0.;
        TH1D* hCent = (TH1D*)inFile.Get(Form("subsample_%d/hCent_%d", iS, iS));
        double delta = (kCentBins[iCent + 1] - kCentBins[iCent]) / N_SUB_CENT;
        for (int iSubCent = 0; iSubCent < N_SUB_CENT; ++iSubCent){
          hNKaonXi->GetXaxis()->SetRange(hCent->FindBin(kCentBins[iCent] + iSubCent * delta), hCent->FindBin(kCentBins[iCent] + iSubCent * delta));
          TH2F* hNKaonXiProj = (TH2F* )hNKaonXi->Project3D("yz");
          double n_ev = hCent->GetBinContent(hCent->FindBin(kCentBins[iCent] + iSubCent * delta));
          double corr = hNKaonXiProj->GetCorrelationFactor();
          double avg_var_k = iC == 0 ? hNKaonXiProj->GetMean(2) - 0.5 : pow(hNKaonXiProj->GetStdDev(2), 2);
          double avg_var_xi = iC == 0 ? hNKaonXiProj->GetMean(1) - 0.5 : pow(hNKaonXiProj->GetStdDev(1), 2);
          n_ev_tot += n_ev;
          avg_corr += n_ev * corr;
          avg_avg_var_k += n_ev * avg_var_k;
          avg_avg_var_xi += n_ev * avg_var_xi;
        }
        avg_corr = avg_corr / n_ev_tot;
        avg_avg_var_k = avg_avg_var_k / n_ev_tot;
        avg_avg_var_xi = avg_avg_var_xi / n_ev_tot;
        hCorrDistr[iC][iCent]->Fill(avg_corr);
        hAvgVarKDistr[iC][iCent]->Fill(avg_avg_var_k);
        hAvgVarXiDistr[iC][iCent]->Fill(avg_avg_var_xi);
        #ifdef CLOSURE_TEST
          TH3F* hNKaonXi_Gen = (TH3F*)inFile.Get(Form("subsample_%d/h%sNKaonXi_Gen_%d", iS, kChargeLabel[iC], iS)); // TO BE IMPROVED TO REDUCE NUMBER OF FILE ACCESSES
          avg_corr = 0.;
          avg_avg_var_k = 0.;
          avg_avg_var_xi = 0.;
          n_ev_tot = 0.;
          delta = (kCentBins[iCent + 1] - kCentBins[iCent]) / N_SUB_CENT;
          for (int iSubCent = 0; iSubCent < N_SUB_CENT; ++iSubCent){
            hNKaonXi_Gen->GetXaxis()->SetRange(hCent->FindBin(kCentBins[iCent] + iSubCent * delta), hCent->FindBin(kCentBins[iCent] + iSubCent * delta));
            TH2F* hNKaonXiProj_Gen = (TH2F* )hNKaonXi_Gen->Project3D("yz");
            double n_ev = hCent->GetBinContent(hCent->FindBin(kCentBins[iCent] + iSubCent * delta));
            double corr = hNKaonXiProj_Gen->GetCorrelationFactor();
            double avg_var_k = iC == 0 ? hNKaonXiProj_Gen->GetMean(2) - 0.5 : pow(hNKaonXiProj_Gen->GetStdDev(2), 2);
            double avg_var_xi = iC == 0 ? hNKaonXiProj_Gen->GetMean(1) - 0.5 : pow(hNKaonXiProj_Gen->GetStdDev(1), 2);
            n_ev_tot += n_ev;
            avg_corr += n_ev * corr;
            avg_avg_var_k += n_ev * avg_var_k;
            avg_avg_var_xi += n_ev * avg_var_xi;
          }
          avg_corr = avg_corr / n_ev_tot;
          avg_avg_var_k = avg_avg_var_k / n_ev_tot;
          avg_avg_var_xi = avg_avg_var_xi / n_ev_tot;
          hCorrDistr_Gen[iC][iCent]->Fill(avg_corr);
          hAvgVarKDistr_Gen[iC][iCent]->Fill(avg_avg_var_k);
          hAvgVarXiDistr_Gen[iC][iCent]->Fill(avg_avg_var_xi);
        #endif // CLOSURE_TEST
      }
      get_subsample_average(iCent, hCorrDistr[iC][iCent], hCorrCent[iC]);
      get_subsample_average(iCent, hAvgVarKDistr[iC][iCent], hAvgVarKCent[iC]);
      get_subsample_average(iCent, hAvgVarXiDistr[iC][iCent], hAvgVarXiCent[iC]);
      #ifdef CLOSURE_TEST 
        get_subsample_average(iCent, hCorrDistr_Gen[iC][iCent], hCorrCent_Gen[iC]);
        get_subsample_average(iCent, hAvgVarKDistr_Gen[iC][iCent], hAvgVarKCent_Gen[iC]);
        get_subsample_average(iCent, hAvgVarXiDistr_Gen[iC][iCent], hAvgVarXiCent_Gen[iC]);
      #endif // CLOSURE_TEST
      //hCorrDistr[iC][iCent]->Write();
    }
    hCorrCent[iC]->Write();
    hAvgVarKCent[iC]->Write();
    hAvgVarXiCent[iC]->Write();
    #ifdef CLOSURE_TEST
      hCorrCent_Gen[iC]->Write();
      hAvgVarKCent_Gen[iC]->Write();
      hAvgVarXiCent_Gen[iC]->Write();
    #endif // CLOSURE_TEST
  }


  TH1D *hCorrCentSmall[2];
  TH1D *hCorrDistrSmall[2][kNCentBinsSmall];
  #ifdef CLOSURE_TEST
    TH1D *hCorrCentSmall_Gen[2];
    TH1D *hCorrDistrSmall_Gen[2][kNCentBinsSmall];
  #endif // CLOSURE_TEST
  for (int iC = 0; iC < 2; ++iC){
    hCorrCentSmall[iC] = new TH1D(Form("h%sCorrCentSmall", kChargeLabel[iC]), ";Centrality (%);#rho_{#Xi K}", kNCentBinsSmall, kCentBinsSmall);
    #ifdef CLOSURE_TEST
      hCorrCentSmall_Gen[iC] = new TH1D(Form("h%sCorrCentSmall_Gen", kChargeLabel[iC]), ";Centrality (%);#rho_{#Xi K}", kNCentBinsSmall, kCentBinsSmall);
    #endif // CLOSUrE_TEST
    for (int iCent = 0; iCent < kNCentBinsSmall; ++iCent){
      hCorrDistrSmall[iC][iCent] = new TH1D(Form("h%sCorrDistrSmall_%d", kChargeLabel[iC], iCent), ";#rho_{#Xi K};Entries", 20000, -2., 2.);
      #ifdef CLOSURE_TEST
        hCorrDistrSmall_Gen[iC][iCent] = new TH1D(Form("h%sCorrDistrSmall_Gen_%d", kChargeLabel[iC], iCent), ";#rho_{#Xi K};Entries", 20000, -2., 2.);
      #endif // CLOSURE_TEST
      for (int iS = 0; iS < N_SAMPLE; ++iS){
        TH3F* hNKaonXi = (TH3F*)inFile.Get(Form("subsample_%d/h%sNKaonXi_%d", iS, kChargeLabel[iC], iS)); // TO BE IMPROVED TO REDUCE NUMBER OF FILE ACCESSES
        hNKaonXi->GetXaxis()->SetRange(iCent + 1, iCent + 1);
        TH2F* hNKaonXiProj = (TH2F* )hNKaonXi->Project3D("yz");
        
        //hNKaonXiProj->Write();
        
        hCorrDistrSmall[iC][iCent]->Fill(hNKaonXiProj->GetCorrelationFactor());
        #ifdef CLOSURE_TEST
          TH3F* hNKaonXi_Gen = (TH3F*)inFile.Get(Form("subsample_%d/h%sNKaonXi_Gen_%d", iS, kChargeLabel[iC], iS)); // TO BE IMPROVED TO REDUCE NUMBER OF FILE ACCESSES
          hNKaonXi_Gen->GetXaxis()->SetRange(iCent + 1, iCent + 1);
          TH2F* hNKaonXiProj_Gen = (TH2F* )hNKaonXi_Gen->Project3D("yz");
          hCorrDistrSmall_Gen[iC][iCent]->Fill(hNKaonXiProj_Gen->GetCorrelationFactor());
        #endif // CLOSURE_TEST
      }
      get_subsample_average(iCent, hCorrDistrSmall[iC][iCent], hCorrCentSmall[iC]);
      #ifdef CLOSURE_TEST
        get_subsample_average(iCent, hCorrDistrSmall_Gen[iC][iCent], hCorrCentSmall_Gen[iC]);
      #endif // CLOSURE_TEST
      //hCorrDistrSmall[iC][iCent]->Write();
    }
    hCorrCentSmall[iC]->Write();
    #ifdef CLOSURE_TEST
      hCorrCentSmall_Gen[iC]->Write();
    #endif // CLOSURE_TEST
  }

  TH1D *hCorrCentEffCorr[2];
  TH1D *hCorrDistrEffCorr[2][kNCentBinsSmall];
  for (int iC = 0; iC < 2; ++iC){
    hCorrCentEffCorr[iC] = new TH1D(Form("h%sCorrCentEffCorr", kChargeLabel[iC]), ";Centrality (%);#rho_{#Xi K}", kNCentBinsSmall, kCentBinsSmall);
    for (int iCent = 0; iCent < kNCentBinsSmall; ++iCent){
      hCorrDistrEffCorr[iC][iCent] = new TH1D(Form("h%sCorrDistrEffCorr_%d", kChargeLabel[iC], iCent), ";#rho_{#Xi K};Entries", 20000, -2., 2.);
    }
  }
  for (int iS = 0; iS < N_SAMPLE; ++iS){
    TH1D* hKaonQ1[2];
    TH1D* hKaonQ1Sq[2];
    TH1D* hKaonQ2[2];
    TH1D* hXiQ1[2];
    TH1D* hXiQ1Sq[2];
    TH1D* hXiQ2[2];
    TH1D* hSameKaonXiQ11[2];
    TH1D* hOppKaonXiQ11[2];
    for (int iM = 0; iM < 2; ++iM){
      hKaonQ1[iM] = (TH1D*)inFile.Get(Form("subsample_%d/h%sKaonQ1_%s_%d", iS, kAntiMatterLabel[iM], kCorrLabel[0], iS));
      hKaonQ1Sq[iM] = (TH1D*)inFile.Get(Form("subsample_%d/h%sKaonQ1Sq_%s_%d", iS, kAntiMatterLabel[iM], kCorrLabel[0], iS));
      hKaonQ2[iM] = (TH1D*)inFile.Get(Form("subsample_%d/h%sKaonQ2_%s_%d", iS, kAntiMatterLabel[iM], kCorrLabel[0], iS));
      hXiQ1[iM] = (TH1D*)inFile.Get(Form("subsample_%d/h%sXiQ1_%s_%d", iS, kAntiMatterLabel[iM], kCorrLabel[0], iS));
      hXiQ1Sq[iM] = (TH1D*)inFile.Get(Form("subsample_%d/h%sXiQ1Sq_%s_%d", iS, kAntiMatterLabel[iM], kCorrLabel[0], iS));
      hXiQ2[iM] = (TH1D*)inFile.Get(Form("subsample_%d/h%sXiQ2_%s_%d", iS, kAntiMatterLabel[iM], kCorrLabel[0], iS));
      hSameKaonXiQ11[iM] = (TH1D*)inFile.Get(Form("subsample_%d/h%sSameKaonXiQ11_%s_%d", iS, kAntiMatterLabel[iM], kCorrLabel[0], iS));
      hOppKaonXiQ11[iM] = (TH1D*)inFile.Get(Form("subsample_%d/h%sOppKaonXiQ11_%s_%d", iS, kAntiMatterLabel[iM], kCorrLabel[0], iS));
    }

    for (int iCent = 0; iCent < kNCentBinsSmall; ++iCent){
      double kaonQ1[] = {0, 0};
      double kaonQ1Sq[] = {0, 0};
      double kaonQ2[] = {0, 0}; 
      double xiQ1[] = {0, 0}; 
      double xiQ1Sq[] = {0, 0}; 
      double xiQ2[] = {0, 0}; 
      double sameKaonXiQ11[] = {0, 0};
      double oppKaonXiQ11[] = {0, 0};
      for (int iM = 0; iM < 2; ++iM){
        kaonQ1[iM] = hKaonQ1[iM]->GetBinContent(iCent + 1);
        kaonQ1Sq[iM] = hKaonQ1Sq[iM]->GetBinContent(iCent + 1);
        kaonQ2[iM] = hKaonQ2[iM]->GetBinContent(iCent + 1);
        xiQ1[iM] = hXiQ1[iM]->GetBinContent(iCent + 1);
        xiQ1Sq[iM] = hXiQ1Sq[iM]->GetBinContent(iCent + 1);
        xiQ2[iM] = hXiQ2[iM]->GetBinContent(iCent + 1);
        sameKaonXiQ11[iM] = hSameKaonXiQ11[iM]->GetBinContent(iCent + 1);
        oppKaonXiQ11[iM] = hOppKaonXiQ11[iM]->GetBinContent(iCent + 1);
      }
      for (int iC = 0; iC < 2; ++iC)
        hCorrDistrEffCorr[iC][iCent]->Fill(compute_correlation(iC, kaonQ1, kaonQ1Sq, kaonQ2, xiQ1, xiQ1Sq, xiQ2, sameKaonXiQ11, oppKaonXiQ11));
    }
  }

  for (int iC = 0; iC < 2; ++iC){
    for (int iCent = 0; iCent < kNCentBinsSmall; ++iCent){
        get_subsample_average(iCent, hCorrDistrEffCorr[iC][iCent], hCorrCentEffCorr[iC]);
      // hCorrDistrEffCorr[iC][iCent]->Write();
    }
    hCorrCentEffCorr[iC]->Write();
  }

  TH1D *hCorrCentEffCorrCBWC[2][2];
  TH1D *hAvgVarKCentEffCorrCBWC[2][2];
  TH1D *hAvgVarXiCentEffCorrCBWC[2][2];
  TH1D *hCorrDistrEffCorrCBWC[2][2][kNCentBins];
  TH1D *hAvgVarKDistrEffCorrCBWC[2][2][kNCentBins];
  TH1D *hAvgVarXiDistrEffCorrCBWC[2][2][kNCentBins];
  for (int iC = 0; iC < 2; ++iC){
    for (int iCorr = 0; iCorr < 2; ++iCorr){
      hCorrCentEffCorrCBWC[iC][iCorr] = new TH1D(Form("h%sCorrCentEffCorrCBWC_%s", kChargeLabel[iC], kCorrLabel[iCorr]), ";Centrality (%);#rho_{#Xi K}", kNCentBins, kCentBins);
      hAvgVarKCentEffCorrCBWC[iC][iCorr] = new TH1D(Form("h%sKCentEffCorrCBWC_%s", kAvgVarLabel[iC], kCorrLabel[iCorr]), Form(";Centrality (%%);C_{%d}^{K}", iC + 1), kNCentBins, kCentBins);
      hAvgVarXiCentEffCorrCBWC[iC][iCorr] = new TH1D(Form("h%sXiCentEffCorrCBWC_%s", kAvgVarLabel[iC], kCorrLabel[iCorr]), Form(";Centrality (%%);C_{%d}^{#Xi}", iC + 1), kNCentBins, kCentBins);
      for (int iCent = 0; iCent < kNCentBins; ++iCent){
        hCorrDistrEffCorrCBWC[iC][iCorr][iCent] = new TH1D(Form("h%sCorrDistrEffCorrCBWC_%s_%d", kChargeLabel[iC], kCorrLabel[iCorr], iCent), ";#rho_{#Xi K};Entries", 20000, -2., 2.);
        hAvgVarKDistrEffCorrCBWC[iC][iCorr][iCent] = new TH1D(Form("h%sKDistrEffCorrCBWC_%s_%d", kAvgVarLabel[iC], kCorrLabel[iCorr], iCent), Form(";C_{%d}^{K};Entries", iC + 1), 20000, -10000., 10000.);
        hAvgVarXiDistrEffCorrCBWC[iC][iCorr][iCent] = new TH1D(Form("h%sXiDistrEffCorrCBWC_%s_%d", kAvgVarLabel[iC], kCorrLabel[iCorr], iCent), Form(";C_{%d}^{#Xi};Entries", iC + 1), 2000, -1000., 1000.);
      }
    }
  }
  for (int iS = 0; iS < N_SAMPLE; ++iS){
    TH1D* hKaonQ1[2];
    TH1D* hKaonQ1Sq[2];
    TH1D* hKaonQ2[2];
    TH1D* hXiQ1[2];
    TH1D* hXiQ1Sq[2];
    TH1D* hXiQ2[2];
    TH1D* hSameKaonXiQ11[2];
    TH1D* hOppKaonXiQ11[2];
    TH1D* hCent = (TH1D*)inFile.Get(Form("subsample_%d/hCent_%d", iS, iS));
    for (int iCorr = 0; iCorr < 2; ++iCorr){
      for (int iM = 0; iM < 2; ++iM){
        hKaonQ1[iM] = (TH1D*)inFile.Get(Form("subsample_%d/h%sKaonQ1_%s_%d", iS, kAntiMatterLabel[iM], kCorrLabel[iCorr], iS));
        hKaonQ1Sq[iM] = (TH1D*)inFile.Get(Form("subsample_%d/h%sKaonQ1Sq_%s_%d", iS, kAntiMatterLabel[iM], kCorrLabel[iCorr], iS));
        hKaonQ2[iM] = (TH1D*)inFile.Get(Form("subsample_%d/h%sKaonQ2_%s_%d", iS, kAntiMatterLabel[iM], kCorrLabel[iCorr], iS));
        hXiQ1[iM] = (TH1D*)inFile.Get(Form("subsample_%d/h%sXiQ1_%s_%d", iS, kAntiMatterLabel[iM], kCorrLabel[iCorr], iS));
        hXiQ1Sq[iM] = (TH1D*)inFile.Get(Form("subsample_%d/h%sXiQ1Sq_%s_%d", iS, kAntiMatterLabel[iM], kCorrLabel[iCorr], iS));
        hXiQ2[iM] = (TH1D*)inFile.Get(Form("subsample_%d/h%sXiQ2_%s_%d", iS, kAntiMatterLabel[iM], kCorrLabel[iCorr], iS));
        hSameKaonXiQ11[iM] = (TH1D*)inFile.Get(Form("subsample_%d/h%sSameKaonXiQ11_%s_%d", iS, kAntiMatterLabel[iM], kCorrLabel[iCorr], iS));
        hOppKaonXiQ11[iM] = (TH1D*)inFile.Get(Form("subsample_%d/h%sOppKaonXiQ11_%s_%d", iS, kAntiMatterLabel[iM], kCorrLabel[iCorr], iS));
      }
      for (int iCent = 0; iCent < kNCentBins; ++iCent){
        double avg_corr[] = {0., 0.};
        double avg_avg_var_k[] = {0., 0.};
        double avg_avg_var_xi[] = {0., 0.};
        double n_ev_tot = 0.;
        double delta = (kCentBins[iCent + 1] - kCentBins[iCent]) / N_SUB_CENT;
        for (int iSubCent = 0; iSubCent < N_SUB_CENT; ++iSubCent){
          double n_ev = hCent->GetBinContent(hCent->FindBin(kCentBins[iCent] + iSubCent * delta + 0.001));
          n_ev_tot += n_ev;
        
          double kaonQ1[] = {0, 0};
          double kaonQ1Sq[] = {0, 0};
          double kaonQ2[] = {0, 0}; 
          double xiQ1[] = {0, 0}; 
          double xiQ1Sq[] = {0, 0}; 
          double xiQ2[] = {0, 0}; 
          double sameKaonXiQ11[] = {0, 0};
          double oppKaonXiQ11[] = {0, 0};
          for (int iM = 0; iM < 2; ++iM){
            kaonQ1[iM] = hKaonQ1[iM]->GetBinContent(hCent->FindBin(kCentBins[iCent] + iSubCent * delta));
            kaonQ1Sq[iM] = hKaonQ1Sq[iM]->GetBinContent(hCent->FindBin(kCentBins[iCent] + iSubCent * delta));
            kaonQ2[iM] = hKaonQ2[iM]->GetBinContent(hCent->FindBin(kCentBins[iCent] + iSubCent * delta));
            xiQ1[iM] = hXiQ1[iM]->GetBinContent(hCent->FindBin(kCentBins[iCent] + iSubCent * delta));
            xiQ1Sq[iM] = hXiQ1Sq[iM]->GetBinContent(hCent->FindBin(kCentBins[iCent] + iSubCent * delta));
            xiQ2[iM] = hXiQ2[iM]->GetBinContent(hCent->FindBin(kCentBins[iCent] + iSubCent * delta));
            sameKaonXiQ11[iM] = hSameKaonXiQ11[iM]->GetBinContent(hCent->FindBin(kCentBins[iCent] + iSubCent * delta));
            oppKaonXiQ11[iM] = hOppKaonXiQ11[iM]->GetBinContent(hCent->FindBin(kCentBins[iCent] + iSubCent * delta));
          }
          double avg_var_xi[2] = {0., 0.};
          double avg_var_k[2] = {0., 0.};
          compute_average_variance(avg_var_k, kaonQ1, kaonQ1Sq, kaonQ2);
          compute_average_variance(avg_var_xi, xiQ1, xiQ1Sq, xiQ2);
          for (int iC = 0; iC < 2; ++iC){
            double corr = compute_correlation(iC, kaonQ1, kaonQ1Sq, kaonQ2, xiQ1, xiQ1Sq, xiQ2, sameKaonXiQ11, oppKaonXiQ11);
            avg_corr[iC] += n_ev * corr;
            avg_avg_var_k[iC] += n_ev * avg_var_k[iC];
            avg_avg_var_xi[iC] += n_ev * avg_var_xi[iC];
          }
        }
        for (int iC = 0; iC < 2; ++iC){
          hCorrDistrEffCorrCBWC[iC][iCorr][iCent]->Fill(avg_corr[iC] / n_ev_tot);
          hAvgVarKDistrEffCorrCBWC[iC][iCorr][iCent]->Fill(avg_avg_var_k[iC] / n_ev_tot);
          hAvgVarXiDistrEffCorrCBWC[iC][iCorr][iCent]->Fill(avg_avg_var_xi[iC] / n_ev_tot);
        }
      }
    }
  }

  for (int iCorr = 0; iCorr < 2; ++iCorr){
    for (int iC = 0; iC < 2; ++iC){
      for (int iCent = 0; iCent < kNCentBins; ++iCent){
        get_subsample_average(iCent, hCorrDistrEffCorrCBWC[iC][iCorr][iCent], hCorrCentEffCorrCBWC[iC][iCorr]);
        get_subsample_average(iCent, hAvgVarKDistrEffCorrCBWC[iC][iCorr][iCent], hAvgVarKCentEffCorrCBWC[iC][iCorr]);
        get_subsample_average(iCent, hAvgVarXiDistrEffCorrCBWC[iC][iCorr][iCent], hAvgVarXiCentEffCorrCBWC[iC][iCorr]);
        // hCorrDistrEffCorrCBWC[iC][iCorr][iCent]->Write();
      }
      hCorrCentEffCorrCBWC[iC][iCorr]->Write();
      hAvgVarKCentEffCorrCBWC[iC][iCorr]->Write();
      hAvgVarXiCentEffCorrCBWC[iC][iCorr]->Write();
    }
  }

  outFile.Close();
  inFile.Close();
}

void compute_average_variance(double res[2], const double Q1[2], const double Q1Sq[2], const double Q2[2]){
  double avg = ( Q1[0] + Q1[1] ) / 2.;
  double avg_sq = ( Q1Sq[0] + Q1Sq[1] ) / 2.;
  double avg_q2 = ( Q2[0] + Q2[1] ) / 2.;
  double sq_avg = ( Q1[0] * Q1[0] + Q1[1] * Q1[1] + 2 * Q1[0] * Q1[1] ) / 4.;
  double var = avg_sq - sq_avg + avg - avg_q2;
  if ( var < 1.e-7 ) {
    res[0] = avg;
    res[1] = 0.;
    return;
  }
  res[0] = avg;
  res[1] = var;
  return;
}

double compute_correlation(const int iC, const double kaonQ1[2], const double kaonQ1Sq[2], const double kaonQ2[2], const double xiQ1[2], const double xiQ1Sq[2], const double xiQ2[2], const double sameKaonXiQ11[2], const double oppKaonXiQ11[2]){
  double avg_var_k[2] = {0., 0.};
  double avg_var_xi[2] = {0., 0.};
  compute_average_variance(avg_var_k, kaonQ1, kaonQ1Sq, kaonQ2);
  compute_average_variance(avg_var_xi, xiQ1, xiQ1Sq, xiQ2);
  double cov_same = ( sameKaonXiQ11[0] + sameKaonXiQ11[1] ) / 2. - avg_var_k[0] * avg_var_xi[0];
  double cov_opp = ( oppKaonXiQ11[0] + oppKaonXiQ11[1] ) / 2. - avg_var_k[0] * avg_var_xi[0];
  if ( avg_var_k[1]  < 1.e-7 || avg_var_xi[1]  < 1.e-7 ) return 0.;
  // std::cout << "average(K) = " << kaonQ1[1] << ", stddev(K) = " << sqrt(var_k_1) << ", average(Xi) = " << xiQ1[0] << ", var(Xi) = " << sqrt(var_xi_0) << std::endl;
  return (iC > 0 ? cov_opp : cov_same) / sqrt( avg_var_k[1] * avg_var_xi[1] );
}

void get_subsample_average(const int iCent, const TH1* distr, TH1* avg){
  double corr = distr->GetMean();
  double corr_sigma = distr->GetStdDev() / sqrt(N_SAMPLE - 1);
  avg->SetBinContent(iCent + 1, corr);
  avg->SetBinError(iCent + 1, corr_sigma);
}
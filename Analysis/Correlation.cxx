#include "../utils/Config.h"

#define CHARGE_XI 0
#define CLOSURE_TEST

const bool test_from_th2 = false;

bool xi_plus = false;
void get_subsample_average(const int iCent, const TH1* distr, TH1* avg);
void compute_average_variance(double res[2][2], const double Q1[2], const double Q1Sq[2], const double Q2[2]);
double compute_covariance(const int iC, const double kaonQ1[2], const double kaonQ1Sq[2], const double kaonQ2[2], const double xiQ1[2], const double xiQ1Sq[2], const double xiQ2[2], const double sameKaonXiQ11[2], const double oppKaonXiQ11[2]);

void Correlation (const char* inFileName = "o_merge_parallel", const char* outFileName = "out_merge_parallel"){
  TFile inFile(Form("%s.root", inFileName));
  TFile outFile(Form("%s.root", outFileName), "recreate");

  TH1D *hCorrCentEffCorrCBWC[2][3];
  TH1D *hAvgVarKCentEffCorrCBWC[2][3];
  TH1D *hAvgVarXiCentEffCorrCBWC[2][3];
  TH1D *hCorrCentEffCorr_[2][3];
  TH1D *hAvgVarKCentEffCorr_[2][3];
  TH1D *hAvgVarXiCentEffCorr_[2][3];
  TH1D *hCorrDistrEffCorrCBWC[2][3][kNCentBins];
  TH1D *hAvgVarKDistrEffCorrCBWC[2][3][kNCentBins];
  TH1D *hAvgVarXiDistrEffCorrCBWC[2][3][kNCentBins];
  TH1D *hCorrDistrEffCorr_[2][3][kNCentBinsSmall];
  TH1D *hAvgVarKDistrEffCorr_[2][3][kNCentBinsSmall];
  TH1D *hAvgVarXiDistrEffCorr_[2][3][kNCentBinsSmall];
  for (int iC = 0; iC < 2; ++iC){
    for (int iCorr = 0; iCorr < 3; ++iCorr){
      hCorrCentEffCorrCBWC[iC][iCorr] = new TH1D(Form("h%sCorrCentEffCorrCBWC_%s", kChargeLabel[iC], kCorrLabel[iCorr]), ";Centrality (%);#rho_{#Xi K}", kNCentBins, kCentBins);
      hAvgVarKCentEffCorrCBWC[iC][iCorr] = new TH1D(Form("h%sKCentEffCorrCBWC_%s", kAvgVarLabel[iC], kCorrLabel[iCorr]), Form(";Centrality (%%);C_{%d}^{K}", iC + 1), kNCentBins, kCentBins);
      hAvgVarXiCentEffCorrCBWC[iC][iCorr] = new TH1D(Form("h%sXiCentEffCorrCBWC_%s", kAvgVarLabel[iC], kCorrLabel[iCorr]), Form(";Centrality (%%);C_{%d}^{#Xi}", iC + 1), kNCentBins, kCentBins);
      hCorrCentEffCorr_[iC][iCorr] = new TH1D(Form("h%sCorrCentEffCorr__%s", kChargeLabel[iC], kCorrLabel[iCorr]), ";Centrality (%);#rho_{#Xi K}", kNCentBinsSmall, kCentBinsSmall);
      hAvgVarKCentEffCorr_[iC][iCorr] = new TH1D(Form("h%sKCentEffCorr__%s", kAvgVarLabel[iC], kCorrLabel[iCorr]), Form(";Centrality (%%);C_{%d}^{K}", iC + 1), kNCentBinsSmall, kCentBinsSmall);
      hAvgVarXiCentEffCorr_[iC][iCorr] = new TH1D(Form("h%sXiCentEffCorr__%s", kAvgVarLabel[iC], kCorrLabel[iCorr]), Form(";Centrality (%%);C_{%d}^{#Xi}", iC + 1), kNCentBinsSmall, kCentBinsSmall);
      for (int iCent = 0; iCent < kNCentBins; ++iCent){
        hCorrDistrEffCorrCBWC[iC][iCorr][iCent] = new TH1D(Form("h%sCorrDistrEffCorrCBWC_%s_%d", kChargeLabel[iC], kCorrLabel[iCorr], iCent), ";#rho_{#Xi K};Entries", 20000, -10., 10.);
        hAvgVarKDistrEffCorrCBWC[iC][iCorr][iCent] = new TH1D(Form("h%sKDistrEffCorrCBWC_%s_%d", kAvgVarLabel[iC], kCorrLabel[iCorr], iCent), Form(";C_{%d}^{K};Entries", iC + 1), 400000, -20000., 20000.);
        hAvgVarXiDistrEffCorrCBWC[iC][iCorr][iCent] = new TH1D(Form("h%sXiDistrEffCorrCBWC_%s_%d", kAvgVarLabel[iC], kCorrLabel[iCorr], iCent), Form(";C_{%d}^{#Xi};Entries", iC + 1), 4000, -200., 200.);
      }
      for (int iCent = 0; iCent < kNCentBinsSmall; ++iCent){
        hCorrDistrEffCorr_[iC][iCorr][iCent] = new TH1D(Form("h%sCorrDistrEffCorr__%s_%d", kChargeLabel[iC], kCorrLabel[iCorr], iCent), ";#rho_{#Xi K};Entries", 20000, -10., 10.);
        hAvgVarKDistrEffCorr_[iC][iCorr][iCent] = new TH1D(Form("h%sKDistrEffCorr__%s_%d", kAvgVarLabel[iC], kCorrLabel[iCorr], iCent), Form(";C_{%d}^{K};Entries", iC + 1), 400000, -20000., 20000.);
        hAvgVarXiDistrEffCorr_[iC][iCorr][iCent] = new TH1D(Form("h%sXiDistrEffCorr__%s_%d", kAvgVarLabel[iC], kCorrLabel[iCorr], iCent), Form(";C_{%d}^{#Xi};Entries", iC + 1), 4000, -200., 200.);
      }
    }
  }
  for (int iS = 0; iS < N_SAMPLE; ++iS){
    std::cout << "subsample = " << iS << "..." << std::endl;
    TH1D* hKaonQ1[2];
    TH1D* hKaonQ1Sq[2];
    TH1D* hKaonQ2[2];
    TH1D* hXiQ1[2];
    TH1D* hXiQ1Sq[2];
    TH1D* hXiQ2[2];
    TH1D* hSameKaonXiQ11[2];
    TH1D* hOppKaonXiQ11[2];
    TH1D* hCent = (TH1D*)inFile.Get(Form("subsample_%s%d/hCent", kSubsampleFlag, iS + 1));
    for (int iCorr = 0; iCorr < (isMC ? 3 : 2); ++iCorr){
      std::cout << "corr = " << iCorr << std::endl;
      for (int iM = 0; iM < 2; ++iM){
        hKaonQ1[iM] = (TH1D*)inFile.Get(Form("subsample_%s%d/h%sKaonQ1_%s", kSubsampleFlag, iS + 1, kAntiMatterLabel[iM], kCorrLabel[iCorr]));
        hKaonQ1Sq[iM] = (TH1D*)inFile.Get(Form("subsample_%s%d/h%sKaonQ1Sq_%s", kSubsampleFlag, iS + 1, kAntiMatterLabel[iM], kCorrLabel[iCorr]));
        hKaonQ2[iM] = (TH1D*)inFile.Get(Form("subsample_%s%d/h%sKaonQ2_%s", kSubsampleFlag, iS + 1, kAntiMatterLabel[iM], kCorrLabel[iCorr]));
        hXiQ1[iM] = (TH1D*)inFile.Get(Form("subsample_%s%d/h%sXiQ1_%s", kSubsampleFlag, iS + 1, kAntiMatterLabel[iM], kCorrLabel[iCorr]));
        hXiQ1Sq[iM] = (TH1D*)inFile.Get(Form("subsample_%s%d/h%sXiQ1Sq_%s", kSubsampleFlag, iS + 1, kAntiMatterLabel[iM], kCorrLabel[iCorr]));
        hXiQ2[iM] = (TH1D*)inFile.Get(Form("subsample_%s%d/h%sXiQ2_%s", kSubsampleFlag, iS + 1, kAntiMatterLabel[iM], kCorrLabel[iCorr]));
        hSameKaonXiQ11[iM] = (TH1D*)inFile.Get(Form("subsample_%s%d/h%sSameKaonXiQ11_%s", kSubsampleFlag, iS + 1, kAntiMatterLabel[iM], kCorrLabel[iCorr]));
        hOppKaonXiQ11[iM] = (TH1D*)inFile.Get(Form("subsample_%s%d/h%sOppKaonXiQ11_%s", kSubsampleFlag, iS + 1, kAntiMatterLabel[iM], kCorrLabel[iCorr]));
      }
      for (int iCent = 0; iCent < kNCentBins; ++iCent){
        double avg_corr[] = {0., 0.};
        double avg_avg_var_k[][2] = {{0., 0.}, {0., 0.}};
        double avg_avg_var_xi[][2] = {{0., 0.}, {0., 0.}};
        double avg_corr_[] = {0., 0.};
        double avg_avg_var_k_[][2] = {{0., 0.}, {0., 0.}};
        double avg_avg_var_xi_[][2] = {{0., 0.}, {0., 0.}};
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
            // std::cout << "c = " << hCent->FindBin(kCentBins[iCent] + iSubCent * delta) << std::endl;
            kaonQ1[iM] = hKaonQ1[iM]->GetBinContent(hCent->FindBin(kCentBins[iCent] + iSubCent * delta + 0.001));
            // std::cout << "ic = " << hCent->FindBin(kCentBins[iCent] + iSubCent * delta + 0.001) << "; kq1 = " << kaonQ1[iM] << std::endl;
            kaonQ1Sq[iM] = hKaonQ1Sq[iM]->GetBinContent(hCent->FindBin(kCentBins[iCent] + iSubCent * delta + 0.001));
            kaonQ2[iM] = hKaonQ2[iM]->GetBinContent(hCent->FindBin(kCentBins[iCent] + iSubCent * delta + 0.001));
            xiQ1[iM] = hXiQ1[iM]->GetBinContent(hCent->FindBin(kCentBins[iCent] + iSubCent * delta + 0.001));
            xiQ1Sq[iM] = hXiQ1Sq[iM]->GetBinContent(hCent->FindBin(kCentBins[iCent] + iSubCent * delta + 0.001));
            xiQ2[iM] = hXiQ2[iM]->GetBinContent(hCent->FindBin(kCentBins[iCent] + iSubCent * delta + 0.001));
            sameKaonXiQ11[iM] = hSameKaonXiQ11[iM]->GetBinContent(hCent->FindBin(kCentBins[iCent] + iSubCent * delta + 0.001));
            oppKaonXiQ11[iM] = hOppKaonXiQ11[iM]->GetBinContent(hCent->FindBin(kCentBins[iCent] + iSubCent * delta + 0.001));
          }
          double avg_var_xi[2][2] = {{0., 0.}, {0., 0.}};
          double avg_var_k[2][2] = {{0., 0.}, {0., 0.}};
          compute_average_variance(avg_var_k, kaonQ1, kaonQ1Sq, kaonQ2);
          compute_average_variance(avg_var_xi, xiQ1, xiQ1Sq, xiQ2);
          for (int iC = 0; iC < 2; ++iC){
            double corr = compute_covariance(iC, kaonQ1, kaonQ1Sq, kaonQ2, xiQ1, xiQ1Sq, xiQ2, sameKaonXiQ11, oppKaonXiQ11);
            avg_corr[iC] += (corr * n_ev);
            avg_corr_[iC] = corr;
          }
          for (int iC = 0; iC < 2; ++iC){
            for (int iM = 0; iM < 2; ++iM){
              avg_avg_var_k[iM][iC] += (n_ev * avg_var_k[iM][iC]);
              //if (hCent->FindBin(kCentBins[iCent] + iSubCent * delta + 0.001) < 10 && iM == 0 && iC == 1)
              //  std::cout << "ic = " << hCent->FindBin(kCentBins[iCent] + iSubCent * delta + 0.001) << "; k_avg = " << avg_avg_var_k[iM][iC]/n_ev_tot << std::endl;
              avg_avg_var_xi[iM][iC] += (n_ev * avg_var_xi[iM][iC]);
              avg_avg_var_k_[iM][iC] = avg_var_k[iM][iC];
              avg_avg_var_xi_[iM][iC] = avg_var_xi[iM][iC];
            }
          }
          for (int iC = 0; iC < 2; ++iC){
            hCorrDistrEffCorr_[iC][iCorr][hCent->FindBin(kCentBins[iCent] + iSubCent * delta + 0.001) - 1]->Fill(avg_corr_[iC] / sqrt( avg_avg_var_k_[iC][1] * avg_avg_var_xi_[1][1] ));
            hAvgVarKDistrEffCorr_[iC][iCorr][hCent->FindBin(kCentBins[iCent] + iSubCent * delta + 0.001) - 1]->Fill(avg_avg_var_k_[0][iC]);
            hAvgVarXiDistrEffCorr_[iC][iCorr][hCent->FindBin(kCentBins[iCent] + iSubCent * delta + 0.001) - 1]->Fill(avg_avg_var_xi_[1][iC]);
            //std::cout << hCent->FindBin(kCentBins[iCent] + iSubCent * delta + 0.001) - 1 << std::endl;
          }
        }
        for (int iC = 0; iC < 2; ++iC){
          hCorrDistrEffCorrCBWC[iC][iCorr][iCent]->Fill(avg_corr[iC]  / sqrt( avg_avg_var_k[iC][1] * avg_avg_var_xi[1][1]));
          double avg_var_k_tmp = iC == 0 ? avg_avg_var_k[0][0] : avg_avg_var_k[0][1];
          double avg_var_xi_tmp = iC == 0 ? avg_avg_var_xi[1][0] : avg_avg_var_xi[1][1];
          hAvgVarKDistrEffCorrCBWC[iC][iCorr][iCent]->Fill(avg_var_k_tmp / n_ev_tot);
          hAvgVarXiDistrEffCorrCBWC[iC][iCorr][iCent]->Fill(avg_var_xi_tmp / n_ev_tot);
        }
      }
    }
  }

  for (int iCorr = 0; iCorr < (isMC ? 3 : 2); ++iCorr){
    for (int iC = 0; iC < 2; ++iC){
      for (int iCent = 0; iCent < kNCentBins; ++iCent){
        get_subsample_average(iCent, hCorrDistrEffCorrCBWC[iC][iCorr][iCent], hCorrCentEffCorrCBWC[iC][iCorr]);
        get_subsample_average(iCent, hAvgVarKDistrEffCorrCBWC[iC][iCorr][iCent], hAvgVarKCentEffCorrCBWC[iC][iCorr]);
        get_subsample_average(iCent, hAvgVarXiDistrEffCorrCBWC[iC][iCorr][iCent], hAvgVarXiCentEffCorrCBWC[iC][iCorr]);
        // hCorrDistrEffCorrCBWC[iC][iCorr][iCent]->Write();
      }
      for (int iCentSmall = 0; iCentSmall < kNCentBinsSmall; ++iCentSmall){
        get_subsample_average(iCentSmall, hCorrDistrEffCorr_[iC][iCorr][iCentSmall], hCorrCentEffCorr_[iC][iCorr]);
        get_subsample_average(iCentSmall, hAvgVarKDistrEffCorr_[iC][iCorr][iCentSmall], hAvgVarKCentEffCorr_[iC][iCorr]);
        get_subsample_average(iCentSmall, hAvgVarXiDistrEffCorr_[iC][iCorr][iCentSmall], hAvgVarXiCentEffCorr_[iC][iCorr]);
        //hAvgVarKDistrEffCorr_[iC][iCorr][iCentSmall]->Write();
      }
      hCorrCentEffCorrCBWC[iC][iCorr]->Write();
      hAvgVarKCentEffCorrCBWC[iC][iCorr]->Write();
      hAvgVarXiCentEffCorrCBWC[iC][iCorr]->Write();
      hCorrCentEffCorr_[iC][iCorr]->Write();
      hAvgVarKCentEffCorr_[iC][iCorr]->Write();
      hAvgVarXiCentEffCorr_[iC][iCorr]->Write();
    }
  }

  if (test_from_th2) {
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
        hCorrDistr[iC][iCent] = new TH1D(Form("h%sCorrDistr_%d", kChargeLabel[iC], iCent), ";#rho_{#Xi K};Entries", 20000, -10., 10.);
        hAvgVarKDistr[iC][iCent] = new TH1D(Form("h%sKDistr_%d", kAvgVarLabel[iC], iCent), Form(";C_{%d}^{K};Entries", iC + 1), 400000, -20000., 20000.);
        hAvgVarXiDistr[iC][iCent] = new TH1D(Form("h%sXiDistr_%d", kAvgVarLabel[iC], iCent), Form(";C_{%d}^{#Xi};Entries", iC + 1), 4000, -200., 200.);
        #ifdef CLOSURE_TEST
          hCorrDistr_Gen[iC][iCent] = new TH1D(Form("h%sCorrDistr_Gen_%d", kChargeLabel[iC], iCent), ";#rho_{#Xi K};Entries", 20000, -10., 10.);
          hAvgVarKDistr_Gen[iC][iCent] = new TH1D(Form("h%sKDistr_Gen_%d", kAvgVarLabel[iC], iCent), Form(";C_{%d}^{K};Entries", iC + 1), 400000, -20000., 20000.);
          hAvgVarXiDistr_Gen[iC][iCent] = new TH1D(Form("h%sXiDistr_Gen_%d", kAvgVarLabel[iC], iCent), Form(";C_{%d}^{#Xi};Entries", iC + 1),4000, -200., 200.);
        #endif // CLOSURE_TEST
        for (int iS = 0; iS < N_SAMPLE; ++iS){
          TH3F *hNKaonXi = (TH3F*)inFile.Get(Form("subsample_%s%d/h%sNKaonXi%s", kSubsampleFlag, iS + 1, kChargeLabel[0], kXiChargeLabel[CHARGE_XI]));
          //std::cout << "histo = " << Form("subsample_%s%d/h%sNKaonXi%s", kSubsampleFlag, iS + 1, kChargeLabel[iC], kXiChargeLabel[CHARGE_XI]) << std::endl;
          double avg_corr = 0.;
          double avg_avg_var_k = 0.;
          double avg_avg_var_xi = 0.;
          double n_ev_tot = 0.;
          TH1D* hCent = (TH1D*)inFile.Get(Form("subsample_%s%d/hCent", kSubsampleFlag, iS + 1));
          double delta = (kCentBins[iCent + 1] - kCentBins[iCent]) / N_SUB_CENT;
          for (int iSubCent = 0; iSubCent < N_SUB_CENT; ++iSubCent){
            TH1F* hNKaonXiProjK = (TH1F* )hNKaonXi->ProjectionY("Kproj", hCent->FindBin(kCentBins[iCent] + iSubCent * delta), hCent->FindBin(kCentBins[iCent] + iSubCent * delta), 1, hNKaonXi->GetNbinsZ());
            TH1F* hNKaonXiProjXi = (TH1F* )hNKaonXi->ProjectionZ("Xiproj", hCent->FindBin(kCentBins[iCent] + iSubCent * delta), hCent->FindBin(kCentBins[iCent] + iSubCent * delta), 1, hNKaonXi->GetNbinsY());
            double n_ev = hNKaonXiProjK->GetEntries(); //hCent->GetBinContent(hCent->FindBin(kCentBins[iCent] + iSubCent * delta));
            //std::cout << "n_ev = " << n_ev << "; " << hCent->GetBinContent(hCent->FindBin(kCentBins[iCent] + iSubCent * delta)) << std::endl;
            double corr = 0.;//hNKaonXiProjK->GetCovariance();
            double avg_var_k = iC == 0 ? (hNKaonXiProjK->GetMean()) : pow(hNKaonXiProjK->GetStdDev(), 2);
            double avg_var_xi = iC == 0 ? (hNKaonXiProjXi->GetMean()) : pow(hNKaonXiProjXi->GetStdDev(), 2);
            n_ev_tot += n_ev;
            avg_corr += (n_ev * corr);
            avg_avg_var_k += (n_ev * avg_var_k);
            avg_avg_var_xi += (n_ev * avg_var_xi);
          }
          avg_corr = avg_corr / n_ev_tot;
          avg_avg_var_k = avg_avg_var_k / n_ev_tot;
          avg_avg_var_xi = avg_avg_var_xi / n_ev_tot;
          hCorrDistr[iC][iCent]->Fill(avg_corr);
          hAvgVarKDistr[iC][iCent]->Fill(avg_avg_var_k);
          hAvgVarXiDistr[iC][iCent]->Fill(avg_avg_var_xi);
          #ifdef CLOSURE_TEST
            TH3F* hNKaonXi_Gen = (TH3F*)inFile.Get(Form("subsample_%s%d/h%sNKaonXi%s_Gen", kSubsampleFlag, iS + 1, kChargeLabel[0], kXiChargeLabel[CHARGE_XI])); // TO BE IMPROVED TO REDUCE NUMBER OF FILE ACCESSES
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
              double avg_var_k = iC == 0 ? (hNKaonXiProj_Gen->GetMean(2) - 0.5) : pow(hNKaonXiProj_Gen->GetStdDev(2), 2);
              double avg_var_xi = iC == 0 ? (hNKaonXiProj_Gen->GetMean(1) - 0.5) : pow(hNKaonXiProj_Gen->GetStdDev(1), 2);
              n_ev_tot += n_ev;
              avg_corr += (n_ev * corr);
              avg_avg_var_k += (n_ev * avg_var_k);
              avg_avg_var_xi += (n_ev * avg_var_xi);
            }
            avg_corr = (avg_corr / n_ev_tot);
            avg_avg_var_k = (avg_avg_var_k / n_ev_tot);
            avg_avg_var_xi = (avg_avg_var_xi / n_ev_tot);
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
      std::cout << "writing..." << std::endl;
        hCorrCent_Gen[iC]->Write();
        hAvgVarKCent_Gen[iC]->Write();
        hAvgVarXiCent_Gen[iC]->Write();
      #endif // CLOSURE_TEST
    }
  }


  outFile.Close();
  inFile.Close();
}

void compute_average_variance(double res[2][2], const double Q1[2], const double Q1Sq[2], const double Q2[2]){
  for (int iC = 0; iC < 2; ++iC){
    double avg = Q1[iC];
    double avg_sq = Q1Sq[iC];
    double avg_q2 = Q2[iC];
    double sq_avg = Q1[iC] * Q1[iC];
    double var = avg_sq -sq_avg + avg - avg_q2;
    if ( std::abs(var) < 1.e-16 ) {
      res[iC][0] = avg;
      res[iC][1] = 0.;
    }
    else {
      res[iC][0] = avg;
      res[iC][1] = var;
    }
  }
}

double compute_covariance(const int iC, const double kaonQ1[2], const double kaonQ1Sq[2], const double kaonQ2[2], const double xiQ1[2], const double xiQ1Sq[2], const double xiQ2[2], const double sameKaonXiQ11[2], const double oppKaonXiQ11[2]){
  double avg_var_k[2][2] = {{0., 0.}, {0., 0.}};
  double avg_var_xi[2][2] = {{0., 0.}, {0., 0.}};
  compute_average_variance(avg_var_k, kaonQ1, kaonQ1Sq, kaonQ2);
  compute_average_variance(avg_var_xi, xiQ1, xiQ1Sq, xiQ2);
  double cov_same = ( sameKaonXiQ11[0] ) - avg_var_k[0][0] * avg_var_xi[1][0];
  double cov_opp = ( oppKaonXiQ11[0] ) - avg_var_k[1][0] * avg_var_xi[1][0];
  if (xi_plus){
    cov_same = ( sameKaonXiQ11[1] ) - avg_var_k[1][0] * avg_var_xi[0][0];
    cov_opp = ( oppKaonXiQ11[1] ) - avg_var_k[0][0] * avg_var_xi[0][0];
  }
  if ( std::abs(avg_var_k[0][1]) < 1.e-7 || std::abs(avg_var_k[1][1]) < 1.e-7 || std::abs(avg_var_xi[1][1]) < 1.e-7  || std::abs(avg_var_xi[0][1]) < 1.e-7 ) return 0.;
  // std::cout << "average(K) = " << kaonQ1[1] << ", stddev(K) = " << sqrt(var_k_1) << ", average(Xi) = " << xiQ1[0] << ", var(Xi) = " << sqrt(var_xi_0) << std::endl;
  return ( iC > 0 ? cov_opp /* / sqrt( avg_var_k[0][1] * avg_var_xi[1][1] ) */ : cov_same /* / sqrt( avg_var_k[1][1] * avg_var_xi[1][1] ) */ );
}

void get_subsample_average(const int iCent, const TH1* distr, TH1* avg){
  double corr = distr->GetMean();
  double corr_sigma = distr->GetStdDev() / sqrt(N_SAMPLE - 1);
  avg->SetBinContent(iCent + 1, corr);
  avg->SetBinError(iCent + 1, corr_sigma);
}

#include "../utils/Config.h"

#define CHARGE_XI 0
#define CLOSURE_TEST

const bool test_from_th2 = false;

bool xi_plus = false;
void get_subsample_average(const int iCent, const TH1* distr, TH1* avg);
void compute_average_variance(double res[2][2], const double C1[2], const double C2[2]);

void CorrelationCp (const char* inFileName = "o_merge_parallel", const char* outFileName = "out_merge_parallel"){
  TFile inFile(Form("%s.root", inFileName));
  TFile outFile(Form("%s.root", outFileName), "recreate");

  TH1D *hAvgVarKCentEffCorrCBWC[2][3];
  TH1D *hAvgVarKCentEffCorr_[2][3];
  TH1D *hAvgVarKDistrEffCorrCBWC[2][3][kNCentBins];
  TH1D *hAvgVarKDistrEffCorr_[2][3][kNCentBinsSmall];
  for (int iC = 0; iC < 2; ++iC){
    for (int iCorr = 0; iCorr < 3; ++iCorr){
      hAvgVarKCentEffCorrCBWC[iC][iCorr] = new TH1D(Form("h%sKCentEffCorrCBWC_%s", kAvgVarLabel[iC], kCorrLabel[iCorr]), Form(";Centrality (%%);C_{%d}^{K}", iC + 1), kNCentBins, kCentBins);
      hAvgVarKCentEffCorr_[iC][iCorr] = new TH1D(Form("h%sKCentEffCorr__%s", kAvgVarLabel[iC], kCorrLabel[iCorr]), Form(";Centrality (%%);C_{%d}^{K}", iC + 1), kNCentBinsSmall, kCentBinsSmall);
      for (int iCent = 0; iCent < kNCentBins; ++iCent){
        hAvgVarKDistrEffCorrCBWC[iC][iCorr][iCent] = new TH1D(Form("h%sKDistrEffCorrCBWC_%s_%d", kAvgVarLabel[iC], kCorrLabel[iCorr], iCent), Form(";C_{%d}^{K};Entries", iC + 1), 400000, -20000., 20000.);
      }
      for (int iCent = 0; iCent < kNCentBinsSmall; ++iCent){
        hAvgVarKDistrEffCorr_[iC][iCorr][iCent] = new TH1D(Form("h%sKDistrEffCorr__%s_%d", kAvgVarLabel[iC], kCorrLabel[iCorr], iCent), Form(";C_{%d}^{K};Entries", iC + 1), 400000, -20000., 20000.);
      }
    }
  }
  for (int iS = 0; iS < N_SAMPLE; ++iS){
    std::cout << "subsample = " << iS << "..." << std::endl;
    TProfile* hKaonC1[2];
    TH1D* hKaonC2[2];
    TH1D* hCent = (TH1D*)inFile.Get(Form("subsample_%s%d/hCent_%d", kSubsampleFlag, iS + 1, iS + 1));
    for (int iCorr = 0; iCorr < (isMC ? 2 : 2); ++iCorr){
      std::cout << "corr = " << iCorr << std::endl;
      for (int iM = 0; iM < 2; ++iM){
        hKaonC1[iM] = (TProfile*)inFile.Get(Form("subsample_%s%d/h%sKaonC1_%s_%d", kSubsampleFlag, iS + 1, kAntiMatterLabel[iM], kCorrLabel[iCorr], iS + 1));
        hKaonC2[iM] = (TH1D*)inFile.Get(Form("subsample_%s%d/h%sKaonC2_%s_%d", kSubsampleFlag, iS + 1, kAntiMatterLabel[iM], kCorrLabel[iCorr], iS + 1));
      }
      for (int iCent = 0; iCent < kNCentBins; ++iCent){
        double avg_avg_var_k[][2] = {{0., 0.}, {0., 0.}};
        double avg_avg_var_k_[][2] = {{0., 0.}, {0., 0.}};
        double n_ev_tot = 0.;
        double delta = (kCentBins[iCent + 1] - kCentBins[iCent]) / N_SUB_CENT;
        for (int iSubCent = 0; iSubCent < N_SUB_CENT; ++iSubCent){
          double n_ev = hCent->GetBinContent(hCent->FindBin(kCentBins[iCent] + iSubCent * delta + 0.001));
          n_ev_tot += n_ev;
        
          double kaonC1[] = {0, 0};
          double kaonC2[] = {0, 0}; 
          for (int iM = 0; iM < 2; ++iM){
            // std::cout << "c = " << hCent->FindBin(kCentBins[iCent] + iSubCent * delta) << std::endl;
            kaonC1[iM] = hKaonC1[iM]->GetBinContent(hCent->FindBin(kCentBins[iCent] + iSubCent * delta + 0.001));
            kaonC2[iM] = hKaonC2[iM]->GetBinContent(hCent->FindBin(kCentBins[iCent] + iSubCent * delta + 0.001));
          }
          double avg_var_xi[2][2] = {{0., 0.}, {0., 0.}};
          double avg_var_k[2][2] = {{0., 0.}, {0., 0.}};
          compute_average_variance(avg_var_k, kaonC1, kaonC2);
          for (int iC = 0; iC < 2; ++iC){
            for (int iM = 0; iM < 2; ++iM){
              std::cout << "ev(" << hCent->FindBin(kCentBins[iCent] + iSubCent * delta + 0.001) << ") = " << n_ev << std::endl;
              avg_avg_var_k[iM][iC] += (n_ev * avg_var_k[iM][iC]);
              avg_avg_var_k_[iM][iC] = avg_var_k[iM][iC];
            }
          }
          for (int iC = 0; iC < 2; ++iC){
            hAvgVarKDistrEffCorr_[iC][iCorr][hCent->FindBin(kCentBins[iCent] + iSubCent * delta + 0.001) - 1]->Fill(avg_avg_var_k_[0][iC]);
            //std::cout << hCent->FindBin(kCentBins[iCent] + iSubCent * delta + 0.001) - 1 << std::endl;
          }
        }
        for (int iC = 0; iC < 2; ++iC){
          double avg_var_k_tmp = iC == 0 ? avg_avg_var_k[0][0] : avg_avg_var_k[0][1];
          hAvgVarKDistrEffCorrCBWC[iC][iCorr][iCent]->Fill(avg_var_k_tmp / n_ev_tot);
        }
      }
    }
  }

  for (int iCorr = 0; iCorr < (isMC ? 2 : 2); ++iCorr){
    for (int iC = 0; iC < 2; ++iC){
      for (int iCent = 0; iCent < kNCentBins; ++iCent){
        get_subsample_average(iCent, hAvgVarKDistrEffCorrCBWC[iC][iCorr][iCent], hAvgVarKCentEffCorrCBWC[iC][iCorr]);
        // hCorrDistrEffCorrCBWC[iC][iCorr][iCent]->Write();
      }
      for (int iCentSmall = 0; iCentSmall < kNCentBinsSmall; ++iCentSmall){
        get_subsample_average(iCentSmall, hAvgVarKDistrEffCorr_[iC][iCorr][iCentSmall], hAvgVarKCentEffCorr_[iC][iCorr]);
        //hAvgVarKDistrEffCorr_[iC][iCorr][iCentSmall]->Write();
      }
      hAvgVarKCentEffCorrCBWC[iC][iCorr]->Write();
      hAvgVarKCentEffCorr_[iC][iCorr]->Write();
    }
  }

  outFile.Close();
  inFile.Close();
}

void compute_average_variance(double res[2][2], const double C1[2], const double C2[2]){
  for (int iC = 0; iC < 2; ++iC){
    double avg = C1[iC];
    double var = C2[iC];
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

void get_subsample_average(const int iCent, const TH1* distr, TH1* avg){
  double corr = distr->GetMean();
  double corr_sigma = distr->GetStdDev() / sqrt(N_SAMPLE - 1);
  avg->SetBinContent(iCent + 1, corr);
  avg->SetBinError(iCent + 1, corr_sigma);
}

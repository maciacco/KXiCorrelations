#include "../utils/Config.h"
#include "../utils/Utils.h"
#include "../utils/RooGausDExp.h"

double purity_error(double sig, double bkg, double sig_err, double bkg_err, double sig_bkg_cov = 0.);

bool fitMass = true;
bool kSaveFits = false;

void Calibration(const char* inFileName = "o_lhc17pq_new_calib", const char* outFileName = "LHC17pq_new_calib"){
  gStyle->SetOptStat(0);

  // killing RooFit output
  RooMsgService::instance().setGlobalKillBelow(RooFit::ERROR);
  RooMsgService::instance().setSilentMode(true);
  gErrorIgnoreLevel = kError; // Suppressing warning outputs
  
  TFile f(Form("%s.root", inFileName));
  TFile o(Form("%s.root", outFileName), "recreate");

  TH3F *hMeanITS;
  TH3F *hSigmaITS;
  TH3F *hMeanTPC;
  TH3F *hSigmaTPC;
  TH3F *hMeanTOF;
  TH3F *hSigmaTOF;

  hMeanITS = new TH3F("fCustomITSpidCalib", "hCustomITSpidCalib", 1, 0, 100, 20, 0, 2, 10, -1, 1);
  hSigmaITS = new TH3F("fCustomITSpidSigma", "hSigmaITS", 1, 0, 100, 20, 0, 2, 10, -1, 1);
  hMeanTPC = new TH3F("fCustomTPCpidCalib", "hCustomTPCpidCalib", 1, 0, 100, 20, 0, 2, 10, -1, 1);
  hSigmaTPC = new TH3F("fCustomTPCpidSigma", "hSigmaTPC", 1, 0, 100, 20, 0, 2, 10, -1, 1);
  hMeanTOF = new TH3F("fCustomTOFpidCalib", "hCustomTOFpidCalib", 1, 0, 100, 20, 0, 2, 10, -1, 1);
  hSigmaTOF = new TH3F("fCustomTOFpidSigma", "hSigmaTOF", 1, 0, 100, 20, 0, 2, 10, -1, 1);

  for (int iE = 0; iE < kNEtaBins; ++iE){
    TH3F *hNsigmaITS = (TH3F*)f.Get(Form("h%sNsigmaITS_%d", kAntiMatterLabel[0], iE));
    TH3F *hNsigmaTPC = (TH3F*)f.Get(Form("h%sNsigmaTPC_%d", kAntiMatterLabel[0], iE));
    TH3F *hNsigmaTOF = (TH3F*)f.Get(Form("h%sNsigmaTOF_%d", kAntiMatterLabel[0], iE));
    TH3F *hNsigmaITS_1 = (TH3F*)f.Get(Form("h%sNsigmaITS_%d", kAntiMatterLabel[1], iE));
    TH3F *hNsigmaTPC_1 = (TH3F*)f.Get(Form("h%sNsigmaTPC_%d", kAntiMatterLabel[1], iE));
    TH3F *hNsigmaTOF_1 = (TH3F*)f.Get(Form("h%sNsigmaTOF_%d", kAntiMatterLabel[1], iE));
    hNsigmaITS->Add(hNsigmaITS_1);
    hNsigmaTPC->Add(hNsigmaTPC_1);
    hNsigmaTOF->Add(hNsigmaTOF_1);

    TH1D *hPurity[kNCentBins];

    for (int iCent = 1; iCent < 1/* kNCentBins */ + 1; ++iCent){
      int iCentMax = hNsigmaTPC->GetNbinsX();
      std::cout << "iCentMax = " << iCentMax << std::endl; 
      for (int iP = 1; iP < 16; ++iP){
        if (iP < 3 || iP > 16) continue;
        TH1D* hNsigmaITSProj = (TH1D*)hNsigmaITS->ProjectionZ(Form("hNsigmaITSProj_%d_%d_%d", iCent, iP, iE), iCent, iCentMax, iP, iP);
        TH1D* hNsigmaTPCProj = (TH1D*)hNsigmaTPC->ProjectionZ(Form("hNsigmaTPCProj_%d_%d_%d", iCent, iP, iE), iCent, iCentMax, iP, iP);
        TH1D* hNsigmaTOFProj = (TH1D*)hNsigmaTOF->ProjectionZ(Form("hNsigmaTOFProj_%d_%d_%d", iCent, iP, iE), iCent, iCentMax, iP, iP);
        TF1 fTPC("fTPC", "gaus(0) + [3]*TMath::Exp([4]*x)", -4., 2.);
        TF1 fTPCBkg("fTPCBkg", "[0]*TMath::Exp([1]*x)", -4., 2.);
        
        //hMassProj->Write();
        if (kRebinNsigma){
          //hNsigmaTPCProj->Rebin(2);
          //hNsigmaTOFProj->Rebin(2);
        }
        if ((hNsigmaITS->GetYaxis()->GetBinCenter(iP) < kITSptCut && hNsigmaITS->GetYaxis()->GetBinCenter(iP) > kPtLowLimitK) || kComputeNSigmaMap){
          RooRealVar ITSSignal("ITSSignal", "n#sigma_{K}", -1.5, 1.5, "a.u.");
          RooDataHist ITSDataHist("ITSDataHist", "ITSDataHist", RooArgList(ITSSignal), hNsigmaITSProj);
          RooRealVar ITSMu("#mu", "ITSMu", -1., 1.);
          RooRealVar ITSSigma("#sigma", "ITSSigma", 0.2, 2.);
          RooRealVar ITSAlphaL("#alpha_{L}", "ITSAlphaL", -1.3, -0.7);
          RooRealVar ITSAlphaR("#alpha_{R}", "ITSAlphaR", 0.7, 1.5);
          RooRealVar ITSTauBkg("#tau", "ITSTauBkg", -100., 0.);
          RooRealVar ITSNSignal("N_{sig}", "ITSNSignal", 0., 1.e9);
          RooRealVar ITSNBkg("N_{bkg}", "ITSNBkg", 0., hNsigmaITS->GetYaxis()->GetBinCenter(iP) < 0.4 ? 1.e7 : 1.e8);
          RooAbsPdf *ITSSignalPDF = new RooGausDExp("ITSSignalPDF", "ITSSignalPDF", ITSSignal, ITSMu, ITSSigma, ITSAlphaL, ITSAlphaR);
          RooAbsPdf *ITSBackgroundPDF = new RooExponential("ITSBackgroundPDF", "ITSBackgroundPDF", ITSSignal, ITSTauBkg);
          RooAddPdf *ITSModel = new RooAddPdf("ITSModel", "ITSModel", RooArgList(*ITSSignalPDF/* , *ITSBackgroundPDF */), RooArgList(ITSNSignal/* , ITSNBkg */));
          for (int i = 0; i < 4; ++i) ITSModel->fitTo(ITSDataHist, RooFit::Save());
          const char* nameITS = Form("fNsigmaITS_%.0f_%.0f_%.1f_%.1f", hNsigmaITS->GetXaxis()->GetBinLowEdge(iCent), hNsigmaITS->GetXaxis()->GetBinUpEdge(iCentMax), hNsigmaITS->GetYaxis()->GetBinLowEdge(iP), hNsigmaITS->GetYaxis()->GetBinUpEdge(iP));
          RooPlot *ITSFrame = (RooPlot*)ITSSignal.frame(RooFit::Name(nameITS));
          ITSDataHist.plotOn(ITSFrame, RooFit::Name("data"));
          ITSModel->plotOn(ITSFrame, RooFit::Name("model"));
          ITSModel->plotOn(ITSFrame, RooFit::Name("model"), RooFit::Components("ITSSignalPDF"), RooFit::LineColor(kRed));
          //ITSModel->plotOn(ITSFrame, RooFit::Name("model"), RooFit::Components("ITSBackgroundPDF"), RooFit::LineColor(kGreen));
          ITSSignal.setRange("signalRange", kNsigmaITScutAsym[0], kNsigmaITScutAsym[1]);

          if (kSaveFits){
            //std::cout << "pt = (" << iP << "), purity = " << purity << ", sigIntegral = " << sigIntegral << ", bkgIntegral = " << bkgIntegral << std::endl;
            TCanvas cITS(nameITS, nameITS);
            cITS.cd();
            ITSFrame->Draw();
            cITS.Write();
          }
          hMeanITS->SetBinContent(iCent, iP, iE + 1, ITSMu.getVal());
          hMeanITS->SetBinError(iCent, iP, iE + 1, ITSSigma.getVal());
          //hSigmaITS->SetBinContent(iCent, iP, iE + 1, ITSSigma.getVal());
        }
        if ((hNsigmaTPC->GetYaxis()->GetBinCenter(iP) < kTPCptCut && hNsigmaTPC->GetYaxis()->GetBinCenter(iP) > kPtLowLimitK) || kComputeNSigmaMap){
          RooRealVar tpcSignal("tpcSignal", "n#sigma_{K}", -1.5, 1.5, "a.u.");
          RooDataHist tpcDataHist("tpcDataHist", "tpcDataHist", RooArgList(tpcSignal), hNsigmaTPCProj);
          RooRealVar tpcMu("#mu", "tpcMu", -1., 1.);
          RooRealVar tpcSigma("#sigma", "tpcSigma", 0.2, 2.);
          RooRealVar tpcAlphaL("#alpha_{L}", "tpcAlphaL", -1.3, -0.7);
          RooRealVar tpcAlphaR("#alpha_{R}", "tpcAlphaR", 0.7, 1.5);
          RooRealVar tpcTauBkg("#tau", "tpcTauBkg", -100., 0.);
          RooRealVar tpcNSignal("N_{sig}", "tpcNSignal", 0., 1.e9);
          RooRealVar tpcNBkg("N_{bkg}", "tpcNBkg", 0., hNsigmaTPC->GetYaxis()->GetBinCenter(iP) < 0.4 ? 1.e7 : 1.e8);
          RooAbsPdf *tpcSignalPDF = new RooGausDExp("tpcSignalPDF", "tpcSignalPDF", tpcSignal, tpcMu, tpcSigma, tpcAlphaL, tpcAlphaR);
          RooAbsPdf *tpcBackgroundPDF = new RooExponential("tpcBackgroundPDF", "tpcBackgroundPDF", tpcSignal, tpcTauBkg);
          RooAddPdf *tpcModel = new RooAddPdf("tpcModel", "tpcModel", RooArgList(*tpcSignalPDF/* , *tpcBackgroundPDF */), RooArgList(tpcNSignal/* , tpcNBkg */));
          for (int i = 0; i < 4; ++i) tpcModel->fitTo(tpcDataHist, RooFit::Save());
          const char* nameTPC = Form("fNsigmaTPC_%.0f_%.0f_%.1f_%.1f", hNsigmaTPC->GetXaxis()->GetBinLowEdge(iCent), hNsigmaTPC->GetXaxis()->GetBinUpEdge(iCentMax), hNsigmaTPC->GetYaxis()->GetBinLowEdge(iP), hNsigmaTPC->GetYaxis()->GetBinUpEdge(iP));
          RooPlot *tpcFrame = (RooPlot*)tpcSignal.frame(RooFit::Name(nameTPC));
          tpcDataHist.plotOn(tpcFrame, RooFit::Name("data"));
          tpcModel->plotOn(tpcFrame, RooFit::Name("model"));
          tpcModel->plotOn(tpcFrame, RooFit::Name("model"), RooFit::Components("tpcSignalPDF"), RooFit::LineColor(kRed));
          //tpcModel->plotOn(tpcFrame, RooFit::Name("model"), RooFit::Components("tpcBackgroundPDF"), RooFit::LineColor(kGreen));
          tpcSignal.setRange("signalRange", kNsigmaTPCcutAsym[0], kNsigmaTPCcutAsym[1]);

          if (kSaveFits){
            //std::cout << "pt = (" << iP << "), purity = " << purity << ", sigIntegral = " << sigIntegral << ", bkgIntegral = " << bkgIntegral << std::endl;
            TCanvas cTPC(nameTPC, nameTPC);
            cTPC.cd();
            tpcFrame->Draw();
            cTPC.Write();
          }
          hMeanTPC->SetBinContent(iCent, iP, iE + 1, tpcMu.getVal());
          hMeanTPC->SetBinError(iCent, iP, iE + 1, tpcSigma.getVal());
          //hSigmaTPC->SetBinContent(iCent, iP, iE + 1, tpcSigma.getVal());
        }
        if ((hNsigmaTOF->GetYaxis()->GetBinCenter(iP) > kTPCptCut && hNsigmaTOF->GetYaxis()->GetBinCenter(iP) < kTOFptCut) || kComputeNSigmaMap){
          RooRealVar tofSignal("tofSignal", "n#sigma_{K}", -5., 5., "a.u.");
          RooDataHist tofDataHist("tofDataHist", "tofDataHist", RooArgList(tofSignal), hNsigmaTOFProj);
          RooRealVar tofMu("#mu", "tofMu", -2., 2.);
          RooRealVar tofSigma("#sigma", "tofSigma", 0.5, 1.5);
          RooRealVar tofAlphaL("#alpha_{L}", "tofAlphaL", -1.5, -0.7);
          RooRealVar tofAlphaR("#alpha_{R}", "tofAlphaR", 0.7, 1.5);
          RooRealVar tofTauBkg("#tau", "tofTauBkg", -10., 0.);
          RooRealVar tofNSignal("N_{sig}", "tofNSignal", 0., 1.e9);
          RooRealVar tofNBkg("N_{bkg}", "tofNBkg", 0., 1.e8);
          RooAbsPdf *tofSignalPDF = new RooGausDExp("tofSignalPDF", "tofSignalPDF", tofSignal, tofMu, tofSigma, tofAlphaL, tofAlphaR);
          RooAbsPdf *tofBackgroundPDF = new RooExponential("tofBackgroundPDF", "tofBackgroundPDF", tofSignal, tofTauBkg);
          RooAddPdf *tofModel = new RooAddPdf("tofModel", "tofModel", RooArgList(*tofSignalPDF, *tofBackgroundPDF), RooArgList(tofNSignal, tofNBkg));
          for (int i = 0; i < 4; ++i) tofModel->fitTo(tofDataHist, RooFit::Save());
          const char* nameTOF = Form("fNsigmaTOF_%.0f_%.0f_%.1f_%.1f", hNsigmaTOF->GetXaxis()->GetBinLowEdge(iCent), hNsigmaTOF->GetXaxis()->GetBinUpEdge(iCentMax), hNsigmaTOF->GetYaxis()->GetBinLowEdge(iP), hNsigmaTOF->GetYaxis()->GetBinUpEdge(iP));
          RooPlot *tofFrame = (RooPlot*)tofSignal.frame(RooFit::Name(nameTOF));
          tofDataHist.plotOn(tofFrame, RooFit::Name("data"));
          tofModel->plotOn(tofFrame, RooFit::Name("model"));
          tofModel->plotOn(tofFrame, RooFit::Name("model"), RooFit::Components("tofSignalPDF"), RooFit::LineColor(kRed));
          tofModel->plotOn(tofFrame, RooFit::Name("model"), RooFit::Components("tofBackgroundPDF"), RooFit::LineColor(kGreen));
          tofSignal.setRange("signalRange", kNsigmaTOFcutAsym[0], kNsigmaTOFcutAsym[1]);

          if (kSaveFits){
            // std::cout << "pt = (" << iP << "), purity = " << purity << std::endl;
            TCanvas cTOF(nameTOF, nameTOF);
            cTOF.cd();
            tofFrame->Draw();
            cTOF.Write();
          }
          hMeanTOF->SetBinContent(iCent, iP, iE + 1, tofMu.getVal());
          hMeanTOF->SetBinError(iCent, iP, iE + 1, tofSigma.getVal());
          //hSigmaTOF->SetBinContent(iCent, iP, iE + 1, tofSigma.getVal());
        }
      }
    }

    o.cd();

    // hNsigmaTPC->Write();
    // hNsigmaTOF->Write();
    // hMass->Write();
  }
  hMeanITS->SetDrawOption("box2");
  hMeanTPC->SetDrawOption("box2");
  hMeanTOF->SetDrawOption("box2");
  hMeanITS->Write();
  hMeanTPC->Write();
  //hSigmaTPC->Write();
  hMeanTOF->Write();
  //hSigmaTOF->Write();
  TCanvas cITS("cITS", "cITS");
  cITS.cd();
  hMeanITS->Draw("box2");
  TCanvas cTPC("cTPC", "cTPC");
  cTPC.cd();
  hMeanTPC->Draw("box2");
  TCanvas cTOF("cTOF", "cTOF");
  cTOF.cd();
  hMeanTOF->Draw("box2");

  cITS.Print("LHC21l5_calib_ITS.png");
  cTPC.Print("LHC21l5_calib_TPC.png");
  cTOF.Print("LHC21l5_calib_TOF.png");

  f.Close();
  o.Close();
}

double purity_error(double sig, double bkg, double sig_err, double bkg_err, double sig_bkg_cov = 0.){
  double t = sig + bkg;
  return bkg / t * TMath::Sqrt(1./t + 1./bkg);
}
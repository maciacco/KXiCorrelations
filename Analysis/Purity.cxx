#include "../utils/Config.h"
#include "../utils/Utils.h"
#include "../utils/RooGausDExp.h"

double purity_error(double sig, double bkg, double sig_err, double bkg_err, double sig_bkg_cov = 0.);

void Purity(const char* inFileName = "o_merge_parallel", const char* outFileName = "Purity"){
  gStyle->SetOptStat(0);

  // killing RooFit output
  RooMsgService::instance().setGlobalKillBelow(RooFit::ERROR);
  RooMsgService::instance().setSilentMode(true);
  gErrorIgnoreLevel = kError; // Suppressing warning outputs
  
  TFile f(Form("%s.root", inFileName));
  TFile o(Form("%s.root", outFileName), "recreate");

  TH3F *hNsigmaTPC = (TH3F*)f.Get("hANsigmaTPC");
  TH3F *hNsigmaTOF = (TH3F*)f.Get("hANsigmaTOF");
  TH3F *hMass = (TH3F*)f.Get("hAMass");

  TH1D *hPurity[kNCentBins];
  TCanvas cPurity("Purity_K", "Purity_K");
  TLegend lPurity(0.5, 0.5, 0.6, 0.7);

  for (int iCent = 1; iCent < kNCentBins + 1; ++iCent){
    hPurity[iCent - 1] = new TH1D(Form("hPurity_%.0f_%.0f", hNsigmaTPC->GetXaxis()->GetBinLowEdge(iCent), hNsigmaTPC->GetXaxis()->GetBinUpEdge(iCent)), ";#it{p}_{T} (GeV/#it{c});S / (S + B)", kNBinsPt, kPtLowLimitK, kPtLowLimitK + kDeltaPt * kNBinsPt);
    for (int iPart = 0; iPart < 1; ++iPart){
      for (int iP = 1; iP < 15; ++iP){
        if (iP < 3 || iP > 10) continue;
        TH1D* hNsigmaTPCProj = (TH1D*)hNsigmaTPC->ProjectionZ(Form("hNsigmaTPCProj_%d_%d", iCent, iP), iCent, iCent, iP, iP);
        TH1D* hNsigmaTOFProj = (TH1D*)hNsigmaTOF->ProjectionZ(Form("hNsigmaTOFProj_%d_%d", iCent, iP), iCent, iCent, iP, iP);
        TH1D* hMassProj = (TH1D*)hMass->ProjectionZ(Form("hMassProj_%d_%d", iCent, iP), iCent, iCent, iP, iP);
        TF1 fTPC("fTPC", "gaus(0) + [3]*TMath::Exp([4]*x)", -4., 2.);
        TF1 fTPCBkg("fTPCBkg", "[0]*TMath::Exp([1]*x)", -4., 2.);
        
        hMassProj->Write();
        if (kRebinNsigma){
          //hNsigmaTPCProj->Rebin(2);
          //hNsigmaTOFProj->Rebin(2);
        }
        double purity = 0., error = 0.;;
        if (hNsigmaTPC->GetYaxis()->GetBinCenter(iP) < kTPCptCut && hNsigmaTPC->GetYaxis()->GetBinCenter(iP) > kPtLowLimitK){
          RooRealVar tpcSignal("tpcSignal", "n#sigma_{K}", -3.5, 3.5, "a.u.");
          RooDataHist tpcDataHist("tpcDataHist", "tpcDataHist", RooArgList(tpcSignal), hNsigmaTPCProj);
          RooRealVar tpcMu("#mu", "tpcMu", -2., 2.);
          RooRealVar tpcSigma("#sigma", "tpcSigma", 0., 2.);
          RooRealVar tpcAlphaL("#alpha_{L}", "tpcAlphaL", -1.3, -0.5);
          RooRealVar tpcAlphaR("#alpha_{R}", "tpcAlphaR", 0.7, 1.5);
          RooRealVar tpcTauBkg("#tau", "tpcTauBkg", -100., 0.);
          RooRealVar tpcNSignal("N_{sig}", "tpcNSignal", 0., 1.e9);
          RooRealVar tpcNBkg("N_{bkg}", "tpcNBkg", 0., hNsigmaTPC->GetYaxis()->GetBinCenter(iP) < 0.3 ? 1.e8 : 1.e9);
          RooAbsPdf *tpcSignalPDF = new RooGausDExp("tpcSignalPDF", "tpcSignalPDF", tpcSignal, tpcMu, tpcSigma, tpcAlphaL, tpcAlphaR);
          RooAbsPdf *tpcBackgroundPDF = new RooExponential("tpcBackgroundPDF", "tpcBackgroundPDF", tpcSignal, tpcTauBkg);
          RooAddPdf *tpcModel = new RooAddPdf("tpcModel", "tpcModel", RooArgList(*tpcSignalPDF, *tpcBackgroundPDF), RooArgList(tpcNSignal, tpcNBkg));
          for (int i = 0; i < 2; ++i) tpcModel->fitTo(tpcDataHist, RooFit::Save());
          const char* nameTPC = Form("fANsigmaTPC_%.0f_%.0f_%.1f_%.1f", hNsigmaTPC->GetXaxis()->GetBinLowEdge(iCent), hNsigmaTPC->GetXaxis()->GetBinUpEdge(iCent), hNsigmaTPC->GetYaxis()->GetBinLowEdge(iP), hNsigmaTPC->GetYaxis()->GetBinUpEdge(iP));
          RooPlot *tpcFrame = (RooPlot*)tpcSignal.frame(RooFit::Name(nameTPC));
          tpcDataHist.plotOn(tpcFrame, RooFit::Name("data"));
          tpcModel->plotOn(tpcFrame, RooFit::Name("model"));
          tpcModel->plotOn(tpcFrame, RooFit::Name("model"), RooFit::Components("tpcSignalPDF"), RooFit::LineColor(kRed));
          tpcModel->plotOn(tpcFrame, RooFit::Name("model"), RooFit::Components("tpcBackgroundPDF"), RooFit::LineColor(kGreen));
          tpcSignal.setRange("signalRange", kNsigmaTPCcutAsym[0], kNsigmaTPCcutAsym[1]);
          double sigIntegral = (((RooAbsPdf *)(tpcModel->pdfList().at(0)))->createIntegral(RooArgSet(tpcSignal), RooFit::NormSet(RooArgSet(tpcSignal)), RooFit::Range("signalRange")))->getVal();
          double bkgIntegral = (((RooAbsPdf *)(tpcModel->pdfList().at(1)))->createIntegral(RooArgSet(tpcSignal), RooFit::NormSet(RooArgSet(tpcSignal)), RooFit::Range("signalRange")))->getVal();
          double bkgTPC = tpcNBkg.getVal() * bkgIntegral;
          double sigTPC = tpcNSignal.getVal() * sigIntegral;
          purity = sigTPC/(sigTPC + bkgTPC);
          error = purity_error(sigTPC, bkgTPC, tpcNSignal.getError() * sigIntegral, tpcNBkg.getError() * bkgIntegral);
          if (purity > 1.e-12){
            //std::cout << "pt = (" << iP << "), purity = " << purity << ", sigIntegral = " << sigIntegral << ", bkgIntegral = " << bkgIntegral << std::endl;
            TCanvas cTPC(nameTPC, nameTPC);
            TLatex t;
            cTPC.cd();
            tpcFrame->Draw();
            t.DrawLatexNDC(0.7, 0.7, Form("Purity = %.3f", purity));
            cTPC.Write();
          }
        }
        else if (hNsigmaTOF->GetYaxis()->GetBinCenter(iP) > kTPCptCut && hNsigmaTOF->GetYaxis()->GetBinCenter(iP) < kTOFptCut){
          RooRealVar tofSignal("tofSignal", "n#sigma_{K}", -5., 5., "a.u.");
          RooDataHist tofDataHist("tofDataHist", "tofDataHist", RooArgList(tofSignal), hNsigmaTOFProj);
          RooRealVar tofMu("#mu", "tofMu", -2., 2.);
          RooRealVar tofSigma("#sigma", "tofSigma", 0.5, 1.5);
          RooRealVar tofAlphaL("#alpha_{L}", "tofAlphaL", -1.5, -0.7);
          RooRealVar tofAlphaR("#alpha_{R}", "tofAlphaR", 0.7, 1.5);
          RooRealVar tofTauBkg("#tau", "tofTauBkg", -5., 0.);
          RooRealVar tofNSignal("N_{sig}", "tofNSignal", 0., 1.e9);
          RooRealVar tofNBkg("N_{bkg}", "tofNBkg", 0., 1.e8);
          RooAbsPdf *tofSignalPDF = new RooGausDExp("tofSignalPDF", "tofSignalPDF", tofSignal, tofMu, tofSigma, tofAlphaL, tofAlphaR);
          RooAbsPdf *tofBackgroundPDF = new RooExponential("tofBackgroundPDF", "tofBackgroundPDF", tofSignal, tofTauBkg);
          RooAddPdf *tofModel = new RooAddPdf("tofModel", "tofModel", RooArgList(*tofSignalPDF, *tofBackgroundPDF), RooArgList(tofNSignal, tofNBkg));
          for (int i = 0; i < 2; ++i) tofModel->fitTo(tofDataHist, RooFit::Save());
          const char* nameTOF = Form("fANsigmaTOF_%.0f_%.0f_%.1f_%.1f", hNsigmaTOF->GetXaxis()->GetBinLowEdge(iCent), hNsigmaTOF->GetXaxis()->GetBinUpEdge(iCent), hNsigmaTOF->GetYaxis()->GetBinLowEdge(iP), hNsigmaTOF->GetYaxis()->GetBinUpEdge(iP));
          RooPlot *tofFrame = (RooPlot*)tofSignal.frame(RooFit::Name(nameTOF));
          tofDataHist.plotOn(tofFrame, RooFit::Name("data"));
          tofModel->plotOn(tofFrame, RooFit::Name("model"));
          tofModel->plotOn(tofFrame, RooFit::Name("model"), RooFit::Components("tofSignalPDF"), RooFit::LineColor(kRed));
          tofModel->plotOn(tofFrame, RooFit::Name("model"), RooFit::Components("tofBackgroundPDF"), RooFit::LineColor(kGreen));
          tofSignal.setRange("signalRange", kNsigmaTOFcutAsym[0], kNsigmaTOFcutAsym[1]);
          double sigIntegralTOF = (((RooAbsPdf *)(tofModel->pdfList().at(0)))->createIntegral(RooArgSet(tofSignal), RooFit::NormSet(RooArgSet(tofSignal)), RooFit::Range("signalRange")))->getVal();
          double bkgIntegralTOF = (((RooAbsPdf *)(tofModel->pdfList().at(1)))->createIntegral(RooArgSet(tofSignal), RooFit::NormSet(RooArgSet(tofSignal)), RooFit::Range("signalRange")))->getVal();
          double bkgTOF = tofNBkg.getVal() * bkgIntegralTOF;
          double sigTOF = tofNSignal.getVal() * sigIntegralTOF;
          purity = sigTOF/(sigTOF + bkgTOF);
          error = purity_error(sigTOF, bkgTOF, tofNSignal.getError() * sigIntegralTOF, tofNBkg.getError() * bkgIntegralTOF);
          if (purity > 1.e-12){
            // std::cout << "pt = (" << iP << "), purity = " << purity << std::endl;
            TCanvas cTOF(nameTOF, nameTOF);
            cTOF.cd();
            tofFrame->Draw();
            TLatex t;
            t.DrawLatexNDC(0.7, 0.7, Form("Purity = %.3f", purity));
            cTOF.Write();
          }
        }

        std::cout << "pt = (" << iP << "), purity = " << purity << std::endl;
        hPurity[iCent - 1]->SetBinContent(iP, purity);
        hPurity[iCent - 1]->SetBinError(iP, error);
      }
    }
    hPurity[iCent - 1]->SetMarkerStyle(20);
    hPurity[iCent - 1]->SetMarkerSize(1.2);
    // hPurity[iCent - 1]->SetMarkerColor(gStyle->GetColorPalette((iCent - 1) * 255 / kNCentBins));
    // hPurity[iCent - 1]->SetLineColor(gStyle->GetColorPalette((iCent - 1) * 255 / kNCentBins));
    hPurity[iCent - 1]->Write();
    cPurity.cd();

    hPurity[iCent - 1]->Draw(iCent == 1 ? "pe x0 plc pmc" : "pe x0 same plc pmc");
  }

  o.cd();
  cPurity.cd();
  lPurity.Draw("same");
  cPurity.Write();
  hNsigmaTPC->Write();
  hNsigmaTOF->Write();
  hMass->Write();
  f.Close();
  o.Close();
}

double purity_error(double sig, double bkg, double sig_err, double bkg_err, double sig_bkg_cov = 0.){
  double t = sig + bkg;
  return bkg / t * TMath::Sqrt(1./t + 1./bkg);
}
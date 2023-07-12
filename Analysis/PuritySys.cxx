#include "../utils/Config_tests.h"
#include "../utils/Utils.h"
#include "../utils/RooGausDExp.h"

double purity_error(double sig, double bkg, double sig_err, double bkg_err, double sig_bkg_cov = 0.);

bool fitMass = true;

void PuritySys(const int iVar = 0, const char* inFileName = "o_lhc18qr_purity_cuts_var", const char* outFileName = "Purity_18qr_var_"){
  int iTpcClsCut = (iVar / 1) % kNTpcClsCuts;
  int iPidCut = (iVar / kNTpcClsCuts) % kNPidCuts;
  int iDcaCut = (iVar / kNTpcClsCuts / kNPidCuts) % kNDcaCuts;
  int iChi2Cut = (iVar / kNTpcClsCuts / kNPidCuts / kNDcaCuts) % kNChi2Cuts;
  int iMassCut = (iVar / 1) % kNMassCuts;
  int iBdtScoreCut = (iVar / 3) % kNBdtCuts;

  if (iVar > 14) fitMass = false;

  gStyle->SetOptStat(0);

  // killing RooFit output
  RooMsgService::instance().setGlobalKillBelow(RooFit::ERROR);
  RooMsgService::instance().setSilentMode(true);
  gErrorIgnoreLevel = kError; // Suppressing warning outputs
  
  TFile f(Form("%s_%d.root", inFileName, iVar));
  TFile o(Form("purity_study/%s_%d.root", outFileName, iVar), "recreate");

  TH3F *hMeanTPC[2];
  TH3F *hSigmaTPC[2];
  TH3F *hMeanTOF[2];
  TH3F *hSigmaTOF[2];
  for (int iM = 0; iM < 2; ++iM){
    hMeanTPC[iM] = new TH3F(Form("hMeanTPC_M%d", iM), Form("hMeanTPC_M%d", iM), 10, 0, 100, 20, 0, 2, 10, -1, 1);
    hSigmaTPC[iM] = new TH3F(Form("hSigmaTPC_M%d", iM), Form("hSigmaTPC_M%d", iM), 10, 0, 100, 20, 0, 2, 10, -1, 1);
    hMeanTOF[iM] = new TH3F(Form("hMeanTOF_M%d", iM), Form("hMeanTOF_M%d", iM), 10, 0, 100, 20, 0, 2, 10, -1, 1);
    hSigmaTOF[iM] = new TH3F(Form("hSigmaTOF_M%d", iM), Form("hSigmaTOF_M%d", iM), 10, 0, 100, 20, 0, 2, 10, -1, 1);
  }
  for (int iE = 0; iE < kNEtaBins; ++iE){
    for (int iM = 0; iM < 2; ++iM){
      TH3F *hNsigmaTPC = (TH3F*)f.Get(Form("h%sNsigmaTPC_%d_%d", kAntiMatterLabel[iM], iE, iVar));
      TH3F *hNsigmaTOF = (TH3F*)f.Get(Form("h%sNsigmaTOF_%d_%d", kAntiMatterLabel[iM], iE, iVar));
      TH3F *hMass = (TH3F*)f.Get(Form("h%sMass_%d", kAntiMatterLabel[iM], iVar));
      TH1D *hPurity[kNCentBins];
      TH1D *hPurityXi[kNCentBins];
      TCanvas cPurity(Form("%sPurity_K_%d_%d_%d_%d_%d", kAntiMatterLabel[iM], iE, iTpcClsCut, iPidCut, iDcaCut, iChi2Cut), Form("%sPurity_K", kAntiMatterLabel[iM]));
      TCanvas cPurityXi(Form("%sPurity_Xi_%d_%d_%d", kAntiMatterLabel[iM], iE, iMassCut, iBdtScoreCut), Form("%sPurity_Xi", kAntiMatterLabel[iM]));
      TLegend lPurity(0.2, 0.2, 0.4, 0.5);
      lPurity.SetTextFont(44);
      lPurity.SetTextSize(25);
      TLegend lPurityXi(0.2, 0.2, 0.4, 0.5);
      lPurityXi.SetTextFont(44);
      lPurityXi.SetTextSize(25);

      for (int iCent = 1; iCent < kNCentBins + 1; ++iCent){
        double ptBinsXi[kNBinsPtXi + 1];
        for (int iB = 0; iB < kNBinsPtXi + 1; ++iB){
          double f = iB < kNBinsPtXi ? 0. : 1.;
          ptBinsXi[iB] = kMinPt + kDeltaPtXi * iB + f * kDeltaPtXi;
        }
        hPurity[iCent - 1] = new TH1D(Form("hPurityK_%.0f_%.0f_%d", hNsigmaTPC->GetXaxis()->GetBinLowEdge(iCent), hNsigmaTPC->GetXaxis()->GetBinUpEdge(iCent), iE), ";#it{p}_{T} (GeV/#it{c});S / (S + B)", kNBinsPt, 0., kDeltaPt * kNBinsPt);
        hPurityXi[iCent - 1] = new TH1D(Form("hPurityXi_%.0f_%.0f_%d", hMass->GetXaxis()->GetBinLowEdge(iCent), hMass->GetXaxis()->GetBinUpEdge(iCent), iE), ";#it{p}_{T} (GeV/#it{c});S / (S + B)", kNBinsPtXi, ptBinsXi);
        for (int iP = 1; iP < 15; ++iP){
          if (iP < 3 || iP > 10) continue;
          TH1D* hNsigmaTPCProj = (TH1D*)hNsigmaTPC->ProjectionZ(Form("hNsigmaTPCProj_%d_%d_%d", iCent, iP, iE), iCent, iCent, iP, iP);
          TH1D* hNsigmaTOFProj = (TH1D*)hNsigmaTOF->ProjectionZ(Form("hNsigmaTOFProj_%d_%d_%d", iCent, iP, iE), iCent, iCent, iP, iP);
          TH1D* hMassProj = (TH1D*)hMass->ProjectionZ(Form("hMassProj_%d_%d", iCent, iP), iCent, iCent, iP, iP);
          TF1 fTPC("fTPC", "gaus(0) + [3]*TMath::Exp([4]*x)", -4., 2.);
          TF1 fTPCBkg("fTPCBkg", "[0]*TMath::Exp([1]*x)", -4., 2.);
          
          //hMassProj->Write();
          if (kRebinNsigma){
            //hNsigmaTPCProj->Rebin(2);
            //hNsigmaTOFProj->Rebin(2);
          }
          double purity = 0., error = 0.;
          if (fitMass){
            RooRealVar *roo_m = new RooRealVar("m", "#it{M} (#Lambda + #pi^{-})", 1.305, 1.34, "GeV/#it{c}^{2}");
            //roo_m->setBins(25);
            hMassProj->Rebin(2);
            RooDataHist roo_data("data", "data", RooArgSet(*roo_m), hMassProj);

            RooRealVar mass("mass", "mass", 1.31, 1.335);
            RooRealVar sigma_left_mc("#sigma", "sigma", 0.001, 0.003);
            RooRealVar sigma_right_mc("sigma_right", "sigma_right", 0.001, 0.003);
            RooRealVar alpha_left_mc("alpha_left", "alpha_left", 0., 5.);
            RooRealVar alpha_right_mc("alpha_right", "alpha_right", 0., 5.);
            RooRealVar n_left_mc("n_left", "n_left", 1., 30.);
            RooRealVar n_right_mc("n_right", "n_right", 1., 30.);
            RooDSCBShape roo_signal("signal", "signal", *roo_m, mass, sigma_left_mc, alpha_left_mc, n_left_mc, alpha_right_mc, n_right_mc); 
            
            RooRealVar roo_n_signal("N_{sig}", "Nsignal", 1., 1.e7);
            RooRealVar roo_n_background("N_{bkg}", "Nbackground", 1., 1.e5);
            RooRealVar roo_slope("slope", "slope", -20., 20.);
            RooExponential roo_bkg("background", "background", *roo_m, roo_slope);

            RooAddPdf roo_model("model", "model", RooArgList(roo_signal, roo_bkg), RooArgList(roo_n_signal, roo_n_background));
            for (int i = 0; i < 3; ++i) roo_model.fitTo(roo_data, RooFit::Save());
            const char* nameMass = Form("f%sMass_%.0f_%.0f_%.1f_%.1f", kAntiMatterLabel[iM], hMass->GetXaxis()->GetBinLowEdge(iCent), hMass->GetXaxis()->GetBinUpEdge(iCent), hMass->GetYaxis()->GetBinLowEdge(iP), hMass->GetYaxis()->GetBinUpEdge(iP));
            RooPlot *massFrame = (RooPlot*)roo_m->frame(RooFit::Name(nameMass), RooFit::Title(" "));
            roo_data.plotOn(massFrame, RooFit::Name("data"));
            roo_model.plotOn(massFrame, RooFit::Name("model"));
            roo_model.plotOn(massFrame, RooFit::Name("model"), RooFit::Components("signal"), RooFit::LineColor(kRed));
            roo_model.plotOn(massFrame, RooFit::Name("model"), RooFit::Components("background"), RooFit::LineColor(kGreen));
            roo_model.plotOn(massFrame, RooFit::Name("model"));
            
            roo_m->setRange("signalRange", 1.32171 - ( 0.003 + 0.0015 * iMassCut), 1.32171 + ( 0.003 + 0.0015 * iMassCut)/* 1.316, 1.328 */);
            
            double sigIntegralTOF = (((RooAbsPdf *)(roo_model.pdfList().at(0)))->createIntegral(RooArgSet(*roo_m), RooFit::NormSet(RooArgSet(*roo_m)), RooFit::Range("signalRange")))->getVal();
            double bkgIntegralTOF = (((RooAbsPdf *)(roo_model.pdfList().at(1)))->createIntegral(RooArgSet(*roo_m), RooFit::NormSet(RooArgSet(*roo_m)), RooFit::Range("signalRange")))->getVal();
            double bkgTOF = roo_n_background.getVal() * bkgIntegralTOF;
            double sigTOF = roo_n_signal.getVal() * sigIntegralTOF;
            purity = sigTOF/(sigTOF + bkgTOF);
            error = purity_error(sigTOF, bkgTOF, roo_n_signal.getError() * sigIntegralTOF, roo_n_background.getError() * bkgIntegralTOF);
            std::cout << "mass : purity = " << purity << std::endl; 
            
            TCanvas cMass(nameMass, nameMass);
            cMass.cd();
            massFrame->Draw();
            TLatex t;
            t.DrawLatexNDC(0.7, 0.7, Form("Purity = %.3f", purity));
            cMass.Write();
            hPurityXi[iCent - 1]->SetBinContent(hPurityXi[iCent - 1]->FindBin(hMass->GetYaxis()->GetBinCenter(iP)), purity);
            hPurityXi[iCent - 1]->SetBinError(hPurityXi[iCent - 1]->FindBin(hMass->GetYaxis()->GetBinCenter(iP)), error);
            //massFrame->Write();
          }
          if ((hNsigmaTPC->GetYaxis()->GetBinCenter(iP) < kTPCptCut && hNsigmaTPC->GetYaxis()->GetBinCenter(iP) > kPtLowLimitK) /* || kComputeNSigmaMap */){
            RooRealVar tpcSignal("tpcSignal", "n#sigma_{K}", -4., 4., "a.u.");
            RooDataHist tpcDataHist("tpcDataHist", "tpcDataHist", RooArgList(tpcSignal), hNsigmaTPCProj);
            RooRealVar tpcMu("#mu", "tpcMu", -2., 2.);
            RooRealVar tpcSigma("#sigma", "tpcSigma", 0.2, 5.);
            RooRealVar tpcAlphaL("#alpha_{L}", "tpcAlphaL", -3., -0.2);
            RooRealVar tpcAlphaR("#alpha_{R}", "tpcAlphaR", 0.2, 3.);
            RooRealVar tpcTauBkg("#tau", "tpcTauBkg", -10., 1.);
            RooRealVar tpcNSignal("N_{sig}", "tpcNSignal", 1., 1.e9);
            RooRealVar tpcNBkg("N_{bkg}", "tpcNBkg", 0., 1.e9);
            RooAbsPdf *tpcSignalPDF = new RooGausDExp("tpcSignalPDF", "tpcSignalPDF", tpcSignal, tpcMu, tpcSigma, tpcAlphaL, tpcAlphaR);
            RooAbsPdf *tpcBackgroundPDF = new RooExponential("tpcBackgroundPDF", "tpcBackgroundPDF", tpcSignal, tpcTauBkg);
            RooAddPdf *tpcModel = new RooAddPdf("tpcModel", "tpcModel", RooArgList(*tpcSignalPDF, *tpcBackgroundPDF), RooArgList(tpcNSignal, tpcNBkg));
            for (int i = 0; i < 4; ++i) tpcModel->fitTo(tpcDataHist, RooFit::Save());
            const char* nameTPC = Form("f%sNsigmaTPC_%.0f_%.0f_%.1f_%.1f", kAntiMatterLabel[iM], hNsigmaTPC->GetXaxis()->GetBinLowEdge(iCent), hNsigmaTPC->GetXaxis()->GetBinUpEdge(iCent), hNsigmaTPC->GetYaxis()->GetBinLowEdge(iP), hNsigmaTPC->GetYaxis()->GetBinUpEdge(iP));
            RooPlot *tpcFrame = (RooPlot*)tpcSignal.frame(RooFit::Name(nameTPC));
            tpcDataHist.plotOn(tpcFrame, RooFit::Name("data"));
            tpcModel->plotOn(tpcFrame, RooFit::Name("model"));
            tpcModel->plotOn(tpcFrame, RooFit::Name("model"), RooFit::Components("tpcSignalPDF"), RooFit::LineColor(kRed));
            tpcModel->plotOn(tpcFrame, RooFit::Name("model"), RooFit::Components("tpcBackgroundPDF"), RooFit::LineColor(kGreen));
            tpcSignal.setRange("signalRange", kNsigmaTPCcutAsymP[0] - (-1. + 0.5 * iPidCut), kNsigmaTPCcutAsymP[1] - 1. + 0.5 * iPidCut);
            double sigIntegral = (((RooAbsPdf *)(tpcModel->pdfList().at(0)))->createIntegral(RooArgSet(tpcSignal), RooFit::NormSet(RooArgSet(tpcSignal)), RooFit::Range("signalRange")))->getVal();
            double bkgIntegral = (((RooAbsPdf *)(tpcModel->pdfList().at(1)))->createIntegral(RooArgSet(tpcSignal), RooFit::NormSet(RooArgSet(tpcSignal)), RooFit::Range("signalRange")))->getVal();
            double bkgTPC = tpcNBkg.getVal() * bkgIntegral;
            double sigTPC = tpcNSignal.getVal() * sigIntegral;
            purity = sigTPC/(sigTPC + bkgTPC);
            error = purity_error(sigTPC, bkgTPC, tpcNSignal.getError() * sigIntegral, tpcNBkg.getError() * bkgIntegral);
            if (purity > 1.e-12){
              std::cout << "pt = (" << iP << "), purity = " << purity << ", sigIntegral = " << sigIntegral << ", nbkg = " << tpcNSignal.getVal() << ", bkgIntegral = " << bkgIntegral << ", nbkg = " << tpcNBkg.getVal() << std::endl;
              TCanvas cTPC(nameTPC, nameTPC);
              TLatex t;
              cTPC.cd();
              tpcFrame->Draw();
              t.DrawLatexNDC(0.7, 0.7, Form("Purity = %.3f", purity));
              // cTPC.Write();
            }
            hMeanTPC[iM]->SetBinContent(iCent, iP, iE + 1, tpcMu.getVal());
            hSigmaTPC[iM]->SetBinContent(iCent, iP, iE + 1, tpcSigma.getVal());
          }
          if ((hNsigmaTOF->GetYaxis()->GetBinCenter(iP) > kTPCptCut && hNsigmaTOF->GetYaxis()->GetBinCenter(iP) < kTOFptCut) /* || kComputeNSigmaMap */){
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
            const char* nameTOF = Form("f%sNsigmaTOF_%.0f_%.0f_%.1f_%.1f", kAntiMatterLabel[iM], hNsigmaTOF->GetXaxis()->GetBinLowEdge(iCent), hNsigmaTOF->GetXaxis()->GetBinUpEdge(iCent), hNsigmaTOF->GetYaxis()->GetBinLowEdge(iP), hNsigmaTOF->GetYaxis()->GetBinUpEdge(iP));
            RooPlot *tofFrame = (RooPlot*)tofSignal.frame(RooFit::Name(nameTOF));
            tofDataHist.plotOn(tofFrame, RooFit::Name("data"));
            tofModel->plotOn(tofFrame, RooFit::Name("model"));
            tofModel->plotOn(tofFrame, RooFit::Name("model"), RooFit::Components("tofSignalPDF"), RooFit::LineColor(kRed));
            tofModel->plotOn(tofFrame, RooFit::Name("model"), RooFit::Components("tofBackgroundPDF"), RooFit::LineColor(kGreen));
            tofSignal.setRange("signalRange", kNsigmaTOFcutAsymP[0] - (- 1. + 0.5 * iPidCut), kNsigmaTOFcutAsymP[1] - 1. + 0.5 * iPidCut);
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
              // cTOF.Write();
            }
            hMeanTOF[iM]->SetBinContent(iCent, iP, iE + 1, tofMu.getVal());
            hSigmaTOF[iM]->SetBinContent(iCent, iP, iE + 1, tofSigma.getVal());
          }

          std::cout << "pt = (" << iP << "), purity = " << purity << std::endl;
          hPurity[iCent - 1]->SetBinContent(hPurity[iCent - 1]->FindBin(hNsigmaTPC->GetYaxis()->GetBinCenter(iP)), purity);
          hPurity[iCent - 1]->SetBinError(hPurity[iCent - 1]->FindBin(hNsigmaTPC->GetYaxis()->GetBinCenter(iP)), error);
        }
        hPurity[iCent - 1]->SetMarkerStyle(20);
        hPurity[iCent - 1]->SetMarkerSize(1.2);
        hPurity[iCent - 1]->SetMarkerColor(colors[iCent - 1]);
        hPurity[iCent - 1]->SetLineColor(colors[iCent - 1]);
        //hPurity[iCent - 1]->Write();
        hPurity[iCent - 1]->GetXaxis()->SetRangeUser(0., 1.);
        hPurity[iCent - 1]->GetYaxis()->SetRangeUser(0.8, 1.05);
        cPurity.cd();
        hPurity[iCent - 1]->Draw(iCent == 1 ? "pe x0" : "pe x0 same");
        lPurity.AddEntry(hPurity[iCent - 1], Form("%.0f-%.0f%%", kCentBins[iCent - 1], kCentBins[iCent]));

        hPurityXi[iCent - 1]->SetMarkerStyle(20);
        hPurityXi[iCent - 1]->SetMarkerSize(1.2);
        hPurityXi[iCent - 1]->SetMarkerColor(colors[iCent - 1]);
        hPurityXi[iCent - 1]->SetLineColor(colors[iCent - 1]);
        //hPurityXi[iCent - 1]->Write();
        hPurityXi[iCent - 1]->GetXaxis()->SetRangeUser(0., 3.);
        hPurityXi[iCent - 1]->GetYaxis()->SetRangeUser(0.8, 1.05);
        cPurityXi.cd();
        hPurityXi[iCent - 1]->Draw(iCent == 1 ? "pe x0" : "pe x0 same");
        lPurityXi.AddEntry(hPurityXi[iCent - 1], Form("%.0f-%.0f%%", kCentBins[iCent - 1], kCentBins[iCent]));
      }

      o.cd();
      cPurity.cd();
      lPurity.Draw("same");
      cPurity.Write();
      cPurity.Print(Form("purity_study/%spurity_%d_%d_%d_%d_%d.pdf", kAntiMatterLabel[iM], iE, iTpcClsCut, iPidCut, iDcaCut, iChi2Cut));
      if (fitMass) {
        cPurityXi.cd();
        lPurityXi.Draw("same");
        cPurityXi.Print(Form("purity_study/%spurityXi_%d_%d_%d.pdf", kAntiMatterLabel[iM], iE, iMassCut, iBdtScoreCut));
        cPurityXi.Write();
      }
      // hNsigmaTPC->Write();
      // hNsigmaTOF->Write();
      // hMass->Write();
    }
  }
  for (int iM = 0; iM < 2; ++iM){
    hMeanTPC[iM]->Write();
    hSigmaTPC[iM]->Write();
    hMeanTOF[iM]->Write();
    hSigmaTOF[iM]->Write();
  }
  f.Close();
  o.Close();
}

double purity_error(double sig, double bkg, double sig_err, double bkg_err, double sig_bkg_cov = 0.){
  double t = sig + bkg;
  return bkg / t * TMath::Sqrt(1./t + 1./bkg);
}
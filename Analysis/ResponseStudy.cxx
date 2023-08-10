#include "../utils/Config.h"

void shift_binning(const TH1D* in, TH1D* out, double shift = -0.5);
void set_hist_style(TH1D* h);
void set_line_style(TF1* f);
void set_text_style(TText* t);
void set_text_style(TLegend* l);

void ResponseStudy(const char* inFileName = "test_mc20e3_mcTruth_var_0", const char* outFileName = "Response"){
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  
  TFile f(Form("%s.root", inFileName));
  TFile o(Form("%s.root", outFileName), "recreate");

  TH3F *hMult = (TH3F*)f.Get(Form("h%sGenRecKaon", kAntiMatterLabel[0]));

  for (int iP = 0; iP < 1; ++iP){
    auto ax = iP == 0 ? hMult->GetYaxis() : hMult->GetZaxis();
    ax->SetTitle(Form("#it{n}_{%s}^{raw}", kPartLetterLabel[iP]));
    o.mkdir(Form("%s_mult", kPartLabel[iP]));
  }
  for (int iCent = 0; iCent < 1; ++iCent){ // kNCentBins
    for (int iPart = 0; iPart < 1; ++iPart){
      for (int iGen = 0; iGen < 200; ++iGen){
        // project along particle multiplicity
        hMult->GetXaxis()->SetRangeUser(kCentBins[iCent], kCentBins[iCent + 1]);
        TH1D *hMultProj = (TH1D*)hMult->ProjectionZ(Form("gen_%d_%d", iCent, iGen), iCent + 1, iCent + 1, iGen + 1, iGen + 1);
        TH1D *hMultProjShift = new TH1D();
        hMultProj->SetName(Form("hMult%s_%d", kPartLabel[iPart], iCent));
        hMultProj->GetYaxis()->SetTitle(Form("#it{p}(#it{n}_{%s}^{raw})", kPartLetterLabel[iPart]));
        hMultProj->GetXaxis()->SetTitle("#it{N}_{rec}");
        hMultProj->Scale(1./hMultProj->Integral());
        shift_binning(hMultProj, hMultProjShift);

        TF1 binom("binom", "TMath::Binomial(TMath::Nint([1]), TMath::Nint(x)) * TMath::Power([0], TMath::Nint(x)) * TMath::Power(1-[0], TMath::Nint([1])-TMath::Nint(x))", 0, 100.);
        binom.SetNpx(10000);
        //binom.SetParLimits(1, 0, 1000.);
        binom.SetParameter(1, iGen);
        binom.SetParLimits(0, 0.01, 0.3);
        for (int i = 0; i < 3; ++i)hMultProjShift->Fit("binom", "NRM+");
        hMultProjShift->Fit("binom", "RM+");
        o.cd(Form("%s_mult", kPartLabel[iPart]));
        // hMultProjShift->Write();

        TCanvas c(Form("cMult%s_%d_%d", kPartLabel[iPart], iCent, iGen), Form("multiplicity %s", kPartLabel[iPart]), 500, 500);
        TLatex t;
        TLegend l(0.65, 0.7, 0.918546, 0.8);
        c.SetLogy();
        c.SetTopMargin(0.02);
        c.SetRightMargin(0.03);
        TF1 binomDraw(binom);
        binomDraw.SetRange(0, 1000);
        set_hist_style(hMultProjShift);
        set_line_style(&binomDraw);
        set_text_style(&t);
        set_text_style(&l);
        hMultProjShift->GetXaxis()->SetRangeUser(0, iPart == 0 ? 40 : 5);
        hMultProjShift->GetYaxis()->SetRangeUser(2.e-9, 4);
        hMultProjShift->Draw("histoe");
        binomDraw.Draw("lsame");
        t.DrawLatexNDC(0.55, 0.9, "Pb#minusPb #sqrt{#it{s}_{NN}} = 5.02 TeV");
        t.DrawLatexNDC(0.55, 0.83, Form("V0M centrality %.0f-%.0f%%", kCentBins[iCent], kCentBins[iCent + 1]));
        l.AddEntry(hMultProjShift, "K^{-}", "pe");
        l.AddEntry(&binomDraw, "Binomial fit", "l");
        l.Draw("same");
        if (iGen == 89)
          c.Print(Form("%s_response_%d.pdf", kPartLabel[iPart], iCent));
        c.Write();
      }
    }
  }

  o.cd();
  hMult->Write();
  f.Close();
  o.Close();
}

void shift_binning(const TH1D* in, TH1D* out, double shift = -0.5){
  int n_bins = in->GetNbinsX();
  double left = in->GetXaxis()->GetBinLowEdge(1);
  double right = in->GetXaxis()->GetBinUpEdge(n_bins);
  out->SetName(Form("%s_shift", in->GetName()));
  out->SetBins(n_bins, left + shift, right + shift);
  out->GetXaxis()->SetTitle(in->GetXaxis()->GetTitle());
  out->GetYaxis()->SetTitle(in->GetYaxis()->GetTitle());
  for (int iB = 0; iB < n_bins; ++iB){
    out->SetBinContent(iB + 1, in->GetBinContent(iB + 1));
    out->SetBinError(iB + 1, in->GetBinError(iB + 1));
  }
}

void set_hist_style(TH1D* h){
  h->SetMarkerStyle(20);
  h->SetMarkerSize(1.2);
  h->SetLineColor(kBlack);
  h->SetMarkerColor(kBlack);
  h->SetDrawOption("histoe");
}

void set_line_style(TF1* f){
  f->SetLineWidth(1);
  f->SetLineColor(kRed);
}

void set_text_style(TText* t){
  t->SetTextFont(kTextFont);
  t->SetTextSize(kTextSize);
}

void set_text_style(TLegend* l){
  l->SetTextFont(kTextFont);
  l->SetTextSize(kTextSize);
}
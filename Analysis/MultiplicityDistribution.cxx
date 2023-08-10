#include "../utils/Config_tests.h"

void shift_binning(const TH1D* in, TH1D* out, double shift = -0.5);
void set_hist_style(TH1D* h);
void set_line_style(TF1* f);
void set_text_style(TText* t);
void set_text_style(TLegend* l);

void MultiplicityDistribution(const char* inFileName = "test_mult_lhc18qr_var_0", const char* outFileName = "Mult18qr_bdt30_test"){
  gStyle->SetOptStat(0);
  
  TFile f(Form("%s.root", inFileName));
  TFile o(Form("%s.root", outFileName), "recreate");

  TH3F *hMult = (TH3F*)f.Get("subsample_test_mult_lhc18qr/hOppNKaonXiMinus");
  for (int iS = 1; iS < N_SAMPLE; ++iS){
    TH3F *hMultTmp = (TH3F*)f.Get(Form("subsample_test_mult_lhc18qr/hOppNKaonXiPlus", iS + 1));
    hMult->Add(hMultTmp);
  }
  hMult->SetName("hMult");

  for (int iP = 0; iP < 2; ++iP){
    auto ax = iP == 0 ? hMult->GetYaxis() : hMult->GetZaxis();
    ax->SetTitle(Form("#it{n}_{%s}^{raw}", kPartLetterLabel[iP]));
    o.mkdir(Form("%s_mult", kPartLabel[iP]));
  }
  for (int iCent = 0; iCent < kNCentBins; ++iCent){
    for (int iPart = 0; iPart < 2; ++iPart){
      // project along particle multiplicity
      hMult->GetXaxis()->SetRangeUser(kCentBins[iCent], kCentBins[iCent + 1]);
      TH1D *hMultProj = (TH1D*)hMult->Project3D(iPart == 0 ? "y" : "z");
      TH1D *hMultProjShift = new TH1D();
      hMultProj->SetName(Form("hMult%s_%d", kPartLabel[iPart], iCent));
      hMultProj->GetYaxis()->SetTitle(Form("#it{p}(#it{n}_{%s}^{raw})", kPartLetterLabel[iPart]));
      hMultProj->Scale(1./hMultProj->GetEntries());
      shift_binning(hMultProj, hMultProjShift);

      TF1 pois("pois", "TMath::Poisson(x, [0])", 0, 1000.);
      pois.SetNpx(10000);
      pois.SetParLimits(0, 0, 1000.);
      hMultProjShift->Fit("pois", "MNL+");
      o.cd(Form("%s_mult", kPartLabel[iPart]));
      // hMultProjShift->Write();

      TCanvas c(Form("cMult%s_%d", kPartLabel[iPart], iCent), Form("multiplicity %s", kPartLabel[iPart]), 500, 600);
      TLatex t;
      TLegend l(0.634085, 0.62087, 0.918546, 0.732174);
      c.SetLogy();
      TF1 poisDraw(pois);
      poisDraw.SetRange(0, 1000);
      set_hist_style(hMultProjShift);
      set_line_style(&poisDraw);
      set_text_style(&t);
      set_text_style(&l);
      hMultProjShift->GetXaxis()->SetRangeUser(0, iPart == 0 ? 50 : 10);
      hMultProjShift->GetYaxis()->SetRangeUser(2.e-12, 4);
      hMultProjShift->Draw("histoe");
      poisDraw.Draw("lsame");
      t.DrawLatexNDC(0.55, 0.8, kCollidingSystemText);
      t.DrawLatexNDC(0.55, 0.75, Form("V0M centrality %.0f-%.0f%%", kCentBins[iCent], kCentBins[iCent + 1]));
      l.AddEntry(hMultProjShift, kParticleAntiparticleText[iPart], "pe");
      l.AddEntry(&poisDraw, "Poisson fit", "l");
      l.Draw("same");
      c.Print(Form("%s_%d.pdf", kPartLabel[iPart], iCent));
      c.Write();
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
#include "../utils/Config.h"

void shift_binning(const TH1D* in, TH1D* out, double shift = -0.5);
void set_hist_style(TH1D* h);
void set_line_style(TF1* f);
void set_text_style(TText* t);
void set_text_style(TLegend* l);

void ResponseStudy(const char* inFileName = "oMC_LHC21l5", const char* outFileName = "Response"){
  gStyle->SetOptStat(0);
  
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
      for (int iGen = 0; iGen < 100; ++iGen){
        // project along particle multiplicity
        hMult->GetXaxis()->SetRangeUser(kCentBins[iCent], kCentBins[iCent + 1]);
        TH1D *hMultProj = (TH1D*)hMult->ProjectionZ(Form("gen_%d_%d", iCent, iGen), iCent + 1, iCent + 1, iGen + 1, iGen + 1);
        TH1D *hMultProjShift = new TH1D();
        hMultProj->SetName(Form("hMult%s_%d", kPartLabel[iPart], iCent));
        hMultProj->GetYaxis()->SetTitle(Form("#it{p}(#it{n}_{%s}^{raw})", kPartLetterLabel[iPart]));
        hMultProj->Scale(1./hMultProj->Integral());
        shift_binning(hMultProj, hMultProjShift);

        TF1 binom("binom", "TMath::Binomial(TMath::Nint([1]), TMath::Nint(x)) * TMath::Power([0], TMath::Nint(x)) * TMath::Power(1-[0], TMath::Nint([1])-TMath::Nint(x))", 0, 1000.);
        binom.SetNpx(10000);
        binom.FixParameter(1, iGen);
        binom.SetParLimits(0, 0, 1);
        for (int i = 0; i < 3; ++i)hMultProjShift->Fit("binom");
        o.cd(Form("%s_mult", kPartLabel[iPart]));
        // hMultProjShift->Write();

        TCanvas c(Form("cMult%s_%d_%d", kPartLabel[iPart], iCent, iGen), Form("multiplicity %s", kPartLabel[iPart]));
        TLatex t;
        TLegend l(0.634085, 0.62087, 0.918546, 0.732174);
        c.SetLogy();
        TF1 binomDraw(binom);
        binomDraw.SetRange(0, 1000);
        set_hist_style(hMultProjShift);
        set_line_style(&binomDraw);
        set_text_style(&t);
        set_text_style(&l);
        hMultProjShift->GetXaxis()->SetRangeUser(0, iPart == 0 ? 80 : 5);
        hMultProjShift->GetYaxis()->SetRangeUser(2.e-9, 4);
        hMultProjShift->Draw("histoe");
        binomDraw.Draw("lsame");
        t.DrawLatexNDC(0.65, 0.8, kCollidingSystemText);
        t.DrawLatexNDC(0.65, 0.75, Form("V0M centrality %.0f-%.0f%%", kCentBins[iCent], kCentBins[iCent + 1]));
        l.AddEntry(hMultProjShift, kParticleAntiparticleText[iPart], "pe");
        l.AddEntry(&binomDraw, "Binomial fit", "l");
        l.Draw("same");
        // c.Print(Form("%s_%d.png", kPartLabel[iPart], iCent));
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
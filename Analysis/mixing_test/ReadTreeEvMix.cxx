#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH3F.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TMath.h>
#include <Riostream.h>
#include "../utils/Config.h"
#include <TRandom3.h>
#include <TROOT.h>
#include <TNtuple.h>
#include <deque>

//#define USING_MC

#ifdef __CINT__
#pragma link C++ class std::vector<MiniKaon>+;
#pragma link C++ class std::vector<MiniKaonMC>+;
#pragma link C++ class std::vector<MiniXi>+;
#pragma link C++ class std::vector<MiniXiMC>+;
#pragma link C++ class std::vector<MicroKaon>+;
#pragma link C++ class std::deque<MiniEvent>+;
#endif

int process_event(Long64_t ev, TTree* t, TBranch *be, TBranch *bk, TBranch *bxi, std::deque<MiniEvent> &q, MiniCollision *c, std::vector<MiniKaon> *k, std::vector<MiniXi> *xi)
{
  Long64_t tentry = t->LoadTree(ev);
  be->GetEntry(tentry);
  bk->GetEntry(tentry);
  bxi->GetEntry(tentry);

  float cent = c->fCent;
  if (cent > kMaxCent) return -1;

  if (kRejectMultXiFromSameDaughters && (c->fTrigger & 16) == 16){
    return -1;
  }

  if ( !isMC && k2018 && ( ( cent > 9.9 && cent < 30 && ( ( c->fTrigger & 1 ) != 1 ) ) || ( cent > 49.9 && ( ( c->fTrigger & 1 ) != 1 ) ) ) ){
    return -1;
  }

  MiniEvent tmpEv(cent);

  UInt_t l_k = 0;
  for (auto k_tmp : *k){
    l_k++;
  }

  for (auto k_tmp : *k){
    bool isK = false;

    #ifdef USING_MC
      if ( std::abs(k_tmp.fPtMC) > kTOFptCut || std::abs(k_tmp.fPtMC) < kPtLowLimitK || k_tmp.fFlag != 1 || std::abs(k_tmp.fEtaMC) > kEtaCut ) continue;
    #endif // USING_MC
    if (
        ( ( ( ((k_tmp.fCutBitMap & kCutDCA) == kCutDCA) || ((k_tmp.fCutBitMap & kCutDCA2) == kCutDCA2) ) && kRequireDCAcut ) || !kRequireDCAcut ) &&
        ( ( ((k_tmp.fCutBitMap & kCutTPCcls) == kCutTPCcls || (k_tmp.fCutBitMap & kCutTPCcls2) == kCutTPCcls2) && kRequireTPCclsCut ) || !kRequireTPCclsCut) &&
        ( ( ((k_tmp.fCutBitMap & kCutChi2) == kCutChi2 || (k_tmp.fCutBitMap & kCutChi22) == kCutChi22) && kRequireChi2Cut ) || !kRequireChi2Cut ) &&
        std::abs(k_tmp.fPt) > kPtLowLimitK && std::abs(k_tmp.fPt) < kTOFptCut &&
        (std::abs(k_tmp.fEta) < kEtaCut)
      )
    {
      if  ((std::abs(k_tmp.fPt) < kTPCptCut && (k_tmp.fNsigmaTPC > kNsigmaTPCcutAsym[0] && k_tmp.fNsigmaTPC < kNsigmaTPCcutAsym[1])) ||
        (std::abs(k_tmp.fPt) > kTPCptCut && (std::abs(k_tmp.fPt) < kTOFptCut) && (std::abs(k_tmp.fNsigmaTPC) < kNsigmaTPCcutPresel) && (k_tmp.fNsigmaTOF > kNsigmaTOFcutAsym[0] && k_tmp.fNsigmaTOF < kNsigmaTOFcutAsym[1])))
      {
        isK = true;
      }
      if (std::abs(k_tmp.fPt) < kITSptCut && (k_tmp.fNsigmaITS < kNsigmaITScutAsym[0] || k_tmp.fNsigmaITS > kNsigmaITScutAsym[1]))
      {
        isK = false;
      }

      //if (!isK) continue;

      #ifdef USING_MC
        if (!k_tmp.fIsReconstructed || k_tmp.fFlag != 1) continue;
      #endif // USING_MC

      //if (k_tmp.fPt < 0) continue;
      tmpEv.fKaons.emplace_back(k_tmp.fPt, isK);
    }
  }

  for (auto xi_tmp : *xi){
    try {
      double pt = xi_tmp.fPt;
    }
    catch (const std::out_of_range& e) {
      continue;
    }
    #ifdef USING_MC
      if ( std::abs(xi_tmp.fPtMC) > kXiUpPtCut || std::abs(xi_tmp.fPtMC) < kXiLowPtCut || std::abs(xi_tmp.fEtaMC) > kEtaCut || xi_tmp.fFlag != 1) continue;
    #endif // USING_MC
    if (
        std::abs(xi_tmp.fEta) < kEtaCut &&
        std::abs(xi_tmp.fPt) > kXiLowPtCut && std::abs(xi_tmp.fPt) < kXiUpPtCut &&
        std::abs(xi_tmp.fMass - kXiMass) < kXiMassCut
      )
    {
      if (kUseBdtInMC){
        if (xi_tmp.fBdtOut < kBdtScoreCut) continue;
      }
      else {
        return -999; // TODO: implement efficiency-driven selection
      }
      #ifdef USING_MC
        if (!xi_tmp.fIsReconstructed || xi_tmp.fFlag != 1 || (xi_tmp.fRecFlag & BIT(0)) != 1 || (xi_tmp.fRecFlag & BIT(1)) != 2) continue;
      #endif // USING_MC

      //if (xi_tmp.fPt > 0) continue;
      tmpEv.fXi.emplace_back(xi_tmp.fPt);
    }
  }
  
  q.emplace_back(tmpEv);
  //std::cout << q.back().fKaons.size() << std::endl;
  //std::cout << q.back().fXi.size() << std::endl;

  return 0;
}

void ReadTreeEvMix(const char* fname = "input_file", const char* ofname = "o")
{

  TFile *fEffK = TFile::Open(Form("%s/%s.root", kDataDir, kEffKFile));
  TFile *fEffXi = TFile::Open(Form("%s/%s.root", kDataDir, kEffXiFile));
  TFile f(Form("%s/%s.root", kDataDir, fname));
  TFile o(Form("%s.root", ofname), "recreate");

  #ifndef USING_MC
    std::vector<MiniKaon> *k = nullptr;
    std::vector<MiniXi> *xi = nullptr;
  #endif // USING_MC
  #ifdef USING_MC
    std::vector<MiniKaonMC> *k = nullptr;
    std::vector<MiniXiMC> *xi = nullptr;
  #endif // USING_MC
  MiniCollision *c = new MiniCollision();
  c->fZ = 0.;
  c->fCent = 0.;
  c->fTrigger = 0x00;
  TTree *t = (TTree*)f.Get("StrangenessTree");
  TBranch *bk = nullptr;
  TBranch *bxi = nullptr;
  if (isMC){
    t->SetBranchAddress("MiniKaonMC", &k, &bk);
    t->SetBranchAddress("MiniXiMC", &xi, &bxi);
  }
  else{
    t->SetBranchAddress("MiniKaon", &k, &bk);
    t->SetBranchAddress("MiniXi", &xi, &bxi);
  }
  TBranch *be = t->GetBranch("MiniCollision");
  be->SetAddress(&c);
  
  TH1D *hCent[N_SAMPLE];
  TH1D *hEffK[2][kNCentBins][kNEtaBins][N_SAMPLE];
  TH1D *hEffXi[2][kNCentBins][kNEtaBins][N_SAMPLE];

  TNtuple evtTuple("evtTuple", "evtTuple", "cent:q1kP:q1kN:q2kP:q2kN:q1xiP:q1xiN:q2xiP:q2xiN");

  for (int iS = 0; iS < N_SAMPLE; ++iS){
    hCent[iS] = new TH1D(Form("hCent"), ";Centrality (%);Entries", kNCentBinsSmall, kCentBinsSmall);
  }
  for (int iC = 0; iC < 2; ++iC){
    for (int iCent = 0; iCent < kNCentBins; ++iCent){
      for (int iEta = 0; iEta < kNEtaBins; ++iEta){
        for (int iS = 0; iS < N_SAMPLE; ++iS){
          if (fEffK){
            hEffK[iC][iCent][iEta][iS] = (TH1D*)fEffK->Get(Form("subsample_%d/h%sEff%s_%d_%d", 1, kAntiMatterLabel[iC], kPartLabel[0], iCent, iEta));
          }
          if (fEffXi){
            hEffXi[iC][iCent][iEta][iS] = kUseBdtInMC || kUseKaonXiEff ? (TH1D*)fEffK->Get(Form("subsample_%d/h%sEff%s_%d_%d", 1, kAntiMatterLabel[iC], kPartLabel[1], iCent, iEta)) : (TH1D*)fEffXi->Get(Form("fPreselEff_vs_pt_%s_%.0f_%.0f", kAntiMatterLabelML[iC], 0., 90.)); //kCentBins[iCent], kCentBins[iCent + 1]));
          }
        }
      }
    }
  }

  Long64_t nEntries = kLimitSample ? kLimitedSample : t->GetEntries();
  TH1D hCentTmp("hCentTmp", "hCentTmp", kNCentBins, kCentBins);
  TH1D hCentSmallTmp("hCentSmallTmp", "hCentSmallTmp", kNCentBinsSmall, kCentBinsSmall);

  double qK_1[kNCentBinsSmall][2][2][N_SAMPLE];
  double qK_11[kNCentBinsSmall][2][2][N_SAMPLE];
  double qK_1Sq[kNCentBinsSmall][2][2][N_SAMPLE];
  double qK_2[kNCentBinsSmall][2][2][N_SAMPLE];

  double qXi_1[kNCentBinsSmall][2][2][N_SAMPLE];
  double qXi_11[kNCentBinsSmall][2][2][N_SAMPLE];
  double qXi_1Sq[kNCentBinsSmall][2][2][N_SAMPLE];
  double qXi_2[kNCentBinsSmall][2][2][N_SAMPLE];

  for (int i = 0; i < 2; ++i){
    for (int j = 0; j < kNCentBinsSmall; ++j){
      for (int is = 0; is < N_SAMPLE; ++is){
        for (int k = 0; k < 2; ++k){ // efficiency corrected and w/o correction
          qK_1[j][i][k][is] = 0;
          qK_11[j][i][k][is] = 0;
          qK_1Sq[j][i][k][is] = 0;
          qK_2[j][i][k][is] = 0;
            
          qXi_1[j][i][k][is] = 0;
          qXi_11[j][i][k][is] = 0;
          qXi_1Sq[j][i][k][is] = 0;
          qXi_2[j][i][k][is] = 0;
        }
      }
    }
  }

  // event deque
  std::deque<MiniEvent> evq;

  Long64_t j = 0;

  // event processing
  for (Long64_t i = 0; i < nEntries && j < nEntries; ++i){
    
    double qK_1_tmp[][2] = {{0, 0}, {0, 0}};
    double qK_1_tmp_update[][2] = {{0, 0}, {0, 0}};
    double qK_1_sq_tmp[][2] = {{0, 0}, {0, 0}};
    double qK_2_tmp[][2] = {{0, 0}, {0, 0}};

    double qXi_1_tmp[][2] = {{0, 0}, {0, 0}};
    double qXi_1_tmp_update[][2] = {{0, 0}, {0, 0}};
    double qXi_1_sq_tmp[][2] = {{0, 0}, {0, 0}};
    double qXi_2_tmp[][2] = {{0, 0}, {0, 0}};

    Long64_t nK[] = {0, 0};
    Long64_t nXi[] = {0, 0};

    const int iS = (int)(gRandom->Rndm() * N_SAMPLE);

    if (!(i%100000)) std::cout << "=========>>>>>> n_ev = " << i << ", q size = " << evq.size() << std::endl;
    
    if (!(i%1000000)) for (long unsigned int i{0}; i < evq.size(); ++i) {evq.pop_back();}

    bool added_ev = false;
    if (evq.empty())
    {
      if (process_event(j, t, be, bk, bxi, evq, c, k, xi) == 0) added_ev = true;

      ++j;
    }

    Long64_t kSize = (Long64_t)(evq.front().fKaons.size());
    Long64_t xiSize = (Long64_t)(evq.front().fXi.size());
    int i2{0};
    int ixi{0};
    for (auto ev : evq)
    {
      if ((ev.fCent / 10) == (evq.front().fCent / 10) && (ev.fKaons.size() > 0)) i2 += 1;
      if ((ev.fCent / 10) == (evq.front().fCent / 10) && (ev.fXi.size() > 0)) ixi += 1;
    }
    //if (i2 > 0) i2 -= 1;
    //if (ixi > 0 ) ixi -= 1;
    //std::cout << "cent = " << evq.front().fCent / 10 << ", size of kaons = " << kSize << ", i2 = " << i2 << std::endl;

    Long64_t j2{0};
    while ((i2 < kSize || ixi < xiSize) && j < nEntries && (evq.size() <= 10000)){
      int res = process_event(j, t, be, bk, bxi, evq, c, k, xi);
      if ( (res == 0) && ((c->fCent / 10) == (evq.front().fCent / 10)) && (k->size() > 0) && (i2 < kSize))
      {
        //std::cout << "c->fCent / 10 = " << c->fCent / 10 << ", evq.front().fCent / 10 = " << evq.front().fCent / 10 << std::endl;
        i2 += 1;
      }
      if ( (res == 0) && ((c->fCent / 10) == (evq.front().fCent / 10)) && (xi->size() > 0) && (ixi < xiSize))
      {
        //std::cout << "c->fCent / 10 = " << c->fCent / 10 << ", evq.front().fCent / 10 = " << evq.front().fCent / 10 << std::endl;
        ixi += 1;
      }
      j += 1;
    }

    if (evq.size() >= 10000) {
      evq.pop_front();
      continue;
    }

    // compute cumulants
    i2 = 0;
    ixi = 0;
    bool swapped_xi_mult = true;
    long unsigned int evMult = evq.front().fKaons.size() + evq.front().fXi.size();
    for (auto ev : evq)
    {
      if (i2 < kSize) {
        if ((ev.fCent / 10) == (evq.front().fCent / 10) && (ev.fKaons.size() > 0)) {
          int index = gRandom->Integer(ev.fKaons.size());
          if (ev.fKaons[index].fIsKaon)
          {
            int im = ev.fKaons[index].fPt > 0 ? 1 : 0;
            int ic = hCentTmp.FindBin(ev.fCent);
            if (evMult < 2){
              im = evq.front().fKaons[i2].fPt > 0 ? 1 : 0;
              ic = hCentTmp.FindBin(evq.front().fCent);
            }
            
            double eff = fEffK ? hEffK[im][ic - 1][0][0]->GetBinContent(hEffK[im][ic - 1][0][0]->FindBin(std::abs(ev.fKaons[index].fPt))) : kDummyEffK;

            qK_1_tmp[im][0] += 1.;
            qK_1_tmp_update[im][0] += 1.;
            qK_2_tmp[im][0] += 1.;
            qK_1_tmp[im][1] += (1./eff);
            qK_1_tmp_update[im][1] += (1./eff);
            double q2 = 1./eff/eff;
            qK_2_tmp[im][1] += q2;
            nK[im] += 1;

            i2 += 1;
          }
        }
      }
        // else
        // {
        //   int im = ev.fKaons[index].fPt > 0 ? 1 : 0;
        //   int ic = hCentTmp.FindBin(ev.fCent);
          
        //   double eff = fEffXi ? hEffXi[im][ic - 1][0][0]->GetBinContent(hEffXi[im][ic - 1][0][0]->FindBin(std::abs(ev.fKaons[index].fPt))) : kDummyEffK;

        //   qXi_1_tmp[im][0] += 1.;
        //   qXi_1_tmp_update[im][0] += 1.;
        //   qXi_2_tmp[im][0] += 1.;
        //   qXi_1_tmp[im][1] += (1./eff);
        //   qXi_1_tmp_update[im][1] += (1./eff);
        //   double q2 = 1./eff/eff;
        //   qXi_2_tmp[im][1] += q2;
        //   nXi[im] += 1;

        //   i2 += 1;
        // }

      if ((ev.fCent / 10) == (evq.front().fCent / 10) && !swapped_xi_mult) {
        xiSize = (Long64_t)(ev.fXi.size());
        swapped_xi_mult = true;
      }
      if (ixi < xiSize) {
        if((ev.fXi.size() > 0) && (ev.fCent / 10) == (evq.front().fCent / 10)) {
          double index = gRandom->Integer(ev.fXi.size());
          {
            int im = ev.fXi[index].fPt > 0 ? 1 : 0;
            int ic = hCentTmp.FindBin(ev.fCent);
            if (evMult < 2){
              im = evq.front().fXi[ixi].fPt > 0 ? 1 : 0;
              ic = hCentTmp.FindBin(evq.front().fCent);
            }
            
            double eff = fEffXi ? hEffXi[im][ic - 1][0][0]->GetBinContent(hEffXi[im][ic - 1][0][0]->FindBin(std::abs(ev.fXi[index].fPt))) : kDummyEffK;

            qXi_1_tmp[im][0] += 1.;
            qXi_1_tmp_update[im][0] += 1.;
            qXi_2_tmp[im][0] += 1.;
            qXi_1_tmp[im][1] += (1./eff);
            qXi_1_tmp_update[im][1] += (1./eff);
            double q2 = 1./eff/eff;
            qXi_2_tmp[im][1] += q2;
            nXi[im] += 1;

            ixi += 1;
          }
        }
      }
    }
    for (int iM = 0; iM < 2; ++iM){
      for (int iCorr = 0; iCorr < 2; ++iCorr){
        double q1_sq = qK_1_tmp[iM][iCorr] * qK_1_tmp[iM][iCorr];
        qK_1_sq_tmp[iM][iCorr] += q1_sq;
        qK_1_tmp_update[iM][iCorr] = 0.;
      }
    }
    for (int iM = 0; iM < 2; ++iM){
      for (int iCorr = 0; iCorr < 2; ++iCorr){
        double q1_sq = qXi_1_tmp[iM][iCorr] * qXi_1_tmp[iM][iCorr];
        qXi_1_sq_tmp[iM][iCorr] += q1_sq;
        qXi_1_tmp_update[iM][iCorr] = 0.;
      }
    }

    // filling outout histograms and tuples
    evtTuple.Fill(evq.front().fCent, qK_1_tmp[1][1], qK_1_tmp[0][1], qK_2_tmp[1][1], qK_2_tmp[0][1], qXi_1_tmp[0][1], qXi_1_tmp[1][1], qXi_2_tmp[0][1], qXi_2_tmp[1][1]);

    if (!evq.empty()) evq.pop_front();
  }

  evtTuple.Write();

  // close stream
  o.Close();
  f.Close();
}

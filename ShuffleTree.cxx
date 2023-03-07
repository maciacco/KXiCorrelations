#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TMath.h>
#include <Riostream.h>
#include "utils/Config.h"
#include "TRandom3.h"
#include "TROOT.h"

#define CLOSURE_TEST

#ifdef __CINT__
#pragma link C++ class std::vector<MiniKaon>+;
#pragma link C++ class std::vector<MiniKaonMC>+;
#pragma link C++ class std::vector<MiniXi>+;
#pragma link C++ class std::vector<MiniXiMC>+;
#endif

void ShuffleTree(const char* inFileName = "AnalysisResults_LHC21l5_child1", const char* outFileName = "AnalysisResults_LHC21l5_child1_shuffle"){
  TFile *in = TFile::Open(Form("%s/mc_tree/%s.root", kDataDir, inFileName));
  TFile *out = TFile::Open(Form("%s/mc_tree/%s.root", kDataDir, outFileName), "recreate");

  std::vector<MiniKaonMC> *k = nullptr;
  std::vector<MiniKaonMC> *k_w = nullptr;
  std::vector<MiniXiMC> *xi = nullptr;
  std::vector<MiniXiMC> *xi_w = nullptr;
  MiniCollision *c = new MiniCollision();
  MiniCollision *c_w = new MiniCollision();
  c->fZ = 0.;
  c->fCent = 0.;
  c->fTrigger = 0x00;
  TTree *t = (TTree*)in->Get("StrangenessTree");
  TTree *t_w = new TTree("StrangenessTree", "StrangenessTree");
  TBranch *bk = nullptr;
  TBranch *bxi = nullptr;
  t->SetBranchAddress("MiniKaonMC", &k, &bk);
  t->SetBranchAddress("MiniXiMC", &xi, &bxi);
  t_w->Branch("MiniKaonMC", &k_w);
  t_w->Branch("MiniXiMC", &xi_w);
  TBranch *be = t->GetBranch("MiniCollision");
  be->SetAddress(&c);
  t_w->Branch("MiniCollision", &c_w);

  Long64_t nEntries = t->GetEntries();

  for (Long64_t i = 0; i < nEntries; ++i){
    Long64_t e = (Long64_t)(gRandom->Rndm() * nEntries);
    
    Long64_t tentry = t->LoadTree(e);
    be->GetEntry(tentry);
    bk->GetEntry(tentry);
    bxi->GetEntry(tentry);

    c_w->fZ = c->fZ;
    c_w->fCent = c->fCent;
    c_w->fTrigger = c->fTrigger;
    k_w->clear();
    xi_w->clear();
    for (UInt_t iK = 0; iK < k->size(); ++iK){
      MiniKaonMC k_tmp;
      k_tmp.fEtaMC = k->at(iK).fEtaMC;
      k_tmp.fPtMC = k->at(iK).fPtMC;
      k_tmp.fIsReconstructed = k->at(iK).fIsReconstructed;
      k_tmp.fFlag = k->at(iK).fFlag;
      k_tmp.fPt = k->at(iK).fPt;
      k_tmp.fEta = k->at(iK).fEta;
      k_tmp.fNsigmaTPC = k->at(iK).fNsigmaTPC;
      k_tmp.fNsigmaTOF = k->at(iK).fNsigmaTOF;
      k_w->push_back(k_tmp);
    }
    for (UInt_t iXi = 0; iXi < xi->size(); ++iXi){
      MiniXiMC xi_tmp;
      xi_tmp.fEtaMC = xi->at(iXi).fEtaMC;
      xi_tmp.fPtMC = xi->at(iXi).fPtMC;
      xi_tmp.fIsReconstructed = xi->at(iXi).fIsReconstructed;
      xi_tmp.fFlag = xi->at(iXi).fFlag;
      xi_tmp.fPt = xi->at(iXi).fPt;
      xi_tmp.fEta = xi->at(iXi).fEta;
      xi_tmp.fMass = xi->at(iXi).fMass;
      xi_tmp.fBdtOut = xi->at(iXi).fBdtOut;
      xi_w->push_back(xi_tmp);
    }
    t_w->Fill();
  }
  t_w->Write();
}
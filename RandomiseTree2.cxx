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

void RandomiseTree2(const char* inFileName = "AnalysisResults_LHC17pq_afterCalib.root"){
  TFile *in = TFile::Open(Form("%s/pp/tree/%s.root", kDataDir, inFileName));
  TFile *out[30];
  for (int i{0}; i < 30; ++i){
    out[i] = TFile::Open(Form("%s/pp/tree/%s.root", kDataDir, inFileName), "recreate"); 
  }

  float rnd = 0.;
  TTree *t = (TTree*)in->Get("StrangenessTree");
  TBranch *br = t->Branch("index", &rnd);

  Long64_t nEntries = t->GetEntries();

  for (Long64_t i = 0; i < nEntries; ++i){
    Long64_t e = (Long64_t)(gRandom->Rndm() * nEntries);
    
    rnd = gRandom->Rndm();
    br->Fill();
  }
  t->Write();
}
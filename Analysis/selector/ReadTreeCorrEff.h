//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Feb 24 11:53:10 2023 by ROOT version 6.26/02
// from TTree StrangenessTree/Tree
// found on file: /data/mciacco/KXiCorrelations/tree_data_full/AnalysisResults_child_1.root
//////////////////////////////////////////////////////////

#ifndef ReadTreeCorrEff_h
#define ReadTreeCorrEff_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector
#include <vector>
#include <TH1D.h>
#include <TH3I.h>
#include <TH3F.h>
#include "../../utils/Config.h"


class ReadTreeCorrEff : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<MiniCollision> fMiniCollision = {fReader, "MiniCollision"};
   TTreeReaderArray<MiniXi> fMiniXi = {fReader, "MiniXi"};
   TTreeReaderArray<MiniKaon> fMiniKaon = {fReader, "MiniKaon"};

   TFile *fileEffK;
   TFile *fileEffXi;

   // histograms
   TH1D *fCent[N_SAMPLE];
   TH1D *fKaonQ1[2][2][N_SAMPLE];
   TH1D *fKaonQ1Sq[2][2][N_SAMPLE];
   TH1D *fKaonQ2[2][2][N_SAMPLE];
   TH1D *fXiQ1[2][2][N_SAMPLE];
   TH1D *fXiQ1Sq[2][2][N_SAMPLE];
   TH1D *fXiQ2[2][2][N_SAMPLE];
   TH1D *fSameKaonXiQ11[2][2][N_SAMPLE];
   TH1D *fOppKaonXiQ11[2][2][N_SAMPLE];
   TH3I *fNKaonXi[2][N_SAMPLE];

   TH1D *fEffK[2][kNCentBins];
   TH1D *fEffXi[2][kNCentBins];
   TH3F *fBDTEffXi; // TO BE IMPROVED -> CENTRALITY DIFFERENTIAL ESTIMATE OF XI EFFICIENCY (ALSO BDT -> SEPARATELY FOR CHARGES)


   ReadTreeCorrEff(TTree * /*tree*/ =0) { }
   virtual ~ReadTreeCorrEff() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(ReadTreeCorrEff,0);

};

#endif
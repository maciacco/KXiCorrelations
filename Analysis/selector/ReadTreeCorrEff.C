#define ReadTreeCorrEff_cxx
// The class definition in ReadTreeCorrEff.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following metfods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("ReadTreeCorrEff.C")
// root> T->Process("ReadTreeCorrEff.C","some options")
// root> T->Process("ReadTreeCorrEff.C+")
//


#include "ReadTreeCorrEff.h"
#include <TH2.h>
#include <TStyle.h>

void ReadTreeCorrEff::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

void ReadTreeCorrEff::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   // create histograms

   fileEffK = TFile::Open(Form("%s/%s.root", kDataDir, kEffKFile));
   fileEffXi = TFile::Open(Form("%s/%s.root", kDataDir, kEffXiFile));

   for (int iS = 0; iS < N_SAMPLE; ++iS){
      fCent[iS] = new TH1D(Form("fCent_%d", iS), ";Centrality (%%);Entries", kNCentBinsSmall, kCentBinsSmall);
   }
   for (int iC = 0; iC < 2; ++iC){
      for (int iS = 0; iS < N_SAMPLE; ++iS){
         for (int iCorr = 0; iCorr < 2; ++iCorr){
            fKaonQ1[iC][iCorr][iS] = new TH1D(Form("f%sKaonQ1_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iS), ";Centrality (%%);q_{1}^{K}", kNCentBinsSmall, kCentBinsSmall);
            fKaonQ1Sq[iC][iCorr][iS] = new TH1D(Form("f%sKaonQ1Sq_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iS), ";Centrality (%%);(q_{1}^{K})^{2}", kNCentBinsSmall, kCentBinsSmall);
            fKaonQ2[iC][iCorr][iS] = new TH1D(Form("f%sKaonQ2_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iS), ";Centrality (%%);q_{2}^{K}", kNCentBinsSmall, kCentBinsSmall);
            fXiQ1[iC][iCorr][iS] = new TH1D(Form("f%sXiQ1_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iS), ";Centrality (%%);q_{1}^{#Xi}", kNCentBinsSmall, kCentBinsSmall);
            fXiQ1Sq[iC][iCorr][iS] = new TH1D(Form("f%sXiQ1Sq_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iS), ";Centrality (%%);(q_{1}^{#Xi})^{2}", kNCentBinsSmall, kCentBinsSmall);
            fXiQ2[iC][iCorr][iS] = new TH1D(Form("f%sXiQ2_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iS), ";Centrality (%%);q_{2}^{#Xi}", kNCentBinsSmall, kCentBinsSmall);
            fSameKaonXiQ11[iC][iCorr][iS] = new TH1D(Form("f%sSameKaonXiQ11_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iS), ";Centrality (%%);q_{11}^{K^{-}#Xi^{-}}", kNCentBinsSmall, kCentBinsSmall);
            fOppKaonXiQ11[iC][iCorr][iS] = new TH1D(Form("f%sOppKaonXiQ11_%s_%d", kAntiMatterLabel[iC], kCorrLabel[iCorr], iS), ";Centrality (%%);q_{11}^{K^{+}#Xi^{-}}", kNCentBinsSmall, kCentBinsSmall);
         }
      }
      for (int iCent = 0; iCent < kNCentBins; ++iCent){
         if (fileEffK){
            fEffK[iC][iCent] = (TH1D*)fileEffK->Get(Form("h%sEff_%d;1", kAntiMatterLabel[iC], iCent));
         }
         if (fileEffXi){
            fEffXi[iC][iCent] = (TH1D*)fileEffXi->Get(Form("h%sEff_%d;2", kAntiMatterLabel[iC], iCent));
         }
      }
   }
   //fBDTEffXi = (TH3F*)fEffBDTXi->Get("hBDTEff");

   for (int iC = 0; iC < 2; ++iC){
      for (int iS = 0; iS < N_SAMPLE; ++iS){
         fNKaonXi[iC][iS] = new TH3I(Form("f%sNKaonXi_%d", kChargeLabel[iC], iS), ";Centrality (%%);K;#Xi", 100, 0, 100, 200, 0, 200, 20, 0, 20);
      }
   }

}

Bool_t ReadTreeCorrEff::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // When processing keyed objects with PROOF, the object is already loaded
   // and is available via the fObject pointer.
   //
   // This function sfould contain the \"body\" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   fReader.SetLocalEntry(entry);

   

   return kTRUE;
}

void ReadTreeCorrEff::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void ReadTreeCorrEff::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
   TFile o("o.root", "recreate");
   
   for (int iS = 0; iS < N_SAMPLE; ++iS){
      o.mkdir(Form("subsample_%d", iS));
      o.cd(Form("subsample_%d", iS));
      for (int iM = 0; iM < 2; ++iM){
         for (int iC = 0; iC < 2; ++iC){
            fKaonQ1[iM][iC][iS]->Write();
            fKaonQ1Sq[iM][iC][iS]->Write();
            fKaonQ2[iM][iC][iS]->Write();
            fXiQ1[iM][iC][iS]->Write();
            fXiQ1Sq[iM][iC][iS]->Write();
            fXiQ2[iM][iC][iS]->Write();
            fSameKaonXiQ11[iM][iC][iS]->Write();
            fOppKaonXiQ11[iM][iC][iS]->Write();
         }
         fNKaonXi[iM][iS]->Write();
      }
      fCent[iS]->Write();
   }

   o.Close();

}

void ReadTreeCorrEff::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t ReadTreeCorrEff::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#include "../utils/Config.h"

#include <TStopwatch.h>
#include <TFile.h>
#include <TProfile.h>
#include <TNtuple.h>
#include <Riostream.h>

void checkFiles(int smpl = 0, int iVarMin = 0, int iVarMax = 135)
{
  // loop over cut variation -> combine K and Xi
  // int iCutK = 40;
  // int iCutXi = 4;
  
  TStopwatch w;
  w.Start();

  int sample = smpl - 1;

  int skippedVar = 0;
  //for (int sample = 0; sample < 1; ++sample){
  for (int iVar = iVarMin; iVar < iVarMax; ++iVar){ 
    std::cout << iVar << std::endl;
    //TFile fin(Form("/data/mciacco/KXiCorrelations/output_sys_dir/18qr/o_%d.root"/* , kResDir */, sample + 1)); 
    TFile *fin = TFile::Open(Form("/data/mciacco/KXiCorrelations/output_sys_dir/17pq/o_%d_var_%d.root"/* , kResDir */, sample + 1, iVar)); 
    
    //TFile fout(Form("%s/output_sys_dir/output_sys_17pq_%d_%d.root", kResDir, sample, iVar), "recreate");
    //std::cout << iVar % ( kNTpcClsCuts * kNDcaCuts * kNPidCuts * kNChi2Cuts ) << "\t" << iVar / ( kNTpcClsCuts * kNDcaCuts * kNPidCuts * kNChi2Cuts ) << std::endl;
    // continue;
    TNtuple *tuple_qmoment = (TNtuple*)fin->Get(Form("evtTuple_%d", iVar ));
    if (!tuple_qmoment)
    {
      std::cout << "no input, skip" << std::endl;
      skippedVar += 1;

      delete fin;
      continue;
    }
    tuple_qmoment->SetCacheSize(0);
    
    int evt[10] = {0};
    int centrality;
          
    float *arg;
    
    float total_event = tuple_qmoment->GetEntriesFast();

    //std::cout << total_event << "\t" << total_event2 << std::endl;

    int readError = 0;
    for (int j = 0; j < total_event; j++)
    {
      if (tuple_qmoment->GetEntry(j) < 0) {readError = -999; break;}
    }
    if (readError < 0) {
      std::cout << "no input, skip" << std::endl;
      skippedVar += 1;

      delete fin;
      continue;
    }
  
    fin->Close();
    delete fin;
  }

  std::cout << "skipped = " << skippedVar << std::endl;
  //}

  w.Stop();
  w.Print();

}

#include "ReadTreeCorrEff.h"

void Launch(){
  TFile f("/data/mciacco/KXiCorrelations/tree/data_test_18q_pass3/AnalysisResults.root");
  TTree* t = (TTree*)f.Get("StrangenessTree");
  ReadTreeCorrEff s(t);
  s.SlaveBegin(t);
  s.Terminate();
}
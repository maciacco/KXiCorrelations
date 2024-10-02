#include "../utils/Config.h"

#include <TStopwatch.h>
#include <TFile.h>
#include <TProfile.h>
#include <TNtuple.h>
#include <Riostream.h>

void sameevent2(int smpl = 0, int iVarMin = 0, int iVarMax = 1)
{
  // loop over cut variation -> combine K and Xi
  // int iCutK = 40;
  // int iCutXi = 4;

  TStopwatch w;
  w.Start();

  int sample = smpl - 1;

  int skippedVar = 0;

  for (int iVar = iVarMin; iVar < iVarMax; ++iVar){

    //TFile fin(Form("/data/mciacco/KXiCorrelations/output_sys_dir/18qr/o_%d.root"/* , kResDir */, sample + 1));
    TFile *fin = TFile::Open(Form("/data/mciacco/KXiCorrelations/output_sys_dir/16qt_3/o_%d_var_%d.root"/* , kResDir */, sample + 1, iVar / ( kNTpcClsCuts * kNDcaCuts * kNPidCuts * kNChi2Cuts )));
    TFile *fin2 = TFile::Open(Form("/data/mciacco/KXiCorrelations/output_sys_dir/16qt_3/o_%d_var_%d.root"/* , kResDir */, sample + 1, iVar % ( kNTpcClsCuts * kNDcaCuts * kNPidCuts * kNChi2Cuts )));
    if (!fin || !fin2 || fin->TestBit(TFile::kZombie) || fin2->TestBit(TFile::kZombie)){
      std::cout << "no input, skip" << std::endl;
      skippedVar += 1;

      delete fin;
      delete fin2;
      continue;
    }
    TFile fout(Form("%s/output_sys_dir/16qt_3/output_sys_16qt_3_%d_%d.root", kResDir, sample, iVar), "recreate");
    std::cout << iVar % ( kNTpcClsCuts * kNDcaCuts * kNPidCuts * kNChi2Cuts ) << "\t" << iVar / ( kNTpcClsCuts * kNDcaCuts * kNPidCuts * kNChi2Cuts ) << std::endl;
    // continue;
    TNtuple *tuple_qmoment = (TNtuple*)fin->Get(Form("evtTuple_%d", iVar / ( kNTpcClsCuts * kNDcaCuts * kNPidCuts * kNChi2Cuts ), sample ));
    TNtuple *tuple_qmoment2 = (TNtuple*)fin2->Get(Form("evtTuple_%d", iVar % ( kNTpcClsCuts * kNDcaCuts * kNPidCuts * kNChi2Cuts ), sample ));

    if (!tuple_qmoment || !tuple_qmoment2)
    {
      std::cout << "no input, skip" << std::endl;
      skippedVar += 1;

      delete fin;
      delete fin2;
      continue;
    }
    tuple_qmoment->SetCacheSize(0);
    tuple_qmoment2->SetCacheSize(0);
    int evt[10] = {0};
    int centrality;

    float *arg;
    float *arg2;

    float total_event = tuple_qmoment->GetEntriesFast();
    float total_event2 = tuple_qmoment2->GetEntriesFast();
    if (total_event != total_event2){
      std::cout << "no input, skip" << std::endl;
      skippedVar += 1;
      continue;
    }
    //std::cout << total_event << "\t" << total_event2 << std::endl;

    TProfile *q1_xi_p = new TProfile("q1_xi_p", "q1_xi_p", kNCentBins, kCentBins);
    TProfile *q2_xi_p = new TProfile("q2_xi_p", "q2_xi_p", kNCentBins, kCentBins);
    TProfile *q1_xi_n = new TProfile("q1_xi_n", "q1_xi_n", kNCentBins, kCentBins);
    TProfile *q2_xi_n = new TProfile("q2_xi_n", "q2_xi_n", kNCentBins, kCentBins);
    TProfile *q1square_xi_p = new TProfile("q1square_xi_p", "q1square_xi_p", kNCentBins, kCentBins);
    TProfile *q1square_xi_n = new TProfile("q1square_xi_n", "q1square_xi_n", kNCentBins, kCentBins);

    TProfile *q1_kaon_p = new TProfile("q1_kaon_p", "q1_kaon_p", kNCentBins, kCentBins);
    TProfile *q2_kaon_p = new TProfile("q2_kaon_p", "q2_kaon_p", kNCentBins, kCentBins);
    TProfile *q1_kaon_n = new TProfile("q1_kaon_n", "q1_kaon_n", kNCentBins, kCentBins);
    TProfile *q2_kaon_n = new TProfile("q2_kaon_n", "q2_kaon_n", kNCentBins, kCentBins);
    TProfile *q1square_kaon_p = new TProfile("q1square_kaon_p", "q1square_kaon_p", kNCentBins, kCentBins);
    TProfile *q1square_kaon_n = new TProfile("q1square_kaon_n", "q1square_kaon_n", kNCentBins, kCentBins);

    TProfile *q1_xi_xi_pn = new TProfile("q1_xi_xi_pn", "q1_xi_xi_pn", kNCentBins, kCentBins);
    TProfile *q1_kaon_kaon_pn = new TProfile("q1_kaon_kaon_pn", "q1_kaon_kaon_pn", kNCentBins, kCentBins);

    TProfile *q1_xi_kaon_pp = new TProfile("q1_xi_kaon_pp", "q1_xi_kaon_pp", kNCentBins, kCentBins);
    TProfile *q1_xi_kaon_pn = new TProfile("q1_xi_kaon_pn", "q1_xi_kaon_pn", kNCentBins, kCentBins);
    TProfile *q1_xi_kaon_np = new TProfile("q1_xi_kaon_np", "q1_xi_kaon_np", kNCentBins, kCentBins);
    TProfile *q1_xi_kaon_nn = new TProfile("q1_xi_kaon_nn", "q1_xi_kaon_nn", kNCentBins, kCentBins);

    int readError = 0;
    for (int j = 0; j < total_event; j++)
    {
      if (!(j % 100000))std::cout << j << std::endl;
      if (tuple_qmoment->GetEntry(j) < 0 || tuple_qmoment2->GetEntry(j) < 0) {readError = -999; break;}
      arg = tuple_qmoment->GetArgs();
      arg2 = tuple_qmoment2->GetArgs();
      centrality = arg[0];
      double q1_p = arg[5];
      double q2_p = arg[7];
      double q1_n = arg[6];
      double q2_n = arg[8];

      double qP1_p = arg2[1];
      double qP2_p = arg2[3];
      double qP1_n = arg2[2];
      double qP2_n = arg2[4];

      q1_xi_p->Fill(centrality, q1_p);
      q2_xi_p->Fill(centrality, q2_p);
      q1_xi_n->Fill(centrality, q1_n);
      q2_xi_n->Fill(centrality, q2_n);
      q1square_xi_p->Fill(centrality, q1_p * q1_p);
      q1square_xi_n->Fill(centrality, q1_n * q1_n);

      q1_kaon_p->Fill(centrality, qP1_p);
      q2_kaon_p->Fill(centrality, qP2_p);
      q1_kaon_n->Fill(centrality, qP1_n);
      q2_kaon_n->Fill(centrality, qP2_n);
      q1square_kaon_p->Fill(centrality, qP1_p * qP1_p);
      q1square_kaon_n->Fill(centrality, qP1_n * qP1_n);

      q1_xi_xi_pn->Fill(centrality, q1_p * q1_n);
      q1_kaon_kaon_pn->Fill(centrality, qP1_p * qP1_n);

      q1_xi_kaon_pp->Fill(centrality, q1_p * qP1_p);
      q1_xi_kaon_pn->Fill(centrality, q1_p * qP1_n);
      q1_xi_kaon_np->Fill(centrality, q1_n * qP1_p);
      q1_xi_kaon_nn->Fill(centrality, q1_n * qP1_n);
    }
    if (readError < 0) {
      std::cout << "no input, skip" << std::endl;
      skippedVar += 1;

      delete fin;
      delete fin2;
      continue;
    }

    fout.mkdir(Form("var_%d", iVar));
    fout.cd(Form("var_%d", iVar));

    q1_xi_p->Write();
    q2_xi_p->Write();
    q1_xi_n->Write();
    q2_xi_n->Write();
    q1square_xi_p->Write();
    q1square_xi_n->Write();
    q1_kaon_p->Write();
    q2_kaon_p->Write();
    q1_kaon_n->Write();
    q2_kaon_n->Write();
    q1square_kaon_p->Write();
    q1square_kaon_n->Write();
    q1_xi_xi_pn->Write();
    q1_kaon_kaon_pn->Write();
    q1_xi_kaon_pp->Write();
    q1_xi_kaon_pn->Write();
    q1_xi_kaon_np->Write();
    q1_xi_kaon_nn->Write();

    fout.Close();
    fin->Close();
    fin2->Close();
    delete fin;
    delete fin2;
  }

  std::cout << "skipped = " << skippedVar << std::endl;
  w.Stop();
  w.Print();

}

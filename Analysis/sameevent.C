#include "../utils/Config.h"

#include <TStopwatch.h>
#include <TFile.h>
#include <TProfile.h>
#include <TNtuple.h>
#include <Riostream.h>

void sameevent(int smpl = 0, int iVarMin = 0, int iVarMax = 1)
{
  // loop over cut variation -> combine K and Xi
  // int iCutK = 40;
  // int iCutXi = 4;
  
  TStopwatch w;
  w.Start();

  int sample = smpl - 1;
  TFile *fin = new TFile(Form("%s/o_15_%d.root", kResDir, sample+1)); 

  for (int iVar = iVarMin; iVar < iVarMax; ++iVar){ 

    TFile *fout = new TFile(Form("%s/output_sys_15o_%d_%d.root", kResDir, sample, iVar), "recreate");
    std::cout << iVar % ( kNTpcClsCuts * kNDcaCuts * kNPidCuts * kNChi2Cuts ) << "\t" << iVar % ( kNMassCuts * kNBdtCuts ) << std::endl;
    // continue;
    TNtuple *tuple_qmoment = (TNtuple*)fin->Get(Form("evtTuple_%d", iVar % ( kNMassCuts * kNBdtCuts )));
    TNtuple *tuple_qmoment2 = (TNtuple*)fin->Get(Form("evtTuple_%d", iVar % ( kNTpcClsCuts * kNDcaCuts * kNPidCuts * kNChi2Cuts )));
    int evt[10] = {0};
    int centrality;
    int *centclass = new int[1200000000];
    float **q1_p = new float*[10];
    float **q2_p = new float*[10];
    float **q1_n = new float*[10];
    float **q2_n = new float*[10];
    float **qP1_p = new float*[10];
    float **qP2_p = new float*[10];
    float **qP1_n = new float*[10];
    float **qP2_n = new float*[10];
    for(int i = 0; i < 10; i++)
    {
      q1_p[i] = new float[120000000];
      q2_p[i] = new float[120000000];
      qP1_p[i] = new float[120000000];
      qP2_p[i] = new float[120000000];
      q1_n[i] = new float[120000000];
      q2_n[i] = new float[120000000];
      qP1_n[i] = new float[120000000];
      qP2_n[i] = new float[120000000];
    }
          
    float *arg;
    float *arg2;
    
    float total_event = tuple_qmoment->GetEntries();
    float total_event2 = tuple_qmoment2->GetEntries();
    //std::cout << total_event << "\t" << total_event2 << std::endl;
    for (int j = 0; j < total_event; j++)
    {
      tuple_qmoment->GetEntry(j);
      tuple_qmoment2->GetEntry(j);
      arg = tuple_qmoment->GetArgs();
      arg2 = tuple_qmoment2->GetArgs();
      centrality = arg[0];
      centclass[j] = centrality;
      q1_p[(int)(centrality/10)][evt[(int)(centrality/10)]] = arg[5];
      q2_p[(int)(centrality/10)][evt[(int)(centrality/10)]] = arg[7];
      q1_n[(int)(centrality/10)][evt[(int)(centrality/10)]] = arg[6];
      q2_n[(int)(centrality/10)][evt[(int)(centrality/10)]] = arg[8];
      
      qP1_p[(int)(centrality/10)][evt[(int)(centrality/10)]] = arg2[1];
      qP2_p[(int)(centrality/10)][evt[(int)(centrality/10)]] = arg2[3];
      qP1_n[(int)(centrality/10)][evt[(int)(centrality/10)]] = arg2[2];
      qP2_n[(int)(centrality/10)][evt[(int)(centrality/10)]] = arg2[4];
      evt[(int)(centrality/10)]=evt[(int)(centrality/10)]+1;
      //if(centrality>90) std::cout << "chk" << "\n";
    }
    
    TProfile *q1_xi_p = new TProfile("q1_xi_p", "q1_xi_p", 10, 0, 100);
    TProfile *q2_xi_p = new TProfile("q2_xi_p", "q2_xi_p", 10, 0, 100);
    TProfile *q1_xi_n = new TProfile("q1_xi_n", "q1_xi_n", 10, 0, 100);
    TProfile *q2_xi_n = new TProfile("q2_xi_n", "q2_xi_n", 10, 0, 100);
    TProfile *q1square_xi_p = new TProfile("q1square_xi_p", "q1square_xi_p", 10, 0, 100);
    TProfile *q1square_xi_n = new TProfile("q1square_xi_n", "q1square_xi_n", 10, 0, 100);
      
    TProfile *q1_kaon_p = new TProfile("q1_kaon_p", "q1_kaon_p", 10, 0, 100);
    TProfile *q2_kaon_p = new TProfile("q2_kaon_p", "q2_kaon_p", 10, 0, 100);
    TProfile *q1_kaon_n = new TProfile("q1_kaon_n", "q1_kaon_n", 10, 0, 100);
    TProfile *q2_kaon_n = new TProfile("q2_kaon_n", "q2_kaon_n", 10, 0, 100);
    TProfile *q1square_kaon_p = new TProfile("q1square_kaon_p", "q1square_kaon_p", 10, 0, 100);
    TProfile *q1square_kaon_n = new TProfile("q1square_kaon_n", "q1square_kaon_n", 10, 0, 100);

    TProfile *q1_xi_xi_pn = new TProfile("q1_xi_xi_pn", "q1_xi_xi_pn", 10, 0, 100);
    TProfile *q1_kaon_kaon_pn = new TProfile("q1_kaon_kaon_pn", "q1_kaon_kaon_pn", 10, 0, 100);

    TProfile *q1_xi_kaon_pp = new TProfile("q1_xi_kaon_pp", "q1_xi_kaon_pp", 10, 0, 100);
    TProfile *q1_xi_kaon_pn = new TProfile("q1_xi_kaon_pn", "q1_xi_kaon_pn", 10, 0, 100);
    TProfile *q1_xi_kaon_np = new TProfile("q1_xi_kaon_np", "q1_xi_kaon_np", 10, 0, 100);
    TProfile *q1_xi_kaon_nn = new TProfile("q1_xi_kaon_nn", "q1_xi_kaon_nn", 10, 0, 100);
  
    int range;  
    for(int CNTCLS = 0; CNTCLS < 9; CNTCLS++)
    {
      range = evt[CNTCLS] - 1;
      //std::cout << CNTCLS << "\t" << range << "\n";
      for(int evtcentclass=0; evtcentclass < range; evtcentclass++)
      {
        q1_xi_p->Fill(CNTCLS * 10, q1_p[CNTCLS][evtcentclass]);
        q2_xi_p->Fill(CNTCLS * 10, q2_p[CNTCLS][evtcentclass]);
        q1_xi_n->Fill(CNTCLS * 10, q1_n[CNTCLS][evtcentclass]);
        q2_xi_n->Fill(CNTCLS * 10, q2_n[CNTCLS][evtcentclass]);
        q1square_xi_p->Fill(CNTCLS * 10, q1_p[CNTCLS][evtcentclass] * q1_p[CNTCLS][evtcentclass]);
        q1square_xi_n->Fill(CNTCLS * 10, q1_n[CNTCLS][evtcentclass] * q1_n[CNTCLS][evtcentclass]);

        q1_kaon_p->Fill(CNTCLS * 10, qP1_p[CNTCLS][evtcentclass]);
        q2_kaon_p->Fill(CNTCLS * 10, qP2_p[CNTCLS][evtcentclass]);
        q1_kaon_n->Fill(CNTCLS * 10, qP1_n[CNTCLS][evtcentclass]);
        q2_kaon_n->Fill(CNTCLS * 10, qP2_n[CNTCLS][evtcentclass]);
        q1square_kaon_p->Fill(CNTCLS * 10, qP1_p[CNTCLS][evtcentclass] * qP1_p[CNTCLS][evtcentclass]);
        q1square_kaon_n->Fill(CNTCLS * 10, qP1_n[CNTCLS][evtcentclass] * qP1_n[CNTCLS][evtcentclass]);
        
        q1_xi_xi_pn->Fill(CNTCLS * 10, q1_p[CNTCLS][evtcentclass] * q1_n[CNTCLS][evtcentclass]);
        q1_kaon_kaon_pn->Fill(CNTCLS * 10, qP1_p[CNTCLS][evtcentclass] * qP1_n[CNTCLS][evtcentclass]);

        q1_xi_kaon_pp->Fill(CNTCLS * 10, q1_p[CNTCLS][evtcentclass] * qP1_p[CNTCLS][evtcentclass]);
        q1_xi_kaon_pn->Fill(CNTCLS * 10, q1_p[CNTCLS][evtcentclass] * qP1_n[CNTCLS][evtcentclass]);
        q1_xi_kaon_np->Fill(CNTCLS * 10, q1_n[CNTCLS][evtcentclass] * qP1_p[CNTCLS][evtcentclass]);
        q1_xi_kaon_nn->Fill(CNTCLS * 10, q1_n[CNTCLS][evtcentclass] * qP1_n[CNTCLS][evtcentclass]);
      }
    }
  
    fout->mkdir(Form("var_%d", iVar));
    fout->cd(Form("var_%d", iVar));

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


    delete q1_xi_p;
    delete q2_xi_p;
    delete q1_xi_n;
    delete q2_xi_n;
    delete q1square_xi_p;
    delete q1square_xi_n;
    delete q1_kaon_p;
    delete q2_kaon_p;
    delete q1_kaon_n;
    delete q2_kaon_n;
    delete q1square_kaon_p;
    delete q1square_kaon_n;
    delete q1_xi_xi_pn;
    delete q1_kaon_kaon_pn;
    delete q1_xi_kaon_pp;
    delete q1_xi_kaon_pn;
    delete q1_xi_kaon_np;
    delete q1_xi_kaon_nn;
    
    for(int i = 0; i < 10; i++)
    {
      delete [] q1_p[i];
      delete [] q2_p[i];
      delete [] qP1_p[i];
      delete [] qP2_p[i];
      delete [] q1_n[i];
      delete [] q2_n[i];
      delete [] qP1_n[i];
      delete [] qP2_n[i];
    }

    delete[] q1_p;
    delete[] q2_p;
    delete[] qP1_p;
    delete[] qP2_p;
    delete[] q1_n;
    delete[] q2_n;
    delete[] qP1_n;
    delete[] qP2_n;

    delete []centclass;
    fout->Close();

    delete fout;
  }

  w.Stop();
  w.Print();

}

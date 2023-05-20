void sameevent_kk()
{
   
  for(int sample=0;sample<32;sample++)
    {if (sample==28) continue;
      TFile *fin=new TFile(Form("o_mix2%d.root",sample+1)); 
      TNtuple *tuple_qmoment=(TNtuple*)fin->Get("evtTuple");
      int evt[10]={0};
      int centrality;
      int *centclass=new int[1200000000];
      float **q1=new float*[10];
      float **q2=new float*[10];
      float **q3=new float*[10];
      float **qP1=new float*[10];
      float **qP2=new float*[10];
      float **qP3=new float*[10];
      for(int i = 0; i < 10; i++)
	{
	  q1[i] = new float[120000000];
	  q2[i] = new float[120000000];
	  q3[i] = new float[120000000];
	  qP1[i] = new float[120000000];
	  qP2[i] = new float[120000000];
	  qP3[i] = new float[120000000];
	}
		      
      float *arg;
      
      float total_event=tuple_qmoment->GetEntries();
      cout<<total_event<<"\n";
      for (int j=0;j<total_event;j++)
	{

	  tuple_qmoment->GetEntry(j);
	  arg=tuple_qmoment->GetArgs();
	  centrality=arg[0];
	  centclass[j]=centrality;
	  q1[(int)(centrality/10)][evt[(int)(centrality/10)]]=arg[6];
	  q2[(int)(centrality/10)][evt[(int)(centrality/10)]]=arg[8];
    
	  qP1[(int)(centrality/10)][evt[(int)(centrality/10)]]=arg[2];
	  qP2[(int)(centrality/10)][evt[(int)(centrality/10)]]=arg[4];
	  evt[(int)(centrality/10)]=evt[(int)(centrality/10)]+1;
	  if(centrality>90) cout<<"chk"<<"\n";
	}
      TRandom2 *r = new TRandom2();
      int random;
      
      TProfile *q1_kaon_neg=new TProfile("q1_kaon_neg","q1_kaon_neg",10,0,100);
      TProfile *q2_kaon_neg=new TProfile("q2_kaon_neg","q2_kaon_neg",10,0,100);
      TProfile *q1square_kaon_neg=new TProfile("q1square_kaon_neg","q1square_kaon_neg",10,0,100);
        
      TProfile *q1_kaon_pos=new TProfile("q1_kaon_pos","q1_kaon_pos",10,0,100);
      TProfile *q2_kaon_pos=new TProfile("q2_kaon_pos","q2_kaon_pos",10,0,100);
      TProfile *q1square_kaon_pos=new TProfile("q1square_kaon_pos","q1square_kaon_pos",10,0,100);
  
      TProfile *q1_kaon_kaon=new TProfile("q1_kaon_kaon","q1_kaon_kaon",10,0,100);
  
  int range;  
  for(int CNTCLS=0;CNTCLS<9;CNTCLS++)
    {

      range=evt[CNTCLS]-1;
      cout<<CNTCLS<<"\t"<<range<<"\n";
      for(int evtcentclass=0;evtcentclass<range;evtcentclass++)
	{
	  random=r->Integer(range);
	  if(random==evtcentclass)
	    {
	      random=r->Integer(range);
	    }
	  q1_kaon_neg->Fill(CNTCLS*10,q1[CNTCLS][evtcentclass]);
	  q2_kaon_neg->Fill(CNTCLS*10,q2[CNTCLS][evtcentclass]);
	  q1square_kaon_neg->Fill(CNTCLS*10,q1[CNTCLS][evtcentclass]*q1[CNTCLS][evtcentclass]);

	  q1_kaon_pos->Fill(CNTCLS*10,qP1[CNTCLS][evtcentclass]);
	  q2_kaon_pos->Fill(CNTCLS*10,qP2[CNTCLS][evtcentclass]);
	  q1square_kaon_pos->Fill(CNTCLS*10,qP1[CNTCLS][evtcentclass]*qP1[CNTCLS][evtcentclass]);
	  
	  q1_kaon_kaon->Fill(CNTCLS*10,qP1[CNTCLS][evtcentclass]*q1[CNTCLS][evtcentclass]);
	}
    }
  TFile *fout=new TFile(Form("output_mixd_kk2_sample%d.root",sample),"RECREATE");
  fout->cd();
  q1_kaon_neg->Write();
  q2_kaon_neg->Write();
  q1square_kaon_neg->Write();
  
  q1_kaon_pos->Write();
  q2_kaon_pos->Write();
  q1square_kaon_pos->Write();
  
  q1_kaon_kaon->Write();
  
  fout->Close();
  
  
  for(int i = 0; i < 10; i++)
    {
      delete [] q1[i];
      delete [] q2[i];
      delete [] qP1[i];
      delete [] qP2[i];
    }
  delete[] q1;
  delete[] q2;
  delete[] qP1;
  delete[] qP2;
    }
}

#include "../utils/Config.h"

void findCut(){
  ofstream out("findCut.dat");
  for (int iVar{0}; iVar < kNTpcClsCuts * kNDcaCuts * kNChi2Cuts * kNPidCuts * kNBdtCuts * kNMassCuts; ++iVar)
  {
    int iTpcClsCut = (iVar / 1) % kNTpcClsCuts;
    int iPidCut = (iVar / kNTpcClsCuts) % kNPidCuts;
    int iDcaCut = (iVar / kNTpcClsCuts / kNPidCuts) % kNDcaCuts;
    int iChi2Cut = (iVar / kNTpcClsCuts / kNPidCuts / kNDcaCuts) % kNChi2Cuts;
    int iMassCut = (iVar / kNTpcClsCuts / kNPidCuts / kNDcaCuts / kNChi2Cuts) % kNMassCuts;
    int iBdtScoreCut = (iVar / kNTpcClsCuts / kNPidCuts / kNDcaCuts / kNChi2Cuts / kNMassCuts) % kNBdtCuts;
    cout << iVar << "\t" << iTpcClsCut << "\t" << iPidCut << "\t" << iDcaCut << "\t" << iChi2Cut << "\t" << iMassCut << "\t" << iBdtScoreCut << endl;
    out << iVar << "\t" << iTpcClsCut << "\t" << iPidCut << "\t" << iDcaCut << "\t" << iChi2Cut << "\t" << iMassCut << "\t" << iBdtScoreCut << endl;
    if (iTpcClsCut == 1 && iPidCut == 2 && iDcaCut == 1 && iChi2Cut == 1 && iMassCut == 1 && iBdtScoreCut == 2) cout << iVar << endl;
  }
  out.close();
}
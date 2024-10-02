// Config.h
// configuration file

#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <TColor.h>

struct MiniCascadeTrain {
  float pt;
  float mass;
  double dcaBachPV;
  double dcaV0PV;
  double dcaV0piPV;
  double dcaV0prPV;
  double dcaV0tracks;
  double dcaBachV0;
  double cosPA;
  double cosPAV0;
  double tpcNsigmaV0Pr;
  unsigned char centrality;
};

struct MiniXi {
  Double32_t fPt; //[-12.7,12.8,8]
  Double32_t fEta; //[-1.27,1.28,8]
  Double32_t fMass; //[1.29,1.35375,8]
  Double32_t fBdtOut; //[0.5,1.,16]
  unsigned char fRecFlag;
};

struct MiniXiMC : public MiniXi {
  Double32_t fPtMC; //[-12.7,12.8,8]
  Double32_t fEtaMC; //[-1.27,1.28,8]
  bool fIsReconstructed;
  unsigned char fFlag;
};

struct MiniKaon {
  Double32_t fPt; //[-3.175,3.2,8]
  Double32_t fEta; //[-1.27,1.28,8]
  Double32_t fNsigmaITS; //[-6.35,6.4,8]
  Double32_t fNsigmaTPC; //[-6.35,6.4,8]
  Double32_t fNsigmaTOF; //[-6.35,6.4,8]
  unsigned char fCutBitMap;
};

struct MiniKaonMC : public MiniKaon {
  Double32_t fPtMC; //[-3.175,3.2,8]
  Double32_t fEtaMC; //[-1.27,1.28,8]
  bool fIsReconstructed;
  unsigned char fFlag;
};

struct MiniCollision {
  Double32_t fZ; //[-12.7,12.8,8]
  unsigned char fCent;
  unsigned char fTrigger;
  unsigned short fNTrk;
  unsigned short fV0MAmp;
};

struct MicroKaon {
  MicroKaon(){};
  MicroKaon(const MicroKaon &other){
    fPt = other.fPt;
    fIsKaon = other.fIsKaon;
  };
  MicroKaon(const double pt, const bool isK) :
    fPt{pt},
    fIsKaon{isK}
  {
    //
  }
  Double32_t fPt; //[-3.175,3.2,8]
  bool fIsKaon;
};

struct MicroXi {
  MicroXi(){};
  MicroXi(const MicroXi &other){
    fPt = other.fPt;
  };
  MicroXi(const double pt) :
    fPt{pt}
  {
    //
  }
  Double32_t fPt; //[-12.7,12.8,8]
};

struct MicroTrack {
  MicroTrack(){};
  MicroTrack(const MicroTrack &other){
    fPt = other.fPt;
    fIsKaon = other.fIsKaon;
    fIsXi = other.fIsXi;
  };
  MicroTrack(const double pt, const bool isK, const bool isXi) :
    fPt{pt},
    fIsKaon{isK},
    fIsXi{isXi}
  {
    //
  }
  double fPt;
  bool fIsKaon;
  bool fIsXi;
};

struct MiniEvent2 {
  MiniEvent2(){};
  MiniEvent2(const MiniEvent2 &other){
    fCent = other.fCent;
    for (auto track : other.fTracks)
    {
      fTracks.emplace_back(track.fPt, track.fIsKaon, track.fIsXi);
    }
  };
  MiniEvent2(const unsigned char cent) :
    fCent{cent}
  {
    //
  }
  unsigned char fCent;
  std::vector<MicroTrack> fTracks;

  bool IsSame(MiniEvent2 other){
    return (this->fCent == other.fCent && this->fTracks.front().fPt == other.fTracks.front().fPt && this->fTracks.front().fIsKaon == other.fTracks.front().fIsKaon && this->fTracks.front().fIsXi == other.fTracks.front().fIsXi);
  }
};

struct MiniEvent {
  MiniEvent(){};
  MiniEvent(const MiniEvent &other){
    fCent = other.fCent;
    for (auto kaon : other.fKaons)
    {
      fKaons.emplace_back(kaon.fPt, kaon.fIsKaon);
    }
    for (auto xi : other.fXi)
    {
      fXi.emplace_back(xi.fPt);
    }
  };
  MiniEvent(const unsigned char cent) :
    fCent{cent}
  {
    //
  }
  unsigned char fCent;
  std::vector<MicroKaon> fKaons;
  std::vector<MicroXi> fXi;
};

constexpr const char* kResDir = "results";
constexpr const char* kDataDir = "/data/mciacco/KXiCorrelations";
constexpr const char* kEffKFile = "efficieny/KaonXiEff_LHC20f11_postCalib_finalBinning";
constexpr const char* kBdtmapName = "bdt_eff_map_lhc20f11_postCalib";
//constexpr const char* kEffKFile = "efficieny/KaonXiEff_LHC22l5_postCalib_1";
//constexpr const char* kEffKFile = "KaonXiEff_LHC22l5_postCalib";
constexpr const char* kEffXiFile = "PreselEff_0_90";
constexpr const char* kEffBDTXiFile = "XiBDTEff";
constexpr const bool kUseKaonXiEff = true;

constexpr double kXiMass{1.32171};

constexpr bool isMC = false;
constexpr int N_SAMPLE = 30;
//constexpr int N_SUB_CENT = 10;
constexpr int DELTA = 1;
constexpr int kNCentBinsMed = 18;
constexpr const char* kSubsampleFlag = "o_16_";
// constexpr double kCentBinsMed[kNCentBinsMed + 1] = {0., 5., 10., 15., 20., 25., 30., 35., 40., 45., 50., 55., 60., 65., 70., 75., 80., 85., 90.};
constexpr int kNCentBinsSmall = 100;
constexpr double kCentBinsSmall[kNCentBinsSmall + 1] = {0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16., 17., 18., 19., 20., 21., 22., 23., 24., 25., 26., 27., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 44., 45., 46., 47., 48., 49., 50., 51., 52., 53., 54., 55., 56., 57., 58., 59., 60., 61., 62., 63., 64., 65., 66., 67., 68., 69., 70., 71., 72., 73., 74., 75., 76., 77., 78., 79., 80., 81., 82., 83., 84., 85., 86., 87., 88., 89., 90., 91., 92., 93., 94., 95., 96., 97., 98., 99., 100.};
// constexpr int kNCentBinsSmall = 9;
// constexpr double kCentBinsSmall[kNCentBinsSmall + 1] = {0., 10., 20., 30., 40., 50., 60., 70., 80., 90.};

constexpr const char* kAntiMatterLabel[2] = {"A", "M"}; // following pdg convention K+ and Xi- are matter, K- and Xi+ antimatter
constexpr const char* kAntiMatterLabelML[2] = {"antimatter", "matter"}; // following pdg convention K+ and Xi- are matter, K- and Xi+ antimatter
constexpr const char* kChargeLabel[2] = {"Same", "Opp"};
constexpr const char* kCorrLabel[3] = {"Uncorr", "Corr", "Gen"};
constexpr const char* kAvgVarLabel[2] = {"Avg", "Var"};
constexpr const char* kPartLabel[2] = {"K", "Xi"};
constexpr const char* kPartLabelExtend[2] = {"Kaon", "Xi"};
constexpr const char* kXiChargeLabel[2] = {"Minus", "Plus"};
constexpr const char* kPartLetterLabel[2] = {"K", "#Xi"};
/* constexpr int kNCentBins = 8;
constexpr double kCentBins[kNCentBins + 1] = {0., 5., 10., 20., 30., 40., 50., 70., 100.}; */
//constexpr double kCentBins[kNCentBins + 1] = {0., 10., 20., 30., 40., 50., 60., 70., 80., 90.};
constexpr int kNCentBins = 9;
constexpr double kCentBins[kNCentBins + 1] = {0., 10., 20., 30., 40., 50., 60., 70., 80., 90.};
// constexpr int kNCentBins = 7;
// constexpr double kCentBins[kNCentBins + 1] = {0., 5., 10., 20., 40., 60., 80., 100.};
//constexpr double kCentBinsSmall[kNCentBinsSmall + 1] = {0., 10., 20., 30., 40., 50., 60., 70., 80., 90.}; //100.};

// fnal PbPb binning
// constexpr int kNCentBins = 7;
// constexpr double kCentBins[kNCentBins + 1] = {0., 10., 30., 40., 50., 70., 80., 90.};

// final pPb binning
// constexpr int kNCentBins = 6;
// constexpr double kCentBins[kNCentBins + 1] = {0., 10., 20., 40., 60., 80., 100.};

constexpr double kEtaCut = 0.8;
constexpr double kNsigmaTPCcut = 5.;
constexpr double kNsigmaTPCcutPresel = 2.5;
constexpr double kNsigmaTOFcut = 5.;
constexpr double kITSptCut = .0; // .4 PbPb
constexpr double kTPCptCut = .4;
constexpr double kTOFptCut = 1.;
constexpr double kXiLowPtCut = 1.;
constexpr double kXiUpPtCut = 3.;
constexpr double kXiMassCut = .006;
constexpr double kXiMassCuts[3] = {.003, .0045, 0.006}; // {0.005, 0.006, 0.007};
constexpr double kBdtEffCut = .7;
constexpr double kBdtScoreCut = 0.99;
//constexpr double kBdtScoreCuts[5] = {0.9750, 0.9775, 0.9800, 0.9825, 0.9850};
//constexpr double kBdtEffCuts[5] = {0.50, 0.525, 0.55, 0.575, 0.60};
constexpr double kBdtEffCuts[] = {0.55, 0.56, 0.57, 0.58, 0.59, 0.60, 0.61, 0.62, 0.63, 0.64}; // {0.25, 0.26, 0.27, 0.28, 0.29, 0.30, 0.31, 0.32, 0.33, 0.34}; //{0.25, 0.275, 0.3, 0.325, 0.35}; // PbPb // {0.55, 0.575, 0.6, 0.625, 0.65}; // pp//  // {0.55, 0.6, 0.65}; // pPb //
constexpr double kPtLowLimitK = 0.2;
constexpr int kLimitedSample = 10000000;

constexpr double kDCAcutPt = 0.5;
constexpr double k_eff_shift = 0.0; // 0.1 PbPb

// k
const int kNTpcClsCuts = 3;
const int kNDcaCuts = 2;
const int kNChi2Cuts = 2;
const int kNPidCuts = 5;
// xi
const int kNBdtCuts = 10;
const int kNMassCuts = 3;

// // k
// const int kNTpcClsCuts = 1;
// const int kNDcaCuts = 1;
// const int kNChi2Cuts = 1;
// const int kNPidCuts = 1;
// // xi
// const int kNBdtCuts = 1;
// const int kNMassCuts = 1;

constexpr char kCutDCA[] = {BIT(0), BIT(0), BIT(0)};
constexpr char kCutTPCcls[] = {BIT(2), BIT(2), BIT(2)};
constexpr char kCutChi2[] = {BIT(4), BIT(4), BIT(4)};
constexpr char kCutDCA2[] = {BIT(0), BIT(1), BIT(1)}; // to change first entry if DCA cut is varied
constexpr char kCutTPCcls2[] = {BIT(2), BIT(3), BIT(3)};
constexpr char kCutChi22[] = {BIT(5), BIT(5), BIT(5)}; // to change first entry if DCA cut is varied
constexpr bool kRequireDCAcut[] = {true, false, false};
constexpr bool kRequireTPCclsCut[] = {true, true, false};
constexpr bool kRequireChi2Cut[] = {true, false, false}; // second variation changed to default

constexpr bool kUseCircularPID = false;
constexpr double kNsigmaITScutAsym[] = {-3., 3.};
constexpr double kNsigmaTPCcutAsym[][2] = {{-2., 2.}, {-2.5, 2.5}, {-3., 3.}, {-3.5, 3.5}, {-4., 4.}};
constexpr double kNsigmaTOFcutAsym[][2] = {{-2., 2.}, {-2.5, 2.5}, {-3., 3.}, {-3.5, 3.5}, {-4., 4.}};
//constexpr double kNsigmaITScutAsym[] = {-5, 5};
//  constexpr double kNsigmaTPCcutAsym[] = {-3, 3}; // for purity routine
//  constexpr double kNsigmaTOFcutAsym[] = {-3., 3.};

constexpr double kCircularCuts[] = {3.};

constexpr int kNBinsPt = 40;
constexpr double kMinPt = 0;
constexpr double kDeltaPt = 0.1;
constexpr int kNBinsPtXi = 7;
constexpr double kMinPtXi = 0;
constexpr double kDeltaPtXi = 0.5;
constexpr int kNEtaBins = 1; // 10
constexpr double kMinEta = -1.;
constexpr double kDeltaEta = 2.; // .2

constexpr double kDummyEffK = 1.; // for code validation purposes
constexpr double kDummyEffXi = 1.; // for code validation purposes
constexpr double kDummyBdtScoreXi = .5; // for code validation purposes

constexpr int kTextFont = 44;
constexpr int kTextSize = 20;
constexpr int kMaxCent = 90;

constexpr const char* kCollidingSystemText = "pp #sqrt{#it{s}} = 5.02 TeV";
constexpr const char* kParticleAntiparticleText[2] = {"K^{+} + K^{-}", "#Xi^{+} + #Xi^{-}"}; // {"( K^{+} + K^{-} ) / 2", "( #Xi^{-} + #bar{#Xi}^{+} ) / 2"};
constexpr const char* kParticleAntiparticleTextAM[2][2] = {{"K^{-}", "K^{+}"}, {"#Xi^{+}", "#Xi^{-}"}}; // {"( K^{+} + K^{-} ) / 2", "( #Xi^{-} + #bar{#Xi}^{+} ) / 2"};

constexpr bool kLimitSample = false;
constexpr bool kUseIndex = true;

constexpr bool kGetBDTEffMap = false;

constexpr bool kRebinNsigma = true;
constexpr bool k2018 = false; // true for PbPb 2018

constexpr bool kNetCorrelation = true;
constexpr bool kUseBdtInMC = true;

constexpr bool kRejectMultXiFromSameDaughters = true;

constexpr bool kComputeNSigmaMap = false;

int colors[] = {TColor::GetColor("#ff3300"), TColor::GetColor("#ec6e0a"), TColor::GetColor("#daaa14"), TColor::GetColor("#c7e51e"), TColor::GetColor("#85dd69"), TColor::GetColor("#42d6b4"), TColor::GetColor("#00ceff"), TColor::GetColor("#009adf"), TColor::GetColor("#0067c0"), TColor::GetColor("#595959"), TColor::GetColor("#0033a1")};

// final results
constexpr int kNPointsPbPb = 8;
constexpr int kNPointspp = 10;

//constexpr double shm_pp[][2] = {{-0.0275668, 0.000531104}, {-0.0291294, 0.000531034}, {-0.0296298, 0.000739823}, {-0.0293525, 0.00066963}, {-0.0299159, 0.000500312}, {-0.0305578, 0.000382911}, {-0.0305624, 0.000431261}};
constexpr double shm_pp[][2] = {{-0.016172, 0.000537196}, {-0.0223345, 0.000562904}, {-0.0249177, 0.000282553}, {-0.0254563, 0.000436144}, {-0.0267958, 0.000532905}, {-0.0290004, 0.000853941}, {-0.0307059, 0.00053685}, {-0.0315178, 0.00066366}, {-0.0318691, 0.000504605}};
constexpr double shm_PbPb[][2] = {{-0.0332519, 0.00080112}, {-0.0323324, 0.00102529}, {-0.0345024, 0.00107273}, {-0.0326442, 0.000845147}, {-0.0337629, 0.00108542}, {-0.0347605, 0.00121336}, {-0.0340746, 0.00126095}, {-0.0313947, 0.0010268}};

constexpr double pythia[] = {-0.0124815, -0.012902, -0.0153011, -0.0158455, -0.0158228, -0.0163589, -0.0166321};
constexpr double hijing[][2] = {{-0.01292, 0.002037}, {-0.01469, 0.001883}, {-0.01455, 0.002376}, {-0.01370, 0.001517}, {-0.01225, 0.001678}, {-0.01042, 0.001781}, {-0.01197, 0.002154}, {-0.01223, 0.001588}};

constexpr double mult_pp[] = {15.3, 11.93, /* 13.615, */ 9.72, 7.76, 6.34, 5.22, 3.94, 2.43};
//constexpr double mult_shm_pp[] = {5., 10., 15., 20., 30., 40., 50.};
constexpr double mult_pythia_angantyr[] = {93.6006, 297.034, 677.216, 1343.3};
constexpr double mult_shm_pp_[] = {2.579, 3.956, 5.692, 7.108, 8.964, 11.49, 13.98, 17.35};
constexpr double mult_shm_pp[] = {2.5, 4.5, 6.3, 8.0, 10.0, 13.75, 16.2, 20.0, 26.0};
constexpr double mult_shm_PbPb_[] = {1764.0, 983.0, 512.0, 318.0, 139.65, 44.9};
constexpr double mult_shm_PbPb[] = {1764.0, 1180.0, 786.0, 512.0, 318.0, 183.0, 96.3, 44.9};
constexpr double mult_pythia[] = {4.07654, 4.28077, 9.07586, 13.7777, 19.4331, 25.124, 30.605};
constexpr double mult_pythia_[] = {2.30545, 3.39399, 4.93719, 6.36435, 7.99441, 10.1399, 12.4428, 15.5011};
constexpr double mult_pPb[] = {/* 45.0, 36.2,  */40.6, 30.5, 23.2, 16.1, 9.8, 4.4};
constexpr double mult_pPb_model[] = {4.4, 9.8, 16.1, 23.2, 30.5, 36.2, 45.0};

constexpr double epos_pPb[][2] = {{-0.01631, 0.0002622}, {-0.02457, 0.0002197}, {-0.02778, 0.0002074}, {-0.02932, 0.0001662}, {-0.02991, 0.0003416}, {-0.03090, 0.0004189}, {-0.031, 0.0004909}};
constexpr double pythia_crmpi_ropoff_netCorr[][2] = {{-0.0101904, 0.000414684}, {-0.011975, 0.000665201}, {-0.0128809, 0.00110034}, {-0.0136778, 0.00075497}, {-0.0156551, 0.000640976}, {-0.0164225, 0.00117345}, {-0.0163105, 0.000609012}, {-0.0186335, 0.00127562}};
constexpr double pythia_crmpi_ropon_netCorr[][2] = {{-0.010878, 0.000494055}, {-0.01316, 0.000587497}, {-0.0133156, 0.000611085}, {-0.0156535, 0.000737707}, {-0.0153109, 0.000978571}, {-0.0166444, 0.000860795}, {-0.018625, 0.000851148}, {-0.0162875, 0.000711206}};
constexpr double pythia_crqcd_netCorr[][2] = {{-0.0116131, 0.00056526}, {-0.0146324, 0.000798673}, {-0.0162674, 0.000720464}, {-0.0175851, 0.00103533}, {-0.0199016, 0.0010604}, {-0.021831, 0.000997782}, {-0.0210498, 0.00172705}, {-0.0251314, 0.0010716}};
constexpr double pythia_angantyr_netCorr[][2] = {{-0.0186075, 0.00252455}, {-0.0169428, 0.00383907}, {-0.0145492, 0.00247115}, {-0.0156016, 0.00308881}};

constexpr double mult_pythia_c2c1[] = {2.53388, 4.02895, 6.05133, 7.832, 10.3005, 13.6431, 17.2718, 21.1207, 27.1577};

constexpr double shm_PbPb_c2c1[][2] = {{0.98663, 0.00138168}, {0.984061, 0.00169225}, {0.981633, 0.00126597}, {0.980026, 0.0013279}, {0.981512, 0.00160361}, {0.9818, 0.00157076}, {0.979801, 0.00102404}, {0.980962, 0.00159702}};
constexpr double shm_pp_c2c1[][2] = {{0.992826, 0.00262663}, {0.996138, 0.00124965}, {0.992758, 0.0010979}, {0.991745, 0.00151156}, {0.988784, 0.00131077}, {0.987259, 0.00144333}, {0.988311, 0.0018368}, {0.98771, 0.00110115}, {0.985959, 0.00079204}};
constexpr double pythia_c2c1[][2] = {{0.969894, 0.00372985}, {0.968498, 0.00269244}, {0.970238, 0.00303011}, {0.961653, 0.00264185}, {0.958441, 0.00260595}, {0.954356, 0.0015908}, {0.951204, 0.00207366}, {0.948031, 0.0017814}, {0.942538, 0.00318293}};
constexpr double pythia_crqcd_c2c1[][2] = {{0.977926, 0.0022903}, {0.975661, 0.00182678}, {0.972281, 0.00268657}, {0.977182, 0.00149907}, {0.97927, 0.00252852}, {0.974666, 0.00174942}, {0.972517, 0.00215869}, {0.971177, 0.0017554}};
constexpr double pythia_crmpi_ropon_c2c1[][2] = {{0.977353, 0.00211111}, {0.9706, 0.00247815}, {0.968706, 0.00221434}, {0.967967, 0.00261047}, {0.959326, 0.00204189}, {0.960804, 0.00190625}, {0.957101, 0.00298722}, {0.951714, 0.00192197}};
constexpr double pythia_crmpi_ropoff_c2c1[][2] = {{0.977788, 0.00178395}, {0.971762, 0.00277661}, {0.969529, 0.00403359}, {0.966469, 0.00202853}, {0.962865, 0.00189807}, {0.958693, 0.00236351}, {0.956138, 0.00421621}, {0.9524, 0.00218701}};
constexpr double pythia_angantyr_c2c1[][2] = {{0.928482, 0.00331044}, {0.906047, 0.0039014}, {0.854768, 0.00645864}, {0.746759, 0.0105543}};
constexpr double hijing_c2c1[][2] = {{0.945067, 0.00244204}, {0.944672, 0.00340008}, {0.943355, 0.00162088}, {0.944432, 0.00195922}, {0.94695, 0.000958743}, {0.943941, 0.00210302}, {0.945566, 0.00245815}, {0.949056, 0.0032301}};
constexpr double epos_pPb_c2c1[][2] = {{0.9894, 0.0007415}, {0.9873, 0.0003582}, {0.9856, 0.0003656}, {0.9845, 0.0001628}, {0.9831, 0.0004556}, {0.9831, 0.0004506}, {0.9826, 0.0006936}};

constexpr double SHM_pPb_boost_mult[] = {4.4, 9.8, 16.1, 23.2, 30.5, 36.2, 45.1};
constexpr double SHM_pPb_woboost_mult[] = {4.4, 9.8, 16.1, 23.2, 30.5, 36.2, 45.1};
constexpr double SHM_pPb_boost[][2] = {{-0.0235018, 0.000675454}, {-0.0276618, 0.000675004}, {-0.0299484, 0.000294994}, {-0.0313987, 0.000490207}, {-0.0318719, 0.000624238}, {-0.0332301, 0.000847792}, {-0.0334602, 0.000590374}};
constexpr double SHM_pPb_woboost[][2] = {{-0.0235403, 0.000434277}, {-0.0269739, 0.000270104}, {-0.029664, 0.000536201}, {-0.0317484, 0.000461504}, {-0.0314932, 0.000478201}, {-0.0331994, 0.000677664}, {-0.0340757, 0.000770227}};

constexpr double pythia_angantyr_pPb_mult[] = {2.96692, 11.4465, 18.4913, 25.5337, 38.5651};
constexpr double pythia_angantyr_pPb[][2] = {{-0.0107123, 0.00074598}, {-0.0156619, 0.000627122}, {-0.0157046, 0.000600899}, {-0.0165176, 0.000521906}, {-0.0150651, 0.000585721}};

constexpr double pythia_angantyr_pPb_crqcd_rope_mult[] = {3.1607, 11.4166, 17.6669, 24.1903, 34.2064};
constexpr double pythia_angantyr_pPb_crqcd_rope[][2] = {{-0.0175068, 0.00204877}, {-0.0169595, 0.00179276}, {-0.0211508, 0.0017223}, {-0.0184069, 0.00202715}, {-0.0197782, 0.00267254}};

constexpr double pythia_angantyr_pPb_c2c1[][2] = {{0.975986, 0.00351429}, {0.958035, 0.0015959}, {0.959044, 0.00129959}, {0.959718, 0.00144488}, {0.960322, 0.000831072}};

constexpr double shm_pbpb_c2c1k[][2] = {{0.848094, 0.00164811}, {0.846901, 0.00105754}, {0.846605, 0.000906364}, {0.847198, 0.00136481}, {0.848438, 0.00136354}, {0.852548, 0.00135875}, {0.851687, 0.00135579}, {0.85501, 0.00128356}};

#endif // CONFIGFILE_H

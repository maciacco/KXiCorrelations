// Config.h
// configuration file

#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <TColor.h>

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
};

constexpr const char* kDataDir = "/data/mciacco/KXiCorrelations";
constexpr const char* kEffKFile = "KaonXiEff_21d6_strinj";
constexpr const char* kEffXiFile = "PreselEff_0_90";
constexpr const char* kEffBDTXiFile = "XiBDTEff";
constexpr const bool kUseKaonXiEff = true;

constexpr double kXiMass{1.32171};

constexpr bool isMC = false;
constexpr int N_SAMPLE = 10;
constexpr int N_SUB_CENT = 10;
constexpr int kNCentBinsMed = 18;
constexpr const char* kSubsampleFlag = "o";
constexpr double kCentBinsMed[kNCentBinsMed + 1] = {0., 5., 10., 15., 20., 25., 30., 35., 40., 45., 50., 55., 60., 65., 70., 75., 80., 85., 90.};
constexpr int kNCentBinsSmall = 90;
constexpr double kCentBinsSmall[kNCentBinsSmall + 1] = {0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16., 17., 18., 19., 20., 21., 22., 23., 24., 25., 26., 27., 28., 29., 30., 31., 32., 33., 34., 35., 36., 37., 38., 39., 40., 41., 42., 43., 44., 45., 46., 47., 48., 49., 50., 51., 52., 53., 54., 55., 56., 57., 58., 59., 60., 61., 62., 63., 64., 65., 66., 67., 68., 69., 70., 71., 72., 73., 74., 75., 76., 77., 78., 79., 80., 81., 82., 83., 84., 85., 86., 87., 88., 89., 90.};
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
constexpr int kNCentBins = 9;
constexpr double kCentBins[kNCentBins + 1] = {0., 10., 20., 30., 40., 50., 60., 70., 80., 90.};

constexpr double kEtaCut = 0.8;
constexpr double kNsigmaTPCcut = 5.;
constexpr double kNsigmaTPCcutPresel = 3.;
constexpr double kNsigmaTOFcut = 5.;
constexpr double kTPCptCut = .5;
constexpr double kTOFptCut = 1.;
constexpr double kXiLowPtCut = 1.;
constexpr double kXiUpPtCut = 3.;
constexpr double kXiMassCut = .006;
constexpr double kBdtEffCut = .7;
constexpr double kBdtScoreCut = 0.99;
constexpr double kPtLowLimitK = 0.2;
constexpr int kLimitedSample = 100000;

constexpr double kNsigmaTPCcutAsym[] = {-2.5, 3.5};
constexpr double kNsigmaTOFcutAsym[] = {-3., 3.};
/* constexpr double kNsigmaTPCcutAsym[] = {-5, 5};
constexpr double kNsigmaTOFcutAsym[] = {-5., 5.}; */

constexpr int kNBinsPt = 40;
constexpr double kMinPt = 0;
constexpr double kDeltaPt = 0.1;
constexpr int kNBinsPtXi = 7;
constexpr double kMinPtXi = 0;
constexpr double kDeltaPtXi = 0.5;
constexpr int kNEtaBins = 1;
constexpr double kMinEta = -1.;
constexpr double kDeltaEta = 2.;

constexpr double kDummyEffK = 1.; // for code validation purposes
constexpr double kDummyEffXi = 1.; // for code validation purposes
constexpr double kDummyBdtScoreXi = .5; // for code validation purposes

constexpr int kTextFont = 44;
constexpr int kTextSize = 20;

constexpr const char* kCollidingSystemText = "Pb-Pb #sqrt{#it{s}_{NN}} = 5.02 TeV";
constexpr const char* kParticleAntiparticleText[2] = {"K^{+}", "#Xi^{-}"}; // {"( K^{+} + K^{-} ) / 2", "( #Xi^{-} + #bar{#Xi}^{+} ) / 2"};
constexpr const char* kParticleAntiparticleTextAM[2][2] = {{"K^{-}", "K^{+}"}, {"#Xi^{+}", "#Xi^{-}"}}; // {"( K^{+} + K^{-} ) / 2", "( #Xi^{-} + #bar{#Xi}^{+} ) / 2"};

constexpr bool kLimitSample = false;
constexpr bool kUseIndex = false;

constexpr bool kRebinNsigma = true;
constexpr bool k2018 = false;

constexpr bool kNetCorrelation = true;
constexpr bool kUseBdtInMC = true;

int colors[] = {TColor::GetColor("#ff3300"), TColor::GetColor("#ec6e0a"), TColor::GetColor("#daaa14"), TColor::GetColor("#c7e51e"), TColor::GetColor("#85dd69"), TColor::GetColor("#42d6b4"), TColor::GetColor("#00ceff"), TColor::GetColor("#009adf"), TColor::GetColor("#0067c0"), TColor::GetColor("#595959"), TColor::GetColor("#0033a1")};

#endif // CONFIGFILE_H

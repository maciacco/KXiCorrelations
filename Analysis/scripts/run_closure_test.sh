root -b -l <<EOF
//.L ReadTreeEffCorr.cxx+
//ReadTreeEffCorr("pp/mc_train/AnalysisResults_mcGP", "oMC_mcGP", true)
.x Correlation.cxx("oMC_mcGP", "outMC_mcGP")
EOF
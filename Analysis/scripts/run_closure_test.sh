root -b -l <<EOF
.L ReadTreeEffCorr.cxx+
ReadTreeEffCorr("mc_tree/AnalysisResults_LHC21d6abc", "oMC_LHC21d6_", true)
.x Correlation.cxx("oMC_LHC21d6_", "outMC_LHC21d6_")
EOF
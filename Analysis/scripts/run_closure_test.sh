root -b -l <<EOF
.L ReadTreeEffCorr.cxx+
ReadTreeEffCorr("mc_tree/AnalysisResults_LHC21d6_strinj", "oMC_LHC21d6_strinj", true)
.x Correlation.cxx("oMC_LHC21d6_strinj", "outMC_LHC21d6_strinj")
EOF
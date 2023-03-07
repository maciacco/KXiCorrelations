root -b -l <<EOF
//.L ReadTreeEffCorr.cxx+
//ReadTreeEffCorr("mc_tree/AnalysisResults_LHC21l5_child1_cpy", "oMC_LHC21l5", true)
.x Correlation.cxx("oMC_LHC21l5", "outMC_LHC21l5")
EOF
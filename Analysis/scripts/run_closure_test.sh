root -b -l <<EOF
.L ReadTreeEffCorr.cxx+
ReadTreeEffCorr("mc_tree/AnalysisResults_mc", "oMC", true)
.x Correlation.cxx("oMC", "outMC")
EOF
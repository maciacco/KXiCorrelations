root -b -l <<EOF
.L ReadTreeEffCorr.cxx+
ReadTreeEffCorr("tree/AnalysisResults_LHC15o_KXI", "o_LHC15o")
.x Correlation.cxx("o_LHC15o", "our_LHC15o")
EOF
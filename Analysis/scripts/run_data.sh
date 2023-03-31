root -b -l <<EOF
.L ReadTreeEffCorr.cxx+
ReadTreeEffCorr("tree/AnalysisResults_prova", "o_prova")
.x Correlation.cxx("o_prova", "out_prova")
EOF
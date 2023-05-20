root -b -l <<EOF
.L ReadTreeEffCorr.cxx+
ReadTreeEffCorr("pp/tree/AnalysisResults_LHC17pq_afterCalib", "o_lhc17pq_afterCalib")
.x Correlation.cxx("o_lhc17pq_afterCalib", "out_pp_afterCalib")
EOF
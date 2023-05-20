num="$1"

root -b -l <<EOF
.L ReadTreeEffCorr.cxx+
ReadTreeEffCorr("tree/dataset_full_merge/AnalysisResults_$num","o_$num")
EOF
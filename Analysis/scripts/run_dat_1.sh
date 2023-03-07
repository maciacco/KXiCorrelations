num="$1"

root -b -l <<EOF
.L ReadTreeEffCorr.cxx+
ReadTreeEffCorr("tree/part_merging_True/AnalysisResults_$num","o$num")
EOF
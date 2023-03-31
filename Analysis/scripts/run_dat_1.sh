num="$1"

root -b -l <<EOF
.L ReadTreeEffCorr.cxx+
ReadTreeEffCorr("tree/15o/AnalysisResults_$num","o$num")
EOF
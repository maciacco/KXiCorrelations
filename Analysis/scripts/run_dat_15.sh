num="$1"

root -b -l <<EOF
.L ReadTreeEffCorr.cxx+
ReadTreeEffCorr("tree/dataset_after_calib_15o/part_merging_True/AnalysisResults-$num","o_15_$num")
EOF
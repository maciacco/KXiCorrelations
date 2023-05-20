num="$1"

root -b -l <<EOF
.L ReadTreeEvMixCp.cxx+
ReadTreeEvMixCp("tree/dataset_full_merge/AnalysisResults_$num","o_mix2$num")
EOF
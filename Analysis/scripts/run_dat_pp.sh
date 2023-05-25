num="$1"

root -b -l <<EOF
.L ReadTreeEffCorr.cxx+
ReadTreeEffCorr("pp/tree_split/part_merging_True/AnalysisResults_merge_$num","/data/mciacco/KXiCorrelations/output_sys_dir/17pq/o_$num")
EOF
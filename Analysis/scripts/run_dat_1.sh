num="$1"
num2="$2"
num3="$3"

root -b -l <<EOF
.L ReadTreeEffCorr.cxx+
ReadTreeEffCorr("tree/dataset_full_merge/AnalysisResults_$num","/data/mciacco/KXiCorrelations/output_sys_dir/18qr/o_$num", $num2, $num3)
EOF
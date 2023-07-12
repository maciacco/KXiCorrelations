num1="$1"
num2="$2"

root -b -l <<EOF
.L ReadTreeEffCorr2.cxx+
ReadTreeEffCorr2("pp/tree/calib_fast_mc/AnalysisResults_LHC17pq_calib", "/data/mciacco/KXiCorrelations/output_sys_dir/17pq_3/o_", $num1, $num2)
EOF
num1="$1"
num2="$2"

root -b -l <<EOF
.L ReadTreeEffCorr2.cxx+
ReadTreeEffCorr2("pp/tree/AnalysisResults_LHC17pq_afterCalib", "/data/mciacco/KXiCorrelations/output_sys_dir/17pq_new/o_", $num1, $num2)
EOF
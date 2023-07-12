num1="$1"
num2="$2"

root -b -l <<EOF
.L ReadTreeEffCorr2.cxx+
ReadTreeEffCorr2("pPb/AnalysisResults_LHC16qt_postCalib", "/data/mciacco/KXiCorrelations/output_sys_dir/16qt_3/o_", $num1, $num2)
EOF
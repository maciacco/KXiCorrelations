child="$1"
runlist="$2"

root -b -l <<EOF
.L ReadTreeEffCorr.cxx+
ReadTreeEffCorr("mc_tree/AnalysisResults_LHC21l5_child${child}_${runlist}","o${child}_${runlist}",true)
EOF
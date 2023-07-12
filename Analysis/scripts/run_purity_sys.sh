for a in {0..89}
do
root -b -l<<EOF
.L ../utils/RooGausDExp.cxx+
.L ../utils/RooDSCBShape.cxx+
.x PuritySys.cxx($a)
EOF
done
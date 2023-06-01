num="$1"
min="$2"
max="$3"

root -b -l <<EOF
.L sameevent2.C+
sameevent2($num, $min, $max)
EOF
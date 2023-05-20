num="$1"
min="$2"
max="$3"

root -b -l <<EOF
.L sameevent.C+
sameevent($num, $min, $max)
EOF
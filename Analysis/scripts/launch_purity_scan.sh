root -b -l<<EOF
.L ReadTreeEffCorr.cxx+
ReadTreeEffCorr("tree/dataset_full_merge/AnalysisResults_LHC18qr_postCalib", "out_purity_18_25", 0.25)
EOF

root -b -l<<EOF
.L ../utils/RooGausDExp.cxx+
.L ../utils/RooDSCBShape.cxx+
.x Purity.cxx("out_purity_18_25_var_0","Purity_18_25")
EOF

root -b -l<<EOF
.L ReadTreeEffCorr.cxx+
ReadTreeEffCorr("tree/dataset_full_merge/AnalysisResults_LHC18qr_postCalib", "out_purity_18_30", 0.30)
EOF

root -b -l<<EOF
.L ../utils/RooGausDExp.cxx+
.L ../utils/RooDSCBShape.cxx+
.x Purity.cxx("out_purity_18_30_var_0","Purity_18_30")
EOF

root -b -l<<EOF
.L ReadTreeEffCorr.cxx+
ReadTreeEffCorr("tree/dataset_full_merge/AnalysisResults_LHC18qr_postCalib", "out_purity_18_35", 0.35)
EOF

root -b -l<<EOF
.L ../utils/RooGausDExp.cxx+
.L ../utils/RooDSCBShape.cxx+
.x Purity.cxx("out_purity_18_35_var_0","Purity_18_35")
EOF

root -b -l<<EOF
.L ReadTreeEffCorr.cxx+
ReadTreeEffCorr("tree/dataset_full_merge/AnalysisResults_LHC18qr_postCalib", "out_purity_18_40", 0.40)
EOF

root -b -l<<EOF
.L ../utils/RooGausDExp.cxx+
.L ../utils/RooDSCBShape.cxx+
.x Purity.cxx("out_purity_18_40_var_0","Purity_18_40")
EOF

root -b -l<<EOF
.L ReadTreeEffCorr.cxx+
ReadTreeEffCorr("tree/dataset_full_merge/AnalysisResults_LHC18qr_postCalib", "out_purity_18_45", 0.45)
EOF

root -b -l<<EOF
.L ../utils/RooGausDExp.cxx+
.L ../utils/RooDSCBShape.cxx+
.x Purity.cxx("out_purity_18_45_var_0","Purity_18_45")
EOF


root -b -l<<EOF
.L ReadTreeEffCorr.cxx+
ReadTreeEffCorr("tree/dataset_full_merge/AnalysisResults_LHC18qr_postCalib", "out_purity_18_50", 0.50)
EOF

root -b -l<<EOF
.L ../utils/RooGausDExp.cxx+
.L ../utils/RooDSCBShape.cxx+
.x Purity.cxx("out_purity_18_50_var_0","Purity_18_50")
EOF


root -b -l<<EOF
.L ReadTreeEffCorr.cxx+
ReadTreeEffCorr("tree/dataset_full_merge/AnalysisResults_LHC18qr_postCalib", "out_purity_18_55", 0.55)
EOF

root -b -l<<EOF
.L ../utils/RooGausDExp.cxx+
.L ../utils/RooDSCBShape.cxx+
.x Purity.cxx("out_purity_18_55_var_0","Purity_18_55")
EOF


root -b -l<<EOF
.L ReadTreeEffCorr.cxx+
ReadTreeEffCorr("tree/dataset_full_merge/AnalysisResults_LHC18qr_postCalib", "out_purity_18_60", 0.60)
EOF

root -b -l<<EOF
.L ../utils/RooGausDExp.cxx+
.L ../utils/RooDSCBShape.cxx+
.x Purity.cxx("out_purity_18_60_var_0","Purity_18_60")
EOF
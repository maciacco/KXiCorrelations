#num="$1"
num1="$1"
num2="$2"

root -b -l <<EOF
.L ReadTreeEffCorr2.cxx+
ReadTreeEffCorr2("tree/dataset_full_merge/AnalysisResults_LHC18qr_postCalib", "/data/mciacco/KXiCorrelations/output_sys_dir/18qr_new/o_", $num1, $num2)
EOF


#rm o*var_60.root o*var_61.root o*var_62.root o*var_63.root o*var_64.root o*var_65.root o*var_66.root o*var_67.root o*var_68.root o*var_69.root o*var_70.root o*var_71.root o*var_72.root o*var_73.root o*var_74.root o*var_75.root o*var_76.root o*var_77.root o*var_78.root o*var_79.root o*var_80.root o*var_81.root o*var_82.root o*var_83.root o*var_84.root o*var_85.root o*var_86.root o*var_87.root o*var_88.root o*var_89.root o*var_90.root o*var_91.root o*var_92.root o*var_93.root o*var_94.root o*var_95.root o*var_96.root o*var_97.root o*var_98.root o*var_99.root o*var_100.root o*var_101.root o*var_102.root o*var_103.root o*var_104.root o*var_105.root o*var_106.root o*var_107.root o*var_108.root o*var_109.root o*var_110.root o*var_111.root o*var_112.root o*var_113.root o*var_114.root o*var_115.root o*var_116.root o*var_117.root o*var_118.root o*var_119.root o*var_120.root o*var_121.root o*var_122.root o*var_123.root o*var_124.root o*var_125.root o*var_126.root o*var_127.root o*var_128.root o*var_129.root o*var_130.root o*var_131.root o*var_132.root o*var_133.root o*var_134.root o*var_135.root
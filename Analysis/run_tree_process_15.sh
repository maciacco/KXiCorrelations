hadd -f -T o_merge_parallel_15o_postCalib.root o_15_1.root o_15_2.root o_15_3.root o_15_4.root o_15_5.root o_15_6.root o_15_7.root o_15_8.root o_15_9.root o_15_10.root
#rm o1.root o2.root o3.root o4.root o5.root o6.root o7.root o8.root o9.root o10.root o11.root o12.root o13.root o14.root o15.root o16.root o17.root o18.root o19.root o20.root o21.root o22.root o23.root o24.root o25.root o26.root o27.root o28.root o29.root o30.root o31.root o32.root

#cat cmds_15 | xargs -P 10 -I CMD bash -c CMD
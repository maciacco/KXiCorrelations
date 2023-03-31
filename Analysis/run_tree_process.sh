hadd -f -T o_merge_parallel_15o_full.root o1.root o2.root o3.root o4.root o5.root o6.root o7.root o8.root o9.root o10.root # o11.root o12.root o13.root o14.root o15.root o16.root
rm o1.root o2.root o3.root o4.root o5.root o6.root o7.root o8.root o9.root o10.root #o11.root o12.root o13.root o14.root o15.root o16.root

#cat cmds | xargs -P 16 -I CMD bash -c CMD
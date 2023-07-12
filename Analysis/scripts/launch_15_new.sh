cat cmds_15_a_reduced | xargs -P 15 -I CMD bash -c CMD
cat cmds_15_b_reduced | xargs -P 15 -I CMD bash -c CMD

cat cmds_sys_PbPb_15 | xargs -P 30 -I CMD bash -c CMD
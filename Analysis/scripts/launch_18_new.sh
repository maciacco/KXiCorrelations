cat cmds_18_a_reduced | xargs -P 15 -I CMD bash -c CMD
cat cmds_18_b_reduced | xargs -P 15 -I CMD bash -c CMD

cat cmds_sys_pp | xargs -P 30 -I CMD bash -c CMD
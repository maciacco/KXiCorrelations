#cat cmds_18_a | xargs -P 13 -I CMD bash -c CMD
#cat cmds_18_b | xargs -P 14 -I CMD bash -c CMD

cat cmds_sys_pp | xargs -P 30 -I CMD bash -c CMD
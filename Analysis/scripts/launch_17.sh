# cat cmds_pp_1 | xargs -P 10 -I CMD bash -c CMD
# cat cmds_pp_2 | xargs -P 10 -I CMD bash -c CMD
# cat cmds_pp_3 | xargs -P 10 -I CMD bash -c CMD
cat cmds_sys_pp | xargs -P 30 -I CMD bash -c CMD
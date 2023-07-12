cat cmds_pp_reduced | xargs -P 20 -I CMD bash -c CMD
cat cmds_sys_pp | xargs -P 30 -I CMD bash -c CMD
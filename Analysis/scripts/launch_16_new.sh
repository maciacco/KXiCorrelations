# cat cmds_pPb_reduced | xargs -P 10 -I CMD bash -c CMD
# cat cmds_pPb_reduced_b | xargs -P 10 -I CMD bash -c CMD
cat cmds_sys_pPb | xargs -P 30 -I CMD bash -c CMD
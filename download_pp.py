import os
import yaml

CHILD = True
# child 1

if not os.path.exists(f'part_merging_{CHILD}'):
        os.makedirs(f'part_merging_{CHILD}')
tree_name = "AnalysisResults"
output_name = "AnalysisResults"

DOWNLOAD = True

run_numbers = [[282341], [282341], [282366], [282366]]
rec = ['pass2_FAST', 'pass2_CENT_woSDD', 'pass2_FAST', 'pass2_CENT_woSDD']
total = [[362], [222], [63], [69]]
period = ['p', 'p', 'q', 'q']
job_id = [1176, 20230427, 1828]

#/alice/data/2017/LHC17p/000282341/pass2_FAST/PWGLF/LF_pp_AOD/1176_20230427-1828_child_1/Stage_1
for i_child, run_number in enumerate(run_numbers):
    run_counter = 1
    for i_run, run in enumerate(run_number):
        input_dir = f'/alice/data/2017/LHC17{period[i_child]}/000{run}/{rec[i_run]}/PWGLF/LF_pp_AOD/{job_id[0]}_{job_id[1]}-{job_id[2]}_child_{i_child + 1}/'
        if DOWNLOAD:
            for ind in range(1, total[i_child][i_run] + 1):
                dir_str = str(ind).zfill(4)
                input_file = input_dir + dir_str + "/" + tree_name
                os.system(f'alien_cp alien://{input_file}.root file:part_merging_{CHILD}/{tree_name}-{run_counter}.root')
                run_counter = run_counter + 1

            #os.system(f'alihadd AnalysisResults_merged_child{i_child}_run{i_run}.root part_merging_{CHILD}/AnalysisResults-*.root')
            #os.system(f'rm part_mergign_{CHILD}/*')
#os.system(f'alihadd  AnalysisResults_child_{CHILD}.root part_merging_{CHILD}/*')


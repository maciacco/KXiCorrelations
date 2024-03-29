import os
import yaml

CHILD = True
# child 1

if not os.path.exists(f'part_merging_{CHILD}'):
        os.makedirs(f'part_merging_{CHILD}')
tree_name = "AnalysisResults"
output_name = "AnalysisResults"

DOWNLOAD = True

run_numbers = [[282341]]
period = ['p']
job_id = [1218, 20230623, 1800]
run_counter = 1

for i_child, run_number in enumerate(run_numbers):
    for run in run_number:
        input_dir = f'/alice/data/2017/LHC17{period[i_child]}/000{run}/pass2_FAST/PWGLF/LF_pp_AOD/{job_id[0]}_{job_id[1]}-{job_id[2]}_child_1/'
        if DOWNLOAD:
            for ind in range(1,395):
                dir_str = str(ind).zfill(4)
                input_file = input_dir + dir_str + "/" + tree_name
                os.system(f'alien_cp alien://{input_file}.root file:part_merging_{CHILD}/{tree_name}-{run_counter}.root')
                run_counter = run_counter + 1

#for i in range(1, int(run_counter/2 + 1)):
#    os.system(f'alihadd  AnalysisResults_{i}.root part_merging_{CHILD}/AnalysisResults-{i}.root part_merging_{CHILD}/AnalysisResults-{2 * i}.root')
#os.system(f'alihadd  AnalysisResults_child_{CHILD}.root part_merging_{CHILD}/*')


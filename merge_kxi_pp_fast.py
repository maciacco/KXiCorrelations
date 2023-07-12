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
period = ['l5c_fast']
job_id = [995, 20230622, 1812]
run_counter = 1

for i_child, run_number in enumerate(run_numbers):
    for run in run_number:
        input_dir = f'/alice/sim/2022/LHC22{period[i_child]}/{run}/PWGLF/LF_pp_MC_AOD/{job_id[0]}_{job_id[1]}-{job_id[2]}_child_1/'
        if DOWNLOAD:
            for ind in range(1,103):
                dir_str = str(ind).zfill(4)
                input_file = input_dir + dir_str + "/" + tree_name
                os.system(f'alien_cp alien://{input_file}.root file:part_merging_{CHILD}/{tree_name}-{run_counter}.root')
                run_counter = run_counter + 1

#for i in range(1, int(run_counter/2 + 1)):
#    os.system(f'alihadd  AnalysisResults_{i}.root part_merging_{CHILD}/AnalysisResults-{i}.root part_merging_{CHILD}/AnalysisResults-{2 * i}.root')
#os.system(f'alihadd  AnalysisResults_child_{CHILD}.root part_merging_{CHILD}/*')


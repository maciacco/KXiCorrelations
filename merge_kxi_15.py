import os
import yaml

CHILD = True
# child 1

if not os.path.exists(f'part_merging_{CHILD}'):
        os.makedirs(f'part_merging_{CHILD}')
tree_name = "AnalysisResults"
output_name = "AnalysisResults"

DOWNLOAD = True

run_numbers = [[246994, 246809, 246424, 246180, 246087, 246001, 245775, 245507, 245397, 246392]]
period = ['o']
job_id = [1482, 20230427, 1824]

run_counter = 1

for i_child, run_number in enumerate(run_numbers):
    for run in run_number:
        input_dir = f'/alice/data/2015/LHC15{period[i_child]}/000{run}/pass2/AOD252/PWGLF/LF_PbPb_AOD/{job_id[0]}_{job_id[1]}-{job_id[2]}/'
        if DOWNLOAD:
            for ind in range(1,2):
                dir_str = str(ind).zfill(4)
                input_file = input_dir + tree_name
                os.system(f'alien_cp alien://{input_file}.root file:part_merging_{CHILD}/{tree_name}-{run_counter}.root')
                run_counter = run_counter + 1

#for i in range(1, int(run_counter/2 + 1)):
#    os.system(f'alihadd  AnalysisResults_{i}.root part_merging_{CHILD}/AnalysisResults-{i}.root part_merging_{CHILD}/AnalysisResults-{2 * i}.root')
#os.system(f'alihadd  AnalysisResults_child_{CHILD}.root part_merging_{CHILD}/*')


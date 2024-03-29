import os
import yaml

CHILD = True
# child 1

if not os.path.exists(f'part_merging_{CHILD}'):
        os.makedirs(f'part_merging_{CHILD}')
tree_name = "AnalysisResults"
output_name = "AnalysisResults"

DOWNLOAD = True

run_numbers = [[296191, 296383, 296510, 295831, 296550, 296415, 296433, 296509, 296377, 295612, 296246, 295725, 296424], [297194, 297450, 297537, 297219, 297544, 297029, 297415, 297479, 296749, 296848, 297481, 297221, 297512, 297441, 296799, 296934, 297195, 296849, 296750]]
total = [[443, 457, 328, 383, 407, 393, 292, 413, 409, 415, 398, 373, 360], [480, 533, 473, 345, 373, 548, 472, 380, 572, 492, 246, 395, 468, 365, 459, 482, 392, 335, 257]]
period = ['q', 'r']
job_id = [1481, 20230427, 1823]


for i_child, run_number in enumerate(run_numbers):
    for i_run, run in enumerate(run_number):
        input_dir = f'/alice/data/2018/LHC18{period[i_child]}/000{run}/pass3/AOD252/PWGLF/LF_PbPb_AOD/{job_id[0]}_{job_id[1]}-{job_id[2]}_child_{i_child + 1}/'
        if DOWNLOAD:
            run_counter = 1
            for ind in range(1, total[i_child][i_run] + 1):
                dir_str = str(ind).zfill(4)
                input_file = input_dir + dir_str + "/" + tree_name
                os.system(f'alien_cp alien://{input_file}.root file:part_merging_{CHILD}/{tree_name}-{run_counter}.root')
                run_counter = run_counter + 1

            os.system(f'alihadd AnalysisResults_merged_child{i_child}_run{i_run}.root part_merging_{CHILD}/AnalysisResults-*.root')
            os.system(f'rm part_mergign_{CHILD}/*')
#os.system(f'alihadd  AnalysisResults_child_{CHILD}.root part_merging_{CHILD}/*')


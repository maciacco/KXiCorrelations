import os
import yaml

CHILD = True
# child 1

if not os.path.exists(f'part_merging_{CHILD}'):
        os.makedirs(f'part_merging_{CHILD}')
tree_name = "AnalysisResults.root"
output_name = "AnalysisResults.root"

DOWNLOAD = True

input_dir = '/alice/data/2018/LHC18r/000297194/pass3/AOD252/PWGLF/LF_PbPb_AOD/1454_20230301-1844_child_2/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/2_1_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_1.root part_merging_{CHILD}/2_1_*')
# os.system(f'rm part_merging_{CHILD}/2_1_*')

input_dir = '/alice/data/2018/LHC18r/000297450/pass3/AOD252/PWGLF/LF_PbPb_AOD/1454_20230301-1844_child_2/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/2_2_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_2.root part_merging_{CHILD}/2_2_*')
# os.system(f'rm part_merging_{CHILD}/2_2_*')

input_dir = '/alice/data/2018/LHC18r/000297537/pass3/AOD252/PWGLF/LF_PbPb_AOD/1454_20230301-1844_child_2/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/2_3_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_3.root part_merging_{CHILD}/2_3_*')
# os.system(f'rm part_merging_{CHILD}/2_3_*')

input_dir = '/alice/data/2018/LHC18r/000297219/pass3/AOD252/PWGLF/LF_PbPb_AOD/1454_20230301-1844_child_2/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/2_4_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_4.root part_merging_{CHILD}/2_4_*')
# os.system(f'rm part_merging_{CHILD}/2_4_*')

input_dir = '/alice/data/2018/LHC18r/000297544/pass3/AOD252/PWGLF/LF_PbPb_AOD/1454_20230301-1844_child_2/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/2_5_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_5.root part_merging_{CHILD}/2_5_*')
# os.system(f'rm part_merging_{CHILD}/2_5_*')

input_dir = '/alice/data/2018/LHC18r/000297029/pass3/AOD252/PWGLF/LF_PbPb_AOD/1454_20230301-1844_child_2/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/2_6_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_6.root part_merging_{CHILD}/2_6_*')
# os.system(f'rm part_merging_{CHILD}/2_6_*')

input_dir = '/alice/data/2018/LHC18r/000297415/pass3/AOD252/PWGLF/LF_PbPb_AOD/1454_20230301-1844_child_2/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/2_7_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_7.root part_merging_{CHILD}/2_7_*')
# os.system(f'rm part_merging_{CHILD}/2_7_*')

input_dir = '/alice/data/2018/LHC18r/000297479/pass3/AOD252/PWGLF/LF_PbPb_AOD/1454_20230301-1844_child_2/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/2_8_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_8.root part_merging_{CHILD}/2_8_*')
# os.system(f'rm part_merging_{CHILD}/2_8_*')

input_dir = '/alice/data/2018/LHC18r/000296749/pass3/AOD252/PWGLF/LF_PbPb_AOD/1454_20230301-1844_child_2/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/2_9_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_9.root part_merging_{CHILD}/2_9_*')
# os.system(f'rm part_merging_{CHILD}/2_9_*')

input_dir = '/alice/data/2018/LHC18r/000296848/pass3/AOD252/PWGLF/LF_PbPb_AOD/1454_20230301-1844_child_2/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/2_10_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_10.root part_merging_{CHILD}/2_10_*')
# os.system(f'rm part_merging_{CHILD}/2_10_*')

input_dir = '/alice/data/2018/LHC18r/000297481/pass3/AOD252/PWGLF/LF_PbPb_AOD/1454_20230301-1844_child_2/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/2_11_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_11.root part_merging_{CHILD}/2_11_*')
# os.system(f'rm part_merging_{CHILD}/2_11_*')

input_dir = '/alice/data/2018/LHC18r/000297221/pass3/AOD252/PWGLF/LF_PbPb_AOD/1454_20230301-1844_child_2/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/2_12_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_12.root part_merging_{CHILD}/2_12_*')
# os.system(f'rm part_merging_{CHILD}/2_12_*')

input_dir = '/alice/data/2018/LHC18r/000297512/pass3/AOD252/PWGLF/LF_PbPb_AOD/1454_20230301-1844_child_2/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/2_13_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_13.root part_merging_{CHILD}/2_13_*')
# os.system(f'rm part_merging_{CHILD}/2_13_*')

input_dir = '/alice/data/2018/LHC18r/000297441/pass3/AOD252/PWGLF/LF_PbPb_AOD/1454_20230301-1844_child_2/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/2_14_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_14.root part_merging_{CHILD}/2_14_*')
# os.system(f'rm part_merging_{CHILD}/2_14_*')

input_dir = '/alice/data/2018/LHC18r/000296799/pass3/AOD252/PWGLF/LF_PbPb_AOD/1454_20230301-1844_child_2/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/2_15_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_15.root part_merging_{CHILD}/2_15_*')
# os.system(f'rm part_merging_{CHILD}/2_15_*')

input_dir = '/alice/data/2018/LHC18r/000296934/pass3/AOD252/PWGLF/LF_PbPb_AOD/1454_20230301-1844_child_2/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/2_16_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_16.root part_merging_{CHILD}/2_16_*')
# os.system(f'rm part_merging_{CHILD}/2_16_*')

input_dir = '/alice/data/2018/LHC18r/000297195/pass3/AOD252/PWGLF/LF_PbPb_AOD/1454_20230301-1844_child_2/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/2_17_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_17.root part_merging_{CHILD}/2_17_*')
# os.system(f'rm part_merging_{CHILD}/2_17_*')

input_dir = '/alice/data/2018/LHC18r/000296849/pass3/AOD252/PWGLF/LF_PbPb_AOD/1454_20230301-1844_child_2/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/2_18_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_18.root part_merging_{CHILD}/2_18_*')
# os.system(f'rm part_merging_{CHILD}/2_18_*')

input_dir = '/alice/data/2018/LHC18r/000296750/pass3/AOD252/PWGLF/LF_PbPb_AOD/1454_20230301-1844_child_2/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/2_19_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_19.root part_merging_{CHILD}/2_19_*')
# os.system(f'rm part_merging_{CHILD}/2_19_*')

input_dir = '/alice/data/2018/LHC18q/000296191/pass3/AOD252/PWGLF/LF_PbPb_AOD//1454_20230301-1844_child_1/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/1_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_20.root part_merging_{CHILD}/1_*')
# os.system(f'rm  part_merging_{CHILD}/20_*')

input_dir = '/alice/data/2018/LHC18q/000296383/pass3/AOD252/PWGLF/LF_PbPb_AOD//1454_20230301-1844_child_1/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/2_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_21.root part_merging_{CHILD}/2_*')
# os.system(f'rm part_merging_{CHILD}/21_*')

input_dir = '/alice/data/2018/LHC18q/000296510/pass3/AOD252/PWGLF/LF_PbPb_AOD//1454_20230301-1844_child_1/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/3_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_22.root part_merging_{CHILD}/3_*')
# os.system(f'rm part_merging_{CHILD}/22_*')

input_dir = '/alice/data/2018/LHC18q/000295831/pass3/AOD252/PWGLF/LF_PbPb_AOD//1454_20230301-1844_child_1/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/12_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_23.root part_merging_{CHILD}/12_*')
# os.system(f'rm part_merging_{CHILD}/23_*')

input_dir = '/alice/data/2018/LHC18q/000296550/pass3/AOD252/PWGLF/LF_PbPb_AOD//1454_20230301-1844_child_1/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/4_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_24.root part_merging_{CHILD}/4_*')
# os.system(f'rm part_merging_{CHILD}/24_*')

input_dir = '/alice/data/2018/LHC18q/000296415/pass3/AOD252/PWGLF/LF_PbPb_AOD//1454_20230301-1844_child_1/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/5_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_25.root part_merging_{CHILD}/5_*')
# os.system(f'rm part_merging_{CHILD}/25_*')

input_dir = '/alice/data/2018/LHC18q/000296433/pass3/AOD252/PWGLF/LF_PbPb_AOD//1454_20230301-1844_child_1/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/6_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_26.root part_merging_{CHILD}/6_*')
# os.system(f'rm part_merging_{CHILD}/26_*')

input_dir = '/alice/data/2018/LHC18q/000296509/pass3/AOD252/PWGLF/LF_PbPb_AOD//1454_20230301-1844_child_1/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/7_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_27.root part_merging_{CHILD}/7_*')
# os.system(f'rm part_merging_{CHILD}/27_*')

input_dir = '/alice/data/2018/LHC18q/000296377/pass3/AOD252/PWGLF/LF_PbPb_AOD//1454_20230301-1844_child_1/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/8_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_28.root part_merging_{CHILD}/8_*')
# os.system(f'rm part_merging_{CHILD}/28_*')

input_dir = '/alice/data/2018/LHC18q/000295612/pass3/AOD252/PWGLF/LF_PbPb_AOD//1454_20230301-1844_child_1/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/9_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_29.root part_merging_{CHILD}/9_*')
# os.system(f'rm part_merging_{CHILD}/29_*')

input_dir = '/alice/data/2018/LHC18q/000296246/pass3/AOD252/PWGLF/LF_PbPb_AOD//1454_20230301-1844_child_1/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/13_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_30.root part_merging_{CHILD}/13_*')
# os.system(f'rm part_merging_{CHILD}/30_*')

input_dir = '/alice/data/2018/LHC18q/000295725/pass3/AOD252/PWGLF/LF_PbPb_AOD//1454_20230301-1844_child_1/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/10_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_31.root part_merging_{CHILD}/10_*')
# os.system(f'rm part_merging_{CHILD}/31_*')

input_dir = '/alice/data/2018/LHC18q/000296424/pass3/AOD252/PWGLF/LF_PbPb_AOD//1454_20230301-1844_child_1/'
if DOWNLOAD:
    for ind in range(1,2):
        dir_str = str(ind).zfill(4)
        input_file = input_dir + tree_name
        os.system(f'alien_cp alien://{input_file} file:part_merging_{CHILD}/11_{ind}{tree_name}')
# os.system(f'alihadd  AnalysisResults_child_{CHILD}_32.root part_merging_{CHILD}/11_*')
# os.system(f'rm part_merging_{CHILD}/32_*')


os.system(f'alihadd  AnalysisResults_child_{CHILD}.root part_merging_{CHILD}/*')

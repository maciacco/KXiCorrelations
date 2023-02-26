#!/usr/bin/env python3
import os
import pickle
import warnings
import argparse

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import ROOT
import uproot
import xgboost as xgb
import yaml


parser = argparse.ArgumentParser(prog='ml_analysis', allow_abbrev=True)
parser.add_argument('-split', action='store_true')
parser.add_argument('-mergecentrality', action='store_true')
parser.add_argument('-eff', action='store_true')
parser.add_argument('-train', action='store_true')
parser.add_argument('-computescoreff', action='store_true')
parser.add_argument('-application', action='store_true')
args = parser.parse_args()

SPLIT = args.split
MAX_EFF = 0.90

# training
TRAINING = not args.application
PLOT_DIR = 'plots'
MAKE_PRESELECTION_EFFICIENCY = args.eff
MAKE_FEATURES_PLOTS = False
MAKE_TRAIN_TEST_PLOT = args.train
OPTIMIZE = False
OPTIMIZED = False
TRAIN = args.train
COMPUTE_SCORES_FROM_EFF = args.computescoreff
MERGE_CENTRALITY = args.mergecentrality

# application
APPLICATION = args.application

# avoid pandas warning
warnings.simplefilter(action='ignore', category=FutureWarning)
ROOT.gROOT.SetBatch()

##################################################################
# read configuration file
##################################################################
config = 'config.yaml'
with open(os.path.expandvars(config), 'r') as stream:
    try:
        params = yaml.full_load(stream)
    except yaml.YAMLError as exc:
        print(exc)

PT_BINS_CENT = params['PT_BINS_CENT']
PT_BINS = params['PT_BINS']
CT_BINS_CENT = params['CT_BINS_CENT']
CT_BINS = params['CT_BINS']
CENTRALITY_LIST = params['CENTRALITY_LIST']
CENTRALITY_BINS = params['CENTRALITY_BINS']
TRAINING_COLUMNS_LIST = params['TRAINING_COLUMNS']
RANDOM_STATE = params['RANDOM_STATE']
HYPERPARAMS = params['HYPERPARAMS']
HYPERPARAMS_RANGES = params['HYPERPARAMS_RANGES']
##################################################################

score_eff_arrays_dict = pickle.load(open("file_score_eff_dict", "rb"))

# split matter/antimatter
SPLIT_LIST = ['all']
if SPLIT:
    SPLIT_LIST = ['antimatter', 'matter']

f_out = ROOT.TFile("XiBDTEff.root", "recreate")

for split in SPLIT_LIST:

    split_ineq_sign = '> -0.1'
    if SPLIT:
        split_ineq_sign = '> 0.5'
        if split == 'antimatter':
            split_ineq_sign = '< 0.5'

    bins_array_cent = np.asarray(CENTRALITY_BINS, dtype=float)
    bins_array_pt = np.asarray(PT_BINS_CENT[0], dtype=float)
    eff_array = np.arange(0.10, MAX_EFF, 0.01)
    bdt_eff_cent_pt = ROOT.TH3F("hBDTEff", ";Centrality (%);#it{p}_{T} (GeV/#it{c});#epsilon_{BDT}", len(bins_array_cent) - 1, bins_array_cent, len(bins_array_pt) - 1, bins_array_pt, len(eff_array) - 1, eff_array)

    for i_cent_bins in range(len(CENTRALITY_LIST)):
        cent_bins = CENTRALITY_LIST[i_cent_bins]
        
        for i_pt_bin, pt_bins in enumerate(PT_BINS):
            if (pt_bins[0] < 0 or pt_bins[1] > 40):
                continue
            bin = f'{split}_{cent_bins[0]}_{cent_bins[1]}_{pt_bins[0]}_{pt_bins[1]}'

            for eff in eff_array:
                score = score_eff_arrays_dict[bin][int(eff*100 - 10)]
                bdt_eff_cent_pt.SetBinContent(i_cent_bins + 1, i_pt_bin + 1, int(eff*100 - 10) + 1, score)

    f_out.cd()
    bdt_eff_cent_pt.Write()

f_out.Close()
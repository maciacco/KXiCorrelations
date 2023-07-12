#!/usr/bin/env python3
import os
import pickle
import warnings
import argparse

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import scipy
import ROOT
import uproot
import xgboost as xgb
import yaml
from hipe4ml import analysis_utils, plot_utils
from hipe4ml.analysis_utils import train_test_generator
from hipe4ml.model_handler import ModelHandler
from hipe4ml.tree_handler import TreeHandler
from sklearn.model_selection import train_test_split
from sklearn.utils.class_weight import compute_sample_weight

parser = argparse.ArgumentParser(prog='ml_analysis', allow_abbrev=True)
parser.add_argument('-split', action='store_true')
parser.add_argument('-dotraining', action='store_true')
parser.add_argument('-generate', action='store_true')
parser.add_argument('-mergecentrality', action='store_true')
parser.add_argument('-train', action='store_true')
parser.add_argument('-computescoreff', action='store_true')
parser.add_argument('-application', action='store_true')
args = parser.parse_args()

SPLIT = args.split
MAX_SCORE = 1.00
MAX_EFF = 1.00
USE_PD = True
DUMP_HYPERPARAMS = True
USE_REAL_DATA = True

PRODUCE_DATASETS = args.generate
use_gpu = True

# avoid pandas warning
warnings.simplefilter(action='ignore', category=FutureWarning)
ROOT.gROOT.SetBatch()
ROOT.EnableImplicitMT(10)

# training
ppPbPb = '_pp5'
RECOMPUTE_DICT = True
PLOT_DIR = 'plots'
MAKE_TRAIN_TEST_PLOT = True
OPTIMIZE = True
OPTIMIZED = True
TRAIN = args.dotraining
COMPUTE_SCORES_FROM_EFF = args.computescoreff
TRAINING = args.train and (COMPUTE_SCORES_FROM_EFF or TRAIN)
MERGE_CENTRALITY = args.mergecentrality
CREATE_TRAIN_TEST = True

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
CENTRALITY_LIST = params['CENTRALITY_LIST']
TRAINING_COLUMNS_LIST = params['TRAINING_COLUMNS']
RANDOM_STATE = params['RANDOM_STATE']
HYPERPARAMS = params['HYPERPARAMS']
HYPERPARAMS_RANGES = params['HYPERPARAMS_RANGES']
HYPERPARAMS_RANGES_HIGH_PT = params['HYPERPARAMS_RANGES_HIGH_PT']
##################################################################

ROOT.gInterpreter.ProcessLine(".L help.h+")
# split matter/antimatter
SPLIT_LIST = ['all']
if SPLIT:
    SPLIT_LIST = ['antimatter', 'matter']

if PRODUCE_DATASETS and not TRAIN:

    for i_cent_bins in range(len(CENTRALITY_LIST)):
        cent_bins = CENTRALITY_LIST[i_cent_bins]

        # get sidebands (both for training and as pseudo-data)
        df_data = ROOT.RDataFrame("XiOmegaTree","/data/mciacco/KXiCorrelations/tree_train/AnalysisResults_data_qr_test_pass3.root")
        df_index = df_data.Define("index_1","gRandom->Rndm()+mass-mass")
        df_index.Filter(f"(mass < 1.31 || mass > 1.333) && centrality > {cent_bins[0]} && centrality < {cent_bins[1]}").Snapshot("LambdaTree",f"/data/mciacco/KXiCorrelations/trainingBackground_{cent_bins[0]}_{cent_bins[1]}.root")

if TRAINING:

    # make plot directory
    if not os.path.isdir(f'{PLOT_DIR}{ppPbPb}'):
        os.mkdir(f'{PLOT_DIR}{ppPbPb}')

    # make dataframe directory
    if not os.path.isdir(f'df_test{ppPbPb}'):
        os.mkdir(f'df_test{ppPbPb}')

    score_eff_arrays_dict = dict()

    for i_cent_bins in range(len(CENTRALITY_LIST)):
        cent_bins = CENTRALITY_LIST[i_cent_bins]

        #df_signal = uproot.open(os.path.expandvars(f"/data/mciacco/KXiCorrelations/pPb/mc_tree/trainingMC/AnalysisResults_mc_train.root"))['XiOmegaTree'].arrays(library="pd")
        df_signal = uproot.open(os.path.expandvars(f"/data/mciacco/KXiCorrelations/pp/mc_train/dataset_after_calib/AnalysisResults_LHC22l5_fast.root"))['XiOmegaTree'].arrays(library="pd")
        #df_signal = uproot.open(os.path.expandvars(f"/data/mciacco/KXiCorrelations/tree_train/AnalysisResultsTrain_0_90.root"))['XiOmegaTreeTrain'].arrays(library="pd")
        #df_signal.dcaV0piPV = df_signal.dcaV0piPV.round(1)
        #df_signal.dcaV0prPV = df_signal.dcaV0prPV.round(1)
        #df_signal.dcaBachPV = df_signal.dcaBachPV.round(1)
        # df_signal.cosPA = df_signal.cosPA.round(6)
        # df_signal.cosPAV0 = df_signal.cosPAV0.round(6)
        # df_signal.dcaBachV0 = df_signal.dcaBachV0.round(2)
        # df_signal.dcaV0tracks = df_signal.dcaV0tracks.round(2)
        # df_signal.dcaV0PV = df_signal.dcaV0PV.round(3)
        # df_signal.tpcNsigmaV0Pr = df_signal.tpcNsigmaV0Pr.round(1)
        
        #df_background = uproot.open(os.path.expandvars(f"/data/mciacco/KXiCorrelations/pPb/trainingDat/AnalysisResults_data_train.root"))['XiOmegaTree'].arrays(library="pd")
        df_background = uproot.open(os.path.expandvars(f"/data/mciacco/KXiCorrelations/pp/tree_train/AnalysisResults_LHC17pq_data.root"))['XiOmegaTree'].arrays(library="pd")
        #df_background = uproot.open(os.path.expandvars(f"/data/mciacco/KXiCorrelations/tree_train/AnalysisResults_data_qr_test_pass3_noBin.root"))['XiOmegaTreeTrain'].arrays(library="pd")
        #df_background.dcaV0piPV = df_background.dcaV0piPV.round(1)
        #df_background.dcaV0prPV = df_background.dcaV0prPV.round(1)
        #df_background.dcaBachPV = df_background.dcaBachPV.round(1)
        # df_background.cosPA = df_background.cosPA.round(6)
        # df_background.cosPAV0 = df_background.cosPAV0.round(6)
        # df_background.dcaBachV0 = df_background.dcaBachV0.round(2)
        # df_background.dcaV0tracks = df_background.dcaV0tracks.round(2)
        # df_background.dcaV0PV = df_background.dcaV0PV.round(3)
        # df_background.tpcNsigmaV0Pr = df_background.tpcNsigmaV0Pr.round(1)
        for pt_bins in PT_BINS:

            for split in SPLIT_LIST:
                split_ineq_sign = '> -0.1'
                if SPLIT:
                    split_ineq_sign = '> 0.5'
                    if split == 'antimatter':
                        split_ineq_sign = '< 0.5'

                bin = f'{split}_{cent_bins[0]}_{cent_bins[1]}_{pt_bins[0]}_{pt_bins[1]}'

                train_test_data = [pd.DataFrame(), pd.DataFrame(), pd.DataFrame(), pd.DataFrame()]
                if CREATE_TRAIN_TEST and (COMPUTE_SCORES_FROM_EFF or TRAIN):
                    df_prompt_ct = df_signal.query(f'(pdg==3312 or pdg==-3312) and pt > {pt_bins[0]} and pt < {pt_bins[1]} and mass > 1.2917100 and mass < 1.3517100 and ct > 0. and ct < 20 and isReconstructed and tpcClV0Pi > 69 and bachBarCosPA < 0.99995 and tpcClV0Pr > 69 and tpcClBach > 69 and radius < 51 and radiusV0 < 100 and dcaV0prPV < 12.7 and dcaV0piPV < 25 and dcaV0PV < 2.5 and dcaBachPV < 12.7 and eta < 0.8 and eta > -0.8 and flag==1 and not isOmega')
                    df_background_ct = df_background.query(f'centrality > {cent_bins[0]-1} and centrality < {cent_bins[1]} and pt > {pt_bins[0]} and pt < {pt_bins[1]} and (mass < 1.31 or mass > 1.333) and mass > 1.2917100 and mass < 1.3517100 and ct > 0. and ct < 20 and tpcClV0Pi > 69 and bachBarCosPA < 0.99995 and tpcClV0Pr > 69 and tpcClBach > 69 and radius < 51 and radiusV0 < 100 and dcaV0prPV < 12.7 and dcaV0piPV < 25 and dcaV0PV < 2.5 and dcaBachPV < 12.7 and eta < 0.8 and eta > -0.8 and not isOmega')

                    n_background = df_background_ct.shape[0]
                    n_prompt = df_prompt_ct.shape[0]
                    print(f"n_prompt = {n_prompt}")

                    # pPb training 
                    # if n_background > n_prompt:
                    #     df_background_ct = df_background_ct.sample(frac=n_prompt/n_background)

                    # PbPb training 
                    # if n_background > n_prompt:
                    #     df_background_ct = df_background_ct.sample(frac=n_prompt/n_background)
                    
                    # pp training
                    if n_prompt > n_background:
                        df_prompt_ct = df_prompt_ct.sample(frac=0.8*n_background/n_prompt)
                    

                    # define tree handlers
                    prompt_tree_handler = TreeHandler()
                    background_tree_handler = TreeHandler()
                    prompt_tree_handler.set_data_frame(df_prompt_ct)
                    background_tree_handler.set_data_frame(df_background_ct)
                    del df_prompt_ct, df_background_ct

                    # split data into training and test set
                    train_test_data = train_test_generator([background_tree_handler, prompt_tree_handler], [0, 1], test_size=0.5, random_state=RANDOM_STATE)
                    train_test_data[0].loc[:,'y_true'] = train_test_data[1]
                    train_test_data[2].loc[:,'y_true'] = train_test_data[3]
                    n_promppt_test = train_test_data[2].query("y_true==1")
                    print(f"n_prompt_test = {n_promppt_test.shape[0]}")
                    train_test_data[0].to_parquet(f'df_test{ppPbPb}/train_data_{cent_bins[0]}_{cent_bins[1]}_{pt_bins[0]}_{pt_bins[1]}.parquet.gzip',compression='gzip')
                    train_test_data[2].to_parquet(f'df_test{ppPbPb}/test_data_{cent_bins[0]}_{cent_bins[1]}_{pt_bins[0]}_{pt_bins[1]}.parquet.gzip',compression='gzip')
                    # continue
                else:
                    train_test_data[0] = pd.read_parquet(f'df_test{ppPbPb}/train_data_{cent_bins[0]}_{cent_bins[1]}_{pt_bins[0]}_{pt_bins[1]}.parquet.gzip')
                    train_test_data[2] = pd.read_parquet(f'df_test{ppPbPb}/test_data_{cent_bins[0]}_{cent_bins[1]}_{pt_bins[0]}_{pt_bins[1]}.parquet.gzip')
                    train_test_data[1] = train_test_data[0]['y_true']
                    train_test_data[3] = train_test_data[2]['y_true']
                ##############################################################
                # TRAINING AND TEST SET PREPARATION
                ##############################################################

                # features plot
                leg_labels = ['background', 'prompt']
                
                model_clf = xgb.XGBClassifier()
                if use_gpu:
                    model_clf = xgb.XGBClassifier(use_label_encoder=False, tree_method="gpu_hist", gpu_id=0)
                else:
                    model_clf = xgb.XGBClassifier(use_label_encoder=False, n_jobs=10, tree_method="hist")
                model_hdl = ModelHandler(model_clf, TRAINING_COLUMNS_LIST)
                model_hdl.set_model_params(HYPERPARAMS)

                # hyperparameters optimization and model training
                print(
                f'Number of candidates ({split}) for training in {pt_bins[0]} <= pt < {pt_bins[1]} cm: {len(train_test_data[0])}')
                print(
                f'prompt candidates: {np.count_nonzero(train_test_data[1] == 1)}; background candidates: {np.count_nonzero(train_test_data[1] == 0)}; n_cand_bkg / n_cand_signal = {np.count_nonzero(train_test_data[1] == 0) / np.count_nonzero(train_test_data[1] == 1)}')
                
                if not os.path.isdir(f'models_test{ppPbPb}'):
                    os.mkdir(f'models_test{ppPbPb}')
                bin_model = bin
                if MERGE_CENTRALITY:
                    bin_model = f'all_0_90_{pt_bins[0]}_{pt_bins[1]}'

                if OPTIMIZE and TRAIN:
                    if pt_bins[0] > 1.4:
                        model_hdl.optimize_params_optuna(train_test_data, HYPERPARAMS_RANGES_HIGH_PT,'roc_auc', nfold=5, timeout=100)
                    else:
                        model_hdl.optimize_params_optuna(train_test_data, HYPERPARAMS_RANGES,'roc_auc', nfold=5, timeout=100)

                isModelTrained = os.path.isfile(f'models_test{ppPbPb}/{bin_model}_optimized_trained')
                print(f'isModelTrained {bin_model}: {isModelTrained}')
                if TRAIN and not isModelTrained:
                    # weights={0:2,1:6,2:3}
                    # sample_weights = compute_sample_weight(class_weight=weights,y=train_test_data[0]['y_true'])
                    model_hdl.train_test_model(train_test_data, return_prediction=True, output_margin=False) #, sample_weight=sample_weights)
                    model_file_name = str(f'models_test{ppPbPb}/{bin_model}_trained')
                    if OPTIMIZE:
                        model_file_name = str(f'models_test{ppPbPb}/{bin_model}_optimized_trained')
                    model_hdl.dump_model_handler(model_file_name)
                elif COMPUTE_SCORES_FROM_EFF and isModelTrained:
                    print('Model trained...')
                    if OPTIMIZED:
                        model_hdl.load_model_handler(f'models_test{ppPbPb}/{bin_model}_optimized_trained')
                    else:
                        model_hdl.load_model_handler(f'models_test{ppPbPb}/{bin_model}_trained')
                else:
                    continue
                model_file_name = str(f'models_test{ppPbPb}/{bin_model}.model')
                model_hdl.dump_original_model(model_file_name,True)

                pt_bins_df_index = int(pt_bins[0]/5)
                
                bin_df = f'{split}_{cent_bins[0]}_{cent_bins[1]}_{pt_bins[0]}_{pt_bins[1]}'
                print(bin_df)
                # get only centrality selected
                train_test_data_cent = [pd.DataFrame(), [], pd.DataFrame(), []]

                train_test_data_cent[0] = train_test_data[0].query(f'(pt >= {pt_bins[0]} and pt < {pt_bins[1]}) or (pt <= -{pt_bins[0]} and pt > -{pt_bins[1]})') # matter {split_ineq_sign}
                train_test_data_cent[2] = train_test_data[2].query(f'(pt >= {pt_bins[0]} and pt < {pt_bins[1]}) or (pt <= -{pt_bins[0]} and pt > -{pt_bins[1]})')
                train_test_data_cent[1] = train_test_data_cent[0]['y_true']
                train_test_data_cent[3] = train_test_data_cent[2]['y_true']

                # get predictions for training and test sets
                print(train_test_data_cent[2])
                #if (pt_bins[0]==0):
                #    continue
                test_y_score = model_hdl.predict(train_test_data_cent[2], output_margin=False)
                print(test_y_score)
                train_y_score = model_hdl.predict(train_test_data_cent[0], output_margin=False)
                train_test_data_cent[0].loc[:,'model_output'] = train_y_score
                train_test_data_cent[2].loc[:,'model_output'] = test_y_score

                # write
                train_test_data_cent[0].to_parquet(f'df_test{ppPbPb}/train_data_{cent_bins[0]}_{cent_bins[1]}_{pt_bins[0]}_{pt_bins[1]}_predict.parquet.gzip',compression='gzip')
                train_test_data_cent[2].to_parquet(f'df_test{ppPbPb}/test_data_{cent_bins[0]}_{cent_bins[1]}_{pt_bins[0]}_{pt_bins[1]}_predict.parquet.gzip',compression='gzip')
                
                # second condition needed because of issue with Qt libraries
                if MAKE_TRAIN_TEST_PLOT:
                    if not os.path.isdir(f'{PLOT_DIR}{ppPbPb}/train_test_out'):
                        os.mkdir(f'{PLOT_DIR}{ppPbPb}/train_test_out')
                    out_figs = plot_utils.plot_output_train_test(model_hdl, train_test_data_cent, bins=50,
                                                    logscale=True, density=True, labels=leg_labels, output_margin=False)
                    out_figs.savefig(f'{PLOT_DIR}{ppPbPb}/train_test_out/{bin_df}_out.pdf')

                    # feat_imp = plot_utils.plot_feature_imp(train_test_data_cent[0], train_test_data_cent[1], model_hdl)
                    # for i_label, label in enumerate(leg_labels):
                    #    feat_imp[i_label].savefig(f'{PLOT_DIR}/train_test_out/feature_imp_training_{bin_df}_{label}.pdf')
                    plot_utils.plot_roc_train_test(
                        train_test_data_cent[3],
                        test_y_score, train_test_data_cent[1],
                        train_y_score, labels=leg_labels)
                    plt.savefig(f'{PLOT_DIR}{ppPbPb}/train_test_out/roc_train_test_{bin_df}.pdf')
                    plt.close('all')

                if COMPUTE_SCORES_FROM_EFF:
                    #pass
                    # get scores corresponding to BDT prompt efficiencies using test sets
                    eff_array = np.arange(0.10, MAX_EFF, 0.01)
                    score_array = analysis_utils.score_from_efficiency_array(
                        train_test_data_cent[3], test_y_score, efficiency_selected=eff_array, keep_lower=False)
                    score_eff_arrays_dict[bin] = score_array

                # get the model hyperparameters
                if DUMP_HYPERPARAMS and TRAIN:
                    if not os.path.isdir(f'hyperparams{ppPbPb}'):
                        os.mkdir(f'hyperparams{ppPbPb}')
                    model_params_dict = model_hdl.get_model_params()
                    with open(f'hyperparams{ppPbPb}/model_params_{bin}.yml', 'w') as outfile:
                        yaml.dump(model_params_dict, outfile, default_flow_style=False)

                    # save roc-auc
                del train_test_data_cent
                ##############################################################

    if COMPUTE_SCORES_FROM_EFF and ( TRAIN or RECOMPUTE_DICT ):
        pickle.dump(score_eff_arrays_dict, open(f"file_score_eff_dict{ppPbPb}", "wb"))


# apply model to data
if APPLICATION:
    if not os.path.isdir(f'df_test{ppPbPb}'):
        os.mkdir(f'df_test{ppPbPb}')
    score_eff_arrays_dict = pickle.load(open(f"file_score_eff_dict{ppPbPb}", "rb"))

    for split in SPLIT_LIST:

        split_ineq_sign = '> -0.1'
        if SPLIT:
            split_ineq_sign = '> 0.5'
            if split == 'antimatter':
                split_ineq_sign = '< 0.5'

        for i_cent_bins in range(len(CENTRALITY_LIST)):
            cent_bins = CENTRALITY_LIST[i_cent_bins]
            
            for pt_bins in PT_BINS:
                if (pt_bins[0] < 0 or pt_bins[1] > 40):
                    continue
                bin = f'{split}_{cent_bins[0]}_{cent_bins[1]}_{pt_bins[0]}_{pt_bins[1]}_data_apply'

                pt_bins_index = int(pt_bins[0]/5 -1)
                model_hdl = ModelHandler()
                bin_model = f'{split}_{cent_bins[0]}_{cent_bins[1]}_{pt_bins[0]}_{pt_bins[-1]}'
                if MERGE_CENTRALITY:
                    bin_model = f'all_{cent_bins[0]}_{cent_bins[1]}_{pt_bins[0]}_{pt_bins[-1]}'
                if OPTIMIZED:
                    model_hdl.load_model_handler(f'models_test{ppPbPb}/{bin_model}_optimized_trained') # *_optimized_trained
                else:
                    print(bin_model)
                    model_hdl.load_model_handler(f'models_test{ppPbPb}/{bin_model}_trained')

                eff_array = np.arange(0.10, MAX_EFF, 0.01)
                if USE_REAL_DATA:
                    if USE_PD:
                        #df_data = pd.read_parquet('df_test{ppPbPb}/data_dataset')
                        df_data = uproot.open(f'/data/mciacco/KXiCorrelations/tree_train/AnalysisResults_data_qr_test_pass3_noBin.root')['XiOmegaTreeTrain'].arrays(library="pd")
                        #df_data = df_data.append(df_data_r, ignore_index=True)
                        df_data_cent = df_data.query(
                        f'( (pt > {pt_bins[0]} and pt < {pt_bins[1]}) or (pt < -{pt_bins[0]} and pt > -{pt_bins[1]}) ) and mass < -1.2917100 and mass > -1.3517100') #f'matter {split_ineq_sign} and centrality > {cent_bins[0]} and centrality < {cent_bins[1]} and pt > {pt_bins[0]} and pt < {pt_bins[1]} and mass > 1.2917100 and mass < 1.3517100 and ct > 0. and ct < 20 and tpcClV0Pi > 69 and bachBarCosPA < 0.99995 and tpcClV0Pr > 69 and tpcClBach > 69 and radius < 51 and radiusV0 < 100 and dcaV0prPV < 12.7 and dcaV0piPV < 25 and dcaV0PV < 10.1 and dcaBachPV < 12.7 and eta < 0.8 and eta > -0.8 and not isOmega')
                        del df_data

                        data_y_score = model_hdl.predict(df_data_cent, output_margin=False)
                        df_data_cent.loc[:,'model_output'] = data_y_score

                        # df_data_cent = df_data_cent.query(f'model_output > {score_eff_arrays_dict[bin][len(eff_array)-1]}')
                        df_data_cent.to_parquet(f'df_test{ppPbPb}/{bin}.parquet.gzip', compression='gzip')
                    else:
                        df_data = TreeHandler()
                        df_data.get_handler_from_large_file(f"/data/mciacco/KXiCorrelations/tree_train/AnalysisResults_data_qr_test_pass3_noBin.root", "XiOmegaTreeTrain",
                            preselection=f'( (pt > {pt_bins[0]} and pt < {pt_bins[1]}) or (pt < -{pt_bins[0]} and pt > -{pt_bins[1]}) ) and mass < -1.2917100 and mass > -1.3517100', #matter {split_ineq_sign} and centrality > {cent_bins[0]} and centrality < {cent_bins[1]}  and pt > {pt_bins[0]} and pt < {pt_bins[1]} and mass > 1.2917100 and mass < 1.3517100 and ct > 0. and ct < 20 and tpcClV0Pi > 69 and bachBarCosPA < 0.99995 and tpcClV0Pr > 69 and tpcClBach > 69 and radius < 51 and radiusV0 < 100 and dcaV0prPV < 12.7 and dcaV0piPV < 25 and dcaV0PV < 10.1 and dcaBachPV < 12.7 and eta < 0.8 and eta > -0.8 and not isOmega',
                            max_workers=4, model_handler=model_hdl, output_margin=False)


                        #df_data.apply_model_handler(model_hdl)
                        #df_data.apply_preselections(f'model_output > {score_eff_arrays_dict[bin][len(eff_array)-1]}')
                        df_data.write_df_to_parquet_files(bin,f"df_test{ppPbPb}/")
                else:
                    bin = f'{split}_{cent_bins[0]}_{cent_bins[1]}_{pt_bins[0]}_{pt_bins[1]}_mc_apply'
                    df_data = uproot.open(f'/data/mciacco/KXiCorrelations/tree_train/AnalysisResults_{cent_bins[0]}_{cent_bins[1]}.root')['XiOmegaTree'].arrays(library="pd")
                    df_data_cent = df_data.query(f'flag==1 and (pdg==3334 or pdg==-3334) and isReconstructed and matter {split_ineq_sign} and pt > {pt_bins[0]} and pt < {pt_bins[1]} and mass > 1.2917100 and mass < 1.3517100 and ct > 0. and ct < 20 and isReconstructed and tpcClV0Pi > 69 and bachBarCosPA < 0.99995 and tpcClV0Pr > 69 and tpcClBach > 69 and radius < 51 and radiusV0 < 100 and dcaV0prPV < 12.7 and dcaV0piPV < 25 and dcaV0PV < 10.1 and dcaBachPV < 12.7 and eta < 0.8 and eta > -0.8 and not isOmega')                   
                    data_y_score = model_hdl.predict(df_data_cent, output_margin=False)
                    print(f"df_shape_0 = {df_data_cent.shape[0]}")
                    df_data_cent.loc[:,'model_output'] = data_y_score.tolist()[:]

                    # df_data_cent = df_data_cent.query(f'model_output > {score_eff_arrays_dict[bin][len(eff_array)-1]}')
                    df_data_cent.to_parquet(f'df_test{ppPbPb}/{bin}.parquet.gzip', compression='gzip')

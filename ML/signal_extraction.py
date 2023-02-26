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
import yaml
from helpers import significance_error, ndarray2roo, purity_error
from hipe4ml import analysis_utils, plot_utils

SPLIT = False
use_crystalball = True
MAX_EFF = 1.
mass_PDG = 1.32171 # GeV/c^2

ROOT.gInterpreter.ProcessLine("#include \"../utils/RooDSCBShape.h\"")
ROOT.gInterpreter.ProcessLine(".L ../utils/RooDSCBShape.cxx+")

# avoid pandas warning
warnings.simplefilter(action='ignore', category=FutureWarning)
ROOT.gROOT.SetBatch()

parser = argparse.ArgumentParser(prog='signal_extraction', allow_abbrev=True)
parser.add_argument('-bkgExpo', action='store_true')
args = parser.parse_args()

BKG_EXPO = args.bkgExpo

##################################################################
# read configuration file
##################################################################
config = 'config.yaml'
with open(os.path.expandvars(config), 'r') as stream:
    try:
        params = yaml.full_load(stream)
    except yaml.YAMLError as exc:
        print(exc)

DATA_PATH = params['DATA_PATH']
PT_BINS = params['PT_BINS']
PT_BINS_CENT = params['PT_BINS_CENT']
CENTRALITY_LIST = [[0,90]] #params['CENTRALITY_LIST']
RANDOM_STATE = params['RANDOM_STATE']
##################################################################

USE_TEST_OUTPUT = True

# split matter/antimatter
SPLIT_LIST = ['all']
if SPLIT:
    SPLIT_LIST = ['antimatter', 'matter']

bkg_shape = 'pol1'
if BKG_EXPO:
    bkg_shape = 'expo'

score_eff_arrays_dict = pickle.load(open("file_score_eff_dict", "rb"))
eff_array = np.arange(0.1, MAX_EFF, 0.01)

for split in SPLIT_LIST:
    split_ineq_sign = '> -0.1'
    if SPLIT:
        split_ineq_sign = '> 0.5'
        if split == 'antimatter':
            split_ineq_sign = '< 0.5'
    df_signal = pd.DataFrame()

    for i_cent_bins in range(len(CENTRALITY_LIST)):
        cent_bins = CENTRALITY_LIST[i_cent_bins]
        if not USE_TEST_OUTPUT:
            df_signal = uproot.open(f"/data/mciacco/KXiCorrelations/AnalysisResults_{cent_bins[0]}_{cent_bins[1]}.root")['XiOmegaTree'].arrays(library="pd")

        for pt_ in PT_BINS:

            bin_df = f'{cent_bins[0]}_{cent_bins[1]}_{pt_[0]}_{pt_[1]}'
            print(f"bin_df = {bin_df}")
            df_data = pd.read_parquet(f'df_test/all_{bin_df}_data_apply.parquet.gzip')
            if USE_TEST_OUTPUT:
                df_signal = pd.read_parquet(f'df_test/test_data_{bin_df}_predict.parquet.gzip')
            else:
                pt_bins_df_index = int(pt_[0]/5 -1)
        
            for pt_bins in zip(PT_BINS_CENT[i_cent_bins][:-1], PT_BINS_CENT[i_cent_bins][1:]):
                if (pt_bins[0] < (pt_[0]-0.01)) or (pt_bins[1] > (pt_[1]+0.01)) or pt_bins[0]<1.:
                    continue
                bin = f'{cent_bins[0]}_{cent_bins[1]}_{pt_bins[0]}_{pt_bins[1]}'
                df_signal_pt = pd.DataFrame()
                df_signal_pt = df_signal.query(f"competingMass > 0.01 and (pdg==3312 or pdg==-3312) and pt > {pt_bins[0]} and pt < {pt_bins[1]} and mass > 1.2917100 and mass < 1.3517100 and ct > 0. and ct < 20 and isReconstructed and tpcClV0Pi > 69 and bachBarCosPA < 0.99995 and tpcClV0Pr > 69 and tpcClBach > 69 and radius < 25 and radiusV0 < 25 and dcaV0prPV < 2.5 and dcaV0piPV < 2.5 and dcaV0PV < 2.5 and dcaBachPV < 2.5 and eta < 0.8 and eta > -0.8 and flag==1 and not isOmega")
                df_signal_pt_all = df_signal.query(f"competingMass > 0.01 and (pdg==3312 or pdg==-3312) and pt > {pt_bins[0]} and pt < {pt_bins[1]} and mass > 1.2917100 and mass < 1.3517100 and ct > 0. and ct < 20 and isReconstructed and tpcClV0Pi > 69 and bachBarCosPA < 0.99995 and tpcClV0Pr > 69 and tpcClBach > 69 and radius < 25 and radiusV0 < 25 and dcaV0prPV < 2.5 and dcaV0piPV < 2.5 and dcaV0PV < 2.5 and dcaBachPV < 2.5 and eta < 0.8 and eta > -0.8 and flag==1 and not isOmega")
                df_data_pt_all = df_data.query(f"competingMass > 0.01 and centrality > {cent_bins[0]} and centrality < {cent_bins[1]} and pt > {pt_bins[0]} and pt < {pt_bins[1]} and mass > 1.2917100 and mass < 1.3517100 and ct > 0. and ct < 20 and tpcClV0Pi > 69 and bachBarCosPA < 0.99995 and tpcClV0Pr > 69 and tpcClBach > 69 and radius < 25 and radiusV0 < 25 and dcaV0prPV < 2.5 and dcaV0piPV < 2.5 and dcaV0PV < 2.5 and dcaBachPV < 2.5 and eta < 0.8 and eta > -0.8 and not isOmega")
                df_data_pt = df_data_pt_all.query(f"matter {split_ineq_sign} and competingMass > 0.01 and centrality > {cent_bins[0]} and centrality < {cent_bins[1]} and pt > {pt_bins[0]} and pt < {pt_bins[1]} and mass > 1.2917100 and mass < 1.3517100 and ct > 0. and ct < 20 and tpcClV0Pi > 69 and bachBarCosPA < 0.99995 and tpcClV0Pr > 69 and tpcClBach > 69 and radius < 25 and radiusV0 < 25 and dcaV0prPV < 2.5 and dcaV0piPV < 2.5 and dcaV0PV < 2.5 and dcaBachPV < 2.5 and eta < 0.8 and eta > -0.8 and not isOmega")

                # score-eff relation
                score_array = analysis_utils.score_from_efficiency_array(df_signal_pt["y_true"], df_signal_pt["model_output"], efficiency_selected=eff_array, keep_lower=False)

                # ROOT.Math.MinimizerOptions.SetDefaultTolerance(1e-2)
                root_file_signal_extraction = ROOT.TFile("SignalExtraction_pt.root", "update") # yields for measurement in SignalExtraction-data.root
                root_file_signal_extraction.mkdir(f'{split}_{bin}_{bkg_shape}')

                # raw yields histogram
                h_raw_yields = ROOT.TH1D("fRawYields", "fRawYields", 101, -0.005, 1.005)

                # purity histogram
                h_purity = ROOT.TH1D("fPurity", "fPurity", 101, -0.005, 1.005)

                # significance histogram
                h_significance = ROOT.TH1D("fSignificance", "fSignificance", 101, -0.005, 1.005)

                for eff_score in zip(eff_array, score_array):
                    if (pt_bins[0] > -0.5) and (eff_score[0] < .3 or eff_score[0] > .7):
                        continue
                    formatted_eff = "{:.2f}".format(eff_score[0])
                    print(f'processing {bin}: eff = {eff_score[0]:.4f}, score = {eff_score[1]:.4f}...')

                    df_data_sel_all = df_data_pt_all.query(f'model_output > {eff_score[1]}')
                    df_data_sel = df_data_pt.query(f'model_output > {eff_score[1]}')
                    df_signal_sel = pd.DataFrame()
                    if USE_TEST_OUTPUT:
                        df_signal_sel = df_signal_pt.query(f'model_output > {eff_score[1]} and y_true == 1')
                    else:
                        df_signal_sel = df_signal_pt

                    # get invariant mass distribution (data and mc)
                    roo_m = ROOT.RooRealVar("m", "#it{M} (#Lambda + #pi^{-})", 1.30, 1.35, "GeV/#it{c}^{2}")
                    roo_data_unbinned_all = ndarray2roo(np.array(df_data_sel_all['mass']), roo_m)
                    roo_data_unbinned = ndarray2roo(np.array(df_data_sel['mass']), roo_m)
                    roo_mc_signal = ndarray2roo(np.array(df_signal_sel['mass']), roo_m)
                    roo_m.setBins(100)
                    roo_data_all = ROOT.RooDataHist('data','data',ROOT.RooArgSet(roo_m),roo_data_unbinned_all)
                    roo_data = ROOT.RooDataHist('data','data',ROOT.RooArgSet(roo_m),roo_data_unbinned)
                    roo_mc_signal = ROOT.RooDataHist('data','data',ROOT.RooArgSet(roo_m),roo_mc_signal)

                    # declare fit model
                    # kde
                    roo_n_signal = ROOT.RooRealVar('N_{signal}', 'Nsignal', 1., 1.e8)
                    #roo_signal = ROOT.RooKeysPdf("signal", "signal", shifted_mass, roo_m,
                            #                  roo_mc_signal, ROOT.RooKeysPdf.NoMirror, 2)

                    # fit mc distribution with dscb
                    mass_mc = ROOT.RooRealVar('mass_','mass_',1.31, 1.335)
                    sigma_left_mc = ROOT.RooRealVar('#sigma','sigma',0.001,0.003)
                    sigma_right_mc = ROOT.RooRealVar('sigma_right','sigma_right',0.001,0.003)
                    alpha_left_mc = ROOT.RooRealVar('alpha_left','alpha_left',0.,5.)
                    alpha_right_mc = ROOT.RooRealVar('alpha_right','alpha_right',0.,5.)
                    n_left_mc = ROOT.RooRealVar('n_left','n_left',1.,30.)
                    n_right_mc = ROOT.RooRealVar('n_right','n_right',1.,30.)
                    roo_signal_mc = ROOT.RooDSCBShape('signal_','signal_',roo_m,mass_mc,sigma_left_mc,alpha_left_mc,n_left_mc,alpha_right_mc,n_right_mc) 
                    roo_signal_plot = ROOT.RooDSCBShape(roo_signal_mc)
                    roo_m.setRange("signal_range",1.30,1.35)
                    for _ in range(2):
                        #roo_signal_mc.fitTo(roo_data_all,ROOT.RooFit.Range("signal_range"))
                        pass
                    # cb signal
                    # alpha_left_mc.setConstant()
                    # alpha_right_mc.setConstant()
                    # n_left_mc.setConstant()
                    # n_right_mc.setConstant()
                    # sigma_left_mc.setConstant()
                    # sigma_right_mc.setConstant()
                    #roo_m.setBins(45)
                    mass = ROOT.RooRealVar('#it{m}_{#Xi}','mass',1.31, 1.33)
                    # sigma_left = ROOT.RooRealVar('sigma','sigma',0.,0.005)
                    # sigma_right = ROOT.RooRealVar('sigma_right','sigma_right',0.,0.005)
                    # alpha_left = ROOT.RooRealVar('alpha_left','alpha_left',0.,3.)
                    # alpha_right = ROOT.RooRealVar('alpha_right','alpha_right',0.,3.)
                    # n_left = ROOT.RooRealVar('n_left','n_left',0.,15.)
                    # n_right = ROOT.RooRealVar('n_right','n_right',0.,15.)
                    roo_signal = ROOT.RooDSCBShape('signal','signal',roo_m,mass,sigma_left_mc,alpha_left_mc,n_left_mc,alpha_right_mc,n_right_mc) 
                    #roo_signal = ROOT.RooDSCBShape('signal','signal',roo_m,mass,sigma_left,alpha_left,n_left,alpha_right,n_right)      
                    roo_signal_copy = ROOT.RooDSCBShape(roo_signal)

                    # background
                    roo_n_background = ROOT.RooRealVar('N_{bkg}', 'Nbackground', 1., 1.e8)
                    roo_a = ROOT.RooRealVar('a', 'a', -10., 10.)
                    roo_b = ROOT.RooRealVar('b', 'b', -10., 10.)
                    roo_slope = ROOT.RooRealVar('slope', 'slope', -20., 20.)
                    roo_bkg = ROOT.RooRealVar()

                    if not BKG_EXPO:
                        roo_bkg = ROOT.RooPolynomial('background', 'background', roo_m, ROOT.RooArgList(roo_a))
                    else:
                        roo_bkg = ROOT.RooExponential('background', 'background', roo_m, roo_slope)

                    # model
                    roo_model = ROOT.RooAddPdf(
                        'model', 'model', ROOT.RooArgList(roo_signal, roo_bkg),
                        ROOT.RooArgList(roo_n_signal, roo_n_background))

                    # fit
                    ROOT.RooMsgService.instance().setGlobalKillBelow(ROOT.RooFit.ERROR)
                    ROOT.RooMsgService.instance().setSilentMode(ROOT.kTRUE)
                    ROOT.gErrorIgnoreLevel = ROOT.kError
                    roo_m.setRange("left",1.30, 1.31)
                    roo_m.setRange("right",1.335, 1.35)
                    # for _ in range(2):
                    #     roo_bkg.fitTo(roo_data_all, ROOT.RooFit.Save(),ROOT.RooFit.Range("left,right"))
                    # # roo_a.setConstant()
                    # # roo_b.setConstant()
                    # # roo_c.setConstant()
                    # for _ in range(2):
                    #     roo_model.fitTo(roo_data_all, ROOT.RooFit.Save(), ROOT.RooFit.Extended(ROOT.kTRUE))
                    # roo_a.setConstant(False)
                    # roo_b.setConstant(False)
                    # roo_c.setConstant(False)
                    # alpha_left_mc.setConstant()
                    # alpha_right_mc.setConstant()
                    # n_left_mc.setConstant()
                    # n_right_mc.setConstant()
                    # sigma_left_mc.setConstant()
                    # sigma_right_mc.setConstant()
                    # roo_a.setConstant()
                    # roo_b.setConstant()
                    #roo_b.setConstant()
                    # for _ in range(2):
                    #     roo_bkg.fitTo(roo_data, ROOT.RooFit.Save(), ROOT.RooFit.Range("left,right"))
                    # roo_a.setConstant()
                    # roo_b.setConstant()
                    # roo_c.setConstant()
                    for _ in range(4):
                        roo_model.fitTo(roo_data, ROOT.RooFit.Save(), ROOT.RooFit.Extended(ROOT.kTRUE))
                    r = roo_model.fitTo(roo_data, ROOT.RooFit.Save(), ROOT.RooFit.Extended(ROOT.kTRUE))
                    #roo_a.setConstant(False)
                    #roo_b.setConstant(False)
                    # roo_c.setConstant(False)
                    # if (i_cent_bins==0 or i_cent_bins==4):
                    #     alpha_left_mc.setConstant()
                    #     alpha_right_mc.setConstant()
                    #     n_left_mc.setConstant()
                    #     n_right_mc.setConstant()
                    #sigma_left_mc.setConstant()
                    # sigma_right_mc.setConstant()
                    #sigma_right_mc.setConstant()
                    # sigma_left_mc.setConstant(False)
                    # sigma_right_mc.setConstant(False)
                    print(f'fit status: {r.status()}')
                    if r.covQual() > 2.5: # and delta_mass.getError() > 1.e-6:

                        # plot
                        nBins = 100
                        xframe = roo_m.frame(1.30, 1.35, nBins)
                        xframe.SetTitle(
                            str(pt_bins[0]) + '#leq #it{c}t<' + str(pt_bins[1]) + ' cm, ' + str(cent_bins[0]) + '-' +
                            str(cent_bins[1]) + '%, BDT efficiency = ' + str(formatted_eff))
                        xframe.SetName(f'fInvMass_{formatted_eff}')
                        roo_data.plotOn(xframe, ROOT.RooFit.Name('data'))
                        roo_model.plotOn(
                            xframe, ROOT.RooFit.Components('background'),
                            ROOT.RooFit.Name('background'),
                            ROOT.RooFit.LineStyle(ROOT.kDashed),
                            ROOT.RooFit.LineColor(ROOT.kGreen))
                        roo_model.plotOn(xframe, ROOT.RooFit.Components('signal'), ROOT.RooFit.Name('signal'),
                                        ROOT.RooFit.LineStyle(ROOT.kDashed), ROOT.RooFit.LineColor(ROOT.kRed))
                        roo_model.plotOn(xframe, ROOT.RooFit.Name('model'), ROOT.RooFit.LineColor(ROOT.kBlue))

                        formatted_chi2 = "{:.2f}".format(xframe.chiSquare('model', 'data'))
                        roo_model.paramOn(xframe, ROOT.RooFit.Parameters(ROOT.RooArgSet(roo_m,mass,sigma_left_mc,roo_n_signal,roo_n_background,roo_a)),ROOT.RooFit.Label(
                            '#chi^{2}/NDF = '+formatted_chi2),
                            ROOT.RooFit.Layout(0.60, 0.85, 0.88))
                        xframe.getAttText().SetTextFont(44)
                        xframe.getAttText().SetTextSize(20)
                        xframe.getAttLine().SetLineWidth(0)

                        print(f'chi2/NDF: {formatted_chi2}, edm: {r.edm()}')
                        chi2_max = 2.
                        if float(formatted_chi2) < chi2_max: # and r.edm() < 1:

                            # fit mc distribution to get sigma and mass
                            roo_mean_mc = ROOT.RooRealVar("mean", "mean", 1.30, 1.35)
                            roo_sigma_mc = ROOT.RooRealVar("sigma", "sigma", 0.0001, 0.0040)
                            gaus = ROOT.RooGaussian('gaus', 'gaus', roo_m, roo_mean_mc, roo_sigma_mc)
                            gaus.fitTo(roo_data)

                            # mass
                            mass_val = mass.getVal()

                            # significance
                            m_set = ROOT.RooArgSet(roo_m)
                            normSet = ROOT.RooFit.NormSet(m_set)
                            roo_m.setRange(
                                'signalRange', mass.getVal() - 3*sigma_left_mc.getVal(),
                                mass.getVal() + 3*sigma_left_mc.getVal())
                            signal_int = (roo_model.pdfList().at(0).createIntegral(
                                m_set, normSet, ROOT.RooFit.Range("signalRange"))).getVal()
                            print(f'signal integral = {signal_int}')
                            bkg_int = (roo_model.pdfList().at(1).createIntegral(
                                m_set, normSet, ROOT.RooFit.Range("signalRange"))).getVal()
                            print(f'background integral = {bkg_int}')
                            sig = signal_int*roo_n_signal.getVal()
                            bkg = bkg_int*roo_n_background.getVal()
                            significance_val = sig/np.sqrt(sig+bkg)
                            significance_err = significance_error(sig, bkg)
                            purity_val = sig/(sig+bkg)
                            purity_err = purity_error(sig, bkg)

                            # fill significance histogram
                            eff_index = h_significance.FindBin(float(formatted_eff))
                            h_significance.SetBinContent(eff_index, significance_val)
                            h_significance.SetBinError(eff_index, significance_err)
                            h_purity.SetBinContent(eff_index, purity_val)
                            h_purity.SetBinError(eff_index, purity_err)

                            if significance_val > 2.95:
                                # fill raw yields histogram
                                n_tot = roo_data.sumEntries("m>1.31 && m<1.333")
                                m_set = ROOT.RooArgSet(roo_m)
                                normSet = ROOT.RooFit.NormSet(m_set)
                                roo_m.setRange(
                                    'signalRange_tot', 1.30, 1.35)
                                bkg_int_tot = (roo_model.pdfList().at(1).createIntegral(
                                    m_set, normSet, ROOT.RooFit.Range("signalRange_tot"))).getVal()
                                n_bkg = bkg_int_tot*roo_n_background.getVal()
                                h_raw_yields.SetBinContent(eff_index, roo_n_signal.getVal())
                                h_raw_yields.SetBinError(eff_index, roo_n_signal.getError())
                                #h_raw_yields.SetBinContent(eff_index, n_tot-n_bkg)
                                #h_raw_yields.SetBinError(eff_index, np.sqrt(n_tot+n_bkg))
                                
                                # write to file
                                root_file_signal_extraction.cd(f'{split}_{bin}_{bkg_shape}')
                                xframe.Write()

                                # draw on canvas and save plots
                                canv = ROOT.TCanvas()
                                canv.cd()
                                text_mass = ROOT.TLatex(
                                    1.302, 0.70 * xframe.GetMaximum(),
                                    "#it{m}_{#Xi} = " + "{:.6f}".format(mass_val) + " GeV/#it{c^{2}}")
                                text_mass.SetTextFont(44)
                                text_mass.SetTextSize(20)
                                text_signif = ROOT.TLatex(1.302, 0.91 * xframe.GetMaximum(),
                                                        "S/#sqrt{S+B} (3#sigma) = " + "{:.3f}".format(significance_val) + " #pm " +
                                                        "{:.3f}".format(significance_err))
                                text_signif.SetTextFont(44)
                                text_signif.SetTextSize(20)
                                text_sig = ROOT.TLatex(1.302, 0.84 * xframe.GetMaximum(), "S (3#sigma) = " + "{:.1f}".format(sig) + " #pm " + "{:.1f}".format(signal_int*roo_n_signal.getError()))
                                text_sig.SetTextFont(44)
                                text_sig.SetTextSize(20)
                                text_bkg = ROOT.TLatex(1.302, 0.77 * xframe.GetMaximum(), "B (3#sigma) = " + "{:.1f}".format(bkg) + " #pm" + "{:.1f}".format(bkg_int*roo_n_background.getError()))
                                text_bkg.SetTextFont(44)
                                text_bkg.SetTextSize(20)
                                text_purity = ROOT.TLatex(1.302, 0.63 * xframe.GetMaximum(), "S/(S+B) = " + "{:.3f}".format(purity_val) + " #pm" + "{:.3f}".format(purity_err))
                                text_purity.SetTextFont(44)
                                text_purity.SetTextSize(20)
                                xframe.Draw("")
                                text_mass.Draw("same")
                                lvert = ROOT.TLine(mass.getVal() - 3*sigma_left_mc.getVal(), 0, mass.getVal() - 3*sigma_left_mc.getVal(), 0.5 * xframe.GetMaximum())
                                rvert = ROOT.TLine(mass.getVal() + 3*sigma_left_mc.getVal(), 0, mass.getVal() + 3*sigma_left_mc.getVal(), 0.5 * xframe.GetMaximum())
                                lvert.SetLineWidth(1)
                                rvert.SetLineWidth(1)
                                lvert.SetLineStyle(ROOT.kDashed)
                                rvert.SetLineStyle(ROOT.kDashed)
                                text_signif.Draw("same")
                                text_sig.Draw("same")
                                text_bkg.Draw("same")
                                text_purity.Draw("same")
                                lvert.Draw("same")
                                rvert.Draw("same")
                                print(
                                    f'significance = {"{:.3f}".format(significance_val)} +/- {"{:.3f}".format(significance_err)}')
                                if not os.path.isdir('plots/signal_extraction_extend'):
                                    os.mkdir('plots/signal_extraction_extend')
                                if not os.path.isdir(f'plots/signal_extraction_extend/{split}_{bin}_{bkg_shape}'):
                                    os.mkdir(f'plots/signal_extraction_extend/{split}_{bin}_{bkg_shape}')
                                canv.Print(f'plots/signal_extraction_extend/{split}_{bin}_{bkg_shape}/{eff_score[0]:.2f}_{bin}.pdf')

                                # plot kde and mc
                                frame = roo_m.frame(1.30, 1.35, 100)
                                frame.SetTitle(str(cent_bins[0])+"-"+str(cent_bins[1])+"%, "+str(pt_bins[0])+"#leq #it{c}t<"+str(pt_bins[1])+" cm, BDT efficiency = "+str(formatted_eff))
                                roo_mc_signal.plotOn(frame)
                                roo_signal_plot.plotOn(frame, ROOT.RooFit.Name("DSCB"))
                                gaus.plotOn(frame, ROOT.RooFit.Name("gaussian"), ROOT.RooFit.LineColor(ROOT.kRed), ROOT.RooFit.LineStyle(ROOT.kDashed))
                                cc = ROOT.TCanvas("cc", "cc")
                                if not os.path.isdir('plots/kde_signal'):
                                    os.mkdir('plots/kde_signal')
                                if not os.path.isdir(f'plots/kde_signal/{split}_{bin}'):
                                    os.mkdir(f'plots/kde_signal/{split}_{bin}')
                                frame.Draw()
                                leg_mc = ROOT.TLegend(0.7, 0.8, 0.85, 0.7)
                                leg_mc.AddEntry(frame.findObject("DSCB"), "DSCB")
                                leg_mc.AddEntry(frame.findObject("gaussian"), "Gaussian")
                                leg_mc.SetTextFont(44)
                                leg_mc.SetTextSize(20)
                                leg_mc.SetBorderSize(0)
                                leg_mc.Draw("same")
                                cc.SetLogy(ROOT.kTRUE)
                                cc.Write()
                                cc.Print(f'plots/kde_signal/{split}_{bin}/{formatted_eff}_{bin}.pdf')

                h_raw_yields.GetXaxis().SetTitle("BDT efficiency")
                h_raw_yields.GetYaxis().SetTitle("#it{N_{raw}}")
                h_raw_yields.Write()

                h_purity.GetXaxis().SetTitle("BDT efficiency")
                h_purity.GetYaxis().SetTitle("S / (S + B)")
                h_purity.Write()

                h_significance.GetXaxis().SetTitle("BDT efficiency")
                h_significance.GetYaxis().SetTitle("S / #sqrt{S + B}")
                h_significance.Write()

                root_file_signal_extraction.Close()

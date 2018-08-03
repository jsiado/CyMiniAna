"""
Created:        3  September 2016
Last Updated:   9  March     2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University
-----

Steering script for making simple histograms.
Primarily want to do this from root histograms (faster to make those in c++)

This can be modified or extended by whomever.

To run:
 python python/runHistogram.py --files <files.txt> --hists <histogramNames.txt> -o <output_path>
"""
import sys
import ROOT
from argparse import ArgumentParser

import util
from Analysis.CyMiniAna.hepPlotter.hepPlotter import HepPlotter
import Analysis.CyMiniAna.hepPlotter.hepPlotterTools as hpt
import Analysis.CyMiniAna.hepPlotter.hepPlotterLabels as hpl


parser = ArgumentParser(description="Histogram Plotter")

parser.add_argument('-f','--files', action='store',default=None,
                    dest='listOfFiles',
                    help='Name of file that contains root files to plot')
parser.add_argument('--hists', action='store',default=None,
                    dest='listOfHists',
                    help='Name of file that contains histograms to plot')
parser.add_argument('-o','--outpath', action='store',default='.',
                    dest='outpath',
                    help='Directory for storing output plots')
parser.add_argument('-t','--selection', action='store',default='events',
                    dest='selection',
                    help='Treename used for making histograms')
results = parser.parse_args()


outpath    = results.outpath
files      = util.file2list(results.listOfFiles)
histograms = util.file2list(results.listOfHists)
selection   = results.selection
betterColors  = hpt.betterColors()['linecolors']
variables     = hpl.variable_labels()
sample_labels = hpl.sample_labels()
metadata = util.loadMetadata("config/sampleMetaData.txt")  # dictionary of metadata; key=primary dataset

#numberOfHists = 0
# Access data -- assumes you are plotting histograms from multiple sources in one figure

for hi,histogram in enumerate(histograms):

    histogramName = histogram[2:].replace("_"+selection,"") # turn "h_jet_pt_events" into "jet_pt"

    print "  :: Plotting "+histogram+"\n"

    ## setup histogram
    hist = HepPlotter("histogram",1)

    hist.ratio_plot  = False       # plot a ratio of things [Data/MC]
    hist.ratio_type  = "ratio"     # "ratio"
    hist.stacked     = False        # stack plots
    hist.rebin       = variables[histogramName].binning
    hist.logplot     = False       # plot on log scale
    hist.x_label     = variables[histogramName].label
    hist.y_label     = "Events"
    hist.y_ratio_label = ""
    hist.lumi          = 'XY.Z'   # in /fb
    hist.format        = 'pdf'       # file format for saving image
    hist.saveAs        = "{0}/hist_{1}".format(outpath,histogramName) # save figure with name
    hist.CMSlabel       = 'top left'  # 'top left', 'top right'; hack code for something else
    hist.CMSlabelStatus = 'Internal'  # ('Simulation')+'Internal' || 'Preliminary' 
    hist.normed         = True

    hist.initialize()
    #file 
    ## Add the data from each file
   #for fi,file in enumerate(files):
    #plotting ttbar
    h_ttbar  = ROOT.TFile.Open('root://cmseos.fnal.gov//store/user/lpctlbsm/wprime-qQ/CyMiniAna/myfiles/ttbarAC.root')
    pd = util.getPrimaryDataset(h_ttbar)
    name = metadata[pd].sampleType      # compare primary dataset with metadatafile 
    if name == "signal":
        name = pd.split("_")[1]
        print "     > Opening data from ",h_ttbar
    h_hist = getattr(h_ttbar,histogram)       # retrieve the histogram
    hist.Add(h_hist,name=name+"_"+histogram,linecolor=betterColors[2],
                 draw='step',label=sample_labels[name].label)
    #numberOfHists+=1
    plot = hist.execute()
    #hist.savefig()
    print "  :: Saved plot to "+hist.saveAs+"\n"
    
    #plotting signal
    h_signal_w25NT12NL  = ROOT.TFile.Open('root://cmseos.fnal.gov//store/user/lpctlbsm/wprime-qQ/CyMiniAna/myfiles/signal_mujets-ejets_W25N_T12N_LH.root')
    pd = util.getPrimaryDataset(h_signal_w25NT12NL)
    name = metadata[pd].sampleType      # compare primary dataset with metadatafile        
    if name == "signal":
        name = pd.split("_")[1]
        print "     > Opening data from ",h_signal_w25NT12NL
    h_hist = getattr(h_signal_w25NT12NL,histogram)       # retrieve the histogram
    hist.Add(h_hist,name=name+"_"+histogram,linecolor=betterColors[1],
                 draw='step',label=sample_labels[name].label)
    #numberOfHists+=1

    plot = hist.execute()
    hist.savefig()
    print "  :: Saved plot to "+hist.saveAs+"\n"

## THE END

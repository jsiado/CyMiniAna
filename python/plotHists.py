"""
Created:         1 February 2018
Last Updated:    1 February 2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University
-----
Steering script for making simple histograms.
Primarily want to do this from root histograms (faster to make those in c++)

This can be modified or extended by whomever.

To run:
python python/hist.py
"""
import os
import sys
import ROOT
from array import array

import util
from Analysis.CyMiniAna.hepPlotter.hepPlotter import HepPlotter
import Analysis.CyMiniAna.hepPlotter.hepPlotterTools as hpt
import Analysis.CyMiniAna.hepPlotter.hepPlotterLabels as hpl


def gethistname(h):
    h = h.replace("h_","").replace("_ejets","").replace("_mujets","")
    return h

cmaDir   = os.environ["CMSSW_BASE"]+"/src/Analysis/CyMiniAna"
metadata = util.loadMetadata(cmaDir+"/config/sampleMetaData.txt")  # dictionary of metadata; key=primary dataset

ttbar_files   = util.file2list("config/samples_cyminiana/listOfTtbarFiles.txt")
ttbar_files  += util.file2list("config/samples_cyminiana/listOfTtbarExtFiles.txt")
signal_files  = util.file2list("config/samples_cyminiana/listOfWp1500NarTp1200NarLHFiles.txt")

outpath  = 'plots/b2g-workshop'
x_labels = hpl.variable_labels()
sample_labels = hpl.sample_labels()

selections = ['mujets','ejets']
histograms = util.file2list("config/listOfHists.txt")
histograms = [i.format(sel) for sel in selections for i in histograms]

## Add the data from each file
h_hist_ttbar  = {}
h_hist_signal = {}

for h in histograms:
    h_hist_ttbar[h]  = None
    h_hist_signal[h] = None


print " > ttbar "
ttbar_pd   = None
ttbar_name = ''
for fi,file in enumerate(ttbar_files):
    file = file.rstrip("\n")
    f = ROOT.TFile.Open(file)
    filename = file.split("/")[-1].split(".")[0]

    if fi==0:
        ttbar_pd   = util.getPrimaryDataset(f)
        ttbar_name = metadata[ttbar_pd].sampleType      # compare primary dataset with metadatafile

    for h in histograms:
        if h_hist_ttbar[h] is None:
            h_hist_ttbar[h] = getattr(f,h)       # retrieve the histogram
            h_hist_ttbar[h].SetDirectory(0)
        else:
            h_temp = getattr(f,h)
            h_temp.SetDirectory(0)
            h_hist_ttbar[h].Add( h_temp )

print " > signal "
wprime_pd   = None
wprime_name = 'wprime'
for fi,file in enumerate(signal_files):
    file = file.rstrip("\n")
    f = ROOT.TFile.Open(file)
    filename = file.split("/")[-1].split(".")[0]

    if fi==0:
        wprime_pd = util.getPrimaryDataset(f)
        wprime_name = metadata[wprime_pd].sampleType      # compare primary dataset with metadatafile

    for h in histograms:
        if h_hist_signal[h] is None:
            h_hist_signal[h] = getattr(f,h)       # retrieve the histogram
            h_hist_signal[h].SetDirectory(0)
        else:
            h_temp = getattr(f,h)
            h_temp.SetDirectory(0)
            h_hist_signal[h].Add( h_temp )



# Access data -- assumes you are plotting histograms from multiple sources in one figure
for _ in range(1):
    for hi,histogram in enumerate(histograms):

        if histogram.startswith("h_mu_") and histogram.endswith("ejets"): continue
        if histogram.startswith("h_el_") and histogram.endswith("mujets"): continue

        histogram = histogram.strip('\n')
        histogramname = gethistname(histogram)
        print "  :: Plotting "+histogram

        ## setup histogram
        hist = HepPlotter("histogram",1)

        hist.ratio_plot  = True       # plot a ratio of things [e.g., Data/MC]
        hist.ratio_type  = "significance"
        hist.stacked     = False      # stack plots
        hist.normed      = True
        hist.rebin       = x_labels[histogramname].binning
        hist.xlim        = None
        hist.logplot     = False       # plot on log scale
        hist.x_label     = x_labels[histogramname].label
        hist.y_label     = "Arbitrary Units"
        hist.y_ratio_label = r"S/$\sqrt{\text{B}}$"
        hist.lumi          = ''   # in /fb
        hist.format        = 'pdf'       # file format for saving image
        hist.saveAs        = outpath+"/"+histogram # save figure with name
        hist.CMSlabel       = 'top left'  # 'top left', 'top right'; hack code for something else
        hist.CMSlabelStatus = 'Internal'  # ('Simulation')+'Internal' || 'Preliminary' 
        hist.numLegendColumns = 1
        if histogram.endswith('mujets'):
            hist.extra_text.Add(sample_labels['mujets'].label,coords=[0.03,0.9])
        elif histogram.endswith('ejets'):
            hist.extra_text.Add(sample_labels['ejets'].label,coords=[0.03,0.9])

        hist.initialize()


        hist.Add(h_hist_ttbar[histogram],name="ttbar_"+histogram,linecolor='red',
                 draw='step',label=sample_labels['ttbar'].label,ratio_den=True,ratio_partner='wprime_'+histogram)

        hist.Add(h_hist_signal[histogram],name="wprime_"+histogram,linecolor='blue',color='blue',
                 draw='step',label=sample_labels[wprime_pd.split("_")[1]].label,ratio_num=True,ratio_partner='ttbar_'+histogram)


        plot = hist.execute()
        hist.savefig()
        print "  :: Saved plot to "+hist.saveAs+"\n"

## THE END ##

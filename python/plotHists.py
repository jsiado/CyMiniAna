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



def getHistograms(files,histograms):
    """Aggregate histograms from many files"""
    pd    = ''
    name  = ''
    hists = {}

    for fi,file in enumerate(files):
        f = ROOT.TFile.Open(file)

        if fi==0:
            pd   = util.getPrimaryDataset(f)
            name = metadata[pd].sampleType     # compare primary dataset with metadatafile

        for h in histograms:
            try:
                h_temp = getattr(f,h)
                h_temp.SetDirectory(0)
                hists[h].Add( h_temp )
            except KeyError:
                hists[h] = getattr(f,h)        # retrieve the histogram
                hists[h].SetDirectory(0)

    return {"hists":hists,"primaryDataset":pd,"name":name}


ttbar_files   = util.file2list("config/samples_cyminiana/listOfTtbarFiles.txt")
ttbar_files  += util.file2list("config/samples_cyminiana/listOfTtbarExtFiles.txt")
signal1_files  = util.file2list("config/samples_cyminiana/listOfWp1500NarTp1200NarLHFiles.txt")
signal2_files  = util.file2list("config/samples_cyminiana/listOfWp2500NarTp1200NarLHFiles.txt")

outpath  = 'plots/b2g-workshop'
x_labels = hpl.variable_labels()
sample_labels = hpl.sample_labels()

selections = ['mujets','ejets']
histograms = util.file2list("config/listOfHists.txt")
histograms = [i.format(sel) for sel in selections for i in histograms]

## Add the data from each file
print " > ttbar "
h_ttbar      = getHistograms(ttbar_files,histograms)
h_hist_ttbar = h_ttbar['hists']
ttbar_pd     = h_ttbar['primaryDataset']
ttbar_name   = h_ttbar['name']
ttbar_label  = sample_labels['ttbar'].label

print " > signal 1 "
h_signal1      = getHistograms(signal1_files,histograms)
h_hist_signal1 = h_signal1['hists']
wprime1_pd     = h_signal1['primaryDataset']
wprime1_name   = h_signal1['name']
wprime1_label  = sample_labels[wprime1_pd.split("_")[1]].label
wprime1_label_short = r"W$^\prime$(1.5)"

print " > signal 2 "
h_signal2      = getHistograms(signal2_files,histograms)
h_hist_signal2 = h_signal2['hists']
wprime2_pd     = h_signal2['primaryDataset']
wprime2_name   = h_signal2['name']
wprime2_label  = sample_labels[wprime2_pd.split("_")[1]].label
wprime2_label_short = r"W$^\prime$(2.5)"

# Access data -- assumes you are plotting histograms from multiple sources in one figure
for hi,histogram in enumerate(histograms):

    selection = "ejets" if histogram.endswith("ejets") else "mujets"

    if histogram.startswith("h_mu_") and selection=="ejets": continue
    if histogram.startswith("h_el_") and selection=="mujets": continue

    histogram = histogram.strip('\n')
    histogramname = gethistname(histogram)
    print "  :: Plotting "+histogram

    ## setup histogram
    hist = HepPlotter("histogram",1)

    hist.ratio_plot  = True       # plot a ratio of things [e.g., Data/MC]
    hist.ratio_type  = "significance"
    hist.ymaxScale   = 1.6
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
    hist.saveAs        = outpath+"/{0}_multiSignal".format(histogram) # save figure with name
    hist.CMSlabel       = 'top left'  # 'top left', 'top right'; hack code for something else
    hist.CMSlabelStatus = 'Simulation Internal'  # ('Simulation')+'Internal' || 'Preliminary' 
    hist.numLegendColumns = 1

    if selection=='mujets':
        hist.extra_text.Add(sample_labels['mujets'].label,coords=[0.03,0.9])
    elif selection=='ejets':
        hist.extra_text.Add(sample_labels['ejets'].label,coords=[0.03,0.9])

    # calculate separations
    signal1 = h_hist_signal1[histogram].Clone() 
    signal2 = h_hist_signal2[histogram].Clone()
    tt      = h_hist_ttbar[histogram].Clone()
    tt.Scale(1./tt.Integral())
    signal1.Scale(1./signal1.Integral())
    signal2.Scale(1./signal2.Integral())
    sep_tt_wprime1 = util.getHistSeparation(signal1,tt)
    sep_tt_wprime2 = util.getHistSeparation(signal2,tt)
    hist.extra_text.Add("Separation({0},{1}) = {2:.2f}".format(ttbar_label,wprime1_label_short,sep_tt_wprime1), coords=[0.03,0.8])
    hist.extra_text.Add("Separation({0},{1}) = {2:.2f}".format(ttbar_label,wprime2_label_short,sep_tt_wprime2), coords=[0.03,0.7])

    hist.initialize()

    hist.Add(h_hist_ttbar[histogram],name="ttbar_"+histogram,linecolor='red',
             draw='step',label=ttbar_label,ratio_den=True,ratio_partner=['wprime1_'+histogram,'wprime2_'+histogram])

    # 1 signal sample
    hist.Add(h_hist_signal1[histogram],name="wprime1_"+histogram,linecolor='blue',color='blue',
             draw='step',label=wprime1_label,ratio_num=True,ratio_partner='ttbar_'+histogram)

    # 2 signal sample
    hist.Add(h_hist_signal2[histogram],name="wprime2_"+histogram, linecolor='green',color='green',
             draw='step',label=wprime2_label,ratio_num=True,ratio_partner='ttbar_'+histogram)

    plot = hist.execute()
    hist.savefig()
    print "  :: Saved plot to "+hist.saveAs+"\n"

## THE END ##

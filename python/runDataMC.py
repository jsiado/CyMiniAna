"""
Created:         1 September 2016
Last Updated:   16 March     2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University
-----

Steering script for making Data/MC plots.
Primarily want to do this from histograms (faster to make those in C++ & 
we feed histograms into the limit-setting framework).

This can be modified or extended by whomever.

To run:
 python python/runDataMC.py --files <files.txt> --hists <histogramNames.txt> -o <output_path>
"""
import os
import sys
import ROOT
from array import array
from argparse import ArgumentParser

import util
from Analysis.CyMiniAna.hepPlotter.hepPlotterDataMC import HepPlotterDataMC
from Analysis.CyMiniAna.hepPlotter.hepPlotterSystematics import HepPlotterSystematics
import Analysis.CyMiniAna.hepPlotter.hepPlotterTools as hpt
import Analysis.CyMiniAna.hepPlotter.hepPlotterLabels as hpl

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
            print file
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



parser = ArgumentParser(description="DataMC Plotter")

parser.add_argument('--hists', action='store',dest='listOfHists',
                    default='config/listOfHists.txt',
                    help='Name of file that contains histograms to plot')
parser.add_argument('--systs', action='store',dest='listOfSysts',
                    default='share/listOfSytsDataMC.txt',
                    help='Name of file that contains detector systematics to plot')
parser.add_argument('-o','--outpath', action='store',default='plots/datamc/',
                    dest='outpath',
                    help='Directory for storing output plots')
results = parser.parse_args()


detectorSystematics = [] 
outpath    = results.outpath
samples    = hpl.sample_labels()     # labels and binnings for samples
variables  = hpl.variable_labels()   # labels and binnings for variables
selections = ['ejets','mujets']
histograms = util.file2list(results.listOfHists)
histograms = [i.format(sel) for sel in selections for i in histograms]

# Load information
ttbar_files   = util.file2list("config/samples_cyminiana/listOfTtbarFiles.txt")
ttbar_files  += util.file2list("config/samples_cyminiana/listOfTtbarExtFiles.txt")
signal_files  = util.file2list("config/samples_cyminiana/listOfWp1500NarTp1200NarLHFiles.txt")

#wjets
#zjets
#singletop
#diboson
ttbar_files = util.file2list("config/samples_cyminiana/listOfTtbarFiles.txt") + util.file2list("config/samples_cyminiana/listOfTtbarExtFiles.txt")
ejets_files  = []
mujets_files = []
for d in ['B','C','D','E','F','G','H','Hv3']:
    ejets_files  += util.file2list("config/samples_cyminiana/listOfSingleElectron{0}Files.txt".format(d))
    mujets_files += util.file2list("config/samples_cyminiana/listOfSingleMuon{0}Files.txt".format(d))


h_ttbar = getHistograms(ttbar_files,histograms)
h_ejets = getHistograms(ejets_files,histograms)
h_mujets = getHistograms(mujets_files,histograms)
h_data   = {'ejets':h_ejets,'mujets':h_mujets}

## Make histograms
for histogram in histograms:

    histogram = histogram.rstrip('\n')
    selection = 'ejets' if histogram.endswith('ejets') else 'mujets'

    # CyMiniAna saves histograms with extra text in the name, remove that here
    histogramName = histogram.replace("h_","").replace("_"+selection,"")

    print "  :: Plotting "+histogram+"\n"

    ## setup histogram
    hist = HepPlotterDataMC()

    hist.drawStatUncertainty = True      # draw stat uncertainty separately
    hist.drawSystUncertainty = False     # draw syst uncertainty separately
    hist.drawStatSystUncertainty = False # draw stat+syst uncertainty
    hist.legendLoc   = 1
    hist.stackSignal = False
    hist.blind       = False
    hist.xlim        = None
    hist.ratio_plot  = True        # plot a ratio of things [Data/MC]
    hist.ratio_type  = "ratio"     # "ratio"
    hist.stacked     = True        # stack backgrounds
    hist.rebin       = variables[histogramName].binning # rebin per histogram
    hist.logplot     = False
    hist.x_label     = variables[histogramName].label
    hist.y_label     = "Events"
    hist.lumi        = '35.9'
    hist.CMSlabel    = 'top left'  # 'top left', 'top right'; hack code for something else
    hist.CMSlabelStatus   = 'Internal'  # ('Simulation')+'Internal' || 'Preliminary' 
    hist.numLegendColumns = 1
    hist.y_ratio_label    = "Data/Pred."
    hist.format           = 'pdf'       # file format for saving image
    hist.saveAs           = outpath+"/datamc_"+histogramName # save figure with name

    hist.initialize()

    # ttbar
    hist.Add(h_ttbar['hists'], name=h_ttbar['name'], sampleType='background')

    # data
    hist.Add(h_data[selection]['hists'], name='data', sampleType='data')

    # make the plot
    plot = hist.execute()
    hist.savefig()
    print "  :: Saved plot to "+hist.saveAs+"\n"


## THE END

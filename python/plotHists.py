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
import sys
import ROOT
from array import array

from Analysis.CyMiniAna.hepPlotter.hepPlotter import HepPlotter
import Analysis.CyMiniAna.hepPlotter.hepPlotterTools as hpt
import Analysis.CyMiniAna.hepPlotter.hepPlotterLabels as hpl


ttbar_files = open("config/ttbar_files.txt","r").readlines()
data_files  = open("config/singlemuons.txt","r").readlines()

outpath  = 'plots'

x_labels = {
'met_met_':     r'E$_{\text{T}}^{\text{Miss}}$ [GeV]',
'jet_pt_':      r'Small-R Jet p$_\text{T}$ [GeV]',
'jet_CSVv2_':   r'Small-R Jet CSV',
'ljet_pt_':     r'Large-R Jet p$_\text{T}$ [GeV]',
'ljet_SDmass_': r'Large-R Jet Soft Drop Mass [GeV]',
'el_pt_':       r'Electron p$_\text{T}$ [GeV]',
'mu_pt_':       r'Muon p$_\text{T}$ [GeV]',
}

histograms = x_labels.keys()
ljet_pt = array('d',[175,200,250,350,500,750,1250,2000])

## Add the data from each file
print " ttbar "
h_hist_top = {}
h_hist = {}

for h in histograms:
    h_hist_top[h] = None
    h_hist[h] = None

for fi,file in enumerate(ttbar_files):
    file = file.rstrip("\n")
    f = ROOT.TFile.Open(file)
    filename = file.split("/")[-1].split(".")[0]

    for h in histograms:
        if h_hist_top[h] is None:
            h_hist_top[h] = getattr(f,"histograms/"+h)       # retrieve the histogram
            h_hist_top[h].SetDirectory(0)
        else:
            h_temp = getattr(f,"histograms/"+h)
            h_temp.SetDirectory(0)
            h_hist_top[h].Add( h_temp )

print " data "

for fi,file in enumerate(data_files):
    file = file.rstrip("\n")
    f = ROOT.TFile.Open(file)
    filename = file.split("/")[-1].split(".")[0]

    for h in histograms:
        if h_hist[h] is None:
            h_hist[h] = getattr(f,"histograms/"+h)       # retrieve the histogram
            h_hist[h].SetDirectory(0)
        else:
            h_temp = getattr(f,"histograms/"+h)
            h_temp.SetDirectory(0)
            h_hist[h].Add( h_temp )



# Access data -- assumes you are plotting histograms from multiple sources in one figure
for hi,histogram in enumerate(histograms):

    histogram = histogram.strip('\n')
    print "  :: Plotting "+histogram+"\n"

    ## setup histogram
    hist = HepPlotter("histogram",1)

    hist.ratio_plot  = True       # plot a ratio of things [Data/MC]
    hist.ratio_type  = "ratio"     # "ratio"
    hist.stacked     = False        # stack plots
    hist.rebin       = ljet_pt if histogram=='ljet_pt_' else 10
    hist.xlim        = (0,1) if histogram=='jet_CSVv2_' else None
    hist.logplot     = False       # plot on log scale
    hist.x_label     = x_labels[histogram]
    hist.y_label     = "Arbitrary Units"
    hist.y_ratio_label = "Data/MC"
    hist.lumi          = 'XY.Z'   # in /fb
    hist.format        = 'png'       # file format for saving image
    hist.saveAs        = outpath+"/h_"+histogram # save figure with name
    hist.numLegendColumns = 1
    hist.CMSlabel       = 'top left'  # 'top left', 'top right'; hack code for something else
    hist.CMSlabelStatus = 'Internal'  # ('Simulation')+'Internal' || 'Preliminary' 

    hist.initialize()


    total_data = h_hist[histogram].Integral()
    h_hist[histogram].Scale( 1/total_data )

    total_ttbar = h_hist_top[histogram].Integral()
    h_hist_top[histogram].Scale( 1/total_ttbar )


    hist.Add(h_hist_top[histogram],name="ttbar_"+histogram,linecolor='red',
             draw='step',label=r't$\bar{\text{t}}$',ratio_den=True,ratio_partner='data_'+histogram)

    hist.Add(h_hist[histogram],name="data_"+histogram,linecolor='black',color='black',
             draw='errorbar',label='Data (Single Muon) 2016',ratio_num=True,ratio_partner='ttbar_'+histogram)


    plot = hist.execute()
    hist.savefig()
    print "  :: Saved plot to "+hist.saveAs+"\n"

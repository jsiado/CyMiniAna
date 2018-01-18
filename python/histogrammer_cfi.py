"""
Created:        14 January   2018
Last Updated:   15 January   2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University
-----

Config file for histogrammer in CMSSW
"""
import FWCore.ParameterSet.Config as cms


hist = cms.EDAnalyzer("histogrammer",
    btageffmap  = cms.string("TbtH_1200_LH_btagEff_loose.root"),
    sjbtagSFcsv = cms.string('subjet_CSVv2_Moriond17_B_H.csv')
)

## THE END

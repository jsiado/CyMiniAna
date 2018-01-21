"""
Created:        14 January   2018
Last Updated:   18 January   2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University
-----

Config file for histogrammer in CMSSW
"""
import FWCore.ParameterSet.Config as cms


hist = cms.EDAnalyzer("histogrammer",
    isMC = cms.bool(False),
    useTruth = cms.bool(False),
    useJets  = cms.bool(False),
    useLargeRJets = cms.bool(False),
    useNeutrinos  = cms.bool(False),
    useLeptons    = cms.bool(False),

    useSystWeights = cms.bool(False),
    weightSystematicsFile = cms.string("SetMe"),
    weightVectorSystematicsFile = cms.string("SetMe"),
)

## THE END

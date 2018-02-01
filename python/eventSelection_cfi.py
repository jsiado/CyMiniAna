"""
Created:        14 January   2018
Last Updated:   18 January   2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University
-----

Config file for eventSelection in CMSSW
"""
import FWCore.ParameterSet.Config as cms


evtSel = cms.EDFilter("eventSelection",
    selection = cms.string("SetMe"),
    cutsfile  = cms.string("SetMe"),
    trigNameLabel = cms.InputTag("TriggerUserData", "triggerNameTree"),
    trigBitLabel  = cms.InputTag("TriggerUserData", "triggerBitTree"),
    HLTPaths = cms.vstring(hltPaths)
)

## THE END

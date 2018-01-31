"""
Created:        26 January   2018
Last Updated:   26 January   2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University
-----

Config file for EventSaverFlatNtuple in CMSSW
"""
import FWCore.ParameterSet.Config as cms


flat = cms.EDAnalyzer("EventSaverFlatNtuple",
    isMC = cms.bool(False),
    useTruth = cms.bool(False),
    useJets  = cms.bool(False),
    useLargeRJets = cms.bool(False),
    useNeutrinos  = cms.bool(False),
    useLeptons    = cms.bool(False),

    rhoLabel = cms.InputTag("vertexInfo","rho"),
    npvLabel = cms.InputTag("vertexInfo","npv"),
    runnoLabel      = cms.InputTag("eventInfo", "evtInfoRunNumber"),
    lumisecLabel    = cms.InputTag("eventInfo", "evtInfoLumiBlock"),
    evtnoLabel      = cms.InputTag("eventInfo", "evtInfoEventNumber"),
    puNtrueIntLabel = cms.InputTag("eventUserData", "puNtrueInt"),
)

## THE END

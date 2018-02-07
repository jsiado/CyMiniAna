"""
Created:        14 December  2017
Last Updated:   15 January   2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University
-----

Config file for CMAProducer
"""
import FWCore.ParameterSet.Config as cms

from Analysis.CyMiniAna.physObjects_cfi import *
from Analysis.CyMiniAna.physObjectsEDMLabels import *


cma = cms.EDProducer("CMAProducer",
    isMC = cms.bool(False),
    useTruth = cms.bool(False),
    useJets  = cms.bool(False),
    useLargeRJets = cms.bool(False),
    useLeptons    = cms.bool(False),
    useNeutrinos  = cms.bool(False),
    buildNeutrinos = cms.bool(False),
    kinematicReco  = cms.bool(False),

    LUMI = cms.double(1.0),
    data_path    = cms.string("data/"),
    metadataFile = cms.string(""),

    # Physics Objects
    # - labels to access data
    neutrinoLabels  = neutrinoLabels,
    muonLabels      = muonLabels,
    electronLabels  = electronLabels,
    jetLabels       = jetLabels,
    largeRjetLabels = largeRJetLabels,
    METLabels = METLabels,
    # - selection on objects (pT,eta,ID,b-tagging,etc.)
    objSelectionParams = objectSelectionParams,
)

## THE END

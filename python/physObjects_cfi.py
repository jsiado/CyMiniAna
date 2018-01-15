"""
Created:        14 January   2018
Last Updated:   14 January   2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University
-----

Configuration options for physics objects when 
running CyMiniAna in CMSSW.

 - modeled after "XYZ_Maker_cfi.py" in 
   https://github.com/dmajumder/VLQAna/tree/CMSSW_8_0_X_NewB2GAnaFW/

Configurations for:
- MET
- Muons
- Electrons
- Jets (AK4)
- LargeRJets (AK8)
"""
import FWCore.ParameterSet.Config as cms

METParameters = cms.PSet(
    METPtMin = cms.double(0),
)

muonParameters = cms.PSet(
    muidtype = cms.string("LOOSE"), 
    muPtMin = cms.double(25),
    muPtMax = cms.double(10000),
    muAbsEtaMax = cms.double(2.4),
    muIsoMin = cms.double(-100),
    muIsoMax = cms.double(0.30), 
)

electronParameters = cms.PSet(
    elidtype = cms.string("TIGHT"),
    elPtMin = cms.double(25),
    elPtMax = cms.double(10000),
    elAbsEtaMax = cms.double(2.4),
    applyIso = cms.bool(True), 
)
### 
# B-tagging
#  https://twiki.cern.ch/twiki/bin/view/CMS/BtagRecommendation80XReReco
# - CSVv2L = 0.5426
# - CSVv2M = 0.8484
# - CSVv2T = 0.9535

jetParameters = cms.PSet(
    jettype      = cms.string('AK4JET'),
    JetIDParams  = cms.string('loose')
    jetPtMin     = cms.double(25),
    jetAbsEtaMax = cms.double(2.5),
    scaleJetP4   = cms.bool(False),
    jecShift     = cms.double(1),
    jerShift     = cms.int32(1), 
    CSVv2        = cms.string('loose')
)

largeRJetParameters = cms.PSet(
    jettype      = cms.string('AK4JET'),
    JetIDParams  = cms.string('tight')
    jetPtMin     = cms.double(200),
    jetAbsEtaMax = cms.double(2.0),
    scaleJetP4   = cms.bool(False),
    jecShift     = cms.double(1),
    jerShift     = cms.int32(1), 
    CSVv2        = cms.string('loose')
)


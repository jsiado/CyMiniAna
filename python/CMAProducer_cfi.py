"""
Created:        14 December  2017
Last Updated:   14 December  2017

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University
-----
Config file for CMAProducer
"""
import FWCore.ParameterSet.Config as cms


cma = cms.EDProducer("CMAProducer",
    runno                      = cms.InputTag("evtcleaner","runno"), 
    lumisec                    = cms.InputTag("evtcleaner","lumisec"), 
    evtno                      = cms.InputTag("evtcleaner","evtno"), 
    isData                     = cms.InputTag("evtcleaner","isData"), 
    hltdecision                = cms.InputTag("evtcleaner","hltdecision"), 
    evttype                    = cms.InputTag("evtcleaner","evttype"),
    evtwtGen                   = cms.InputTag("evtcleaner","evtwtGen"),
    evtwtPV                    = cms.InputTag("evtcleaner","evtwtPV"),
    evtwtPVBG                  = cms.InputTag("evtcleanerBG","evtwtPV"),
    evtwtPVH                   = cms.InputTag("evtcleanerH","evtwtPV"),
    evtwtPVLow                 = cms.InputTag("evtcleaner","evtwtPVLow"),
    evtwtPVHigh                = cms.InputTag("evtcleaner","evtwtPVHigh"),
    npv                        = cms.InputTag("evtcleaner","npv"),
    npuTrue                    = cms.InputTag("evtcleaner","npuTrue"),
    htHat                      = cms.InputTag("evtcleaner","htHat"),
    lhewtids                   = cms.InputTag("evtcleaner","lhewtids"),
    lhewts                     = cms.InputTag("evtcleaner","lhewts"), 

    elselParams                = defaultElectronMakerParameters.clone(
      elPtMin = cms.double(50),
      applyIso = cms.bool(False), 
      ), 

    muselParams                = defaultMuonMakerParameters.clone(
      muidtype = cms.string("LOOSE"), 
      muPtMin = cms.double(47),
      muIsoMin = cms.double(0.00),
      muIsoMax = cms.double(1000), 
      ), 

    jetAK4selParams            = defaultAK4JetSelectionParameters,
    jetAK8selParams            = defaultAK8CHSJetSelectionParameters,
    jetHTaggedselParams        = defaultCHSHJetSelectionParameters,
    leadingJetPtMin            = cms.double  (400.), 
    leadingJetPrunedMassMin    = cms.double  (50.), 

    HTMin                      = cms.double  (0.), 
    doBTagSFUnc                = cms.bool(False), 
    storePreselEvts            = cms.bool(False), 
    doPreselOnly               = cms.bool(False),
    applyBTagSFs               = cms.bool(True),
    btageffmap                 = cms.string("TbtH_1200_LH_btagEff_loose.root"),
    sjbtagSFcsv                = cms.string('subjet_CSVv2_Moriond17_B_H.csv')
)

## THE END

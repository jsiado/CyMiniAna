"""
Created:      13 December 2017
Last Updated: 13 December 2017

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University
---

Configuration file for running CyMiniAna
over EDMNtuples (from B2GAnaFW).
Based on VLQAna:
  https://github.com/dmajumder/VLQAna

Requires a CMSSW environment!

To run:
  python python/cma_cfg.py <config.txt>

where <config.txt> is the configuration file
"""
import sys, os

import FWCore.ParameterSet.Config as cms

from configuration import Configuration
from inputFiles_cfi import *
from Analysis.VLQAna.VLQAna_cfi import *
from Analysis.VLQAna.JetMaker_cfi import *
from Analysis.EventCounter.eventcounter_cfi import eventCounter



## Register configuration options
##  don't use VarParsing, switch to config file instead
config = Configuration( sys.argv[1] )

nEventsToProcess = config.nEventsToProcess()
outputFileName   = config.outputFileName()
dataFilePath     = config.dataDirectory()    # os.environ["$CMSSW_BASE]+"/src/Analysis/CyMiniAna/data"
isMC     = config.isMC()
jerShift = config.jerShift()
jecShift = config.jecShift()


## PROCESS
process = cms.Process("CyMiniAna")

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(FileNames[config.FileNames])
                           )
process.maxEvents    = cms.untracked.PSet( input = cms.untracked.int32(nEventsToProcess) )
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(outputFileName)
                                  )

## Event Cleaner
process.load("Analysis.VLQAna.EventCleaner_cff") 
process.evtcleaner.hltORAND = cms.string (config.hltORAND)  
process.evtcleaner.hltPaths = cms.vstring (hltpathsOr)  
process.evtcleaner.cleanEvents = cms.bool(config.cleanEvents())
process.evtcleaner.isData = (not isMC)
process.evtcleaner.DoPUReweightingOfficial = config.doPUReweightingOfficial()
process.evtcleaner.storeLHEWts = config.storeLHEWts()
process.evtcleaner.File_PUDistData      = cms.string(os.path.join(dataFilePath,'RunII2016Rereco_25ns_PUXsec69000nb.root'))
process.evtcleaner.File_PUDistDataLow   = cms.string(os.path.join(dataFilePath,'RunII2016Rereco_25ns_PUXsec65550nb.root'))
process.evtcleaner.File_PUDistDataHigh  = cms.string(os.path.join(dataFilePath,'RunII2016Rereco_25ns_PUXsec72450nb.root'))
process.evtcleaner.File_PUDistMC        = cms.string(os.path.join(dataFilePath,'PUDistMC_Summer2016_25ns_Moriond17MC_PoissonOOTPU.root'))

process.evtcleanerBG = process.evtcleaner.clone()
process.evtcleanerBG.File_PUDistData = cms.string(os.path.join(dataFilePath,'RunII2016Rereco_25ns_RunsBtoG_PUXsec69000nb.root'))

process.evtcleanerH = process.evtcleaner.clone()
process.evtcleanerH.File_PUDistData  = cms.string(os.path.join(dataFilePath,'RunII2016Rereco_25ns_RunH_PUXsec69000nb.root'))


# Setup JET uncertainty payloads
if isMC:
    ### Careful, to be reset when B2GAnaFW_v80X_v2.4 MC are used
    for par in ['jetAK4selParams', 'jetAK8selParams', 'jetHTaggedselParams', 'jetAntiHTaggedselParams', 'jetTopTaggedselParams', 'jetAntiTopTaggedselParams']:
        if 'AK4' in par:
            jetType = 'AK4PFchs'
        else:
            jetType = 'AK8PFchs'
            payLoadTypes = ['L2Relative', 'L3Absolute']
            payLoadFiles = []
            for payLoadType in payLoadTypes:
                payLoadFiles.append(os.path.join(dataFilePath,'Summer16_23Sep2016V4_MC_'+payLoadType+'_'+jetType+'.txt'))   

            setattr(getattr(ana, par), 'jecAK8GroomedPayloadNames', cms.vstring(payLoadFiles))

            setattr(getattr(getattr(ana, par), 'JetSubstrParams'), 'jettau1Label'         ,cms.InputTag("jetsAK8CHS", "jetAK8CHStau1CHS"))
            setattr(getattr(getattr(ana, par), 'JetSubstrParams'), 'jettau2Label'         ,cms.InputTag("jetsAK8CHS", "jetAK8CHStau2CHS"))
            setattr(getattr(getattr(ana, par), 'JetSubstrParams'), 'jettau3Label'         ,cms.InputTag("jetsAK8CHS", "jetAK8CHStau3CHS"))
            setattr(getattr(getattr(ana, par), 'JetSubstrParams'), 'jetPrunedMassLabel'   ,cms.InputTag("jetsAK8CHS", "jetAK8CHSprunedMassCHS"))
            setattr(getattr(getattr(ana, par), 'JetSubstrParams'), 'jetTrimmedMassLabel'  ,cms.InputTag("jetsAK8CHS", "jetAK8CHStrimmedMassCHS"))
            setattr(getattr(getattr(ana, par), 'JetSubstrParams'), 'jetFilteredMassLabel' ,cms.InputTag("jetsAK8CHS", "jetAK8CHSfilteredMassCHS"))
            setattr(getattr(getattr(ana, par), 'JetSubstrParams'), 'jetSoftDropMassLabel' ,cms.InputTag("jetsAK8CHS", "jetAK8CHSsoftDropMassCHS"))

        setattr(getattr(ana,par), 'jecUncPayloadName', cms.string(os.path.join(dataFilePath, 'Summer16_23Sep2016V4_MC_Uncertainty_'+jetType+'.txt')))


process.ana = ana.clone()
process.ana.jetAK4selParams.jecShift = jecShift 
process.ana.jetAK4selParams.jerShift = jerShift 
process.ana.jetAK8selParams.jecShift = jecShift 
process.ana.jetAK8selParams.jerShift = jerShift 

process.ana.jetHTaggedselParams.jecShift     = jecShift 
process.ana.jetHTaggedselParams.jerShift     = jerShift 
process.ana.jetTopTaggedselParams.jecShift   = jecShift 
process.ana.jetTopTaggedselParams.jerShift   = jerShift 
process.ana.jetAntiHTaggedselParams.jecShift = jecShift 
process.ana.jetAntiHTaggedselParams.jerShift = jerShift 

process.ana.doBTagSFUnc     = config.doBTagSFUnc()
process.ana.storePreselEvts = config.storePreselEvts()
process.ana.doPreselOnly    = config.doPreselOnly()
process.ana.HTMin           = config.HTMin()
process.ana.applyBTagSFs    = config.applyBTagSFs()
process.ana.btageffmap      = cms.string(os.path.join(dataFilePath,config.btageffmap)) 
process.ana.sjbtagSFcsv     = cms.string(os.path.join(dataFilePath,"subjet_CSVv2_Moriond17_B_H.csv")) 

# CHS Jets
process.anaCHS = process.ana.clone()
            
# PUPPI Jets
process.anaPuppi = process.ana.clone(
     jetAK8selParams         = defaultAK8PuppiJetSelectionParameters,
     jetHTaggedselParams     = defaultPuppiHJetSelectionParameters,
     jetAntiHtaggedSelParams = defaultPuppiHJetSelectionParameters.clone(
                                   subjetCSVMin = cms.double(-1000000),
                                   subjetCSVMax = defaultPuppiHJetSelectionParameters.subjetCSVMin,
                                   ),
     jetTopTaggedselParams = defaultPuppiTJetSelectionParameters.clone(),
    )


process.allEvents          = eventCounter.clone(isMC=isMC)
process.cleanedEvents      = eventCounter.clone(isMC=isMC)
process.finalEvents        = eventCounter.clone(isMC=isMC)
process.finalEventsPuppi   = eventCounter.clone(isMC=isMC)
process.finalEventsDoubleB = eventCounter.clone(isMC=isMC)


## PROCESS PATHS
#  - CHS
process.p = cms.Path(
      process.allEvents
     *process.evtcleaner
     *process.evtcleanerBG
     *process.evtcleanerH
     *process.cleanedEvents
     *process.anaCHS 
     *process.finalEvents
    )

#  - PUPPI
process.pPuppi = cms.Path(
      process.allEvents
     *process.evtcleaner
     *process.anaPuppi
     *process.finalEventsPuppi
    )

## OUTPUT
process.out = cms.OutputModule("PoolOutputModule",
                               SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('evtcleaner')),
                              )

process.schedule = cms.Schedule(process.p)
#process.outpath = cms.EndPath(process.out)

open('dump.py','w').write(process.dumpPython())


## THE END ##

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
import os
import sys

import FWCore.ParameterSet.Config as cms

from configuration import Configuration
from Analysis.VLQAna.VLQAna_cfi import *    # 'ana'
from Analysis.VLQAna.JetMaker_cfi import *  # jet parameters
from Analysis.EventCounter.eventcounter_cfi import eventCounter



## Configuration options
##  don't use VarParsing, switch to config file instead
config = Configuration( sys.argv[1] )

nEventsToProcess = config.nEventsToProcess()
outputFileName   = config.outputFileName()
dataFilePath     = config.dataDirectory()    # os.environ["$CMSSW_BASE]+"/src/Analysis/CyMiniAna/data"
isMC      = config.isMC()
jerShift  = config.jerShift()
jecShift  = config.jecShift()
filenames = config.filesToProcess()


## PROCESS
process = cms.Process("CyMiniAna")

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(filenames)  #inputFiles_cfi: FileNames[config.FileNames])
                           )
process.maxEvents    = cms.untracked.PSet( input = cms.untracked.int32(nEventsToProcess) )
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(outputFileName)
                                  )

## CMAProducer
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


## Cutflow counting
process.allEvents          = eventCounter.clone(isMC=isMC)
process.cleanedEvents      = eventCounter.clone(isMC=isMC)
process.finalEvents        = eventCounter.clone(isMC=isMC)
process.finalEventsPuppi   = eventCounter.clone(isMC=isMC)


## PROCESS PATHS
#  - Count events; event cleaning; count events; anaCHS; count events
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

#  - Count events; event cleaning; count events; anaPuppi; count events
#  - PUPPI
process.pPuppi = cms.Path(
      process.allEvents*
      process.evtcleaner*
      process.anaPuppi*
      process.finalEventsPuppi
    )

process.p = cms.Path(
                     process.initial*
                     process.analysis*
                     process.histogrammer*
                     process.finalize
                    )


## OUTPUTMODULE
process.out = cms.OutputModule("PoolOutputModule",
                               SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('evtcleaner')),
                              )

process.schedule = cms.Schedule(process.p)
#process.outpath = cms.EndPath(process.out)

open('dump.py','w').write(process.dumpPython())


## THE END ##

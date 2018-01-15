"""
Created:      13 December 2017
Last Updated: 14 January  2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University
---

Primary script for running CyMiniAna
over EDMNtuples (from B2GAnaFW) in CMSSW.
Based on VLQAna:
  https://github.com/dmajumder/VLQAna

  Requires a CMSSW environment!

To run:
  cmsRun python/cma_cfg.py <config.txt>

where <config.txt> is the configuration file
for basic options.
"""
import os
import sys

import FWCore.ParameterSet.Config as cms

from configuration import Configuration
from Analysis.CyMiniAna.CMAProducer_cfi import *  # 'cma'
import histogrammer_cfi as hist
import eventSelection_cfi as evtSel
import physObjects_cfi as phys
import physObjectsEDMLabels as labels


## Configuration options ##
config = Configuration( sys.argv[1] )

nEventsToProcess = config.nEvents()
outputFileName   = config.outputFileName()
filenames = config.filesToProcess()   # list of files
isMC      = config.isMC( filesnames[0] )   # check the first file to see if we are running over MC or Data

## PROCESS
process = cms.Process("CyMiniAna")

process.source       = cms.Source("PoolSource",fileNames = cms.untracked.vstring(filenames) )
process.maxEvents    = cms.untracked.PSet( input = cms.untracked.int32(nEventsToProcess) )
process.TFileService = cms.Service("TFileService",fileName = cms.string(outputFileName) )

## CMAProducer
process.ana = cma.clone()

# set CMAProducer options
process.isMC(isMC)

# CHS Jets
process.anaCHS = process.ana.clone(#set any different options)

# PUPPI Jets
process.anaPuppi = process.ana.clone(
    jetAK8selParams = defaultAK8PuppiJetSelectionParameters,
    )


## PROCESS PATHS

#  - CHS
#  - Count events; event cleaning; count events; anaCHS; count events
process.p = cms.Path(
      process.allEvents*
      process.cleanedEvents*
      process.anaCHS*
      process.finalEvents
    )
#  - PUPPI (same as CHS Jets)

# CMA
# use EventCounter from Devdatta
process.p = cms.Path(
    process.initial*
    process.ana*
    process.evtSel*
    process.histogrammer*
    process.final
)


## OUTPUTMODULE
process.out      = cms.OutputModule("PoolOutputModule")
process.schedule = cms.Schedule(process.p)
#process.outpath = cms.EndPath(process.out)

open('dump.py','w').write(process.dumpPython())

## THE END ##

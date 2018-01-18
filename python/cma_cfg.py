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

from Analysis.EventCounter.eventcounter_cfi import eventCounter
from Analysis.CyMiniAna.CMAProducer_cfi import *  # 'cma'
from Analysis.CyMiniAna.configuration import Configuration
from Analysis.CyMiniAna.histogrammer_cfi import hist
from Analysis.CyMiniAna.eventSelection_cfi import evtSel
import Analysis.CyMiniAna.physObjects_cfi as phys
import Analysis.CyMiniAna.physObjectsEDMLabels as labels

## Configuration options ##
print " Setup configuration "

if sys.argv[0] == "cmsRun":
    argument = 2
else:
    argument = 1

config = Configuration( sys.argv[argument] )
config.initialize()

nEventsToProcess = config.nEvents()
outputFileName   = config.outputFileName()
filenames = config.filenames()   # list of files
isMC      = config.isMC( filenames[0] )   # check the first file to see if we are running over MC or Data

## PROCESS
print " Begin the process "
process = cms.Process("CyMiniAna")

process.source       = cms.Source("PoolSource",fileNames = cms.untracked.vstring(filenames) )
process.maxEvents    = cms.untracked.PSet( input = cms.untracked.int32(nEventsToProcess) )
process.TFileService = cms.Service("TFileService",fileName = cms.string(outputFileName) )

## Histogrammer
print " Histogrammer "
process.histogrammer = hist.clone()

## Event Selection
print " Event selection "
process.evtSel = evtSel.clone()

## CMAProducer
process.ana = cma.clone()

# set CMAProducer options
process.ana.isMC = cms.bool(isMC)
process.ana.useJets  = cms.bool(config.useJets())
process.ana.useLargeRJets  = cms.bool(config.useLargeRJets())
process.ana.useLeptons     = cms.bool(config.useLeptons())
process.ana.useNeutrinos   = cms.bool(config.useNeutrinos())
process.ana.buildNeutrinos = cms.bool(config.buildNeutrinos())
process.ana.kinematicReco  = cms.bool(config.kinematicReco())
process.ana.metadataFile   = cms.string(config.metadataFile())
process.ana.LUMI = cms.double(config.LUMI())
process.ana.useTruth = cms.bool(config.useTruth())

process.initial = eventCounter.clone(isData=(not isMC))
process.final   = eventCounter.clone(isData=(not isMC))

# CHS Jets
# process.anaCHS = process.ana.clone()#set any different options)
# PUPPI Jets
#process.anaPuppi = process.ana.clone(
#    jetAK8selParams = defaultAK8PuppiJetSelectionParameters,
#    )


## PROCESS PATHS
#  - CHS
#  - Count events; event cleaning; count events; anaCHS; count events
#process.p = cms.Path(
#      process.allEvents*
#      process.cleanedEvents*
#      process.anaCHS*
#      process.finalEvents
#    )
#  - PUPPI (same as CHS Jets)


# CMA
# use EventCounter from Devdatta
print " Set the path "
process.p = cms.Path(
    process.initial*
    process.ana*
    process.evtSel*
    process.histogrammer*
    process.final
)


## OUTPUTMODULE
print " Set the output "
process.out      = cms.OutputModule("PoolOutputModule")
process.schedule = cms.Schedule(process.p)
#process.outpath = cms.EndPath(process.out)

print " Dump python "
open('dump.py','w').write(process.dumpPython())

## THE END ##

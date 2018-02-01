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
- Muons
- Electrons
- Jets (AK4)
  B-tagging
    https://twiki.cern.ch/twiki/bin/view/CMS/BtagRecommendation80XReReco
- LargeRJets (AK8)

"""
import FWCore.ParameterSet.Config as cms


objectSelectionParams = cms.PSet(

 # muonParameters
    mu_id     = cms.string("loose"),
    mu_pt     = cms.double(30),
    mu_eta    = cms.double(2.4),
    mu_IsoMin = cms.double(-100),
    mu_IsoMax = cms.double(100),

 # electronParameters
    el_pt  = cms.double(30),
    el_eta = cms.double(2.4),
    el_id  = cms.string("tight"),
    el_iso = cms.string(""),
    el_applyIso = cms.bool(True),

 # neutrinoParameters
    nu_pt  = cms.double(0.0),
    nu_eta = cms.double(3.0),

 # jetParameters AK4
    jet_id    = cms.string('loose'),
    jet_pt    = cms.double(25),
    jet_eta   = cms.double(2.5),

 # largeRJetParameters AK8
    ljet_id  = cms.string('loose'),
    ljet_pt  = cms.double(200),
    ljet_eta = cms.double(2.0),
)

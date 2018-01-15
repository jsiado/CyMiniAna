"""
Created:        14 January   2018
Last Updated:   14 January   2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University
-----
Labels for accessing data in EDM files using `cms.InputTag` method

Labels for
- MET
- Muons
- Electrons
- Jets (AK4)
- LargeRJets (AK8) 
  - Soft Drop
"""
import FWCore.ParameterSet.Config as cms

METLabels = cms.PSet(
    metFullPhiLabel        = cms.InputTag("metFull","metFullPhi"),
    metFullPtLabel         = cms.InputTag("metFull","metFullPt"),
    metFullPxLabel         = cms.InputTag("metFull","metFullPx"),
    metFullPyLabel         = cms.InputTag("metFull","metFullPy"),
    metFulluncorPhiLabel   = cms.InputTag("metFull","metFulluncorPhi"),
    metFulluncorPtLabel    = cms.InputTag("metFull","metFulluncorPt"),
    metFulluncorSumEtLabel = cms.InputTag("metFull","metFulluncorSumEt"),
    metNoHFPhiLabel        = cms.InputTag("metNoHF","metNoHFPhi"),
    metNoHFPtLabel         = cms.InputTag("metNoHF","metNoHFPt"),
    metNoHFPxLabel         = cms.InputTag("metNoHF","metNoHFPx"),
    metNoHFPyLabel         = cms.InputTag("metNoHF","metNoHFPy"),
    metNoHFuncorPhiLabel   = cms.InputTag("metNoHF","metNoHFuncorPhi"),
    metNoHFuncorPtLabel    = cms.InputTag("metNoHF","metNoHFuncorPt"),
    metNoHFuncorSumEtLabel = cms.InputTag("metNoHF","metNoHFuncorSumEt"),
)

muonLabels = cms.PSet(
    muChargeLabel                   = cms.InputTag("muons", "muCharge"), 
    muDxyLabel                      = cms.InputTag("muons", "muDxy"), 
    muDzLabel                       = cms.InputTag("muons", "muDz"), 
    muELabel                        = cms.InputTag("muons", "muE"), 
    muEtaLabel                      = cms.InputTag("muons", "muEta"), 
    muGenMuonChargeLabel            = cms.InputTag("muons", "muGenMuonCharge"), 
    muGenMuonELabel                 = cms.InputTag("muons", "muGenMuonE"), 
    muGenMuonEtaLabel               = cms.InputTag("muons", "muGenMuonEta"), 
    muGenMuonPhiLabel               = cms.InputTag("muons", "muGenMuonPhi"), 
    muGenMuonPtLabel                = cms.InputTag("muons", "muGenMuonPt"), 
    muGlbTrkNormChi2Label           = cms.InputTag("muons", "muGlbTrkNormChi2"), 
    muInTrkNormChi2Label            = cms.InputTag("muons", "muInTrkNormChi2"), 
    muIsGlobalMuonLabel             = cms.InputTag("muons", "muIsGlobalMuon"), 
    muIsLooseMuonLabel              = cms.InputTag("muons", "muIsLooseMuon"), 
    muIsPFMuonLabel                 = cms.InputTag("muons", "muIsPFMuon"), 
    muIsSoftMuonLabel               = cms.InputTag("muons", "muIsSoftMuon"), 
    muIsTightMuonLabel              = cms.InputTag("muons", "muIsTightMuon"), 
    muIsTrackerMuonLabel            = cms.InputTag("muons", "muIsTrackerMuon"), 
    muIso04Label                    = cms.InputTag("muons", "muIso04"), 
    muKeyLabel                      = cms.InputTag("muons", "muKey"), 
    muNumberMatchedStationsLabel    = cms.InputTag("muons", "muNumberMatchedStations"), 
    muNumberOfPixelLayersLabel      = cms.InputTag("muons", "muNumberOfPixelLayers"), 
    muNumberOfValidTrackerHitsLabel = cms.InputTag("muons", "muNumberOfValidTrackerHits"), 
    muNumberTrackerLayersLabel      = cms.InputTag("muons", "muNumberTrackerLayers"), 
    muNumberValidMuonHitsLabel      = cms.InputTag("muons", "muNumberValidMuonHits"), 
    muNumberValidPixelHitsLabel     = cms.InputTag("muons", "muNumberValidPixelHits"), 
    muPhiLabel                      = cms.InputTag("muons", "muPhi"), 
    muPtLabel                       = cms.InputTag("muons", "muPt"), 
    muSumChargedHadronPtLabel       = cms.InputTag("muons", "muSumChargedHadronPt"), 
    muSumNeutralHadronPtLabel       = cms.InputTag("muons", "muSumNeutralHadronPt"), 
    muSumPUPtLabel                  = cms.InputTag("muons", "muSumPUPt"), 
    muSumPhotonPtLabel              = cms.InputTag("muons", "muSumPhotonPt"), 
    muYLabel                        = cms.InputTag("muons", "muY"),
)

electronLabels = cms.PSet(
    elChargeLabel            = cms.InputTag("electrons", "elCharge"),
    elDxyLabel               = cms.InputTag("electrons", "elDxy"),
    elDzLabel                = cms.InputTag("electrons", "elDz"),
    elELabel                 = cms.InputTag("electrons", "elE"),
    elEtaLabel               = cms.InputTag("electrons", "elEta"),
    elHoELabel               = cms.InputTag("electrons", "elHoE"),
    elRelIsoEALabel          = cms.InputTag("electrons", "elIso03"),
    elKeyLabel               = cms.InputTag("electrons", "elKey"),
    elPhiLabel               = cms.InputTag("electrons", "elPhi"),
    elPtLabel                = cms.InputTag("electrons", "elPt"),
    eldEtaInLabel            = cms.InputTag("electrons", "eldEtaIn"),
    eldEtaInSeedLabel        = cms.InputTag("electrons", "eldEtaInSeed"),
    eldPhiInLabel            = cms.InputTag("electrons", "eldPhiIn"),
    elfull5x5sieeLabel       = cms.InputTag("electrons", "elfull5x5siee"),
    elhasMatchedConVetoLabel = cms.InputTag("electrons", "elhasMatchedConVeto"),
    elvidLooseLabel          = cms.InputTag("electrons", "elvidLoose"),
    elvidMediumLabel         = cms.InputTag("electrons", "elvidMedium"),
    elvidTightLabel          = cms.InputTag("electrons", "elvidTight"),
    elvidVetoLabel           = cms.InputTag("electrons", "elvidVeto"),
    elvidHEEPLabel           = cms.InputTag("electrons", "elvidHEEP"),
    elmissHitsLabel          = cms.InputTag("electrons", "elmissHits"),
    elooEmooPLabel           = cms.InputTag("electrons", "elooEmooP"),
    elscEtaLabel             = cms.InputTag("electrons", "elSCEta"),
)


jetType   = "CHS"  # Puppi

jetLabels = cms.PSet(
    npvLabel                = cms.InputTag("vertexInfo", "npv"),
    rhoLabel                = cms.InputTag("fixedGridRhoFastjetAll"),
    jetPtLabel              = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"Pt"),
    jetEtaLabel             = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"Eta"),
    jetPhiLabel             = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"Phi"),
    jetEnergyLabel          = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"E"),
    jetPartonFlavourLabel   = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"PartonFlavour"),
    jetHadronFlavourLabel   = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"HadronFlavour"),
    jetCSVLabel             = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"CSVv2"),
    jetCMVALabel            = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"CMVAv2"),
    jetCvsBLabel            = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"CvsB"),
    jetCvsLLabel            = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"CvsL"),
    jetJECLabel             = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"jecFactor0"),
    jetnHadEnergyLabel      = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"neutralHadronEnergyFrac"),
    jetnEMEnergyLabel       = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"neutralEmEnergyFrac"),
    jetHFHadronEnergyLabel  = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"HFHadronEnergy"),
    jetcHadEnergyLabel      = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"chargedHadronEnergyFrac"),
    jetcEMEnergyLabel       = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"chargedEmEnergyFrac"),
    jetcMultipLabel         = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"chargedMultiplicity"),
    jetnMultipLabel         = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"neutralMultiplicity"),
    jetMuonEnergyLabel      = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"MuonEnergy"),
    jetYLabel               = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"Y"),
    jetAreaLabel            = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"jetArea"),
    jetGenJetPtLabel        = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"GenJetPt"),
    jetGenJetEtaLabel       = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"GenJetEta"),
    jetGenJetPhiLabel       = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"GenJetPhi"),
    jetGenJetELabel         = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"GenJetE"),
    jetGenJetChargeLabel    = cms.InputTag("jetsAK4"+jetType, "jetAK4"+jetType+"GenJetCharge"),
)

largeRJetLabels = defaultAK4JetParameters.clone( 
    npvLabel                = cms.InputTag("vertexInfo", "npv"),
    rhoLabel                = cms.InputTag("fixedGridRhoFastjetAll"),
    jetPtLabel              = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"Pt"),
    jetEtaLabel             = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"Eta"),
    jetPhiLabel             = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"Phi"),
    jetEnergyLabel          = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"E"),
    jetPartonFlavourLabel   = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"PartonFlavour"),
    jetHadronFlavourLabel   = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"HadronFlavour"),
    jetCSVLabel             = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"CSVv2"),
    jetCMVALabel            = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"CMVAv2"),
    jetCvsBLabel            = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"CvsB"),
    jetCvsLLabel            = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"CvsL"),
    jetDoubleBAK8Label      = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"DoubleBAK8"),
    jetDoubleBCA15Label     = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"DoubleBCA15"),
    jetJECLabel             = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"jecFactor0"),
    jetnHadEnergyLabel      = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"neutralHadronEnergyFrac"),
    jetnEMEnergyLabel       = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"neutralEmEnergyFrac"),
    jetcHadEnergyLabel      = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"chargedHadronEnergyFrac"),
    jetcEMEnergyLabel       = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"chargedEmEnergyFrac"),
    jetcMultipLabel         = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"chargedMultiplicity"),
    jetnMultipLabel         = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"neutralMultiplicity"),
    jetMuonEnergyLabel      = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"MuonEnergy"),
    jetYLabel               = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"Y"),
    jetAreaLabel            = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"jetArea"),
    jetGenJetPtLabel        = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"GenJetPt"),
    jetGenJetEtaLabel       = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"GenJetEta"),
    jetGenJetPhiLabel       = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"GenJetPhi"),
    jetGenJetELabel         = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"GenJetE"),
    jetGenJetChargeLabel    = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"GenJetCharge"),
    jettau1Label            = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"tau1"+jetType), 
    jettau2Label            = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"tau2"+jetType),  
    jettau3Label            = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"tau3"+jetType),  
    jetPrunedMassLabel      = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"prunedMass"+jetType),
    jetTrimmedMassLabel     = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"trimmedMass"+jetType),
    jetFilteredMassLabel    = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"filteredMass"+jetType),
    jetSoftDropMassLabel    = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"softDropMass"+jetType),
    vjetsjIdx0Label         = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"vSubjetIndex0"),  
    vjetsjIdx1Label         = cms.InputTag("jetsAK8"+jetType, "jetAK8"+jetType+"vSubjetIndex1"),  
    jetPtLabel              = cms.InputTag("subjetsAK8"+jetType, "subjetAK8"+jetType+"Pt"),
    jetEtaLabel             = cms.InputTag("subjetsAK8"+jetType, "subjetAK8"+jetType+"Eta"),
    jetPhiLabel             = cms.InputTag("subjetsAK8"+jetType, "subjetAK8"+jetType+"Phi"),
    jetEnergyLabel          = cms.InputTag("subjetsAK8"+jetType, "subjetAK8"+jetType+"E"),
    jetJECLabel             = cms.InputTag("subjetsAK8"+jetType, "subjetAK8"+jetType+"jecFactor0"),
    jetCSVLabel             = cms.InputTag("subjetsAK8"+jetType, "subjetAK8"+jetType+"CSVv2"),
    jetCMVALabel            = cms.InputTag("subjetsAK8"+jetType, "subjetAK8"+jetType+"CMVAv2"),
    jetPartonFlavourLabel   = cms.InputTag("subjetsAK8"+jetType, "subjetAK8"+jetType+"PartonFlavour"),
    jetHadronFlavourLabel   = cms.InputTag("subjetsAK8"+jetType, "subjetAK8"+jetType+"HadronFlavour"),
    jetGenJetPtLabel        = cms.InputTag("subjetsAK8"+jetType, "subjetAK8"+jetType+"GenJetPt"),
    jetGenJetEtaLabel       = cms.InputTag("subjetsAK8"+jetType, "subjetAK8"+jetType+"GenJetEta"),
    jetGenJetPhiLabel       = cms.InputTag("subjetsAK8"+jetType, "subjetAK8"+jetType+"GenJetPhi"),
    jetGenJetELabel         = cms.InputTag("subjetsAK8"+jetType, "subjetAK8"+jetType+"GenJetE"),
    jetGenJetChargeLabel    = cms.InputTag("subjetsAK8"+jetType, "subjetAK8"+jetType+"GenJetCharge"),
)
largeRJetSoftDropLabels = cms.PSet(
    jetPtLabel     = cms.InputTag("genJetsAK8SoftDrop", "genJetsAK8SoftDropPt"),    
    jetEtaLabel    = cms.InputTag("genJetsAK8SoftDrop", "genJetsAK8SoftDropEta"),    
    jetPhiLabel    = cms.InputTag("genJetsAK8SoftDrop", "genJetsAK8SoftDropPhi"),    
    jetEnergyLabel = cms.InputTag("genJetsAK8SoftDrop", "genJetsAK8SoftDropE"),    
    jetChargeLabel = cms.InputTag("genJetsAK8SoftDrop", "genJetsAK8SoftDropCharge"),    
    jetPtLabel     = cms.InputTag("genJetsAK8SoftDropSubjets", "genJetsAK8SoftDropSubJetsPt"),
    jetEtaLabel    = cms.InputTag("genJetsAK8SoftDropSubjets", "genJetsAK8SoftDropSubJetsEta"),
    jetPhiLabel    = cms.InputTag("genJetsAK8SoftDropSubjets", "genJetsAK8SoftDropSubJetsPhi"),
    jetEnergyLabel = cms.InputTag("genJetsAK8SoftDropSubjets", "genJetsAK8SoftDropSubJetsE"),
    jetChargeLabel = cms.InputTag("genJetsAK8SoftDropSubjets", "genJetsAK8SoftDropSubJetsCharge"),
)

## THE END ##

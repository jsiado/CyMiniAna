#ifndef ELECTRONS_H
#define ELECTRONS_H

#include <vector>
#include <iostream>

#include "TROOT.h"
#include "TFile.h"
#include "TH2D.h"
#include "TLorentzVector.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Analysis/CyMiniAna/interface/physicsObjects.h"
#include "Analysis/CyMiniAna/interface/objectSelection.h"

// Electrons Class
class Electrons {
  public:
    // Constructor
    explicit Electrons(edm::ParameterSet const& iConfig, edm::ConsumesCollector && iC);

    // Destructor
    virtual ~Electrons();

    // Container of electrons (defined in interface/physicsObjects.h)
    std::vector<Electron> execute(const edm::Event& evt, const objectSelection& obj);

    void getSF( Electron& el ) const;
    void setElectronID( Electron& el ) const;
    bool electronISO( const Electron& el, const std::string& ID ) const;

  private:

    // Physics information
    std::vector<Electron> m_electrons;

    // SFs for ID and RECO
    std::vector<std::string> m_listOfSFs = {"veto","loose","medium","tight","reco"};

    // Setup physics information from EDMntuples
    edm::ParameterSet m_labels;

    // ************
    // Setup to read EDMntuple format
    // -- following this example:
    //    https://github.com/dmajumder/VLQAna
    // ----------member data ---------------------------
    edm::Handle<std::vector<float>> h_elCharge;
    edm::Handle<std::vector<float>> h_elDxy;
    edm::Handle<std::vector<float>> h_elDz;
    edm::Handle<std::vector<float>> h_elE;
    edm::Handle<std::vector<float>> h_elEta;
    edm::Handle<std::vector<float>> h_elHoE;
    edm::Handle<std::vector<float>> h_elRelIsoEA;
    edm::Handle<std::vector<float>> h_elKey;
    edm::Handle<std::vector<float>> h_elPhi;
    edm::Handle<std::vector<float>> h_elPt;
    edm::Handle<std::vector<float>> h_eldEtaIn;
    edm::Handle<std::vector<float>> h_eldEtaInSeed;
    edm::Handle<std::vector<float>> h_eldPhiIn;
    edm::Handle<std::vector<float>> h_elfull5x5siee;
    edm::Handle<std::vector<float>> h_elhasMatchedConVeto;
    edm::Handle<std::vector<float>> h_elvidLoose;
    edm::Handle<std::vector<float>> h_elvidMedium;
    edm::Handle<std::vector<float>> h_elvidTight;
    edm::Handle<std::vector<float>> h_elvidVeto;
    edm::Handle<std::vector<float>> h_elvidHEEP;
    edm::Handle<std::vector<float>> h_elmissHits;
    edm::Handle<std::vector<float>> h_elooEmooP;
    edm::Handle<std::vector<float>> h_elscEta;

    // Tokens
    edm::EDGetTokenT<std::vector<float>> t_elCharge;
    edm::EDGetTokenT<std::vector<float>> t_elDxy;
    edm::EDGetTokenT<std::vector<float>> t_elDz;
    edm::EDGetTokenT<std::vector<float>> t_elE;
    edm::EDGetTokenT<std::vector<float>> t_elEta;
    edm::EDGetTokenT<std::vector<float>> t_elHoE;
    edm::EDGetTokenT<std::vector<float>> t_elRelIsoEA;
    edm::EDGetTokenT<std::vector<float>> t_elKey;
    edm::EDGetTokenT<std::vector<float>> t_elPhi;
    edm::EDGetTokenT<std::vector<float>> t_elPt;
    edm::EDGetTokenT<std::vector<float>> t_eldEtaIn;
    edm::EDGetTokenT<std::vector<float>> t_eldEtaInSeed;
    edm::EDGetTokenT<std::vector<float>> t_eldPhiIn;
    edm::EDGetTokenT<std::vector<float>> t_elfull5x5siee;
    edm::EDGetTokenT<std::vector<float>> t_elhasMatchedConVeto;
    edm::EDGetTokenT<std::vector<float>> t_elvidLoose;
    edm::EDGetTokenT<std::vector<float>> t_elvidMedium;
    edm::EDGetTokenT<std::vector<float>> t_elvidTight;
    edm::EDGetTokenT<std::vector<float>> t_elvidVeto;
    edm::EDGetTokenT<std::vector<float>> t_elvidHEEP;
    edm::EDGetTokenT<std::vector<float>> t_elmissHits;
    edm::EDGetTokenT<std::vector<float>> t_elooEmooP;
    edm::EDGetTokenT<std::vector<float>> t_elscEta;


    // -- different values for each ID if the electron is in the barrel or endcap
    // -- each map represents
    //    variable = { ID, { {barrel=true,value}, {barrel=false,value} },...}
    // -- to quickly access the correct value for a variable given the ID (veto,loose,etc.)
    // -- and if the electron is in the barrel or endcap
    double m_barrel_eta = 1.479;  // parameters depend on detector region
    bool m_applyIso;

    typedef std::map<std::string,std::map<bool,float>> valueMap;
    typedef std::map<std::string,std::map<bool,int>> valueMapInt;

    const valueMap m_dEtaInSeed = {
        {"veto",   { {true,0.00895}, {false,0.00749} } },
        {"tight",  { {true,0.00605}, {false,0.00308} } },
        {"loose",  { {true,0.00868}, {false,0.00477} } },
        {"medium", { {true,0.00609}, {false,0.00311} } },
    };
    const valueMap m_dPhiIn = {
        {"veto",   { {true,0.213}, {false,0.228} } },
        {"tight",  { {true,0.0394}, {false,0.0816} } },
        {"loose",  { {true,0.213}, {false,0.222} } },
        {"medium", { {true,0.045}, {false,0.103} } },
    };
    const valueMap m_full5x5siee = {
        {"veto",   { {true,0.037}, {false,0.0115} } },
        {"tight",  { {true,0.0292}, {false,0.00998} } },
        {"loose",  { {true,0.0314}, {false,0.011} } },
        {"medium", { {true,0.0298}, {false,0.00998} } },
    };
    const valueMap m_HoE = {
        {"veto",   { {true,0.211},  {false,0.356} } },
        {"tight",  { {true,0.0641}, {false,0.0414} } },
        {"loose",  { {true,0.101},  {false,0.298} } },
        {"medium", { {true,0.0878}, {false,0.253} } },
    };
    const valueMap m_Dxy = {
        {"veto",   { {true,0.10}, {false,0.05} } },
        {"tight",  { {true,0.10}, {false,0.05} } },
        {"loose",  { {true,0.10}, {false,0.05} } },
        {"medium", { {true,0.10}, {false,0.05} } },
    };
    const valueMap m_Dz = {
        {"veto",   { {true,0.20}, {false,0.10} } },
        {"tight",  { {true,0.20}, {false,0.10} } },
        {"loose",  { {true,0.20}, {false,0.10} } },
        {"medium", { {true,0.20}, {false,0.10} } },
    };
    const valueMap m_ooEmooP = {
        {"veto",   { {true,0.15},   {false,0.299} } },
        {"tight",  { {true,0.0129}, {false,0.0129} } },
        {"loose",  { {true,0.14},   {false,0.241} } },
        {"medium", { {true,0.13},   {false,0.134} } },
    };
    const valueMap m_RelIsoEA = {
        {"veto",   { {true,0.159},  {false,0.175} } },
        {"tight",  { {true,0.0571}, {false,0.0588} } },
        {"loose",  { {true,0.107},  {false,0.0994} } },
        {"medium", { {true,0.0821}, {false,0.0695} } },
    };
    const valueMapInt m_missHits = {
        {"veto",   { {true,3}, {false,2} } },
        {"tight",  { {true,1}, {false,1} } },
        {"loose",  { {true,1}, {false,1} } },
        {"medium", { {true,1}, {false,1} } },
    };

};

#endif

#ifndef OBJECTSELECTION_H
#define OBJECTSELECTION_H

#include <vector>
#include <iostream>

#include "TROOT.h"
#include "TLorentzVector.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Analysis/CyMiniAna/interface/physicsObjects.h"



// Object Selection Class
class objectSelection {
  public:

    // Constructor
    objectSelection(edm::ParameterSet const& cfg);  //const edm::ParameterSet& cfg);

    // Destructor
    ~objectSelection();

    void initialize();

    bool pass( const Electron& el, bool isGen=false ) const;
    bool pass( const Muon& mu,     bool isGen=false ) const;
    bool pass( const Neutrino& nu, bool isGen=false ) const;
    bool pass( const Jet& jet,     bool isGen=false ) const;
    bool pass( const Ljet& ljet,   bool isGen=false ) const;

    // ID and Isolation Functions
    bool electronID( const Electron& el ) const;
    bool applyElectronIsolation() const;

    bool muonID( const Muon& mu ) const;
    bool muonISO( const Muon& mu ) const;

    bool jetID( const Jet& jet ) const;
    bool ljetID( const Ljet& ljet ) const;

  private:

    const edm::ParameterSet* m_config;

    // Electron
    bool m_applyIso;
    double m_el_pt;
    double m_el_eta;
    std::string m_el_ID;
    std::string m_el_ISO;

    // Muon
    double m_mu_pt;
    double m_mu_eta;
    std::string m_mu_ID;
    std::string m_mu_ISO;
    double m_mu_IsoMin;
    double m_mu_IsoMax;

    // Neutrino
    double m_nu_pt;
    double m_nu_eta;

    // Jets
    double m_jet_pt;
    double m_jet_eta;
    std::string m_jet_ID; // quality

    // Large-R Jets
    double m_ljet_pt;
    double m_ljet_eta;
    unsigned int m_ljet_nsubjets=2;
    std::string m_ljet_ID; // tagging, substructure



    // ID/Isolation values

    // ELECTRONS
    // -- different values for each ID if the electron is in the barrel or endcap
    // -- each map represents
    //    variable = { ID, { {barrel=true,value}, {barrel=false,value} },...}
    // -- to quickly access the correct value for a variable given the ID (veto,loose,etc.)
    // -- and if the electron is in the barrel or endcap
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
// THE END

#ifndef OBJECTSELECTION_H
#define OBJECTSELECTION_H

#include <vector>
#include <iostream>

#include "TROOT.h"
#include "TLorentzVector.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Analysis/CyMiniAna/interface/physicsObjects.h"



// Object Selection Class
class objectSelection {
  public:

    // Constructor
    explicit objectSelection(const ParameterSet& cfg);

    // Destructor
    virtual ~objectSelection();

    void initialize(const ParameterSet & cfg);

    bool pass( const Electron& el, bool isGen=false );
    bool pass( const Muon& mu,     bool isGen=false );
    bool pass( const Neutrino& nu, bool isGen=false );
    bool pass( const Jet& jet,     bool isGen=false );
    bool pass( const Ljet& ljet,   bool isGen=false );

  private:

    edm::ParameterSet* m_config;

    m_lep_iso_map = {"loose": lep_iso::LOOSE, "medium": lep_iso::MEDIUM, "tight": lep_iso::TIGHT};
    m_lep_id_map  = {"veto":lep_id::VETO, "loose": lep_id::LOOSE, 
                     "medium": lep_id::MEDIUM,  "tight": lep_id::TIGHT};
    m_jet_id_map  = {"loose": jet_id::LOOSE,  "medium": jet_id::MEDIUM,  "tight": jet_id::TIGHT};

    // Electron
    float m_barrel_eta = 1.479;  // parameters depend on detector region
    float m_el_pt;
    float m_el_eta;
    lep_id m_el_ID;
    lep_iso m_el_ISO;

    // Muon
    float m_mu_pt;
    float m_mu_eta;
    lep_id m_mu_ID;
    lep_iso m_mu_ISO;
    float m_mu_IsoMin;
    float m_mu_IsoMax;

    // Neutrino
    float m_nu_pt;
    float m_nu_eta;

    // Jets
    float m_jet_pt;
    float m_jet_eta;
    jet_id m_jet_ID; // quality

    // Large-R Jets
    float m_ljet_pt;
    float m_ljet_eta;
    jet_id m_ljet_ID; // tagging, substructure



    // Tables of values
    // -- different values for each ID if the electron is in the barrel or endcap
    typedef std::map<lep_id,std::map<bool,float>> valueMap;
    typedef std::map<lep_id,std::map<bool,int>> valueMapInt;

    valueMap m_dEtaInSeed = {
        {lep_id::VETO,   { {true,0.00895}, {false,0.00749} } },
        {lep_id::TIGHT,  { {true,0.00605}, {false,0.00308} } },
        {lep_id::LOOSE,  { {true,0.00868}, {false,0.00477} } },
        {lep_id::MEDIUM, { {true,0.00609}, {false,0.00311} } },
    };
    valueMap m_dPhiIn = {
        {lep_id::VETO,   { {true,0.213}, {false,0.228} } },
        {lep_id::TIGHT,  { {true,0.0394}, {false,0.0816} } },
        {lep_id::LOOSE,  { {true,0.213}, {false,0.222} } },
        {lep_id::MEDIUM, { {true,0.045}, {false,0.103} } },
    };
    valueMap m_full5x5siee = {
        {lep_id::VETO,   { {true,0.037}, {false,0.0115} } },
        {lep_id::TIGHT,  { {true,0.0292}, {false,0.00998} } },
        {lep_id::LOOSE,  { {true,0.0314}, {false,0.011} } },
        {lep_id::MEDIUM, { {true,0.0298}, {false,0.00998} } },
    };
    valueMap m_HoE = {
        {lep_id::VETO,   { {true,0.211},  {false,0.356} } },
        {lep_id::TIGHT,  { {true,0.0641}, {false,0.0414} } },
        {lep_id::LOOSE,  { {true,0.101},  {false,0.298} } },
        {lep_id::MEDIUM, { {true,0.0878}, {false,0.253} } },
    };
    valueMap m_Dxy = {
        {lep_id::VETO,   { {true,0.10}, {false,0.05} } },
        {lep_id::TIGHT,  { {true,0.10}, {false,0.05} } },
        {lep_id::LOOSE,  { {true,0.10}, {false,0.05} } },
        {lep_id::MEDIUM, { {true,0.10}, {false,0.05} } },
    };
    valueMap m_Dz = {
        {lep_id::VETO,   { {true,0.20}, {false,0.10} } },
        {lep_id::TIGHT,  { {true,0.20}, {false,0.10} } },
        {lep_id::LOOSE,  { {true,0.20}, {false,0.10} } },
        {lep_id::MEDIUM, { {true,0.20}, {false,0.10} } },
    };
    valueMap m_ooEmooP = {
        {lep_id::VETO,   { {true,0.15},   {false,0.299} } },
        {lep_id::TIGHT,  { {true,0.0129}, {false,0.0129} } },
        {lep_id::LOOSE,  { {true,0.14},   {false,0.241} } },
        {lep_id::MEDIUM, { {true,0.13},   {false,0.134} } },
    };
    valueMap m_RelIsoEA = {
        {lep_id::VETO,   { {true,0.159},  {false,0.175} } },
        {lep_id::TIGHT,  { {true,0.0571}, {false,0.0588} } },
        {lep_id::LOOSE,  { {true,0.107},  {false,0.0994} } },
        {lep_id::MEDIUM, { {true,0.0821}, {false,0.0695} } },
    };
    valueMapInt m_missHits = {
        {lep_id::VETO,   { {true,3}, {false,2} } },
        {lep_id::TIGHT,  { {true,1}, {false,1} } },
        {lep_id::LOOSE,  { {true,1}, {false,1} } },
        {lep_id::MEDIUM, { {true,1}, {false,1} } },
    };
}
// THE END
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
    explicit objectSelection();

    // Destructor
    virtual ~objectSelection();

    void initialize(const ParameterSet & cfg);

    bool pass( const CmaBase& ob );
    bool pass( const Electron& el, bool isGen=false );
    bool pass( const Muon& mu,     bool isGen=false );
    bool pass( const Neutrino& nu, bool isGen=false );
    bool pass( const Jet& jet,     bool isGen=false );
    bool pass( const Ljet& ljet,   bool isGen=false );

  private:

    enum class el_id {LOOSE, MEDIUM, TIGHT};
    enum class el_iso {LOOSE, MEDIUM, TIGHT};

    enum class mu_id {LOOSE, MEDIUM, TIGHT};
    enum class jet_id {LOOSE, MEDIUM, TIGHT};
    enum class ljet_id {LOOSE, MEDIUM, TIGHT};

    float m_el_pt;
    float m_el_eta;
    float m_el_ID;
    float m_el_ISO;

    // Muon
    float m_mu_pt;
    float m_mu_eta;
    float m_mu_ID;
    float m_mu_ISO;

    // Neutrino
    float m_nu_pt;
    float m_nu_eta;

    // Jets
    float m_jet_pt;
    float m_jet_eta;
    float m_jet_ID;
    float m_jet_ISO; // quality

    // Large-R Jets
    float m_ljet_pt;
    float m_ljet_eta;
    float m_ljet_ID; // tagging, substructure

}
// THE END
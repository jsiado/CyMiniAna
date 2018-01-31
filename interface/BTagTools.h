#ifndef BTAGTOOLS_H
#define BTAGTOOLS_H

#include <vector>
#include <iostream>

#include "TROOT.h"
#include "TLorentzVector.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondFormats/BTauObjects/interface/BTagCalibration.h"
#include "CondTools/BTau/interface/BTagCalibrationReader.h"

#include "Analysis/CyMiniAna/interface/physicsObjects.h"

// BTagTools Class
class BTagTools {
  public:
    // Constructor
    explicit BTagTools(bool isBoosted=false);

    // Destructor
    virtual ~BTagTools();

    std::map<std::string,double> execute(const Jet& jet) const;
    void getBtagDecisions(Jet& jet) const;

  private:

    // b-tagging SF
    // setup calibration + reader
    BTagCalibration *m_calib;             // ("CSVv2", ".csv");
    BTagCalibrationReader *m_btag_reader; //(BTagEntry::OP_LOOSE,  // operating point
                                          //"central",            // central sys type
                                          //{"up", "down"});      // other sys types

    bool m_isBoosted;  // Boosted or AK4 jet scenarios

    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation80XReReco
    float m_CSVv2L=0.5426;
    float m_CSVv2M=0.8484;
    float m_CSVv2T=0.9535;
};

#endif

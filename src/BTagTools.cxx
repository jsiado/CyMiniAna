/*
Created:        31 January 2018
Last Updated:   31 January 2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University

-----

Interface with btagging tools to get SFs and uncertainties
*/
#include "Analysis/CyMiniAna/interface/BTagTools.h"


BTagTools::BTagTools(bool isBoosted) : 
  m_isBoosted(isBoosted){
    /* Setup btagging tools
       CSV files downloaded on 31Jan2018 from
        https://twiki.cern.ch/twiki/bin/view/CMS/BtagRecommendation80XReReco#Boosted_event_topologies
    */
    std::string measurement_type("");
    if (m_isBoosted){
        m_calib = new BTagCalibration("CSVv2", /*"data/*/"subjet_CSVv2_Moriond17_B_H.csv");
        measurement_type = "lt";
    }
    else{
        m_calib = new BTagCalibration("CSVv2", /*"data/*/"CSVv2_Moriond17_B_H.csv");
        measurement_type = "mujets";
    }


    m_btag_reader = new BTagCalibrationReader(BTagEntry::OP_LOOSE,"central",{"up", "down"});
    m_btag_reader->load(*m_calib,             // calibration instance
                        BTagEntry::FLAV_B,    // btag flavour
                        measurement_type);    // measurement type
}

BTagTools::~BTagTools() {
    delete m_calib;
    delete m_btag_reader;
}


std::map<std::string,double> BTagTools::execute(const Jet& jet) const{
    /* Get the SF */
    BTagEntry::JetFlavor jetFlav;
    if (jet.true_flavor==5)      jetFlav = BTagEntry::FLAV_B;
    else if (jet.true_flavor==4) jetFlav = BTagEntry::FLAV_C;
    else jetFlav = BTagEntry::FLAV_UDSG;

    double btagSF    = m_btag_reader->eval_auto_bounds("central", jetFlav /*BTagEntry::FLAV_B*/, jet.p4.Eta(), jet.p4.Pt());
    double btagSF_UP = m_btag_reader->eval_auto_bounds("up",      jetFlav /*BTagEntry::FLAV_B*/, jet.p4.Eta(), jet.p4.Pt());
    double btagSF_DN = m_btag_reader->eval_auto_bounds("down",    jetFlav /*BTagEntry::FLAV_B*/, jet.p4.Eta(), jet.p4.Pt());

    std::map<std::string,double> SFs = {
                 {"central",btagSF},
                 {"up",     btagSF_UP},
                 {"down",   btagSF_DN},
    };

    return SFs;
}


void BTagTools::getBtagDecisions(Jet& jet) const{
    /* Check b-tagging for a jet */
    if (jet.CSVv2 >= m_CSVv2L){
        jet.isbtagged.at("L") = true;
        if (jet.CSVv2 >= m_CSVv2M){
            jet.isbtagged.at("M") = true;
            // tight WP only for AK4 jets
            if (!m_isBoosted && jet.CSVv2 >= m_CSVv2T){
                jet.isbtagged.at("T") = true;
            }
        }
    }

    return;
}

// THE END

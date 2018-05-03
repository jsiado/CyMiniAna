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


BTagTools::BTagTools(const bool& isBoosted, const std::string& data_path) :
  m_isBoosted(isBoosted),
  m_data_path(data_path){
    /* Setup btagging tools
       CSV files downloaded on 31Jan2018 from
        https://twiki.cern.ch/twiki/bin/view/CMS/BtagRecommendation80XReReco#Boosted_event_topologies
    */
    std::string measurement_type("");
    if (m_isBoosted){
        m_calib = new BTagCalibration("CSVv2", m_data_path+"subjet_CSVv2_Moriond17_B_H.csv");
        measurement_type = "lt";
    }
    else{
        m_calib = new BTagCalibration("CSVv2", m_data_path+"CSVv2_Moriond17_B_H.csv");
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


float BTagTools::getBTagSF(const std::vector<Jet>& jets) const{
    /* Calculate the event weight due to b-tagging 
       Method 1a: https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagSFMethods#1a_Event_reweighting_using_scale
       - accessed on 7 Feb 2018
       Efficiencies will be calculated following:
         https://github.com/rappoccio/usercode/blob/Dev_53x/EDSHyFT/plugins/BTaggingEffAnalyzer.cc
         https://github.com/rappoccio/usercode/blob/Dev_53x/EDSHyFT/test/bTaggingEfficiency/README.txt
    */
    float weight(1.0);
    float pMC(1.0);
    float pDATA(1.0);

    weight = pDATA/pMC;

    return weight;
}

// THE END

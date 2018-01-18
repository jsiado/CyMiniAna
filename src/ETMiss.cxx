/*
Created:        --
Last Updated:   18 December 2017

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University

-----

Build Electrons from EDMntuples
*/
#include "Analysis/CyMiniAna/interface/ETMiss.h"


using namespace edm; 


ETMiss::ETMiss(edm::ParameterSet const& iConfig, edm::ConsumesCollector && iC) : 
  m_labels(iConfig.getParameter<edm::ParameterSet>("METLabels")){
    m_useTruth = iConfig.getParameter<bool>("useTruth");
    t_metFullPhi = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("metFullPhiLabel"));
    t_metFullPt = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("metFullPtLabel"));
    t_metFullPx = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("metFullPxLabel"));
    t_metFullPy = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("metFullPyLabel"));
    t_metFulluncorPhi = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("metFulluncorPhiLabel"));
    t_metFulluncorPt = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("metFulluncorPtLabel"));
    t_metFulluncorSumEt = iC.consumes<std::vector<float>>(m_labels.getParameter<edm::InputTag>("metFulluncorSumEtLabel"));
    if (m_useTruth){
    }
}

ETMiss::~ETMiss() {}


MET ETMiss::execute(const edm::Event& evt, const objectSelection& obj){
    /* Build the MET */
    evt.getByToken(t_metFullPhi, h_metFullPhi);
    evt.getByToken(t_metFullPt,  h_metFullPt);
    evt.getByToken(t_metFullPx,  h_metFullPx); // should be accessible using pT and phi
    evt.getByToken(t_metFullPy,  h_metFullPy); // should be accessible using pT and phi
    evt.getByToken(t_metFulluncorPhi,   h_metFulluncorPhi);
    evt.getByToken(t_metFulluncorPt,    h_metFulluncorPt);
    evt.getByToken(t_metFulluncorSumEt, h_metFulluncorSumEt);

    float metFullPt = (h_metFullPt.product())->at(0);

    m_MET = {};
    m_MET.p4.SetPtEtaPhiM( metFullPt, 0, h_metFullPhi.product()->at(0), 0);
    m_MET.uncorrPt    = h_metFulluncorPt.product()->at(0);
    m_MET.uncorrPhi   = h_metFulluncorPhi.product()->at(0);
    m_MET.uncorrSumEt = h_metFulluncorSumEt.product()->at(0);

    return m_MET;
}


MET ETMiss::execute_truth(const edm::Event& evt, const objectSelection& obj){
    /* Build Generator MET */
    m_truth_MET = {};
    return m_truth_MET;
}

// THE END

/*
Created:        --
Last Updated:   25 April 2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University
-----

Tool for reconstructing the neutrino
- 1-lepton: Use W-mass constraint
*/
#include "Analysis/CyMiniAna/interface/neutrinoReco.h"


NeutrinoReco::NeutrinoReco( configuration& cmaConfig ) :
  m_config(&cmaConfig){
    m_nu = {};
    m_lepton = {};
    m_met = {};

    //TFile wmass_hists = TFile::Open("");       // sample directly from histogram
    //m_wmass_hist = (TH1D*)wmass_hists.Get("");
    r = new TRandom3();
    m_sampling = 10000;

    m_isImaginary = false;   // keep track of real/imaginary solutions
  }

NeutrinoReco::~NeutrinoReco() {
    delete r;
}


void NeutrinoReco::setObjects(Lepton& lepton, MET& met){
    /* Set lepton and MET from one function call */
    setLepton(lepton);
    setMET(met);

    return;
}

void NeutrinoReco::setLepton(Lepton& lepton){
    /* Set the lepton (easily test different methods without passing this repeatedly) */
    m_lepton = lepton;
    return;
}

void NeutrinoReco::setMET(MET& met){
    /* Set the MET (easily test different methods without passing this repeatedly) */
    m_met = met;
    return;
}


float NeutrinoReco::execute(const bool standard, const float wmass){
    /* Build the neutrino
       STANDARD:
       - Use the W-mass constraint
         See AN2015-107-v9 (Equation 3)
         > http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2015/107
         For imaginary solutions, take the real part
         For multiple real solutions, choose the smallest one
       SAMPLING:
       - Generate many values for the W mass and take median of resulting pz solutions
    */
    float pz(-999.);
    m_pz_solutions.clear();          // keep track of pz solutions (in case you want them all later)
    m_nu.p4.SetPtEtaPhiM(m_met.p4.Pt(),0.,m_met.p4.Phi(),0.);

    cma::DEBUG("NEUTRINORECO : Reconstructing the neutrino");
    if (standard){
        // choose the smallest solution
        nu_pz(wmass);
        pz = m_pz_solutions.at(0);
        if (m_pz_solutions.size()>1)
            pz = (m_pz_solutions.at(0)<m_pz_solutions.at(1)) ? m_pz_solutions.at(0) : m_pz_solutions.at(1);
    }
    else {
        sampling();
        pz = cma::median( m_pz_solutions );
    }

    return pz;
}


std::vector<float> NeutrinoReco::pzSolutions(){
    /* Return all the solutions to the user */
    return m_pz_solutions;
}


void NeutrinoReco::nu_pz(float wmass){
    /* Calculate the neutrino pz */
    float lepPt = m_lepton.p4.Pt();
    float nuPt  = m_nu.p4.Pt();

    float A = -1. * pow(lepPt,2);

    // standard neutrino pz from w-mass constraint
    float mu = 0.5 * pow(wmass,2) + (lepPt * nuPt);
    
    float B  = mu * m_lepton.p4.Pz();
    float C  = pow(mu,2) - pow(m_lepton.p4.E(),2) * pow(nuPt,2);

    float discriminant = pow(B,2) - A*C;      // radicand

    m_isImaginary = false;
    if (discriminant<0) {
        // Imaginary! Take the real part of the solution for pz
        m_isImaginary = true;
        m_pz_solutions.push_back(-B/A);
    }
    else {
        discriminant = sqrt(discriminant);
        float pz1 = (-B-discriminant) / A;
        float pz2 = (-B+discriminant) / A;

        m_pz_solutions.push_back(pz1);
        m_pz_solutions.push_back(pz2);
    }

    return;
}


void NeutrinoReco::sampling(){
    /* Sample the value of the W mass from Gaussian 
       Produce N pz solutions (use all real solutions, not just smallest)
       -> Final pz choice is the median of the distribution
    */
    for (unsigned int ww=0;ww<m_sampling;ww++){
        float tmp_wmass = r->Gaus(80.1,6.2);
        nu_pz(tmp_wmass);
    }

    return;
}

// THE END //

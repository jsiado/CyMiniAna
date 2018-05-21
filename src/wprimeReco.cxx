/*
Created:        --
Last Updated:   15 March 2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University

-----

Tool for performing reconstruction of Wprime->qQ system
*/
#include "Analysis/CyMiniAna/interface/wprimeReco.h"

WprimeReco::WprimeReco( configuration& cmaConfig ) :
  m_config(&cmaConfig){}

WprimeReco::~WprimeReco() {}


void WprimeReco::initialize(){
    return;
}


void WprimeReco::setLepton(Lepton& lepton){
    /* Set the lepton (easily test different methods without passing this repeatedly) */
    m_lepton = lepton;
    return;
}


void WprimeReco::setBtagJets(std::vector<int> bjets){
    /* Indices of b-tagged jets */
    m_btag_jets = bjets;
    return;
}


void WprimeReco::setJets(std::vector<Jet>& jets){
    /* Set the jets (easily test different methods without passing this repeatedly) */
    m_jets = jets;
    return;
}


void WprimeReco::setNeutrino(Neutrino& nu){
    /* Set the neutrino (easily test different methods without passing this repeatedly) */
    m_neutrino = nu;
    return;
}


Wprime WprimeReco::execute(){
    /* Reconstruct wprime system 
       - Constraints:
         In W'->bT->bbW->bblv, expect that the b energy and T energy are similar.
         Furthermore, the mass of the reconstructed Wprime should be greater than
           the mass of the reconstructed VLQ.
    */
    m_wprime = {};
    unsigned int nbtags = m_btag_jets.size();

    if (nbtags>=2){
        Jet bjet0 = m_jets.at( m_btag_jets.at(0) );
        Jet bjet1 = m_jets.at( m_btag_jets.at(1) );

        if (nbtags>2){
            for (unsigned int b=2; b<nbtags; b++){
                Jet thisjet = m_jets.at( m_btag_jets.at(b) );
                if (thisjet.p4.Pt() > bjet0.p4.Pt()){
                    bjet1 = bjet0;
                    bjet0 = thisjet;
                }
                else if (thisjet.p4.Pt() > bjet1.p4.Pt())
                    bjet1 = thisjet;
            }
        } // end getting the highest pT b-tagged jets for >2 b-tags

        getWprime( bjet0, bjet1 );
    } // 2 b-tags
    else if (nbtags==1){
        unsigned int jind = m_btag_jets.at(0);
        Jet bjet = m_jets.at(jind);                 // b-tagged jet

        Wprime tmp_wp;
        float tmp_Aenergy(100.);                    // initialize to number larger than asymmetry permits (-1,1)
        for (const auto& j : m_jets){
            if ( std::abs(j.index) == jind) continue;
            getWprime( bjet,j );                    // resets m_wprime
            if (m_wprime.A_energy < tmp_Aenergy){
                tmp_wp      = m_wprime;
                tmp_Aenergy = m_wprime.A_energy;
            }
        }
        m_wprime = tmp_wp;                          // reset m_wprime
    } // 1 b-tag
    else
        cma::DEBUG("WPRIMERECO : Not enough b-tags");

    return m_wprime;
}


void WprimeReco::getWprime( const Jet& j0, const Jet& j1 ){
    /* Use lepton, neutrino, & b-jets to get Wprime/VLQ system */
    Wprime Wprime0;
    Wprime Wprime1;
    VLQ VLQ0;
    VLQ VLQ1;

    // Test0: VLQ = bjet1+lepton+neutrino
    VLQ0.p4     = j1.p4 + m_lepton.p4 + m_neutrino.p4;
    Wprime0.p4  = j0.p4 + VLQ0.p4;
    Wprime0.vlq = VLQ0;
    Wprime0.jet = j0;
    Wprime0.A_energy = (VLQ0.p4.E() - j0.p4.E()) / (VLQ0.p4.E() + j0.p4.E());
    bool massComp0   = (Wprime0.p4.M() > VLQ0.p4.M());

    // Test1: VLQ = j0+lepton+neutrino
    VLQ1.p4     = j0.p4 + m_lepton.p4 + m_neutrino.p4;
    Wprime1.p4  = j1.p4 + VLQ1.p4;
    Wprime1.vlq = VLQ1;
    Wprime1.jet = j1;
    Wprime1.A_energy = (VLQ1.p4.E() - j1.p4.E()) / (VLQ1.p4.E() + j1.p4.E());
    bool massComp1   = (Wprime1.p4.M() > VLQ1.p4.M());

    // Set the Wprime system
    if (massComp0 && !massComp1)
        m_wprime = Wprime0;
    else if (massComp1 && !massComp0)
        m_wprime = Wprime1;
    else if (massComp0 && massComp1){
        // Use the solution with the smallest energy asymmetry
        m_wprime = (Wprime0.A_energy < Wprime1.A_energy) ? Wprime0 : Wprime1;
    }

    return;
}


// THE END //

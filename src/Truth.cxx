/*
Created:         7 February 2018
Last Updated:    7 February 2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University

-----

Build Truth information from EDMntuples
*/
#include "Analysis/CyMiniAna/interface/Truth.h"


using namespace edm; 


Truth::Truth(edm::ParameterSet const& iConfig, edm::ConsumesCollector && iC) : 
  t_genParticles(iC.consumes<std::vector<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("genParticles"))){
}

Truth::~Truth() {}


std::vector<Parton> Truth::execute(const edm::Event& evt){
    /* Build the truth partons */
    m_truth.clear();

    evt.getByToken(t_genParticles, h_genParticles);

    // create temporary vector of genParticles
    std::vector<reco::GenParticle> genParticles;

    unsigned int j(0);
    for ( const auto& igen : *(h_genParticles.product())) {
        unsigned int absPdgId = std::abs( igen.pdgId() );
        int parent_pdgId(0);
        if (igen.numberOfMothers()>0 && igen.mother(0)!=nullptr)
            parent_pdgId = igen.mother(0)->pdgId();

        // Check that this particle has a PDGID of interest, or that its parent does
        if ( std::find(m_goodIDs.begin(), m_goodIDs.end(), absPdgId) == m_goodIDs.end() && 
             std::find(m_goodIDs.begin(), m_goodIDs.end(), std::abs(parent_pdgId)) == m_goodIDs.end() ) 
            continue;
        genParticles.push_back( igen );
        j++;
    }

    m_truth.resize(j);
    unsigned int i(0);
    // Use genParticles to create vector of Parton with references to parent/child
    for (const auto& gen : genParticles){
        int pdgId(gen.pdgId());

        Parton p;

        float eta(gen.eta());
        if (std::abs(eta)>10) continue;  //  protect against ROOT errors

        p.p4.SetPtEtaPhiM( gen.pt(), eta, gen.phi(), gen.mass() );
        p.charge = gen.charge();
        p.pdgId = pdgId;
        p.index = j;
        p.parent0_idx = -1;
        p.parent1_idx = -1;
        p.child0_idx  = -1;
        p.child1_idx  = -1;

        unsigned int initial_index(0);
        unsigned int last_index(0);
        // find parents
        if (gen.numberOfMothers()>0 && gen.mother(0)!=nullptr){
            initial_index = 0;  // earlier in the decay chain than this particle
            last_index = i;
            p.parent0_idx = findPartonIndex( genParticles, *gen.mother(0), initial_index, last_index );

            if (gen.numberOfMothers()>1 && gen.mother(1)!=nullptr){
                p.parent1_idx = findPartonIndex( genParticles, *gen.mother(1), initial_index, last_index );
            } // end if >1 parents
        } // end if >0 parents

        // find children
        if (gen.numberOfDaughters()>0 && gen.daughter(0)!=nullptr) {
            initial_index = i;                   // later in the decay chain than this particle
            last_index = genParticles.size();
            p.child0_idx = findPartonIndex( genParticles, *gen.daughter(0), initial_index, last_index );

            if (gen.numberOfDaughters()>1 && gen.daughter(1)!=nullptr){
                p.child1_idx = findPartonIndex( genParticles, *gen.daughter(1), initial_index, last_index );
            } // end if >1 children
        } // end if >0 children



        // simple booleans for type
        unsigned int abs_pdgId = std::abs(p.pdgId);
        p.isTop = ( abs_pdgId==6 ) ? 1 : 0;
        p.isW   = ( abs_pdgId==24 ) ? 1 : 0;
        p.isLepton = ( abs_pdgId>=11 && abs_pdgId<=16 ) ? 1 : 0;
        p.isQuark  = ( abs_pdgId<7 ) ? 1 : 0;

        if (p.isLepton){
            p.isTau  = ( abs_pdgId==15 ) ? 1 : 0;
            p.isMuon = ( abs_pdgId==13 ) ? 1 : 0;
            p.isElectron = ( abs_pdgId==11 ) ? 1 : 0;
            p.isNeutrino = ( abs_pdgId==12 || abs_pdgId==14 || abs_pdgId==16 ) ? 1 : 0;
        }
        else if (p.isQuark){
            p.isLight  = ( abs_pdgId<5 ) ? 1 : 0;
            p.isBottom = ( abs_pdgId==5 ) ? 1 : 0;
        }

        m_truth.at(i) = p;
        i++;
    }

    return m_truth;
}


int Truth::findPartonIndex( const std::vector<reco::GenParticle>& items, const reco::Candidate& item, const unsigned int init_idx, const unsigned int last_idx) const{
    /* loop over particles (in the decay chain before this particle) to get mother(s) --
       match based on the address. (easier than checking many attributes?)
    */
    int p0_idx(-1);
    for (unsigned int p0=init_idx; p0<last_idx; p0++){
        // compare pdgId, status, charge
        if ( items.at(p0).charge() == item.charge() &&
             items.at(p0).status() == item.status() &&
             items.at(p0).pdgId()  == item.pdgId() ){
            p0_idx = p0;
            break;
        }
    } // end loop over truth particles

    return p0_idx;
}

// THE END

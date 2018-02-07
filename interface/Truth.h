#ifndef TRUTH_H
#define TRUTH_H

#include <vector>
#include <iostream>

#include "TROOT.h"
#include "TLorentzVector.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "Analysis/CyMiniAna/interface/physicsObjects.h"

// Truth Class
class Truth {
  public:
    // Constructor
    explicit Truth(edm::ParameterSet const& iConfig, edm::ConsumesCollector && iC);

    // Destructor
    virtual ~Truth();

    // Container of Parton (defined in interface/physicsObjects.h)
    std::vector<Parton> execute(const edm::Event& evt);

    // find index of parents/children by matching addresses
    int findPartonIndex( const std::vector<reco::GenParticle>& items, const reco::Candidate& item, const unsigned int init_idx, const unsigned int last_idx) const;

  private:

    // Physics information
    std::vector<Parton> m_truth;
    std::vector<unsigned int> m_goodIDs = {6,15,23,24,25,7000001,8000001,9900113};
    // Zprime 9900113
    // VLQ    7000001,8000001
    // Wprime

    // Setup physics information from EDMntuples

    // ************
    // Setup to read EDMntuple format
    // -- following this example:
    //    https://github.com/dmajumder/VLQAna
    // ----------member data ---------------------------
    edm::Handle<std::vector<reco::GenParticle>> h_genParticles;

    // Tokens
    edm::EDGetTokenT<std::vector<reco::GenParticle>> t_genParticles;
};

#endif

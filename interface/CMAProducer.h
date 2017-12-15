#ifndef EVENT_H
#define EVENT_H

#include <set>
#include <list>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TLorentzVector.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "Analysis/CyMiniAna/interface/CMAProducer.h"
#include "Analysis/CyMiniAna/interface/cmaEvent.h"


// CMAProducer Class
class CMAProducer : public edm::EDProducer {
  public:
    // Constructor
    explicit CMAProducer(const edm::ParameterSet&);

    // Destructor
    virtual ~CMAProducer();

  private:

    virtual void beginJob(const edm::EventSetup&);
    virtual void produce(edm::Event&, const edm::EventSetup&);
    virtual void endJob();

    // Setup physics information
    cmaEvent* m_event;

    edm::InputTag m_src;

    // ************
    // Setup to read EDMntuple format
    // -- following this example:
    //    https://github.com/dmajumder/VLQAna
    // ----------member data ---------------------------
    edm::EDGetTokenT<int>            t_runno;
    edm::EDGetTokenT<int>            t_lumisec;
    edm::EDGetTokenT<int>            t_evtno;
    edm::EDGetTokenT<bool>           t_isData;
    edm::EDGetTokenT<bool>           t_hltdecision;
    edm::EDGetTokenT<string>         t_evttype;
    edm::EDGetTokenT<double>         t_evtwtGen;
    edm::EDGetTokenT<double>         t_evtwtPV;
    edm::EDGetTokenT<double>         t_evtwtPVBG;
    edm::EDGetTokenT<double>         t_evtwtPVH;
    edm::EDGetTokenT<double>         t_evtwtPVLow;
    edm::EDGetTokenT<double>         t_evtwtPVHigh;
    edm::EDGetTokenT<unsigned>       t_npv;
    edm::EDGetTokenT<int>            t_npuTrue;
    edm::EDGetTokenT<double>         t_htHat;
    edm::EDGetTokenT<vector<int>>    t_lhewtids;
    edm::EDGetTokenT<vector<double>> t_lhewts;

    // Electron handles
    edm::Handle<std::vector<float>> h_elCharge;
    edm::Handle<std::vector<float>> h_elDxy;
    edm::Handle<std::vector<float>> h_elDz;
    edm::Handle<std::vector<float>> h_elE;
    edm::Handle<std::vector<float>> h_elEta;
    edm::Handle<std::vector<float>> h_elHoE;
    edm::Handle<std::vector<float>> h_elRelIsoEA;
    edm::Handle<std::vector<float>> h_elKey;
    edm::Handle<std::vector<float>> h_elPhi;
    edm::Handle<std::vector<float>> h_elPt;
    edm::Handle<std::vector<float>> h_eldEtaIn;
    edm::Handle<std::vector<float>> h_eldEtaInSeed;
    edm::Handle<std::vector<float>> h_eldPhiIn;
    edm::Handle<std::vector<float>> h_elfull5x5siee;
    edm::Handle<std::vector<float>> h_elhasMatchedConVeto;
    edm::Handle<std::vector<float>> h_elvidLoose;
    edm::Handle<std::vector<float>> h_elvidMedium;
    edm::Handle<std::vector<float>> h_elvidTight;
    edm::Handle<std::vector<float>> h_elvidVeto;
    edm::Handle<std::vector<float>> h_elvidHEEP;
    edm::Handle<std::vector<float>> h_elmissHits;
    edm::Handle<std::vector<float>> h_elooEmooP;
    edm::Handle<std::vector<float>> h_elscEta;
};

#endif

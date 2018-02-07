#ifndef EVENTSELECTION_H
#define EVENTSELECTION_H

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include <boost/dynamic_bitset.hpp>
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TRandom3.h"
#include "TF1.h"
#include "TMath.h"
#include "TLorentzVector.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Analysis/CyMiniAna/interface/tools.h"
#include "Analysis/CyMiniAna/interface/physicsObjects.h"
#include "Analysis/CyMiniAna/interface/objectSelection.h"


class eventSelection : public edm::EDFilter {
  public:
    // constructor and destructor
    explicit eventSelection(const edm::ParameterSet&);
    virtual ~eventSelection();

    // External access to information in this class
    virtual std::vector<std::string> cutNames();     // Return a vector of the cut names (for labeling bins in cutflow histograms)
    virtual unsigned int numberOfCuts();             // Return the number of cuts (for binning cutflow histograms)

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:

    virtual void beginJob() override;
    virtual bool filter(edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;
    virtual void beginRun(edm::Run const& run, edm::EventSetup const& es);

    // Run once at the start of the job to setup the cuts: called from beginJob()
    virtual void identifySelection();
    virtual void getCutNames();

    // Run for every event (in every systematic) that needs saving: called from filter()
    //virtual bool applySelection(Event &event, TH1D &cutflow, TH1D &cutflow_unweighted);

    // struct for holding information on a 'cut'
    //  ideally this could be extended so that cuts are parsed & written by code, not humans!
    struct Cut{
        std::string name;       // name of cut
        std::string comparison; // sign of cut (<,<=,>,>=,==,!=), and '<<' = window cut; '>>' = window veto
        float value;            // float value -- cast to int if needed
        float value_lower;      // float for lower bound of window cut
        float value_upper;      // float for upper bound of window cut
    };

    // cutflow histograms
    edm::Service<TFileService> m_fs;
    std::map<std::string,TH1D*> m_hists;     // map of histograms (cutflows, others)

    // cut information
    std::string m_level;     // useful if you want to define one 'family' of selections 
                             // e.g., ABCD method for selection='qcd'; m_level = 'A', or something 
    std::string m_selection;
    std::string m_cutsfile;
    unsigned int m_numberOfCuts;
    std::vector<std::string> m_cutflowNames;
    std::vector<Cut> m_cuts;

    // booleans for each selection
    bool m_preSelection;
    bool m_dummySelection;
    bool m_allHadDNNSelection;
    bool m_exampleSelection;
    bool m_example2Selection;

    std::vector<std::string> m_hltPaths;

    // Handles and Tokens
    edm::EDGetTokenT<std::vector<Electron>> t_electrons;
    edm::EDGetTokenT<std::vector<Muon>> t_muons;
    edm::EDGetTokenT<std::vector<Neutrino>> t_neutrinos;
    edm::EDGetTokenT<std::vector<Jet>> t_jets;
    edm::EDGetTokenT<std::vector<Ljet>> t_ljets;
    edm::EDGetTokenT<MET> t_met;
    edm::EDGetTokenT<double> t_HT;
    edm::EDGetTokenT<double> t_ST;
    edm::EDGetTokenT<std::vector<float>> t_trigBit;
    edm::InputTag t_trigName;

    edm::Handle<std::vector<Electron>> m_electrons;
    edm::Handle<std::vector<Muon>> m_muons;
    edm::Handle<std::vector<Neutrino>> m_neutrinos;
    edm::Handle<std::vector<Jet>> m_jets;
    edm::Handle<std::vector<Ljet>> m_ljets;
    edm::Handle<MET> m_met;
    edm::Handle<double> m_HT;
    edm::Handle<double> m_ST;
    edm::Handle<std::vector<float>> h_trigBit;
    edm::Handle<std::vector<std::string>> h_trigName;
};

#endif

#ifndef EVENTSELECTION_H
#define EVENTSELECTION_H

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TRandom3.h"
#include "TEfficiency.h"
#include "TF1.h"
#include "TMath.h"
#include "TRandom.h"
#include "TLorentzVector.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Analysis/CyMiniAna/interface/Event.h"
#include "Analysis/CyMiniAna/interface/configuration.h"
#include "Analysis/CyMiniAna/interface/objectSelection.h"


class eventSelection : public edm::EDFilter {
  public:
    // constructor and destructor
    explicit eventSelection(const edm::ParameterSet&);
    virtual ~eventSelection();

    // External access to information in this class
    virtual std::vector<std::string> cutNames();     // Return a vector of the cut names (for labeling bins in cutflow histograms)
    virtual unsigned int numberOfCuts();             // Return the number of cuts (for binning cutflow histograms)

  private:

    virtual void beginJob() override;
    virtual bool filter(edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;

    // Run once at the start of the job to setup the cuts: called from beginJob()
    virtual void identifySelection();
    virtual void getCutNames();

    // Run for every event (in every systematic) that needs saving: called from filter()
    virtual bool applySelection(Event &event, TH1D &cutflow, TH1D &cutflow_unweighted);

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
    bool m_dummySelection;
    bool m_allHadDNNSelection;
    bool m_exampleSelection;
    bool m_example2Selection;
};

#endif

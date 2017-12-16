/*
Created:        --
Last Updated:   14 December 2017

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University

-----

Event Selection script

*/
#include "Analysis/CyMiniAna/interface/eventSelection.h"



eventSelection::eventSelection(const edm::ParameterSet& cfg) :
  m_selection("SetMe"),
  m_cutsfile("SetMe"),
  m_numberOfCuts(0),
  m_dummySelection(false),
  m_allHadDNNSelection(false){
    m_cuts.resize(0);
    m_cutflowNames.clear();

    m_selection = cfg.getParameter<std::string>("selection");
    m_cutsfile  = cfg.getParameter<std::string>("cutsfile");
  }

eventSelection::~eventSelection() {}


void eventSelection::beginJob() {
    /* Setup the class by identifying the selection and cuts */
    // Identify the selection this instance will apply
    identifySelection();

    std::ifstream file = cma::open_file(m_cutsfile);

    // Read one line at a time into the vector of Cut structs:
    // this only stores information, but can be expanded
    m_cuts.clear();
    std::string line;
    if (file.is_open()){
        while(std::getline(file, line)){
            std::stringstream  lineStream(line);
            Cut tmp_cut;
            // read line
            lineStream >> tmp_cut.name >> tmp_cut.comparison >> tmp_cut.value;
            m_cuts.push_back(tmp_cut);
        }
        file.close();
    } // end reading cuts file

    // Get the number of cuts (for cutflow histogram binning)
    m_numberOfCuts = m_cuts.size();

    // Get the names of cuts (for cutflow histogram bin labeling)
    m_cutflowNames.clear();
    getCutNames();

    // Setup histograms
    m_hists["cutflow"]     = m_fs->make<TH1D>( (m_selection+"_cutflow").c_str(),           (m_selection+"_cutflow").c_str(),           m_numberOfCuts+1,0,m_numberOfCuts+1);
    m_hists["cutflow_unw"] = m_fs->make<TH1D>( (m_selection+"_cutflow_unweighted").c_str(),(m_selection+"_cutflow_unweighted").c_str(),m_numberOfCuts+1,0,m_numberOfCuts+1);

    m_hists["cutflow"]->GetXaxis()->SetBinLabel(1,"INITIAL");
    m_hists["cutflow_unw"]->GetXaxis()->SetBinLabel(1,"INITIAL");

    for (unsigned int c=1;c<=m_numberOfCuts;++c){
        m_hists["cutflow"]->GetXaxis()->SetBinLabel(c+1,m_cutflowNames.at(c-1).c_str());
        m_hists["cutflow_unw"]->GetXaxis()->SetBinLabel(c+1,m_cutflowNames.at(c-1).c_str());
    }

    return;
}


void eventSelection::identifySelection(){
    /* Set the booleans for applying the selection below */
    m_dummySelection     = false;
    m_allHadDNNSelection = false;

    // Define selections to be exclusive (if/else if), but this could be modified
    if (m_selection.compare("allHadDNN")==0) 
        m_allHadDNNSelection = true;
    else
        m_dummySelection = true;

    return;
}



bool eventSelection::filter(edm::Event& evt, const edm::EventSetup& iSetup) {
    /* Filter events based on a selection 
       Apply cuts 

       Two cutflows:  
         "cutflow"            event weights
         "cutflow_unweighted" no event weights -> raw event numbers

       Example Cut::
          if (n_jets==3 && n_ljets<1)  FAIL
          else :                       PASS & fill cutflows
    */
    bool passSelection(true);
    double first_bin(0.5);            // first bin value in cutflow histogram ("INITIAL")

    float nominal_weight(1.0);  // = event.nominal_weight();

    // fill cutflow histograms with initial value (before any cuts)
    m_hists["cutflow"]->Fill(first_bin,nominal_weight); // event weights
    m_hists["cutflow_unw"]->Fill( first_bin );          // raw event numbers

    // no selection applied
    if (m_dummySelection)
        passSelection = true;                          // event 'passed'  

    // selection for all-hadronic DNN 
    else if (m_allHadDNNSelection){
        std::vector<Jet> jets;// = event.jets();          // access some event information

        // cut0 :: >=1 jets 
        if ( jets.size()<1 )                           // check if the event passes the cut!
            passSelection = false;
        else{
            m_hists["cutflow"]->Fill(first_bin+1,nominal_weight);  // fill cutflow
            m_hists["cutflow_unw"]->Fill(first_bin+1);
            passSelection = true;
        }
    }

    return passSelection;
}


void eventSelection::getCutNames(){
    /* Get the cut names (for labeling bins in cutflow histograms) and store in vector */
    m_cutflowNames.clear();
    for (const auto& cut : m_cuts)
        m_cutflowNames.push_back( cut.name );

    return;
}

std::vector<std::string> eventSelection::cutNames(){
    /* Return a vector of the cut names (for labeling bins in cutflow histograms) */
    return m_cutflowNames;
}

unsigned int eventSelection::numberOfCuts(){
    /* Return the number of cuts (number of bins in cutflow histograms) */
    return m_numberOfCuts;
}

// the end

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
  m_preSelection(false),
  t_electrons(consumes<std::vector<Electron>>(edm::InputTag("CMAProducer","electrons","CyMiniAna"))),
  t_muons(consumes<std::vector<Muon>>(edm::InputTag("CMAProducer","muons","CyMiniAna"))),
  t_neutrinos(consumes<std::vector<Neutrino>>(edm::InputTag("CMAProducer","neutrinos","CyMiniAna"))),
  t_jets(consumes<std::vector<Jet>>(edm::InputTag("CMAProducer","jets","CyMiniAna"))),
  t_ljets(consumes<std::vector<Ljet>>(edm::InputTag("CMAProducer","ljets","CyMiniAna"))),
  t_met(consumes<MET>(edm::InputTag("CMAProducer","MET","CyMiniAna"))),
  t_HT(consumes<double>(edm::InputTag("CMAProducer","HT","CyMiniAna"))),
  t_ST(consumes<double>(edm::InputTag("CMAProducer","ST","CyMiniAna"))),
  t_trigBit(consumes<std::vector<float>>(cfg.getParameter<edm::InputTag>("trigBitLabel"))),
  t_trigName(cfg.getParameter<edm::InputTag>("trigNameLabel")){
    m_cuts.resize(0);
    m_cutflowNames.clear();

    m_selection = cfg.getParameter<std::string>("selection");
    m_cutsfile  = cfg.getParameter<std::string>("cutsfile");
    m_hltPaths  = cfg.getParameter<std::vector<std::string>>("HLTPaths");

    consumes<std::vector<std::string>, edm::InRun>(t_trigName);  // treat this with label rather than token
  }

eventSelection::~eventSelection() {}


void eventSelection::beginJob() {
    /* Setup the class by identifying the selection and cuts */
    identifySelection();  // identify the selection to apply in filter()

    // Read file with cuts one line at a time into the vector of 'Cut' structs:
    // this only stores information, but can be expanded later
    std::ifstream file = cma::open_file(m_cutsfile);
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

    // Cutflow histogram setup
    m_numberOfCuts = m_cuts.size();  // cutflow histogram binning
    m_cutflowNames.clear();          // cutflow histogram bin labeling
    getCutNames();

    m_hists["cutflow"]     = m_fs->make<TH1D>( (m_selection+"_cutflow").c_str(),           (m_selection+"_cutflow").c_str(),           m_numberOfCuts+1,0,m_numberOfCuts+1);
    m_hists["cutflow_unw"] = m_fs->make<TH1D>( (m_selection+"_cutflow_unweighted").c_str(),(m_selection+"_cutflow_unweighted").c_str(),m_numberOfCuts+1,0,m_numberOfCuts+1);

    m_hists["cutflow"]->GetXaxis()->SetBinLabel(1,"INITIAL");
    m_hists["cutflow_unw"]->GetXaxis()->SetBinLabel(1,"INITIAL");

    for (unsigned int c=1;c<=m_numberOfCuts;++c){
        m_hists["cutflow"]->GetXaxis()->SetBinLabel(c+1,m_cutflowNames.at(c-1).c_str());
        m_hists["cutflow_unw"]->GetXaxis()->SetBinLabel(c+1,m_cutflowNames.at(c-1).c_str());
    }
    
    // add more histograms to 'm_hists' if desired

    return;
}


void eventSelection::beginRun(edm::Run const& run, edm::EventSetup const& es){
    /* Start of each run */
    run.getByLabel(t_trigName, h_trigName);
    return;
}


void eventSelection::identifySelection(){
    /* Set the booleans for applying the selection below */
    m_preSelection = (m_selection.compare("pre")==0);

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
    evt.getByToken( t_electrons, m_electrons );
    evt.getByToken( t_muons, m_muons );
    evt.getByToken( t_neutrinos, m_neutrinos );
    evt.getByToken( t_jets, m_jets );
    evt.getByToken( t_ljets, m_ljets );
    evt.getByToken( t_met, m_met );
    evt.getByToken( t_HT,  m_HT );
    evt.getByToken( t_ST,  m_ST );
    evt.getByToken( t_trigBit,  h_trigBit);

    bool passSelection(true);         // use a bool rather than return after a failed cut
                                      // to examine all cuts in cutflow (make N-1 plots,
                                      // sequential plots)
    std::vector<bool> passSels;       // bookkeeping for each of the individual cuts
    double first_bin(0.5);            // first bin value in cutflow histogram ("INITIAL")
    double second_bin(1.5);           // second bin value in cutflow histogram (the first actual cut)

    float nominal_weight(1.0);
    if (!evt.isRealData()){
        // do something to get a proper weight (CMAProducer?)
    }

    // fill cutflow histograms with initial value (before any cuts)
    m_hists["cutflow"]->Fill(first_bin,nominal_weight); // event weights
    m_hists["cutflow_unw"]->Fill( first_bin );          // raw event numbers

    // no selection applied
    if (m_preSelection){
        passSels.resize(2);

        // Cut1 : Triggers
        const size_t ntrigs (m_hltPaths.size());
        boost::dynamic_bitset<> hltdecisions(std::max(int(ntrigs),1)); // true if any bits are set, otherwise false.

        for ( size_t i=0; i<ntrigs; ++i) {
            const std::string& myhltpath = m_hltPaths.at(i);
            std::vector<std::string>::const_iterator it;
            for (it = h_trigName.product()->begin(); it != (h_trigName.product())->end(); ++it ) {
                if ( it->find(myhltpath) < std::string::npos) {
                    hltdecisions[i] = (h_trigBit.product())->at( it - (h_trigName.product())->begin() );
                } // end if the trigger is in the path
            } // end loop over trigger name
        } // end loop over triggers

        passSels.at(0) = ( !hltdecisions.any() );   // if !any(), then all triggers failed


        // Cut2 : Exactly 1 lepton
        unsigned int NElectrons = (*m_electrons.product()).size();
        unsigned int NMuons     = (*m_muons.product()).size();

        passSels.at(1) = (NElectrons + NMuons != 1); // if this is more than 1, then we have too many leptons!



        // fill cutflows (sequentially)
        // break once a cut fails
        for (unsigned int c=0;c<m_numberOfCuts;c++){
            if (passSels.at(c)){
                m_hists["cutflow"]->Fill(second_bin+c,nominal_weight);  // fill cutflow
                m_hists["cutflow_unw"]->Fill(second_bin+c);             // first bin is "INITIAL"
            }
            else{
                passSelection = false;
                break;
            }
        }
    } // end pre-selection

    return passSelection;
}


void eventSelection::endJob(){
    /* Clean-up */
    return;
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

void eventSelection::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    /* Method fills 'descriptions' with the allowed parameters for the module
       The following says we do not know what parameters are allowed so do no validation
       Please change this to state exactly what you do use, even if it is no parameters
    */
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);

    return;
}

DEFINE_FWK_MODULE(eventSelection);
// the end

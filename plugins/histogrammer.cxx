/*
Created:        --
Last Updated:   14 December 2017

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University

-----

Make histograms for systematic uncertainties (& nominal) 
to go into plots || Combine
*/
#include "Analysis/CyMiniAna/interface/histogrammer.h"

using namespace edm;

histogrammer::histogrammer( const ParameterSet & cfg ) :
  //m_src( cfg.getUntrackedParameter<edm::InputTag>("src") ),
  m_putOverflowInLastBin(true),
  m_putUnderflowInFirstBin(true),
  t_electrons(consumes<std::vector<Electron>>(edm::InputTag("ana","electrons","CyMiniAna"))),
  t_muons(consumes<std::vector<Muon>>(edm::InputTag("ana","muons","CyMiniAna"))),
  t_neutrinos(consumes<std::vector<Neutrino>>(edm::InputTag("ana","neutrinos","CyMiniAna"))),
  t_jets(consumes<std::vector<Jet>>(edm::InputTag("ana","jets","CyMiniAna"))),
  t_ljets(consumes<std::vector<Ljet>>(edm::InputTag("ana","ljets","CyMiniAna"))),
  t_met(consumes<MET>(edm::InputTag("ana","MET","CyMiniAna"))),
  t_HT(consumes<double>(edm::InputTag("ana","HT","CyMiniAna"))),
  t_ST(consumes<double>(edm::InputTag("ana","ST","CyMiniAna"))){
    usesResource("TFileService");

    m_map_histograms1D.clear();
    m_map_histograms2D.clear();
    m_map_histograms3D.clear();

    // options set by config
    m_isMC           = cfg.getParameter<bool>("isMC");
    m_useJets        = cfg.getParameter<bool>("useJets");
    m_useLargeRJets  = cfg.getParameter<bool>("useLargeRJets");
    m_useLeptons     = cfg.getParameter<bool>("useLeptons");
    m_useNeutrinos   = cfg.getParameter<bool>("useNeutrinos");
    m_useSystWeights = cfg.getParameter<bool>("useSystWeights");

    // Obtain lists of systematics (treated as weights)
    std::string listOfWeightSystematics = cfg.getParameter<std::string>("weightSystematicsFile");
    cma::read_file( listOfWeightSystematics, m_listOfWeightSystematics);

    std::string weightVectorSystematicsFile = cfg.getParameter<std::string>("weightVectorSystematicsFile");
    std::vector<std::string> weightVectorSystematics;
    cma::read_file(weightVectorSystematicsFile,weightVectorSystematics);   // read config file and get systematics name

    for (const auto& weightVectorSystematic : weightVectorSystematics){
        // split config items by space
        std::istringstream cfg(weightVectorSystematic);
        std::istream_iterator<std::string> start(cfg), stop;
        std::vector<std::string> tokens(start, stop);

        m_mapOfWeightVectorSystematics.insert( std::pair<std::string,unsigned int>( tokens.at(0),std::stoi(tokens.at(1)) ) );
    }
}


histogrammer::~histogrammer() {}


/**** INITIALIZE HISTOGRAMS ****/
// -- 1D Histograms
void histogrammer::init_hist( const std::string& name, const unsigned int nBins, const double x_min, const double x_max ){
    /* Initialize histogram -- equal bins */
    m_map_histograms1D[name] = m_fs->make<TH1D>(name.c_str(), name.c_str(),nBins,x_min,x_max);
    m_map_histograms1D[name]->Sumw2();

    return;
}
void histogrammer::init_hist( const std::string& name, const unsigned int nBins, const double *xbins ){
    /* Initialize histogram -- variable bins */
    m_map_histograms1D[name] = m_fs->make<TH1D>(name.c_str(), name.c_str(),nBins,xbins);
    m_map_histograms1D[name]->Sumw2();

    return;
}
// -- 2D Histograms
void histogrammer::init_hist( const std::string& name, const unsigned int nBinsX, const double x_min, const double x_max,
                              const unsigned int nBinsY, const double y_min, const double y_max ){
    /* Initialize histogram -- equal bins */
    m_map_histograms2D[name] = m_fs->make<TH2D>(name.c_str(), name.c_str(),
                                        nBinsX,x_min,x_max,nBinsY,y_min,y_max);
    m_map_histograms2D[name]->Sumw2();

    return;
}
void histogrammer::init_hist( const std::string& name, const unsigned int nBinsX, const double *xbins,
                              const unsigned int nBinsY, const double *ybins ){
    /* Initialize histogram -- variable bins */
    m_map_histograms2D[name] = m_fs->make<TH2D>(name.c_str(), name.c_str(),
                                           nBinsX,xbins,nBinsY,ybins);
    m_map_histograms2D[name]->Sumw2();

    return;
}
// -- 3D Histograms
void histogrammer::init_hist( const std::string& name, const unsigned int nBinsX, const double x_min, const double x_max,
                              const unsigned int nBinsY, const double y_min, const double y_max,
                              const unsigned int nBinsZ, const double z_min, const double z_max ){
    /* Initialize histogram -- equal bins */
    m_map_histograms3D[name] = m_fs->make<TH3D>(name.c_str(), name.c_str(),
                                            nBinsX,x_min,x_max,nBinsY,y_min,y_max,nBinsZ,z_min,z_max);
    m_map_histograms3D[name]->Sumw2();

    return;
}
void histogrammer::init_hist( const std::string& name, const unsigned int nBinsX, const double *xbins,
                              const unsigned int nBinsY, const double *ybins,
                              const unsigned int nBinsZ, const double *zbins ){
    /* Initialize histogram -- variable bins */
    m_map_histograms3D[name] = m_fs->make<TH3D>(name.c_str(), name.c_str(),
                                           nBinsX,xbins,nBinsY,ybins,nBinsZ,zbins);
    m_map_histograms3D[name]->Sumw2();

    return;
}

void histogrammer::bookHists( const std::string name ){
    /* 
      Book histograms -- modify/inherit this function for analysis-specific hists 

      @param name   This is the string used to identify histograms for different systematics/event weights
    */
    if (m_useJets){
        init_hist("n_jets_"+name,   31, -0.5,  30.5);
        init_hist("n_btags_"+name,  11, -0.5,  10.5);

        init_hist("jet_pt_"+name,  500, 0.0, 2000);
        init_hist("jet_eta_"+name,  50, -2.5, 2.5);
        init_hist("jet_phi_"+name,  64, -3.2, 3.2);
        init_hist("jet_CSVv2_"+name, 200, -1,1);
    }

    if (m_useLargeRJets){
        init_hist("n_ljets_"+name,   11, -0.5,  10.5);

        init_hist("ljet_pt_"+name,  500, 0.0, 2000);
        init_hist("ljet_eta_"+name,  50, -2.5, 2.5);
        init_hist("ljet_phi_"+name,  64, -3.2, 3.2);
        init_hist("ljet_SDmass_"+name, 300,0.0,300);
    }

    if (m_useLeptons){
        init_hist("el_pt_"+name,  500, 0.0, 2000);
        init_hist("el_eta_"+name,  50, -2.5, 2.5);
        init_hist("el_phi_"+name,  64, -3.2, 3.2);
        init_hist("mu_pt_"+name,  500, 0.0, 2000);
        init_hist("mu_eta_"+name,  50, -2.5, 2.5);
        init_hist("mu_phi_"+name,  64, -3.2, 3.2);
    }

    if (m_useNeutrinos){
        init_hist("nu_pt_"+name,  500, 0.0, 2000);
        init_hist("nu_eta_"+name,  50, -2.5, 2.5);
        init_hist("nu_phi_"+name,  64, -3.2, 3.2);
    }

    // kinematics
    init_hist("met_met_"+name, 500,  0.0,  500);
    init_hist("met_phi_"+name, 6.4, -3.2,  3.2);
    init_hist("ht_"+name,     5000,  0.0, 5000);

    // DNN
    init_hist("dnn_"+name,  100, 0.0,   1.);

/*  VLQ/Wprime
    init_hist("top_pt_"+name,  2000, 0.0, 2000);
    init_hist("top_eta_"+name,   50,-2.5,  2.5);
    init_hist("top_phi_"+name,   64,-3.2,  3.2);
    init_hist("top_m_"+name,    200, 100,  300);
    init_hist("antitop_pt_"+name,  2000, 0.0, 2000);
    init_hist("antitop_eta_"+name,   50,-2.5,  2.5);
    init_hist("antitop_phi_"+name,   64,-3.2,  3.2);
    init_hist("antitop_m_"+name,    200, 100,  300);

    init_hist("mttbar_"+name,  5000, 0.0, 5000);
    init_hist("pTttbar_"+name,  300, 0.0,  300);
*/
    return;
}

void histogrammer::beginJob(){
    /* Setup some values and book histograms */
    bookHists( m_name );    // Book histograms for "nominal" events

    // weight systematics in the nominal tree
    if (m_isMC && m_useSystWeights){
        for (const auto& syst : m_listOfWeightSystematics){
            bookHists( m_name+syst );
        } // end weight systematics

        // vector weight systematics
        for (const auto& syst : m_mapOfWeightVectorSystematics){
            for (unsigned int el=0;el<syst.second;++el){
                std::string weightIndex = std::to_string(el);
                bookHists( m_name+weightIndex+"_"+syst.first );
            } // end components of vector
        } // end vector weight systematics
    } // end if MC and save weight systematics

    return;
}





/**** FILL HISTOGRAMS ****/

void histogrammer::fill( const std::string& name, const double& value, const double& weight ){
    /* TH1D */
    TH1D* this_hist = m_map_histograms1D.at(name);

    this_hist->Fill(value,weight);

    return;
}

void histogrammer::fill( const std::string& name, 
                         const double& xvalue, const double& yvalue, const double& weight ){
    /* TH2D */
    TH2D* this_hist = m_map_histograms2D.at(name);

    this_hist->Fill(xvalue,yvalue,weight);

    return;
}

void histogrammer::fill( const std::string& name, 
                         const double& xvalue, const double& yvalue, const double& zvalue, const double& weight ){
    /* TH3D */
    TH3D* this_hist = m_map_histograms3D.at(name);

    this_hist->Fill(xvalue,yvalue,zvalue,weight);

    return;
}


void histogrammer::fill( const std::string& name, const edm::Event& event, double event_weight){
    /* Fill histograms -- just use information from the event and fill histogram
       This is the function to modify / inherit for analysis-specific purposes
    */
    float HT(0.0);
    if (m_useJets){
        fill("n_jets_"+name,  (*m_jets.product()).size(), event_weight);
        fill("n_btags_"+name, 0, event_weight);

        for (const auto& jet : *m_jets.product()){
            fill("jet_pt_"+name,   jet.p4.Pt(),  event_weight);
            fill("jet_eta_"+name,  jet.p4.Eta(), event_weight);
            fill("jet_phi_"+name,  jet.p4.Phi(), event_weight);
            fill("jet_CSVv2_"+name, jet.CSVv2,   event_weight);
            HT+=jet.p4.Pt();
        }
    }

    if (m_useLargeRJets){
        fill("n_ljets_"+name,  (*m_ljets.product()).size(), event_weight);

        for (const auto& jet : *m_ljets.product()){
            fill("ljet_pt_"+name,   jet.p4.Pt(),  event_weight);
            fill("ljet_eta_"+name,  jet.p4.Eta(), event_weight);
            fill("ljet_phi_"+name,  jet.p4.Phi(), event_weight);
            fill("ljet_SDmass_"+name, jet.softDropMass,   event_weight);
        }
    }

    if (m_useLeptons){
        for (const auto& el : *m_electrons.product()){
            fill("el_pt_"+name,  el.p4.Pt(),  event_weight);
            fill("el_eta_"+name, el.p4.Eta(), event_weight);
            fill("el_phi_"+name, el.p4.Phi(), event_weight);
        }
        for (const auto& mu : *m_muons.product()){
            fill("mu_pt_"+name,  mu.p4.Pt(),  event_weight);
            fill("mu_eta_"+name, mu.p4.Eta(), event_weight);
            fill("mu_phi_"+name, mu.p4.Phi(), event_weight);
        }
    }

    if (m_useNeutrinos){
        for (const auto& nu : *m_neutrinos.product()){
            fill("nu_pt_"+name,  nu.p4.Pt(),  event_weight);
            fill("nu_eta_"+name, nu.p4.Eta(), event_weight);
            fill("nu_phi_"+name, nu.p4.Phi(), event_weight);
        }
    }

    // kinematics
    fill("ht_"+name,      HT /*(m_HT.product())*/, event_weight);
    fill("met_met_"+name, (*m_met.product()).p4.Phi(), event_weight);
    fill("met_phi_"+name, (*m_met.product()).p4.Pt(),  event_weight);

    // DNN
    fill("dnn_"+name, 1.0, event_weight); // N/A

    LogDebug("Fill VLQ/Wprime");
/*    fill("top_pt_"+name,  top.p4.Pt(),  event_weight);
    fill("top_eta_"+name, top.p4.Eta(), event_weight);
    fill("top_phi_"+name, top.p4.Phi(), event_weight);
    fill("top_m_"+name,   top.p4.M(),   event_weight);
    fill("antitop_pt_"+name,  antitop.p4.Pt(),  event_weight); 
    fill("antitop_eta_"+name, antitop.p4.Eta(), event_weight);
    fill("antitop_phi_"+name, antitop.p4.Phi(), event_weight);
    fill("antitop_m_"+name,   antitop.p4.M(),   event_weight);

    fill("mttbar_"+name,  (top.p4+antitop.p4).M(), event_weight);
    fill("pTttbar_"+name, (top.p4+antitop.p4).Pt(), event_weight);
*/
    LogDebug("End histograms");

    return;
}


void histogrammer::analyze( const edm::Event& event, const edm::EventSetup& ) {
    /* Fill histograms -- fill histograms based on treename or systematic weights ("nominal" but different weight)
       This is the function to modify / inherit for analysis-specific purposes
    */
    event.getByToken( t_electrons, m_electrons );
    event.getByToken( t_muons, m_muons );
    event.getByToken( t_neutrinos, m_neutrinos );
    event.getByToken( t_jets, m_jets );
    event.getByToken( t_ljets, m_ljets );
    event.getByToken( t_met, m_met );
    event.getByToken( t_HT,  m_HT );
    event.getByToken( t_ST,  m_ST );

    double event_weight(1.0);  // = event.nominal_weight();

    fill( m_name, event, event_weight );


    // if there are systematics stored as weights (e.g., b-tagging, pileup, etc.)
    // the following calls the fill() function with different event weights
    // to make histograms
    bool isNominal(true);
    if (m_isMC && isNominal && m_useSystWeights){
        // weight systematics
        event_weight = 1.0;
        for (const auto& syst : m_listOfWeightSystematics){
            event_weight = 1.0; //event.getSystEventWeight( syst );
            fill( m_name+syst, event, event_weight );
        } // end weight systematics

        // vector weight systematics
        event_weight = 1.0;
        for (const auto& syst : m_mapOfWeightVectorSystematics){
            for (unsigned int el=0;el<syst.second;++el){
                event_weight = 1.0; //event.getSystEventWeight( syst.first, el );
                std::string weightIndex = std::to_string(el);

                fill( m_name+weightIndex+"_"+syst.first, event, event_weight );
            } // end components of vector
        } // end vector weight systematics
    } // end if nominal and doSystWeights

    return;
}



/**** OVER/UNDERFLOW ****/

void histogrammer::overUnderFlow(){
    /* Call overflow and underflow functions at once */
    overFlow();
    underFlow();
    return;
}


void histogrammer::overFlow() {
    /* Add overflow to last bin */
    if (!m_putOverflowInLastBin){
        return;
    }
    else{
        // loop over 1D histograms
        for (const auto& hist : m_map_histograms1D){
            unsigned int numBins = hist.second->GetNbinsX();
            double overflowContent = hist.second->GetBinContent(numBins + 1);

            hist.second->AddBinContent(numBins,overflowContent); // add overflow to last bin
            hist.second->SetBinContent(numBins+1, 0);            // set overflow to 0
        }
        // loop over 2D histograms
        for (const auto& hist : m_map_histograms2D){
            unsigned int numBinsX = hist.second->GetXaxis()->GetNbins();
            unsigned int numBinsY = hist.second->GetYaxis()->GetNbins();

            // x-axis :: overflow in y
            for (unsigned int xx=1;xx<numBinsX+1;++xx){
                double overflowContent = hist.second->GetBinContent(xx,numBinsY+1);

                int lastBin     = hist.second->GetBin(xx,numBinsY);
                int overflowBin = hist.second->GetBin(xx,numBinsY+1);
                hist.second->AddBinContent(lastBin,overflowContent); // add overflow to last bin
                hist.second->SetBinContent(overflowBin,0);           // set overflow to 0
            }
            // y-axis :: overflow in x
            for (unsigned int yy=1;yy<numBinsY;++yy){
                double overflowContent = hist.second->GetBinContent(numBinsX+1,yy);

                int lastBin     = hist.second->GetBin(numBinsX,yy);
                int overflowBin = hist.second->GetBin(numBinsX+1,yy);
                hist.second->AddBinContent(lastBin,overflowContent); // add overflow to last bin
                hist.second->SetBinContent(overflowBin,0);           // set overflow to 0
            }
        } // end 2D histogram overflow
    } // end else put overflow in first bin

    return;
}

void histogrammer::underFlow() {
    /* Add underflow to first bin */
    if (!m_putUnderflowInFirstBin){
        return;
    }
    else{
        // loop over 1D histograms
        for (const auto& hist : m_map_histograms1D){
            double underflowContent = hist.second->GetBinContent(0);

            hist.second->AddBinContent(1, underflowContent);  // add underflow to first bin
            hist.second->SetBinContent(0, 0);                 // set underflow to 0
        }
        // loop over 2D histograms
        for (const auto& hist : m_map_histograms2D){
            unsigned int numBinsX = hist.second->GetXaxis()->GetNbins();
            unsigned int numBinsY = hist.second->GetYaxis()->GetNbins();

            // x-axis :: underflow in y
            for (unsigned int xx=1;xx<numBinsX+1;++xx){
                double underflowContent = hist.second->GetBinContent(xx,numBinsY+1);

                int firstBin     = hist.second->GetBin(xx,1);
                int underflowBin = hist.second->GetBin(xx,0);
                hist.second->AddBinContent(firstBin,underflowContent); // add overflow to last bin
                hist.second->SetBinContent(underflowBin,0);            // set overflow to 0
            }
            // y-axis :: underflow in x
            for (unsigned int yy=1;yy<numBinsY;++yy){
                double underflowContent = hist.second->GetBinContent(0,yy);

                int firstBin     = hist.second->GetBin(1,yy);
                int underflowBin = hist.second->GetBin(0,yy);
                hist.second->AddBinContent(firstBin,underflowContent); // add overflow to last bin
                hist.second->SetBinContent(underflowBin,0);           // set overflow to 0
            }
        } // end 2D histogram underflow
    } // end else put underflow in first bin
}

DEFINE_FWK_MODULE(histogrammer);
// THE END

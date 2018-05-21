/*
Created:        --
Last Updated:   29 August    2017

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University

-----

Make histograms for systematic uncertainties (& nominal) 
to go into plots || TRexFitter

*/
#include "Analysis/CyMiniAna/interface/histogrammer.h"


histogrammer::histogrammer( configuration& cmaConfig, std::string name ) :
  m_config(&cmaConfig),
  m_name(name),
  m_putOverflowInLastBin(true),
  m_putUnderflowInFirstBin(true){
    m_map_histograms1D.clear();
    m_map_histograms2D.clear();
    m_map_histograms3D.clear();

    m_isMC  = m_config->isMC();

    m_useJets = m_config->useJets();
    m_useLargeRJets = m_config->useLargeRJets();
    m_useLeptons    = m_config->useLeptons();
    m_useNeutrinos  = m_config->useNeutrinos();

    if (m_name.length()>0  && m_name.substr(m_name.length()-1,1).compare("_")!=0)
        m_name = m_name+"_"; // add '_' to end of string, if needed
  }

histogrammer::~histogrammer() {}


/**** INITIALIZE HISTOGRAMS ****/

// -- 1D Histograms
void histogrammer::init_hist( const std::string& name, const unsigned int nBins, const double x_min, const double x_max ){
    /* Initialize histogram -- equal bins */
    m_map_histograms1D["h_"+name] = new TH1D(("h_"+name).c_str(), ("h_"+name).c_str(),nBins,x_min,x_max);
    m_map_histograms1D["h_"+name]->Sumw2();

    return;
}
void histogrammer::init_hist( const std::string& name, const unsigned int nBins, const double *xbins ){
    /* Initialize histogram -- variable bins */
    m_map_histograms1D["h_"+name] = new TH1D(("h_"+name).c_str(), ("h_"+name).c_str(),nBins,xbins);
    m_map_histograms1D["h_"+name]->Sumw2();

    return;
}
// -- 2D Histograms
void histogrammer::init_hist( const std::string& name, const unsigned int nBinsX, const double x_min, const double x_max,
                              const unsigned int nBinsY, const double y_min, const double y_max ){
    /* Initialize histogram -- equal bins */
    m_map_histograms2D["h_"+name] = new TH2D(("h_"+name).c_str(), ("h_"+name).c_str(),
                                            nBinsX,x_min,x_max,nBinsY,y_min,y_max);
    m_map_histograms2D["h_"+name]->Sumw2();

    return;
}
void histogrammer::init_hist( const std::string& name, const unsigned int nBinsX, const double *xbins,
                              const unsigned int nBinsY, const double *ybins ){
    /* Initialize histogram -- variable bins */
    m_map_histograms2D["h_"+name] = new TH2D(("h_"+name).c_str(), ("h_"+name).c_str(),
                                           nBinsX,xbins,nBinsY,ybins);
    m_map_histograms2D["h_"+name]->Sumw2();

    return;
}
// -- 3D Histograms
void histogrammer::init_hist( const std::string& name, const unsigned int nBinsX, const double x_min, const double x_max,
                              const unsigned int nBinsY, const double y_min, const double y_max,
                              const unsigned int nBinsZ, const double z_min, const double z_max ){
    /* Initialize histogram -- equal bins */
    m_map_histograms3D["h_"+name] = new TH3D(("h_"+name).c_str(), ("h_"+name).c_str(),
                                            nBinsX,x_min,x_max,nBinsY,y_min,y_max,nBinsZ,z_min,z_max);
    m_map_histograms3D["h_"+name]->Sumw2();

    return;
}
void histogrammer::init_hist( const std::string& name, const unsigned int nBinsX, const double *xbins,
                              const unsigned int nBinsY, const double *ybins,
                              const unsigned int nBinsZ, const double *zbins ){
    /* Initialize histogram -- variable bins */
    m_map_histograms3D["h_"+name] = new TH3D(("h_"+name).c_str(), ("h_"+name).c_str(),
                                           nBinsX,xbins,nBinsY,ybins,nBinsZ,zbins);
    m_map_histograms3D["h_"+name]->Sumw2();

    return;
}


void histogrammer::initialize( TFile& outputFile, bool doSystWeights ){
    /* Setup some values and book histograms */
    m_doSystWeights = doSystWeights;
    outputFile.cd();


    // loop over selections (typically only one treename)
    for (const auto& sel : m_config->selections() ) {
        bookHists( m_name+sel );

        // weight systematics
        if (m_isMC && m_doSystWeights){
            for (const auto& syst : m_config->listOfWeightSystematics()){
                bookHists( m_name+sel+syst );
            } // end weight systematics

            // vector weight systematics
            for (const auto& syst : m_config->mapOfWeightVectorSystematics()){
                for (unsigned int el=0;el<syst.second;++el){
                    std::string weightIndex = std::to_string(el);
                    bookHists( m_name+sel+weightIndex+"_"+syst.first );
                } // end components of vector
            } // end vector weight systematics
        } // end if MC and save weight systematics
    } // end loop over selections

    return;
}


void histogrammer::bookHists( std::string name ){
    /* 
      Book histograms -- modify/inherit this function for analysis-specific hists 

      @param name   This is the string used to identify histograms for different systematics/event weights
    */
    m_names.resize(0); // append names to this to keep track of later
    cma::DEBUG("HISTOGRAMMER : Book histograms "+name);

    if (m_useJets){
        init_hist("n_jets_"+name,   31, -0.5,  30.5);
        init_hist("n_btags_"+name,  11, -0.5,  10.5);

        init_hist("jet0_pt_"+name,    2000,  0.0, 2000.0);
        init_hist("jet1_pt_"+name,    2000,  0.0, 2000.0);
        init_hist("jet0_bdisc_"+name,  100,  0.0,    1.0);
        init_hist("jet1_bdisc_"+name,  100,  0.0,    1.0);

        init_hist("jet_pt_"+name,     2000,  0.0, 2000.0);
        init_hist("jet_eta_"+name,      50, -2.5,    2.5);
        init_hist("jet_phi_"+name,      64, -3.2,    3.2);
        init_hist("jet_bdisc_"+name,   100,  0.0,    1.0);
    }

    if (m_useLargeRJets){
        init_hist("n_ljets_"+name,       31, -0.5,   30.5);
        init_hist("ljet_pt_"+name,     2000,  0.0, 2000.0);
        init_hist("ljet_eta_"+name,      50, -2.5,    2.5);
        init_hist("ljet_phi_"+name,      64, -3.2,    3.2);
        init_hist("ljet_SDmass_"+name,  500,  0.0,  500.0);
        init_hist("ljet_charge_"+name, 1000, -5.0,    5.0);
        init_hist("ljet_tau1_"+name,    200,  0.0,    2.0);
        init_hist("ljet_tau2_"+name,    200,  0.0,    2.0);
        init_hist("ljet_tau3_"+name,    200,  0.0,    2.0);
        init_hist("ljet_tau21_"+name,   100,  0.0,    1.0);
        init_hist("ljet_tau32_"+name,   100,  0.0,    1.0);
        init_hist("ljet_subjet0_bdisc_"+name, 100, 0.0, 1.0);
        init_hist("ljet_subjet1_bdisc_"+name, 100, 0.0, 1.0);
        init_hist("ljet_subjet0_charge_"+name,1000, -5.0, 5.0);
        init_hist("ljet_subjet1_charge_"+name,1000, -5.0, 5.0);
    }

    if (m_useLeptons){
        init_hist("el_pt_"+name,  500, 0.0, 2000);
        init_hist("el_eta_"+name,  50, -2.5, 2.5);
        init_hist("el_phi_"+name,  64, -3.2, 3.2);
        init_hist("el_charge_"+name, 240, -1.2, 1.2);

        init_hist("mu_pt_"+name,  500, 0.0, 2000);
        init_hist("mu_eta_"+name,  50, -2.5, 2.5);
        init_hist("mu_phi_"+name,  64, -3.2, 3.2);
        init_hist("mu_charge_"+name, 240, -1.2, 1.2);
    }

    if (m_useNeutrinos){
        init_hist("nu_pt_"+name,     500, 0.0, 2000);
        init_hist("nu_eta_"+name,     50, -2.5, 2.5);
        init_hist("nu_phi_"+name,     64, -3.2, 3.2);
        init_hist("nu_eta_smp_"+name, 50, -2.5, 2.5);
        init_hist("nu_pz_samples_"+name, 500, -2000,2000);

        init_hist("w_mass_"+name, 200,0,200);
        init_hist("w_pt_"+name,  1000,0,1000);
        init_hist("w_mass_smp_"+name, 200,0,200);
        init_hist("w_pt_smp_"+name,  1000,0,1000);

        if (m_config->useTruth()) {
            // compare reco neutrinos to truth neutrinos
            init_hist("nu_deltaPz_"+name,      1000,-2000,2000);
            init_hist("nu_deltaPz_smp_"+name,  1000,-2000,2000);
            init_hist("nu_deltaEta_"+name,       50,-5,5);
            init_hist("nu_deltaEta_smp_"+name,   50,-5,5);
            init_hist("nu_deltaR_"+name,         50, 0,5);
            init_hist("nu_deltaR_smp_"+name,     50, 0,5);
            init_hist("nu_deltaR_viper_"+name,   50, 0,5);

            // 2D histograms
//            init_hist("nu_truth_pz_deltaPz_"+name,     500,-2000,2000, 500,-2000,2000);
//            init_hist("nu_truth_pz_deltaPz_smp_"+name, 500,-2000,2000, 500,-2000,2000);
//            init_hist("nu_truth_eta_deltaEta_"+name,     50,-5,5, 50,-5,5);
//            init_hist("nu_truth_eta_deltaEta_smp_"+name, 50,-5,5, 50,-5,5);
        }
    }

    // kinematics
    init_hist("mtw_"+name,     500,  0.0, 2000);
    init_hist("met_met_"+name, 500,  0.0, 2000);
    init_hist("met_phi_"+name,  64, -3.2,  3.2);
    init_hist("ht_"+name,     5000,  0.0, 5000);
    init_hist("st_"+name,     5000,  0.0, 5000);

    init_hist("wprimeMass_"+name, 5000, 0, 5000);
    init_hist("vlqMass_"+name,    5000, 0, 5000);
    init_hist("wprimeMass_nusmp_"+name, 5000, 0, 5000);
    init_hist("vlqMass_nusmp_"+name,    5000, 0, 5000);

    //  VLQ/Wprime
    if (m_config->useTruth() && m_config->isSignal()){
        init_hist("jet_pt_truth_quark_Wprime_"+name,  2000, 0., 2000.);
        init_hist("jet_bdisc_truth_quark_Wprime_"+name,100, 0.,    1.);

        init_hist("truth_wprime_mass_"+name,  1000, 0, 5000 );
        init_hist("truth_vlq_mass_"+name,     1000, 0, 5000 );

        init_hist("truth_quark_vlq_energy_"+name, 1000,0,5000,1000,0,5000); // quark vs vlq energy
        init_hist("truth_quark_vlq_energy_diff_"+name,  2000,-5000,5000);   // quark vs vlq energy
        init_hist("truth_quark_vlq_energy_asymm_"+name,  100,-1,1);         // quark vs vlq energy

        init_hist("truth_quark_pt_"+name,     1000, 0, 2000 );
        init_hist("truth_vlq_boson_pt_"+name, 1000, 0, 2000 );
        init_hist("truth_vlq_quark_pt_"+name, 1000, 0, 2000 );

        init_hist("truth_deltaR_wprime_quark-vlq_"+name, 500, 0, 5 );  // deltaR( VLQ, quark ) from Wprime decay
        init_hist("truth_deltaR_vlq_boson-quark_"+name,  500, 0, 5 );  // deltaR( boson, quark ) from VLQ decay
        init_hist("truth_deltaR_boson_decays_"+name,     500, 0, 5 );  // deltaR( child0, child1 ) from boson decay

        // Mass resolution
        init_hist("vlq_massResolution_"+name,         2000, -5000, 5000);             // truth-reco
        init_hist("vlq_massResolution_norm_"+name,     100, -1, 1);                   // (truth-reco)/truth
        init_hist("vlqMass_truth_vs_reco_"+name,      1000, 0, 5000, 1000, 0, 5000);  // truth vs reco
        init_hist("wprime_massResolution_"+name,      2000, -5000, 5000);             // truth-reco
        init_hist("wprime_massResolution_norm_"+name,  100, -1, 1);                   // (truth-reco)/truth
        init_hist("wprimeMass_truth_vs_reco_"+name,   1000, 0, 5000, 1000, 0, 5000);  // truth vs reco

        // Mass resolution with neutrino sampling
        init_hist("vlq_massResolution_norm_nusmp_"+name,     100, -1, 1);                   // (truth-reco)/truth
        init_hist("vlqMass_truth_vs_reco_nusmp_"+name,      1000, 0, 5000, 1000, 0, 5000);  // truth vs reco
        init_hist("wprime_massResolution_norm_nusmp_"+name,  100, -1, 1);                   // (truth-reco)/truth
        init_hist("wprimeMass_truth_vs_reco_nusmp_"+name,   1000, 0, 5000, 1000, 0, 5000);  // truth vs reco
    }
/*
    init_hist("wprime_mass_"+name,  6000, 0.0, 6000);
    init_hist("vlq_mass_"+name,     5000, 0.0, 5000);
*/
    return;
}




/**** FILL HISTOGRAMS ****/

void histogrammer::fill( const std::string& name, const double& value, const double& weight ){
    /* TH1D */
    TH1D* this_hist = m_map_histograms1D.at("h_"+name);

    this_hist->Fill(value,weight);

    return;
}

void histogrammer::fill( const std::string& name, 
                         const double& xvalue, const double& yvalue, const double& weight ){
    /* TH2D */
    TH2D* this_hist = m_map_histograms2D.at("h_"+name);

    this_hist->Fill(xvalue,yvalue,weight);

    return;
}

void histogrammer::fill( const std::string& name, 
                         const double& xvalue, const double& yvalue, const double& zvalue, const double& weight ){
    /* TH3D */
    TH3D* this_hist = m_map_histograms3D.at("h_"+name);

    this_hist->Fill(xvalue,yvalue,zvalue,weight);

    return;
}


void histogrammer::fill( Event& event, const std::vector<unsigned int>& evtsel_decisions ){
    /* Fill histograms -- fill histograms based on selection, tree, or systematic weights ("nominal" but different weight)
       This is the function to modify / inherit for analysis-specific purposes
    */
    double event_weight = event.nominal_weight();

    std::vector<std::string> selections = m_config->selections();
    for (unsigned int ss=0, size=selections.size(); ss<size; ss++){
        std::string sel( selections.at(ss) );
        if (!evtsel_decisions.at(ss)) continue;
        fill( m_name+sel, event, event_weight );

        // if there are systematics stored as weights (e.g., b-tagging, pileup, etc.)
        // the following calls the fill() function with different event weights
        // to make histograms
        bool isNominal = m_config->isNominalTree( event.treeName() );
        if (m_isMC && isNominal && m_doSystWeights){
            // weight systematics
            event_weight = 1.0;
            for (const auto& syst : m_config->listOfWeightSystematics()){
                event_weight = event.getSystEventWeight( syst );
                fill( m_name+sel+syst, event, event_weight );
            } // end weight systematics

            // vector weight systematics
            event_weight = 1.0;
            for (const auto& syst : m_config->mapOfWeightVectorSystematics()){
                for (unsigned int el=0;el<syst.second;++el){
                    event_weight = event.getSystEventWeight( syst.first, el );
                    std::string weightIndex = std::to_string(el);

                    fill( m_name+sel+weightIndex+"_"+syst.first, event, event_weight );
                } // end components of vector
            } // end vector weight systematics
        } // end if nominal and doSystWeights
    } // end loop over selections

    return;
}


void histogrammer::fill( const std::string& name, Event& event, double event_weight){
    /* Fill histograms -- just use information from the event and fill histogram
       This is the function to modify / inherit for analysis-specific purposes
    */
    cma::DEBUG("HISTOGRAMMER : Fill histograms "+name);
    cma::DEBUG("HISTOGRAMMER : event weight = "+std::to_string(event_weight) );

    // physics information
    std::vector<Jet> jets = event.jets();
    std::vector<Ljet> ljets = event.ljets();
    std::vector<Lepton> leptons = event.leptons();
    //std::vector<Muon> muons = event.muons();
    //std::vector<Electron> electrons = event.electrons();
    std::vector<Neutrino> neutrinos = event.neutrinos();
    MET met = event.met();
    std::vector<Parton> partons = event.truth_partons();
    Wprime wpreco = event.wprime();
    Wprime wpreco_smp = event.wprime_sampling();

    if (m_useJets){
        cma::DEBUG("HISTOGRAMMER : Fill small-R jets");
        fill("n_btags_"+name, event.btag_jets().size(), event_weight );
        fill("n_jets_"+name, jets.size(), event_weight );

        if (jets.size()>1){
            Jet jet0 = jets.at(0);
            Jet jet1 = jets.at(1);
            fill("jet0_pt_"+name,  jet0.p4.Pt(),   event_weight);
            fill("jet0_bdisc_"+name, jet0.bdisc,   event_weight);
            fill("jet1_pt_"+name,  jet1.p4.Pt(),   event_weight);
            fill("jet1_bdisc_"+name, jet1.bdisc,   event_weight);
        }

        for (const auto& jet : jets){
            if (!jet.isGood) continue;
            fill("jet_pt_"+name,  jet.p4.Pt(),   event_weight);
            fill("jet_eta_"+name, jet.p4.Eta(),  event_weight);
            fill("jet_phi_"+name, jet.p4.Phi(),  event_weight);
            fill("jet_bdisc_"+name, jet.bdisc,  event_weight);
        }
    }

    if (m_useLargeRJets){
        cma::DEBUG("HISTOGRAMMER : Fill large-R jets");
        fill("n_ljets_"+name, ljets.size(), event_weight );

        for (const auto& ljet : ljets){
            fill("ljet_pt_"+name,    ljet.p4.Pt(),  event_weight);
            fill("ljet_eta_"+name,   ljet.p4.Eta(), event_weight);
            fill("ljet_phi_"+name,   ljet.p4.Phi(), event_weight);
            fill("ljet_SDmass_"+name,ljet.softDropMass, event_weight);
            fill("ljet_charge_"+name,ljet.charge,event_weight);

            fill("ljet_tau1_"+name,  ljet.tau1,  event_weight);
            fill("ljet_tau2_"+name,  ljet.tau2,  event_weight);
            fill("ljet_tau3_"+name,  ljet.tau3,  event_weight);
            fill("ljet_tau21_"+name, ljet.tau21, event_weight);
            fill("ljet_tau32_"+name, ljet.tau32, event_weight);

            if (ljet.subjets.size()>0){
                fill("ljet_subjet0_bdisc_"+name, ljet.subjets.at(0).bdisc, event_weight);
                fill("ljet_subjet0_charge_"+name,ljet.subjets.at(0).charge,event_weight);
                if (ljet.subjets.size()>1){
                    fill("ljet_subjet1_bdisc_"+name, ljet.subjets.at(1).bdisc, event_weight);
                    fill("ljet_subjet1_charge_"+name,ljet.subjets.at(1).charge,event_weight);
                }
            }
        } // end loop over ljets
    } // end if use ljets

    if (m_useLeptons){
        cma::DEBUG("HISTOGRAMMER : Fill leptons");
        for (const auto& el : leptons){
            if (el.isMuon || !el.isGood) continue;
            fill("el_pt_"+name,  el.p4.Pt(),  event_weight);
            fill("el_eta_"+name, el.p4.Eta(), event_weight);
            fill("el_phi_"+name, el.p4.Phi(), event_weight);
            fill("el_charge_"+name, el.charge, event_weight);
        }

        for (const auto& mu : leptons){
            if (mu.isElectron || !mu.isGood) continue;
            fill("mu_pt_"+name,  mu.p4.Pt(),  event_weight);
            fill("mu_eta_"+name, mu.p4.Eta(), event_weight);
            fill("mu_phi_"+name, mu.p4.Phi(), event_weight);
            fill("mu_charge_"+name, mu.charge, event_weight);
        }
    }


    if (m_config->useNeutrinos()){
        cma::DEBUG("HISTOGRAMMER : Fill neutrinos");
        Neutrino nu = neutrinos.at(0);
        TLorentzVector tmp_nu;
        float nuE = sqrt( pow(nu.p4.Px(),2) + pow(nu.p4.Py(),2) + pow(nu.pz_sampling,2));
        tmp_nu.SetPxPyPzE( nu.p4.Px(), nu.p4.Py(), nu.pz_sampling, nuE );

        fill("nu_pt_"+name,  nu.p4.Pt(),  event_weight);
        fill("nu_eta_"+name, nu.p4.Eta(), event_weight);
        fill("nu_phi_"+name, nu.p4.Phi(), event_weight);
        fill("nu_eta_smp_"+name, tmp_nu.Eta(), event_weight);

        if (leptons.size()>0){
            TLorentzVector wBoson     = nu.p4 + leptons.at(0).p4;
            TLorentzVector wBoson_smp = tmp_nu+ leptons.at(0).p4;

            fill("w_mass_"+name, wBoson.M(),  event_weight);
            fill("w_pt_"+name,   wBoson.Pt(), event_weight);
            fill("w_mass_smp_"+name, wBoson_smp.M(),  event_weight);
            fill("w_pt_smp_"+name,   wBoson_smp.Pt(), event_weight);
        }
//        for (const auto pz : nu.pz_samplings)
//            fill("nu_pz_samples_"+name, pz, 1.0);   // look at the distribution of pz

        if (m_config->useTruth()){
            cma::DEBUG("HISTOGRAMMER : Fill neutrinos -- truth info");
            std::vector<Parton> truth_partons = event.truth_partons();

            float tru_pz(-999.);
            float tru_eta(-999.);
            Parton p;
            unsigned int n_wdecays2leptons(0);
            // Get truth neutrino
            for (const auto& pa : truth_partons){
                if (pa.isW && pa.child0_idx>=0 && pa.child1_idx>=0) {
                    Parton child0 = truth_partons.at( pa.child0_idx );
                    Parton child1 = truth_partons.at( pa.child1_idx );
                    if (child0.isNeutrino) {
                        n_wdecays2leptons++;
                        p = child0;
                    }
                    else if (child1.isNeutrino) {
                        n_wdecays2leptons++;
                        p = child1;
                    }
                }
            }
            if (n_wdecays2leptons==1){
                tru_pz  = p.p4.Pz();
                tru_eta = p.p4.Eta();

                float deltaPz     = tru_pz - nu.p4.Pz();
                float deltaPz_smp = tru_pz - nu.pz_sampling;
                fill("nu_deltaPz_"+name, deltaPz, event_weight);           // standard reconstruction
                fill("nu_deltaPz_smp_"+name, deltaPz_smp, event_weight);   // sampling reconstruction

                float deltaEta     = tru_eta - nu.p4.Eta();
                float deltaEta_smp = tru_eta - tmp_nu.Eta();
                float deltaR       = p.p4.DeltaR(nu.p4);
                float deltaR_smp   = p.p4.DeltaR(tmp_nu);
                float deltaR_viper(0.0);  // not here yet

                fill("nu_deltaEta_"+name,     deltaEta,     event_weight);
                fill("nu_deltaEta_smp_"+name, deltaEta_smp, event_weight);
                fill("nu_deltaR_"+name,       deltaR,       event_weight);
                fill("nu_deltaR_smp_"+name,   deltaR_smp,   event_weight);
                fill("nu_deltaR_viper_"+name, deltaR_viper, event_weight);

                fill("nu_truth_pz_deltaPz_"+name,     tru_pz, deltaPz,     event_weight);
                fill("nu_truth_pz_deltaPz_smp_"+name, tru_pz, deltaPz_smp, event_weight);
                fill("nu_truth_eta_deltaEta_"+name,     tru_eta, deltaEta, event_weight);
                fill("nu_truth_eta_deltaEta_smp_"+name, tru_eta, deltaEta_smp, event_weight);
            } // end if truth_leptons == 1
        } // end truth
    } // end use neutrinos


    // kinematics
    cma::DEBUG("HISTOGRAMMER : Fill kinematics");
    fill("mtw_"+name,     met.mtw,      event_weight);
    fill("met_met_"+name, met.p4.Pt(),  event_weight);
    fill("met_phi_"+name, met.p4.Phi(), event_weight);
    fill("ht_"+name,      event.HT(),   event_weight);
    fill("st_"+name,      event.ST(),   event_weight);

    if (m_config->useWprime()){
        fill("wprimeMass_"+name, wpreco.p4.M(),     event_weight);
        fill("vlqMass_"+name,    wpreco.vlq.p4.M(), event_weight);
        fill("wprimeMass_nusmp_"+name, wpreco_smp.p4.M(),     event_weight);
        fill("vlqMass_nusmp_"+name,    wpreco_smp.vlq.p4.M(), event_weight);
    }

    if (m_config->useTruth() && m_config->isSignal()){
        cma::DEBUG("HISTOGRAMMER : Fill truth information");

        TruthWprime wp = event.truth_wprime();

        float ptmin(0.0);
        float drmin(0.4);
        int j_index(-1);
        for (const auto& j : jets) {
            if (j.p4.DeltaR(wp.quark.p4)<drmin && j.p4.Pt()>ptmin) {
                j_index = j.index;
                drmin = j.p4.DeltaR(wp.quark.p4);
                ptmin = j.p4.Pt();
            }
        }
        if (j_index>=0) {
            fill("jet_pt_truth_quark_Wprime_"+name,   jets.at(j_index).p4.Pt(), event_weight);
            fill("jet_bdisc_truth_quark_Wprime_"+name,jets.at(j_index).bdisc,   event_weight);
        }

        TLorentzVector wp_quark = wp.quark.p4;
        TLorentzVector wp_vlq   = wp.vlq.p4;
        TLorentzVector wp_vlq_quark = wp.vlq_quark.p4;
        TLorentzVector wp_vlq_boson = wp.vlq_boson.p4;
        TLorentzVector wp_boson_child0 = wp.BosonChildren[0].p4;  // first child from boson (boson from VLQ)
        TLorentzVector wp_boson_child1 = wp.BosonChildren[1].p4;  // second child from boson (boson from VLQ)

        fill("truth_wprime_mass_"+name,  wp.wprime.p4.M(), event_weight );
        fill("truth_vlq_mass_"+name,     wp_vlq.M(),    event_weight );
        fill("truth_quark_pt_"+name,     wp_quark.Pt(), event_weight );
        fill("truth_vlq_boson_pt_"+name, wp_vlq_boson.Pt(), event_weight );
        fill("truth_vlq_quark_pt_"+name, wp_vlq_quark.Pt(), event_weight );

        fill("truth_quark_vlq_energy_"+name,       wp_quark.Pt(), wp_vlq.Pt(), event_weight); // quark vs vlq energy
        fill("truth_quark_vlq_energy_diff_"+name,  (wp_quark.Pt()-wp_vlq.Pt()),event_weight); // quark vs vlq energy
        fill("truth_quark_vlq_energy_asymm_"+name, (wp_quark.Pt()-wp_vlq.Pt())/(wp_quark.Pt()+wp_vlq.Pt()),event_weight); // quark vs vlq energy

        fill("truth_deltaR_wprime_quark-vlq_"+name, wp_quark.DeltaR( wp_vlq ), event_weight );            // deltaR( VLQ, quark ) from Wprime decay
        fill("truth_deltaR_vlq_boson-quark_"+name,  wp_vlq_boson.DeltaR( wp_vlq_quark ), event_weight );  // deltaR( boson, quark ) from VLQ decay
        fill("truth_deltaR_boson_decays_"+name,     wp_boson_child0.DeltaR( wp_boson_child1 ), event_weight );  // deltaR( child0, child1 ) from boson decay

        // Mass resolution
        fill("vlq_massResolution_"+name,         wp_vlq.M() -wpreco.vlq.p4.M(), event_weight);              // truth-reco
        fill("vlq_massResolution_norm_"+name,    (wp_vlq.M()-wpreco.vlq.p4.M())/wp_vlq.M(), event_weight); // (truth-reco)/truth
        fill("vlqMass_truth_vs_reco_"+name,      wp_vlq.M(), wpreco.vlq.p4.M(), event_weight);             // truth vs reco
        fill("wprime_massResolution_"+name,      wp.wprime.p4.M() -wpreco.p4.M(), event_weight);         // truth-reco
        fill("wprime_massResolution_norm_"+name, (wp.wprime.p4.M()-wpreco.p4.M())/wp.wprime.p4.M(),event_weight); // (truth-reco)/truth
        fill("wprimeMass_truth_vs_reco_"+name,   wp.wprime.p4.M(), wpreco.p4.M(), event_weight);         // truth vs reco
        // Mass resolution with sampling neutrino
        fill("vlq_massResolution_norm_nusmp_"+name,    (wp_vlq.M()-wpreco_smp.vlq.p4.M())/wp_vlq.M(), event_weight);
        fill("vlqMass_truth_vs_reco_nusmp_"+name,      wp_vlq.M(), wpreco_smp.vlq.p4.M(), event_weight);
        fill("wprime_massResolution_norm_nusmp_"+name, (wp.wprime.p4.M()-wpreco_smp.p4.M())/wp.wprime.p4.M(),event_weight);
        fill("wprimeMass_truth_vs_reco_nusmp_"+name,   wp.wprime.p4.M(), wpreco_smp.p4.M(), event_weight);
    }

/*
    cma::DEBUG("HISTOGRAMMER : Fill VLQ/Wprime");
*/

    cma::DEBUG("HISTOGRAMMER : End histograms");

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
        cma::INFO("HISTOGRAMMER : Not putting overflow in last bin(s)");
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
        cma::INFO("HISTOGRAMMER : Not putting underflow in first bin(s)");
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

// THE END

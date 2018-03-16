/*
Created:        --
Last Updated:   13 December 2017

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
University of Michigan, Ann Arbor, MI 48109

-----

Event class
Contains all the objects (& structs) with event information
Assumes a flat ntuple data structure

*/
#include "Analysis/CyMiniAna/interface/Event.h"

// constructor
Event::Event( TTreeReader &myReader, configuration &cmaConfig ) :
  m_config(&cmaConfig),
  m_ttree(myReader),
  m_treeName("SetMe"),
  m_fileName("SetMe"),
  m_DNN(0.0){
    m_treeName = m_ttree.GetTree()->GetName();      // for systematics
    m_fileName = m_config->filename();              // for accessing file metadata

    // Set options for the class
    m_isMC         = m_config->isMC();              // simulated or real sample
    m_useTruth     = m_config->useTruth();          // access and use truth information
    m_grid         = m_config->isGridFile();        // file directly from original analysis team
    m_useJets      = m_config->useJets();           // use AK4 jets in analysis
    m_useLjets     = m_config->useLargeRJets();     // use AK8 jets in analysis
    m_useLeptons   = m_config->useLeptons();        // use leptons in analysis
    m_useNeutrinos = m_config->useNeutrinos();      // use neutrinos in analysis
    m_useWprime    = m_config->useWprime();         // use reconstructed Wprime in analysis
    m_useDNN       = m_config->useDNN();            // use DNN in analysis


    m_neutrinoReco = m_config->neutrinoReco();      // reconstruct neutrino
    m_wprimeReco   = m_config->wprimeReco();        // reconstruct Wprime
    m_DNNinference = m_config->DNNinference();      // use DNN to predict values
    m_DNNtraining  = m_config->DNNtraining();       // load DNN features (save/use later)
    m_getDNN = (m_DNNinference || m_DNNtraining);

    // b-tagging working points
    m_cMVAv2L = m_config->cMVAv2L();
    m_cMVAv2M = m_config->cMVAv2M();
    m_cMVAv2T = m_config->cMVAv2T();
    m_CSVv2L  = m_config->CSVv2L();
    m_CSVv2M  = m_config->CSVv2M();
    m_CSVv2T  = m_config->CSVv2T();


    //** Access branches from Tree **//
    m_eventNumber  = new TTreeReaderValue<int>(m_ttree,"eventNumber");
    m_runNumber    = new TTreeReaderValue<int>(m_ttree,"runNumber");
    m_rho          = new TTreeReaderValue<float>(m_ttree,"rho");
    m_npv          = new TTreeReaderValue<int>(m_ttree,"npv");
    m_lumiblock    = new TTreeReaderValue<int>(m_ttree,"lumiBlock");
    m_NGoodVtx     = new TTreeReaderValue<int>(m_ttree,"nGoodVtx");
    m_LHAPDF_ID    = new TTreeReaderValue<int>(m_ttree,"LHA_PDF_ID");
    m_NIsoTrk      = new TTreeReaderValue<int>(m_ttree,"nIsoTrk");
    m_true_pileup  = new TTreeReaderValue<int>(m_ttree,"true_pileup");

    //m_HLT_Ele45_WPLoose_Gsf = new TTreeReaderValue<int>(m_ttree,"HLT_Ele45_WPLoose_Gsf");
    //m_HLT_Mu50   = new TTreeReaderValue<int>(m_ttree,"HLT_Mu50");
    //m_HLT_TkMu50 = new TTreeReaderValue<int>(m_ttree,"HLT_TkMu50");

    /** JETS **/
    if (m_useJets){
      // small-R jet information
      m_jet_pt   = new TTreeReaderValue<std::vector<float>>(m_ttree,"jet_pt");
      m_jet_eta  = new TTreeReaderValue<std::vector<float>>(m_ttree,"jet_eta");
      m_jet_phi  = new TTreeReaderValue<std::vector<float>>(m_ttree,"jet_phi");
      m_jet_e    = new TTreeReaderValue<std::vector<float>>(m_ttree,"jet_e");
      m_jet_charge = new TTreeReaderValue<std::vector<float>>(m_ttree,"jet_charge");
      m_jet_CSVv2  = new TTreeReaderValue<std::vector<float>>(m_ttree,"jet_CSV");
      //m_jet_JERSF  = new TTreeReaderValue<std::vector<float>>(m_ttree,"jet_jer");
      m_jet_jer_up   = new TTreeReaderValue<std::vector<float>>(m_ttree,"jet_jer_up");
      m_jet_jer_down = new TTreeReaderValue<std::vector<float>>(m_ttree,"jet_jer_down");
    }

    if (m_useLjets){
      // large-R Jet information
      m_ljet_pt   = new TTreeReaderValue<std::vector<float>>(m_ttree,"ljet_pt");
      m_ljet_eta  = new TTreeReaderValue<std::vector<float>>(m_ttree,"ljet_eta");
      m_ljet_phi  = new TTreeReaderValue<std::vector<float>>(m_ttree,"ljet_phi");
      m_ljet_e    = new TTreeReaderValue<std::vector<float>>(m_ttree,"ljet_e");
      m_ljet_tau1 = new TTreeReaderValue<std::vector<float>>(m_ttree,"ljet_tau1");
      m_ljet_tau2 = new TTreeReaderValue<std::vector<float>>(m_ttree,"ljet_tau2");
      m_ljet_tau3 = new TTreeReaderValue<std::vector<float>>(m_ttree,"ljet_tau3");
      m_ljet_tau21  = new TTreeReaderValue<std::vector<float>>(m_ttree,"ljet_tau21");
      m_ljet_tau32  = new TTreeReaderValue<std::vector<float>>(m_ttree,"ljet_tau32");
      m_ljet_charge = new TTreeReaderValue<std::vector<float>>(m_ttree,"ljet_charge");
      m_ljet_CSVv2  = new TTreeReaderValue<std::vector<float>>(m_ttree,"ljet_CSV");
      m_ljet_SDmass = new TTreeReaderValue<std::vector<float>>(m_ttree,"ljet_SDmass");
      m_ljet_ID_loose  = new TTreeReaderValue<std::vector<int>>(m_ttree,"ljet_ID_loose");
      m_ljet_ID_medium = new TTreeReaderValue<std::vector<int>>(m_ttree,"ljet_ID_medium");
      m_ljet_ID_tight  = new TTreeReaderValue<std::vector<int>>(m_ttree,"ljet_ID_tight");
      m_ljet_ID_tightlepveto = new TTreeReaderValue<std::vector<int>>(m_ttree,"ljet_ID_tightlepveto");
      m_ljet_jer_up   = new TTreeReaderValue<std::vector<float>>(m_ttree,"ljet_jer_up");
      m_ljet_jer_down = new TTreeReaderValue<std::vector<float>>(m_ttree,"ljet_jer_down");

      m_ljet_subjetIndex0 = new TTreeReaderValue<std::vector<int>>(m_ttree,"ljet_subjetIndex0");
      m_ljet_subjetIndex1 = new TTreeReaderValue<std::vector<int>>(m_ttree,"ljet_subjetIndex1");
      m_ljet_subjet_pt    = new TTreeReaderValue<std::vector<float>>(m_ttree,"ljet_subjet_pt");
      m_ljet_subjet_eta   = new TTreeReaderValue<std::vector<float>>(m_ttree,"ljet_subjet_eta");
      m_ljet_subjet_phi   = new TTreeReaderValue<std::vector<float>>(m_ttree,"ljet_subjet_phi");
      m_ljet_subjet_e     = new TTreeReaderValue<std::vector<float>>(m_ttree,"ljet_subjet_e");
      m_ljet_subjet_charge = new TTreeReaderValue<std::vector<float>>(m_ttree,"ljet_subjet_charge");
      m_ljet_subjet_CSVv2  = new TTreeReaderValue<std::vector<float>>(m_ttree,"ljet_subjet_CSV");
    }

    /** LEPTONS **/
    if (m_useLeptons){
      m_el_pt  = new TTreeReaderValue<std::vector<float>>(m_ttree,"el_pt");
      m_el_eta = new TTreeReaderValue<std::vector<float>>(m_ttree,"el_eta");
      m_el_phi = new TTreeReaderValue<std::vector<float>>(m_ttree,"el_phi");
      m_el_e   = new TTreeReaderValue<std::vector<float>>(m_ttree,"el_e");
      m_el_charge = new TTreeReaderValue<std::vector<float>>(m_ttree,"el_charge");
      m_el_iso    = new TTreeReaderValue<std::vector<int>>(m_ttree,"el_iso");
      m_el_veto   = new TTreeReaderValue<std::vector<int>>(m_ttree,"el_ID_veto");
      m_el_loose  = new TTreeReaderValue<std::vector<int>>(m_ttree,"el_ID_loose");
      m_el_medium = new TTreeReaderValue<std::vector<int>>(m_ttree,"el_ID_medium");
      m_el_tight  = new TTreeReaderValue<std::vector<int>>(m_ttree,"el_ID_tight");

      m_mu_pt  = new TTreeReaderValue<std::vector<float>>(m_ttree,"mu_pt");
      m_mu_eta = new TTreeReaderValue<std::vector<float>>(m_ttree,"mu_eta");
      m_mu_phi = new TTreeReaderValue<std::vector<float>>(m_ttree,"mu_phi");
      m_mu_e   = new TTreeReaderValue<std::vector<float>>(m_ttree,"mu_e");
      m_mu_charge = new TTreeReaderValue<std::vector<float>>(m_ttree,"mu_charge");
      m_mu_iso    = new TTreeReaderValue<std::vector<int>>(m_ttree,"mu_iso");
      m_mu_loose  = new TTreeReaderValue<std::vector<int>>(m_ttree,"mu_ID_loose");
      m_mu_medium = new TTreeReaderValue<std::vector<int>>(m_ttree,"mu_ID_medium");
      //m_mu_medium2016 = new TTreeReaderValue<std::vector<int>>(m_ttree,"mu_ID_medium2016");
      m_mu_tight   = new TTreeReaderValue<std::vector<int>>(m_ttree,"mu_ID_tight");
    }

    /** EVENT KINEMATICS **/
    m_met_met  = new TTreeReaderValue<float>(m_ttree,"met_met");
    m_met_phi  = new TTreeReaderValue<float>(m_ttree,"met_phi");
    m_met_met_sf = new TTreeReaderValue<float>(m_ttree,"met_met_sf");
    m_met_phi_sf = new TTreeReaderValue<float>(m_ttree,"met_met_sf");

    // DNN material
    if (!m_getDNN && m_useDNN)  // always false for now
        m_dnn_score = new TTreeReaderValue<float>(m_ttree,"dnn_score");

    /** CUSTOM RECONSTRUCTED OBJECTS **/
    if (m_useNeutrinos && !m_neutrinoReco){
      // Neutrino isn't stored in the baseline ntuples, requires reconstruction
      m_nu_pt  = new TTreeReaderValue<std::vector<float>>(m_ttree, "nu_pt");
      m_nu_eta = new TTreeReaderValue<std::vector<float>>(m_ttree, "nu_eta");
      m_nu_phi = new TTreeReaderValue<std::vector<float>>(m_ttree, "nu_phi");
    }

    if (m_wprimeReco){
      // Wprime/VLQ information not stored in baseline ntuples
      // Designed for Wprime -> bT -> bbW -> bblv
      // -- use lepton + reconstructed neutrino as 'lv'
      m_vlq_pt  = new TTreeReaderValue<float>(m_ttree, "vlq_pt");
      m_vlq_eta = new TTreeReaderValue<float>(m_ttree, "vlq_eta");
      m_vlq_phi = new TTreeReaderValue<float>(m_ttree, "vlq_phi");
      m_vlq_e   = new TTreeReaderValue<float>(m_ttree, "vlq_e");
      m_jet_vlq_index    = new TTreeReaderValue<int>(m_ttree, "jet_vlq_index");       // b-jet (from T decay) index in m_jets

      m_wprime_pt  = new TTreeReaderValue<float>(m_ttree, "wprime_pt");
      m_wprime_eta = new TTreeReaderValue<float>(m_ttree, "wprime_eta");
      m_wprime_phi = new TTreeReaderValue<float>(m_ttree, "wprime_phi");
      m_wprime_e   = new TTreeReaderValue<float>(m_ttree, "wprime_e");
      m_jet_wprime_index    = new TTreeReaderValue<int>(m_ttree, "jet_wprime_index"); // b-jet (from Wprime decay) index in m_jets
    }

    /** EVENT WEIGHTS AND TRUTH INFORMATION **/
    m_xsection       = 1.0;
    m_kfactor        = 1.0;
    m_sumOfWeights   = 1.0;
    m_LUMI           = m_config->LUMI();

    // MC information
    if (m_isMC){
      m_weight_mc    = new TTreeReaderValue<float>(m_ttree,"weight_mc");
      m_treeXSection     = new TTreeReaderValue<float>(m_ttree,"xsection");      //m_config->XSectionMap( m_fileName );
      m_treeKFactor      = new TTreeReaderValue<float>(m_ttree,"kfactor");       //m_config->KFactorMap(  m_fileName );
      m_treeSumOfWeights = new TTreeReaderValue<float>(m_ttree,"sumOfWeights");  //m_config->sumWeightsMap( m_fileName );

      //m_mc_ht = new TTreeReaderValue<float>(m_ttree,"evt_Gen_Ht");

      m_mc_pt  = new TTreeReaderValue<std::vector<float>>(m_ttree,"mc_pt");
      m_mc_eta = new TTreeReaderValue<std::vector<float>>(m_ttree,"mc_eta");
      m_mc_phi = new TTreeReaderValue<std::vector<float>>(m_ttree,"mc_phi");
      m_mc_e   = new TTreeReaderValue<std::vector<float>>(m_ttree,"mc_e");
      m_mc_charge  = new TTreeReaderValue<std::vector<float>>(m_ttree,"mc_charge");
      m_mc_pdgId   = new TTreeReaderValue<std::vector<float>>(m_ttree,"mc_pdgId");
      m_mc_mom_idx = new TTreeReaderValue<std::vector<float>>(m_ttree,"mc_mom_dix");

      if (m_config->useTruth()){
        m_truth_jet_pt  = new TTreeReaderValue<std::vector<float>>(m_ttree,"truth_jet_pt");
        m_truth_jet_eta = new TTreeReaderValue<std::vector<float>>(m_ttree,"truth_jet_eta");
        m_truth_jet_phi = new TTreeReaderValue<std::vector<float>>(m_ttree,"truth_jet_phi");
        m_truth_jet_e   = new TTreeReaderValue<std::vector<float>>(m_ttree,"truth_jet_e");

        m_truth_ljet_pt  = new TTreeReaderValue<std::vector<float>>(m_ttree,"truth_ljet_pt");
        m_truth_ljet_eta = new TTreeReaderValue<std::vector<float>>(m_ttree,"truth_ljet_eta");
        m_truth_ljet_phi = new TTreeReaderValue<std::vector<float>>(m_ttree,"truth_ljet_phi");
        m_truth_ljet_e   = new TTreeReaderValue<std::vector<float>>(m_ttree,"truth_ljet_e");
        m_truth_ljet_charge = new TTreeReaderValue<std::vector<float>>(m_ttree,"truth_ljet_charge");
      } // end useTruth
    } // end isMC


    /** External Tools to perform specific tasks **/
    // Kinematic reconstruction tools
    m_neutrinoTool = new NeutrinoReco(cmaConfig);
    m_neutrinoTool->initialize();

    m_wprimeTool = new WprimeReco(cmaConfig);
    m_wprimeTool->initialize();

    // Truth matching tool -- called from physics object functions after truth information setup
    m_truthMatchingTool = new truthMatching(cmaConfig);
    m_truthMatchingTool->initialize();

    // Deep learning
    m_deepLearningTool = new DeepLearning(cmaConfig);
} // end constructor


Event::~Event() {}



void Event::initialize_eventWeights(){
    /* Create vectors of the systematics that are weights for the nominal events
       Must be called from the constructor for the access to TTreeReaderValues to work!
    */
    std::map<std::string,unsigned int> mapWeightSystematics = m_config->mapOfWeightVectorSystematics();

    m_listOfWeightSystematics = m_config->listOfWeightSystematics();

    m_weightSystematicsFloats.clear();
    m_weightSystematicsVectorFloats.clear();

    // systematics from the nominal tree that are floats
    for (const auto& nom_syst : m_listOfWeightSystematics){
        if (!m_config->useLeptons() && nom_syst.find("leptonSF")!=std::string::npos)
            continue;
        m_weightSystematicsFloats[nom_syst] = new TTreeReaderValue<float>(m_ttree,nom_syst.c_str());
    }

    // systematics from the nominal tree that are vectors
    for (const auto& syst : mapWeightSystematics)
        m_weightSystematicsVectorFloats[syst.first] = new TTreeReaderValue<std::vector<float>>(m_ttree,syst.first.c_str());

    return;
}



void Event::updateEntry(Long64_t entry){
    /* Update the entry -> update all TTree variables */
    cma::DEBUG("EVENT : Update Entry "+std::to_string(entry) );
    m_entry = entry;

    // make sure the entry exists
    if(isValidRecoEntry())
        m_ttree.SetEntry(m_entry);
    else
        cma::ERROR("EVENT : Invalid Reco entry "+std::to_string(m_entry)+"!");

    return;
}

void Event::clear(){
    /* Clear many of the vectors/maps for each event -- SAFETY PRECAUTION */
    m_truth_ljets.clear();
    m_truth_jets.clear();
    m_truth_leptons.clear();
    m_truth_neutrinos.clear();

    m_jets.clear();
    m_ljets.clear();
    m_leptons.clear();
    m_neutrinos.clear();

    m_btag_jets.clear();
    m_btag_jets_default.clear();
    m_weight_btag_default = 1.0;
    m_nominal_weight = 1.0;

    m_HT = 0;
    m_ST = 0;

    return;
}


void Event::execute(Long64_t entry){
    /* Get the values from the event */
    cma::DEBUG("EVENT : Execute event " );

    // Load data from root tree for this event
    updateEntry(entry);

    // Reset many event-level values
    clear();

    // Get the event weights (for cutflow & histograms)
    initialize_weights();
    cma::DEBUG("EVENT : Setup weights ");

    // Truth Information
    if (m_useTruth){
        initialize_truth();
        cma::DEBUG("EVENT : Setup truth information ");
    }

    // Jets
    if (m_useJets){
        initialize_jets();
        cma::DEBUG("EVENT : Setup small-R jets ");
    }

    // Large-R Jets
    if (m_useLjets){
        initialize_ljets();
        cma::DEBUG("EVENT : Setup large-R jets ");
    }

    // Leptons
    if (m_useLeptons){
        initialize_leptons();
        cma::DEBUG("EVENT : Setup leptons ");
    }

    // Get some kinematic variables (MET, HT, ST)
    initialize_kinematics();
    cma::DEBUG("EVENT : Setup kinematic variables ");

    // -- External Tools -- //

    // Neutrinos
    if (m_useNeutrinos){
        // relies on kinematic reconstruction, unless the information is saved in root file
        initialize_neutrinos();
        cma::DEBUG("EVENT : Setup neutrinos ");
    }

    // VLQ/Wprime system
    if (m_useWprime){
        // relies on other event information unless stored in root file
    }

    // DNN
    if (m_useDNN){
        if (m_getDNN) deepLearning();                // calculate (inference or training)
        else m_DNN = 0.0; //*(*m_dnn_score); // load from ntuple
    }
    else m_DNN = 0.0;

    cma::DEBUG("EVENT : Setup Event ");

    return;
}


void Event::initialize_truth(){
    /* Setup truth information (MC and physics objects) */
    m_truth_partons.clear();

    for (unsigned int i=0,size=(*m_mc_pt)->size();i<size;i++){
        Parton p;
        p.p4.SetPtEtaPhiE( (*m_mc_pt)->at(i), (*m_mc_eta)->at(i), (*m_mc_phi)->at(i), (*m_mc_e)->at(i) );

        p.pdgId = (*m_mc_pdgId)->at(i);
        p.index = i;
        p.parent0_idx = (*m_mc_mom_idx)->at(i);

        // simple booleans for type
        unsigned int abs_pdgId = std::abs( (*m_mc_pdgId)->at(i) );
        //p.isWprime
        //p.isVLQ
        p.isTop = ( abs_pdgId==6 );
        p.isW   = ( abs_pdgId==24 );
        p.isZ   = ( abs_pdgId==23 );
        p.isHiggs  = ( abs_pdgId==25 );
        p.isLepton = ( abs_pdgId>=11 && abs_pdgId<=16 );
        p.isQuark  = ( abs_pdgId<7 );

        if (p.isLepton){
            p.isTau  = ( abs_pdgId==15 );
            p.isMuon = ( abs_pdgId==13 );
            p.isElectron = ( abs_pdgId==11 );
            p.isNeutrino = ( abs_pdgId==12 || abs_pdgId==14 || abs_pdgId==16 );
        }
        else if (p.isQuark){
            p.isLight  = ( abs_pdgId<5 );
            p.isBottom = ( abs_pdgId==5 );
        }


        m_truth_partons.push_back(p);
    }

    return;
}


void Event::initialize_jets(){
    /* Setup struct of jets (small-r) and relevant information 
     * b-tagging: https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation80XReReco
     */
    m_jets.resize((*m_jet_pt)->size());
    for (const auto& btagWP : m_config->btagWkpts() ){
        m_btag_jets[btagWP].clear();
    }

    for (unsigned int i=0,size=(*m_jet_pt)->size(); i<size; i++){
        Jet jet;
        jet.p4.SetPtEtaPhiE( (*m_jet_pt)->at(i),(*m_jet_eta)->at(i),(*m_jet_phi)->at(i),(*m_jet_e)->at(i));

        jet.cMVAv2 = (*m_jet_CSVv2)->at(i);
        jet.index  = i;

        getBtaggedJets(jet);

        m_jets[i] = jet;
    }

    m_btag_jets_default = m_btag_jets.at(m_config->jet_btagWkpt());

    return;
}


void Event::initialize_ljets(){
    /* Setup struct of large-R jets and relevant information */
    m_ljets.resize((*m_ljet_pt)->size());

    for (unsigned int i=0,size=(*m_ljet_pt)->size(); i<size; i++){
        Ljet ljet;
        ljet.p4.SetPtEtaPhiE( (*m_ljet_pt)->at(i),(*m_ljet_eta)->at(i),(*m_ljet_phi)->at(i),(*m_ljet_e)->at(i));
        ljet.charge    = (*m_ljet_charge)->at(i);

        ljet.tau1  = (*m_ljet_tau1)->at(i);
        ljet.tau2  = (*m_ljet_tau2)->at(i);
        ljet.tau3  = (*m_ljet_tau3)->at(i);
        ljet.tau21 = (*m_ljet_tau21)->at(i);
        ljet.tau32 = (*m_ljet_tau32)->at(i);

        ljet.softDropMass  = (*m_ljet_SDmass)->at(i);
        ljet.vSubjetIndex0 = (*m_ljet_subjetIndex0)->at(i);
        ljet.vSubjetIndex1 = (*m_ljet_subjetIndex1)->at(i);

        m_ljets[i] = ljet;
    }

    return;
}


void Event::initialize_leptons(){
    /* Setup struct of lepton and relevant information */
    m_leptons.clear();
    m_electrons.clear();
    m_muons.clear();

    // Muons
    unsigned int nMuons = (*m_mu_pt)->size();
    m_muons.resize(nMuons);

    for (unsigned int i=0; i<nMuons; i++){
        Muon mu;
        mu.p4.SetPtEtaPhiE( (*m_mu_pt)->at(i),(*m_mu_eta)->at(i),(*m_mu_phi)->at(i),(*m_mu_e)->at(i));

        mu.charge = (*m_mu_charge)->at(i);
        mu.loose  = (*m_mu_loose)->at(i);
        mu.medium = (*m_mu_medium)->at(i);
        mu.tight  = (*m_mu_tight)->at(i);
        mu.iso    = (*m_mu_iso)->at(i);
        mu.isGood = (mu.p4.Pt()>50 && std::abs(mu.p4.Eta())<2.1);

        m_muons[i] = mu;
    }

    // Electrons
    unsigned int nElectrons = (*m_el_pt)->size();
    m_electrons.resize(nElectrons);
    for (unsigned int i=0; i<nElectrons; i++){
        Electron el;
        el.p4.SetPtEtaPhiE( (*m_el_pt)->at(i),(*m_el_eta)->at(i),(*m_el_phi)->at(i),(*m_el_e)->at(i));

        el.charge = (*m_el_charge)->at(i);
        el.loose  = (*m_el_loose)->at(i);
        el.medium = (*m_el_medium)->at(i);
        el.tight  = (*m_el_tight)->at(i);
        el.iso    = (*m_el_iso)->at(i);
        el.isGood = (el.p4.Pt()>50 && std::abs(el.p4.Eta())<2.1);

        m_electrons[i] = el;
    }

    return;
}


void Event::initialize_neutrinos(){
    /* Build the neutrinos */
    m_neutrinos.clear();

    if (m_neutrinoReco)
        m_neutrinoTool->execute();
    else{
        for (unsigned int i=0,size=(*m_nu_pt)->size();i<size;i++){
            Neutrino nu;
            nu.p4.SetPtEtaPhiM( (*m_nu_pt)->at(i), (*m_nu_eta)->at(i), (*m_nu_phi)->at(i), 0.0);
            m_neutrinos.push_back(nu);
        }
    }

    return;
}


void Event::initialize_weights(){
    /* Event weights */
    m_nominal_weight = 1.0;

    m_xsection     = (**m_treeXSection);
    m_kfactor      = (**m_treeKFactor);
    m_sumOfWeights = (**m_treeSumOfWeights);


    m_weight_btag.clear();
    if (m_isMC){
        m_nominal_weight  = (**m_weight_pileup) * (**m_weight_mc) * (**m_weight_btagSF);
        m_nominal_weight *= (m_xsection) * (m_kfactor) * m_LUMI / (m_sumOfWeights);
/*      // weights not in CMS (so far):
        m_weight_btag["70"] = (**m_weight_btag_70);
        m_weight_btag["77"] = (**m_weight_btag_77);
        m_weight_btag_default = m_weight_btag[m_config->jet_btagWkpt()];
        m_nominal_weight *= m_weight_btag_default;
*/
    }

    return;
}


void Event::initialize_kinematics(){
    /* Kinematic variables (HT, ST, MET) 
       Recalculate HT and ST (in case the jets/leptons change)
     */
    m_HT = 0.0;
    m_ST = 0.0;

    // Get hadronic transverse energy
    if (m_config->useJets()){
        // include small-R jet pT
        for (auto &small_jet : m_jets ){
            m_HT += small_jet.p4.Pt();
        }
    }
    else{
        // include large-R jet pT
        for (auto &large_jet : m_ljets){
            m_HT += large_jet.p4.Pt();
        }
    }

    // set MET
    m_metmet = **m_met_met;
    m_metphi = **m_met_phi;

    // Get MET and lepton transverse energy
    m_ST += m_HT;
    m_ST += m_metmet;
    if (m_config->useLeptons()){
        for (const auto& lep : m_leptons)
            m_ST += lep.p4.Pt(); 
    }

    return;
}




/*** GETTER FUNCTIONS ***/


//BUILD VLQ / WPrime
//    USE ROOT FILE INFORMATION IF "m_kinematicReco"==false
//    ELSE USE ALGORITHM



void Event::deepLearning(){
    /* Dan Guest's lightweight DNN framework */
    if (m_DNNinference){
        // do some inference and get DNN scores
        cma::DEBUG("EVENT : Calculate DNN ");
        m_deepLearningTool->inference();
    }
    else{
        // setting up training -- likely for flatTree4ML and histogrammer4ML
        m_deepLearningTool->training();
        std::map<std::string,double> features = m_deepLearningTool->features();
        // store the features somewhere (depends on what we're doing, leave empty for now)
    }

    return;
}


void Event::getBtaggedJets( Jet& jet ){
    /* Determine the b-tagging */
    jet.isbtagged["L"] = false;
    jet.isbtagged["M"] = false;
    jet.isbtagged["T"] = false;

    if (jet.CSVv2 > m_CSVv2L){
        jet.isbtagged["L"] = true;
        m_btag_jets["L"].push_back(jet.index);  // 0 = index of this jet
        if (jet.CSVv2 > m_CSVv2M){
            jet.isbtagged["M"] = true;
            m_btag_jets["M"].push_back(jet.index);
            if (jet.CSVv2 > m_CSVv2T){
                jet.isbtagged["T"] = true;
                m_btag_jets["T"].push_back(jet.index);
            }
        }
    }

    return;
}


double Event::getSystEventWeight( const std::string &syst, const int weightIndex ){
    /* Calculate the event weight given some systematic
       -- only call for nominal events and systematic weights
       -- for non-nominal tree systematics, use the nominal event weight

       @param syst          Name of systematic (nominal or some weight systematic)
       @param weightIndex   Index of btagging SF; default to -1
    */
    double syst_event_weight(1.0);

    if (syst.compare("nominal")==0){
        // nominal event weight
        syst_event_weight  = m_nominal_weight;
    }
    else if (syst.find("pileup")!=std::string::npos){
        // pileup event weight
        syst_event_weight  = (**m_weight_mc);
        syst_event_weight *= m_weight_btag_default;
        syst_event_weight *= (m_xsection) * (m_kfactor) * (m_LUMI);
        syst_event_weight /= (m_sumOfWeights);

        syst_event_weight *= **m_weightSystematicsFloats.at(syst);
    }
    else if (syst.find("leptonSF")!=std::string::npos){
        // leptonSF event weight
        syst_event_weight  = (**m_weight_pileup) * (**m_weight_mc);
        syst_event_weight *= m_weight_btag_default;
        syst_event_weight *= (m_xsection) * (m_kfactor) * (m_LUMI);
        syst_event_weight /= (m_sumOfWeights);

        syst_event_weight *= **m_weightSystematicsFloats.at(syst);
    }
    else if (syst.find("bTagSF")!=std::string::npos){
        // bTagSF event weight -- check indices for eigenvector systematics
        syst_event_weight  = (**m_weight_pileup) * (**m_weight_mc);
        syst_event_weight *= (m_xsection) * (m_kfactor) * (m_LUMI);
        syst_event_weight /= (m_sumOfWeights);
    }
    else{
        // safety to catch something weird -- just return 1.0
        cma::WARNING("EVENT : Passed systematic variation, "+syst+", to Event::getSystEventWeight() ");
        cma::WARNING("EVENT : that is inconsistent with the CyMiniAna options of ");
        cma::WARNING("EVENT :     nominal, jvt, pileup, leptonSF, and bTagSF. ");
        cma::WARNING("EVENT : Returning a weight of 1.0. ");
        syst_event_weight = 1.0;
    }

    return syst_event_weight;
}




/*** RETURN PHYSICS INFORMATION ***/
std::vector<int> Event::btag_jets(const std::string &wkpt){
    /* Small-R Jet b-tagging */
    std::string tmp_wkpt(wkpt);
    if(m_btag_jets.find(wkpt) == m_btag_jets.end()){
        cma::WARNING("EVENT : B-tagging working point "+wkpt+" does not exist.");
        cma::WARNING("EVENT : Return vector of b-tagged jets for default working point "+m_config->jet_btagWkpt());
        tmp_wkpt = m_config->jet_btagWkpt();
    }
    return m_btag_jets.at(tmp_wkpt);
}


float Event::met( const std::string& met_name ){
    // MET
    float met_value(0.0);
    if (met_name.compare("met")==0)
        met_value = m_metmet;
    else if (met_name.compare("phi")==0)
        met_value = m_metphi;
    else{
        cma::WARNING("EVENT : Request for MET variable that is neither 'met' nor 'phi'");
        cma::WARNING("EVENT : Returning 0.0");
    }

    return met_value;
}



/*** RETURN WEIGHTS ***/
float Event::weight_mc(){
    return **m_weight_mc;
}
float Event::weight_pileup(){
    return **m_weight_pileup;
}

float Event::weight_btag(const std::string &wkpt){
    std::string tmp_wkpt(wkpt);
    if(m_weight_btag.find(wkpt) == m_weight_btag.end()){
        cma::WARNING("EVENT : B-tagging working point "+wkpt+" does not exist");
        cma::WARNING("EVENT : Return calo-jet b-tag SF for default working point "+m_config->jet_btagWkpt());
        tmp_wkpt = m_config->jet_btagWkpt();
    }
    return m_weight_btag[tmp_wkpt];
}


// Get weight systematics
std::map<std::string,float> Event::weightSystematicsFloats(){
    /* systematics floats */
    std::map<std::string,float> tmp_weightSystematicsFloats;
    for (const auto& wsf : m_weightSystematicsFloats)
        tmp_weightSystematicsFloats[wsf.first] = **wsf.second;

    return tmp_weightSystematicsFloats;
}

std::map<std::string,std::vector<float> > Event::weightSystematicsVectorFloats(){
    /* weight systematics stored as vectors */
    std::map<std::string,std::vector<float> > tmp_weightSystematicsVectorFloats;
    return tmp_weightSystematicsVectorFloats;
}



/*** RETURN EVENT INFORMATION ***/
void Event::truth(){
    /* Do something with truth information (possibly change type and return information?) */
    return;
}

int Event::eventNumber(){
    return **m_eventNumber;
}

//unsigned int Event::runNumber(){
int Event::runNumber(){
    return **m_runNumber;
}

int Event::lumiblock(){
    return **m_lumiblock;
}




/*** DELETE VARIABLES ***/
void Event::finalize(){
    // delete variables
    cma::DEBUG("EVENT : Finalize() ");
    delete m_eventNumber;
    delete m_runNumber;
    delete m_lumiblock;

    if (m_config->useLargeRJets()){
      delete m_ljet_pt;          // jetAK8_Pt
      delete m_ljet_eta;         // jetAK8_Eta
      delete m_ljet_phi;         // jetAK8_Phi
      delete m_ljet_e;           // jetAK8_E
      delete m_ljet_tau1;
      delete m_ljet_tau2;
      delete m_ljet_tau3;
      delete m_ljet_tau21;
      delete m_ljet_tau32;
      delete m_ljet_charge;      // jetAK8_Charge
      delete m_ljet_SDmass;  // jetAK8_softDropMass
      delete m_ljet_CSVv2;                   // jetAK8_CSVv2
      delete m_ljet_jer_up;                 // jetAK8_JERSFUp
      delete m_ljet_jer_down;               // jetAK8_JERSFDown
      delete m_ljet_subjetIndex0;           // jetAK8_vSubjetIndex0
      delete m_ljet_subjetIndex1;           // jetAK8_vSubjetIndex1
      delete m_ljet_subjet_pt;                      // subjetAK8_Pt
      delete m_ljet_subjet_eta;                     // subjetAK8_Eta
      delete m_ljet_subjet_phi;                     // subjetAK8_Phi
      delete m_ljet_subjet_e;                       // subjetAK8_E
      delete m_ljet_subjet_charge;                  // subjetAK8_Charge
      delete m_ljet_subjet_CSVv2;
    }
    if (m_config->useLeptons()){
      delete m_el_pt;             // el_Pt
      delete m_el_eta;            // el_Eta
      delete m_el_phi;            // el_Phi
      delete m_el_e;              // el_E
      delete m_el_charge;         // el_Charge
      delete m_el_iso;
      delete m_el_veto;
      delete m_el_loose;
      delete m_el_medium;
      delete m_el_tight;

      delete m_mu_pt;         // mu_Pt
      delete m_mu_eta;        // mu_Eta
      delete m_mu_phi;        // mu_Phi
      delete m_mu_e;          // mu_E
      delete m_mu_charge;     // mu_Charge
      delete m_mu_iso;
      delete m_mu_loose;      // mu_IsLooseMuon
      delete m_mu_medium;     // mu_IsMediumMuon
      delete m_mu_medium2016; // mu_IsMediumMuon2016
      delete m_mu_tight;      // mu_IsTightMuon
    }
    if (m_config->useJets()){
    }

    delete m_met_met;          // met_Pt
    delete m_met_phi;          // met_Phi
    delete m_met_met_sf;
    delete m_met_phi_sf;

    // Event info 
    delete m_eventNumber;          // evt_EventNumber
    delete m_runNumber;            // evt_RunNumber
    delete m_rho;               // evt_rho
    delete m_lumiblock;            // evt_LumiBlock
    delete m_treeXSection;       // evt_XSec
    delete m_treeKFactor;
    delete m_treeSumOfWeights;
    delete m_NGoodVtx;             // evt_NGoodVtx
    delete m_LHAPDF_ID;            // evt_LHA_PDF_ID
    delete m_NIsoTrk;              // evt_NIsoTrk
    delete m_true_pileup;

    delete m_HLT_Ele45_WPLoose_Gsf;          // HLT_Ele45_WPLoose_Gsf
    delete m_HLT_Mu50;            // HLT_Mu50
    delete m_HLT_TkMu50;          // HLT_TkMu50

    if (m_isMC){
      delete m_weight_mc;
      delete m_weight_pileup;
      delete m_weight_pileup_UP;
      delete m_weight_pileup_DOWN;

      if (m_config->useTruth()){
        delete m_mc_ht;

        delete m_truth_jet_pt;           // jetAK4_GenJetPt
        delete m_truth_jet_eta;          // jetAK4_GenJetEta
        delete m_truth_jet_phi;          // jetAK4_GenJetPhi
        delete m_truth_jet_e;            // jetAK4_GenJetCharge

        delete m_truth_ljet_pt;            // jetAK8_GenJetPt
        delete m_truth_ljet_eta;           // jetAK8_GenJetEta
        delete m_truth_ljet_phi;           // jetAK8_GenJetPhi
        delete m_truth_ljet_e;             // jetAK8_GenJetE
        delete m_truth_ljet_charge;        // jetAK8_GenJetCharge
        delete m_truth_ljet_subjet_pt;     // subjetAK8_GenJetPt
        delete m_truth_ljet_subjet_eta;    // subjetAK8_GenJetEta
        delete m_truth_ljet_subjet_phi;    // subjetAK8_GenJetPhi
        delete m_truth_ljet_subjet_e;      // subjetAK8_GenJetE
        delete m_truth_ljet_subjet_charge; // subjetAK8_GenJetCharge
      } // end useTruth
    } // end isMC

    return;
}

// THE END

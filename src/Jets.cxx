        m_jetPtMin     = iConfig.getParameter<float>("jetPtMin");
        m_jetAbsEtaMax = iConfig.getParameter<float>("jetAbsEtaMax");

        t_jetPt   = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetPtLabel"));
        t_jetEta  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetEtaLabel"));
        t_jetPhi  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetPhiLabel"));
        t_jetE    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetEnergyLabel"));
        t_jetCSV  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetCSVLabel"));
        t_jetCMVA = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetCMVALabel"));
        t_jetCvsB = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetCvsBLabel"));
        t_jetCvsL = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetCvsLLabel"));
        t_jetJEC  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetJECLabel"));
        t_jetY    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetYLabel"));
        t_jetArea = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetAreaLabel"));
        t_jetnHadEnergy = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetnHadEnergyLabel"));
        t_jetnEMEnergy  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetnEMEnergyLabel"));
        t_jetcHadEnergy = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetcHadEnergyLabel"));
        t_jetcEMEnergy  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetcEMEnergyLabel"));
        t_jetcMultip    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetcMultipLabel"));
        t_jetnMultip    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetnMultipLabel"));
        t_jetHadronFlavour  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetPartonFlavourLabel"));
        t_jetPartonFlavour  = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetHadronFlavourLabel"));

        if (m_useTruth){
            t_jetGenPt     = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetGenPtLabel"));
            t_jetGenEta    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetGenEtaLabel"));
            t_jetGenPhi    = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetGenPhiLabel"));
            t_jetGenE      = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetGenELabel"));
            t_jetGenCharge = consumes<std::vector<float>>(iConfig.getParameter<edm::InputTag>("jetGenChargeLabel"));
        }





    evt.getByToken (t_rho,	 h_rho);
    evt.getByToken (t_jetPt,     h_jetPt);
    evt.getByToken (t_jetEta,    h_jetEta);
    evt.getByToken (t_jetPhi,    h_jetPhi);
    evt.getByToken (t_jetEnergy, h_jetEnergy);
    evt.getByToken (t_jetCSV,    h_jetCSV);
    evt.getByToken (t_jetCMVA,   h_jetCMVA);
    evt.getByToken (t_jetCvsB,   h_jetCvsB);
    evt.getByToken (t_jetCvsL,   h_jetCvsL);
    evt.getByToken (t_jetJEC,    h_jetJEC);
    evt.getByToken (t_jetY,	 h_jetY);
    evt.getByToken (t_jetArea,   h_jetArea);
    evt.getByToken (t_jetnHadEnergy, h_jetnHadEnergy);
    evt.getByToken (t_jetnEMEnergy,  h_jetnEMEnergy);
    evt.getByToken (t_jetcHadEnergy, h_jetcHadEnergy);
    evt.getByToken (t_jetcEMEnergy,  h_jetcEMEnergy);
    evt.getByToken (t_jetcMultip,    h_jetcMultip);
    evt.getByToken (t_jetnMultip,    h_jetnMultip);
    evt.getByToken (t_jetHadronFlavour, h_jetHadronFlavour);
    evt.getByToken (t_jetPartonFlavour, h_jetPartonFlavour);

    for (unsigned ijet=0, size=(h_jetPt.product())->size(); ijet<size; ++ijet) {
        Jet jet;

        // pT & |eta| min
        float jetPt  = (h_jetPt.product())->at(ijet);
        float jetEta = (h_jetEta.product())->at(ijet);
        if (abs(jetEta) > m_jetAbsEtaMax || jetPt < m_jetPtMin) continue;

        jet.p4.SetPtEtaPhiE( jetPt, jetEta, (h_jetPhi.product())->at(ijet), (h_jetE.product())->at(ijet));

        m_jets.push_back(jet);
    }


    // Generator jets
    m_truth_jets.clear()
    if (m_isMC && m_useTruth){
        evt.getByToken(t_jetGenPt,  h_jetGenPt);
        evt.getByToken(t_jetGenEta, h_jetGenEta);
        evt.getByToken(t_jetGenPhi, h_jetGenPhi);
        evt.getByToken(t_jetGenE,   h_jetGenE);

        for (unsigned ijet=0, size=(h_jetGenPt.product())->size(); ijet<size; ++ijet) {

            Jet jet;
            jet.p4.SetPtEtaPhiE( (h_jetGenPt.product())->at(ijet),  (h_jetGenPt.product())->at(ijet),
                                 (h_jetGenPhi.product())->at(ijet), (h_jetGenE.product())->at(ijet));

            m_truth_jets.push_back(jet);
        }
    }

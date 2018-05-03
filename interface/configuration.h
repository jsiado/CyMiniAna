#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "TROOT.h"
#include "TFile.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"

#include <iostream>
#include <sstream>

#include "Analysis/CyMiniAna/interface/tools.h"


class configuration {
  public:
    // Default - so root can load based on a name;
    configuration( const std::string &configFile );
    //configuration( const configuration& );
    configuration& operator=( const configuration& rhs );

    // Default - so we can clean up;
    virtual ~configuration();

    // Run once at the start of the job;
    virtual void initialize();
    std::string getConfigOption( std::string item );

    // Print configuration
    virtual void print();

    // Type of File(s) being processed
    virtual bool isMC() {return m_isMC;}              // must call "inspectFile(file)" or "isMC(file)" first!
    virtual bool isMC( TFile& file );
    bool isGridFile() {return m_isGridFile;}
    bool isExtendedSample(){ return m_isExtendedSample;}

    // object declarations
    virtual bool useJets() {return m_useJets;}
    virtual bool useLargeRJets() {return m_useLargeRJets;}
    virtual bool useNeutrinos() {return m_useNeutrinos;}
    virtual bool useLeptons() {return m_useLeptons;}
    virtual bool useTruth() {return m_useTruth;}
    virtual bool useDNN() {return m_useDNN;}
    virtual bool useWprime() {return m_useWprime;}

    std::string jet_btagWkpt() {return m_jet_btag_wkpt;}
    std::vector<std::string> btagWkpts() {return m_btag_WPs;}
    float cMVAv2L() {return m_cMVAv2L;}
    float cMVAv2M() {return m_cMVAv2M;}
    float cMVAv2T() {return m_cMVAv2T;}
    float CSVv2L()  {return m_CSVv2L;}
    float CSVv2M()  {return m_CSVv2M;}
    float CSVv2T()  {return m_CSVv2T;}

    std::vector<std::string> ejetsTriggers() {return m_ejetsTriggers;}
    std::vector<std::string> mujetsTriggers() {return m_mujetsTriggers;}

    // functions about the TTree
    virtual bool isNominalTree();
    virtual bool isNominalTree( const std::string &tree_name );
    std::vector<std::string> treeNames() {return m_treeNames;}
    void setTreename(std::string treeName);
    std::string treename() {return m_treename;}

    // functions about the file
    bool checkPrimaryDataset(const std::vector<std::string>& files);
    void readMetadata(TFile& file, const std::string& metadataTreeName);
    virtual void inspectFile( TFile& file, const std::string& metadataTreeName="" );
    std::vector<std::string> filesToProcess() {return m_filesToProcess;}
    bool recalculateMetadata() {return m_recalculateMetadata;}
    void setFilename(std::string fileName);
    std::string filename(){ return m_filename;}
    std::string primaryDataset(){ return m_primaryDataset;}
    unsigned int NTotalEvents(){ return m_NTotalEvents;}

    // return some values from config file
    std::string verboseLevel() {return m_verboseLevel;}
    std::vector<std::string> selections() {return m_selections;}
    std::vector<std::string> cutsfiles() {return m_cutsfiles;}
    std::string outputFilePath() {return m_outputFilePath;}
    std::string customDirectory() {return m_customDirectory;}
    std::string configFileName() {return m_configFile;}
    std::string getAbsolutePath() {return m_cma_absPath;}
    int nEventsToProcess() {return m_nEventsToProcess;}
    unsigned long long firstEvent() {return m_firstEvent;}
    bool makeTTree() {return m_makeTTree;}
    bool makeHistograms() {return m_makeHistograms;}
    bool makeEfficiencies() {return m_makeEfficiencies;}

    // information for event weights
    std::string metadataFile() {return m_metadataFile;}
    std::map<std::string,Sample> mapOfSamples(){return m_mapOfSamples;}
    Sample sample() {return m_mapOfSamples.at(m_primaryDataset);}
    virtual double LUMI() {return m_LUMI;}

    double XSectionMap ( std::string mcChannelNumber);
    double KFactorMap ( std::string mcChannelNumber );
    double sumWeightsMap ( std::string mcChannelNumber );

    // weight systematics
    bool calcWeightSystematics() {return m_calcWeightSystematics;}
    std::map<std::string,unsigned int> mapOfWeightVectorSystematics() {return m_mapOfWeightVectorSystematics;}
    std::vector<std::string> listOfWeightSystematics() {return m_listOfWeightSystematics;}
    std::string listOfWeightSystematicsFile() {return m_listOfWeightSystematicsFile;}
    std::string listOfWeightVectorSystematicsFile() {return m_listOfWeightVectorSystematicsFile;}

    // DNN
    std::string dnnFile() {return m_dnnFile;}
    std::string dnnKey() {return m_dnnKey;}       // key for lwtnn to use in map
    bool DNNinference(){ return m_DNNinference;}
    bool DNNtraining(){ return m_DNNtraining;}

    // Reco/Truth event loops
    bool doRecoEventLoop() {return m_doRecoEventLoop;}
    bool doTruthEventLoop() {return m_doTruthEventLoop;}
    bool matchTruthToReco() {return m_matchTruthToReco;} // true  -- match truth events to reco events (loop over reco events)
    void setMatchTruthToReco(bool truthToReco);

    // misc. for dilepton ttbar
    bool kinematicReco() {return m_kinematicReco;}
    bool neutrinoReco(){ return m_neutrinoReco;}
    bool wprimeReco(){ return m_wprimeReco;}

  protected:

    void check_btag_WP(const std::string &wkpt);

    std::map<std::string,std::string> m_map_config;
    const std::string m_configFile;

    // type of file(s)
    bool m_isMC;
    bool m_isGridFile;
    bool m_isExtendedSample;
    bool m_isQCD;
    bool m_isTtbar;
    bool m_isWjets;
    bool m_isSingleTop;
    bool m_isDiboson;
    bool m_isZjets;

    // object declarations
    bool m_useTruth;
    bool m_useJets;
    bool m_useLeptons;
    bool m_useLargeRJets;
    bool m_useNeutrinos;
    bool m_useDNN;
    bool m_useWprime;

    // luminosity
    double m_LUMI = 35870;   // 2015+2016 luminosity (/pb)

    // return some values from config file
    std::string m_input_selection;
    std::vector<std::string> m_selections;
    std::vector<std::string> m_cutsfiles;
    std::string m_treename;
    std::string m_filename;
    std::string m_primaryDataset;
    unsigned int m_NTotalEvents;
    std::string m_verboseLevel;
    int m_nEventsToProcess;
    unsigned long long m_firstEvent;
    std::string m_outputFilePath;
    std::string m_customDirectory;
    bool m_makeTTree;
    bool m_makeHistograms;
    bool m_makeEfficiencies;
    std::string m_cma_absPath;
    std::string m_metadataFile;
    bool m_DNNinference;
    bool m_DNNtraining;
    std::string m_dnnFile;
    std::string m_dnnKey;

    bool m_doRecoEventLoop;
    bool m_doTruthEventLoop;
    bool m_matchTruthToReco;

    std::string m_jet_btag_wkpt;   // "L","M","T"
    std::string m_tjet_btag_wkpt;
    std::string m_toptag_wkpt;

    // b-tagging (https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation80XReReco)
    // isBTagged = (jet.cMVAv2 > wkpt)
    std::vector<std::string> m_btag_WPs = {"L","M","T"};
    float m_cMVAv2L=-0.5884;
    float m_cMVAv2M=0.4432;
    float m_cMVAv2T=0.9432;
    float m_CSVv2L=0.5426;
    float m_CSVv2M=0.8484;
    float m_CSVv2T=0.9535;

    std::vector<std::string> m_filters = {"goodVertices",
        "eeBadScFilter",
        "HBHENoiseFilter",
        "HBHENoiseIsoFilter",
        "globalTightHalo2016Filter",
        "EcalDeadCellTriggerPrimitiveFilter"};

    std::vector<std::string> m_ejetsTriggers  = {"HLT_Ele45_CaloIdVT_GsfTrkIdT_PFJet200_PFJet50","HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165","HLT_Ele115_CaloIdVT_GsfTrkIdT"};
    std::vector<std::string> m_mujetsTriggers = {"HLT_Mu40_Eta2P1_PFJet200_PFJet50","HLT_Mu50","HLT_TkMu50"};

    bool m_recalculateMetadata;

    std::vector<std::string> m_filesToProcess;
    std::vector<std::string> m_treeNames;

    bool m_calcWeightSystematics;
    std::map<std::string,unsigned int> m_mapOfWeightVectorSystematics;
    std::vector<std::string> m_listOfWeightSystematics;
    std::string m_listOfWeightSystematicsFile;
    std::string m_listOfWeightVectorSystematicsFile;

    Sample m_sample;                               // struct of information for current sample
    std::map<std::string,Sample> m_mapOfSamples;  // map of Sample structs
    std::map<std::string, float> m_XSection; // map file to XSection
    std::map<std::string, float> m_KFactor;  // map file to KFactor
    std::map<std::string, float> m_AMI;      // map file to sum of weights
    std::map<std::string, unsigned int> m_NEvents;   // map file to total number of events

    std::vector<std::string> m_qcdSelections = {"0b0t","0b1t","0b2t","1b0t",
                                                "1b1t","1b2t","2b0t","2b1t", "2b2t"};

    double m_minDNN  = 0.0;   // min. value in the DNN discriminant
    double m_maxDNN  = 1.0;   // max. value in the DNN discriminant

    bool m_kinematicReco;
    bool m_neutrinoReco;
    bool m_wprimeReco;

    std::vector<std::string> m_mcFiles;

    // Primary dataset names for different samples in analysis
    std::map<std::string,std::string> m_mapOfPrimaryDatasets = {
        {"ttbar","TT_TuneCUETP8M2T4_13TeV-powheg-pythia8"},
        {"ttbar-ext","TT_TuneCUETP8M2T4_13TeV-powheg-pythia8-ext"},
        {"ttbarGOOD","TT_TuneCUETP8M1_13TeV-powheg-pythia8"},
        {"ttbarGEN","TT_TuneCUETP8M1_13TeV-powheg-pythia8"},
        {"singletop_schan","ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1"},
        {"singletop_tchan_top","ST_t-channel_top_4f_inclusiveDecays_TuneCUETP8M2T4_13TeV-powhegV2-madspin"},
        {"singletop_tchan_antitop","ST_t-channel_antitop_4f_inclusiveDecays_TuneCUETP8M2T4_13TeV-powhegV2-madspin"},
        {"singletop_tWchan_antitop","ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1"},
        {"singletop_tWchan_top","ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1"},
        {"wjets1","WJetsToLNu_Pt-100To250_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8"},
        {"wjets2","WJetsToLNu_Pt-250To400_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8"},
        {"wjets3","WJetsToLNu_Pt-400To600_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8"},
        {"wjets4","WJetsToLNu_Pt-600ToInf_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8"},
        {"diboson-ww","WW_TuneCUETP8M1_13TeV-pythia8"},
        {"diboson-wz","WZ_TuneCUETP8M1_13TeV-pythia8"},
        {"diboson-zz","ZZ_TuneCUETP8M1_13TeV-pythia8"},
        {"zjets","DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"},
        {"qcd","QCD_TuneCUETP8M1_13TeV_pythia8"},
        {"qcd_mu_pt0080","QCD_Pt-80to120_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8"},
        {"qcd_mu_pt0120","QCD_Pt-120to170_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8"},
        {"qcd_mu_pt0170","QCD_Pt-170to300_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8"},
        {"qcd_mu_pt0300","QCD_Pt-300to470_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8"},
        {"qcd_mu_pt0470","QCD_Pt-470to600_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8"},
        {"qcd_mu_pt0600","QCD_Pt-600to800_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8"},
        {"qcd_mu_pt0800","QCD_Pt-800to1000_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8"},
        {"qcd_mu_pt1000","QCD_Pt-1000toInf_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8"},
        {"qcd_el_pt0080","QCD_Pt-80to120_EMEnriched_TuneCUETP8M1_13TeV_pythia8"},
        {"qcd_el_pt0120","QCD_Pt-120to170_EMEnriched_TuneCUETP8M1_13TeV_pythia8"},
        {"qcd_el_pt0170","QCD_Pt-170to300_EMEnriched_TuneCUETP8M1_13TeV_pythia8"},
        {"qcd_el_pt0300","QCD_Pt-300toInf_EMEnriched_TuneCUETP8M1_13TeV_pythia8"},
        {"qcd_b_pt080","QCD_Pt_80to170_bcToE_TuneCUETP8M1_13TeV_pythia8"},
        {"qcd_b_pt170","QCD_Pt_170to250_bcToE_TuneCUETP8M1_13TeV_pythia8"},
        {"qcd_b_pt250","QCD_Pt_250toInf_bcToE_TuneCUETP8M1_13TeV_pythia8"},
    };

    std::vector<std::string> m_qcdFiles   = {"qcd","qcd_mu_pt0080","qcd_mu_pt0120","qcd_mu_pt0170","qcd_mu_pt0300",
                                             "qcd_mu_pt0470","qcd_mu_pt0600","qcd_mu_pt0800","qcd_mu_pt1000",
                                             "qcd_el_pt0080","qcd_el_pt0120","qcd_el_pt0170","qcd_el_pt0300",
                                             "qcd_b_pt080","qcd_b_pt170","qcd_b_pt250"};
    std::vector<std::string> m_ttbarFiles = {"ttbarGOOD","ttbarGEN","ttbar","ttbar-ext"};
    std::vector<std::string> m_wjetsFiles = {"wjets1","wjets2","wjets3","wjets4"};
    std::vector<std::string> m_singleTopFiles = {"singletop_schan","singletop_tWchan_antitop",
                                                 "singletop_tWchan_top","singletop_tchan_antitop",
                                                 "singletop_tchan_top","singletop_tchan_top"};
    std::vector<std::string> m_dibosonFiles = {"diboson-ww","diboson-wz","diboson-zz"};
    std::vector<std::string> m_zjetsFiles = {"zjets"};

    std::map<std::string,std::string> m_defaultConfigs = {
             {"isZeroLeptonAnalysis",  "false"},
             {"isOneLeptonAnalysis",   "false"},
             {"isTwoLeptonAnalysis",   "false"},
             {"useJets",               "false"},
             {"useLeptons",            "false"},
             {"useLargeRJets",         "false"},
             {"useNeutrinos",          "false"},
             {"neutrinoReco",          "false"},
             {"useTruth",              "false"},
             {"wprimeReco",            "false"},
             {"jet_btag_wkpt",         "M"},
             {"makeTTree",             "false"},
             {"makeHistograms",        "false"},
             {"makeEfficiencies",      "false"},
             {"NEvents",               "-1"},
             {"firstEvent",            "0"},
             {"isExtendedSample",      "false"},
             {"input_selection",       "grid"},
             {"selection",             "example"},
             {"output_path",           "./"},
             {"customDirectory",      ""},
             {"calcWeightSystematics", "false"},
             {"weightSystematicsFile",       "config/weightSystematics.txt"},
             {"weightVectorSystematicsFile", "config/weightVectorSystematics.txt"},
             {"cutsfile",              "examples/config/cuts_example.txt"},
             {"inputfile",             "examples/config/miniSL_ALLfiles.txt"},
             {"treenames",             "examples/config/treenames_nominal"},
             {"treename",              "tree/eventVars"},
             {"metadataFile",          "config/sampleMetaData.txt"},
             {"verboseLevel",          "INFO"},
             {"dnnFile",               "config/keras_ttbar_DNN.json"},
             {"dnnKey",                "dnn"},
             {"useDNN",                "false"},
             {"DNNinference",          "false"},
             {"DNNtraining",           "false"},
             {"doRecoEventLoop",       "true"},
             {"kinematicReco",         "false"} };
};

#endif

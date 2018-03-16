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

    // object declarations
    virtual bool useJets() {return m_useJets;}
    virtual bool useLargeRJets() {return m_useLargeRJets;}
    virtual bool useNeutrinos() {return m_useNeutrinos;}
    virtual bool useLeptons() {return m_useLeptons;}
    virtual bool useTruth() {return m_useTruth;}
    virtual bool useTtbar() {return m_useTtbar;}
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


    // functions about the TTree
    virtual bool isNominalTree();
    virtual bool isNominalTree( const std::string &tree_name );
    std::vector<std::string> treeNames() {return m_treeNames;}
    void setTreename(std::string treeName);
    std::string treename() {return m_treename;}

    // functions about the file
    std::vector<std::string> filesToProcess() {return m_filesToProcess;}
    void setFilename(std::string fileName);
    std::string filename() {return m_filename;}
    std::string primaryDataset() {return m_primaryDataset;}
    unsigned int NTotalEvents() {return m_NTotalEvents;}

    // return some values from config file
    std::string verboseLevel() {return m_verboseLevel;}
    std::string selection() {return m_selection;}
    std::string cutsfile() {return m_cutsfile;}
    std::string outputFilePath() {return m_outputFilePath;}
    std::string customFileEnding() {return m_customFileEnding;}
    std::string configFileName() {return m_configFile;}
    std::string getAbsolutePath() {return m_cma_absPath;}
    int nEventsToProcess() {return m_nEventsToProcess;}
    unsigned long long firstEvent() {return m_firstEvent;}
    bool makeTTree() {return m_makeTTree;}
    bool makeHistograms() {return m_makeHistograms;}
    bool makeEfficiencies() {return m_makeEfficiencies;}

    // information for event weights
    void inspectFile( TFile& file );
    std::string metadataFile() {return m_metadataFile;}

    std::map<std::string,Sample> mapOfSamples(){return m_mapOfSamples;}
    Sample sample(){return m_mapOfSamples.at(m_primaryDataset);}

    double XSectionMap ( std::string mcChannelNumber);
    double KFactorMap ( std::string mcChannelNumber );
    double sumWeightsMap ( std::string mcChannelNumber );
    virtual double LUMI() {return m_LUMI;}

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
    float beamEnergy() {return m_beamEnergy;}           // 13000.;
    double topQuarkMass() {return m_topQuarkMass;}      // 172.5
    double bQuarkMass() {return m_bQuarkMass;}          // 4.18
    double WMass() {return m_WMass;}                    // 80.2

  protected:

    void check_btag_WP(const std::string &wkpt);

    std::map<std::string,std::string> m_map_config;
    const std::string m_configFile;

    // type of file(s)
    bool m_isMC;
    bool m_isGridFile;

    // object declarations
    bool m_useTruth;
    bool m_useJets;
    bool m_useLeptons;
    bool m_useLargeRJets;
    bool m_useNeutrinos;
    bool m_useTtbar;
    bool m_useDNN;
    bool m_useWprime;

    // luminosity
    double m_LUMI = 35.89;   // 2015+2016 luminosity

    // return some values from config file
    std::string m_input_selection;
    std::string m_selection;
    std::string m_cutsfile;
    std::string m_treename;
    std::string m_filename;
    std::string m_primaryDataset;
    unsigned int m_NTotalEvents;
    std::string m_verboseLevel;
    int m_nEventsToProcess;
    unsigned long long m_firstEvent;
    std::string m_outputFilePath;
    std::string m_customFileEnding;
    bool m_makeTTree;
    bool m_makeHistograms;
    bool m_makeEfficiencies;
    std::string m_sumWeightsFiles;
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

    std::vector<std::string> m_filesToProcess;
    std::vector<std::string> m_treeNames;

    bool m_calcWeightSystematics;
    std::map<std::string,unsigned int> m_mapOfWeightVectorSystematics;
    std::vector<std::string> m_listOfWeightSystematics;
    std::string m_listOfWeightSystematicsFile;
    std::string m_listOfWeightVectorSystematicsFile;

    std::map<std::string,Sample> m_mapOfSamples;  // map of Sample structs
    std::map<std::string, float> m_XSection; // map file to XSection
    std::map<std::string, float> m_KFactor;  // map file to KFactor
    std::map<std::string, float> m_AMI;      // map file to sum of weights
    std::map<std::string, unsigned int> m_NEvents;   // map file to total number of events

    std::vector<std::string> m_qcdSelections = {"0b0t","0b1t","0b2t","1b0t",
                                                "1b1t","1b2t","2b0t","2b1t", "2b2t"};

    double m_minDNN  = 0.0;   // min. value in the DNN discriminant
    double m_maxDNN  = 1.0;   // max. value in the DNN discriminant

    // -- Top Mass Variables -- //
    const double m_electronMass = 0.000511;
    const double m_muonMass     = 0.105658;
    const double m_bQuarkMass   = 4.8;
    const double m_WMass        = 80.4;
    const double m_topQuarkMass = 172.5;
    const float m_beamEnergy    = 13000.;
    const int SENTINEL    = -1000;
    const int NCHAN       = 4;
    const double m_sqrt_s = 13000;      // center-of-mass energy

    bool m_kinematicReco;
    bool m_neutrinoReco;
    bool m_wprimeReco;

    std::vector<std::string> m_mcFiles;

    std::map<std::string,std::string> m_defaultConfigs = {
             {"useJets",               "false"},
             {"useLeptons",            "false"},
             {"useLargeRJets",         "false"},
             {"useNeutrinos",          "false"},
             {"useTruth",              "false"},
             {"neutrinoReco",          "false"},
             {"wprimeReco",            "false"},
             {"jet_btag_wkpt",         "M"},
             {"makeTTree",             "false"},
             {"makeHistograms",        "false"},
             {"makeEfficiencies",      "false"},
             {"NEvents",               "-1"},
             {"firstEvent",            "0"},
             {"input_selection",       "grid"},
             {"selection",             "example"},
             {"output_path",           "./"},
             {"customFileEnding",      ""},
             {"calcWeightSystematics", "false"},
             {"weightSystematicsFile",       "config/weightSystematics.txt"},
             {"weightVectorSystematicsFile", "config/weightVectorSystematics.txt"},
             {"cutsfile",              "examples/config/cuts_example.txt"},
             {"inputfile",             "examples/config/miniSL_ALLfiles.txt"},
             {"treenames",             "examples/config/treenames_nominal"},
             {"sumWeightsFiles",       "examples/config/miniSL_ALLMCFiles.txt"},
             {"verboseLevel",          "INFO"},
             {"dnnFile",               "config/keras_DNN_dummy.json"},
             {"dnnKey",                "dnn"},
             {"DNNinference",          "false"},
             {"DNNtraining" ,          "false"},
             {"doRecoEventLoop",       "true"},
             {"doTruthEventLoop",      "false"},
             {"kinematicReco",        "true"} };
};

#endif

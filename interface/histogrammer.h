#ifndef HISTOGRAMMER_H
#define HISTOGRAMMER_H

#include <string>
#include <map>
#include <vector>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TSystem.h"

#include "Analysis/CyMiniAna/interface/tools.h"

class histogrammer : public edm::one::EDAnalyzer<edm::one::SharedResources> {
  public:

    histogrammer( const edm::ParameterSet & );
    virtual ~histogrammer();


    /* Book histograms */
    /* initialize histograms (1D) */
    virtual void init_hist( const std::string& name, 
                                  const unsigned int nBins, const double x_min, const double x_max );
    virtual void init_hist( const std::string& name, 
                                  const unsigned int nBins, const double *xbins );
    /* initialize histograms (2D) */
    virtual void init_hist( const std::string& name, 
                                  const unsigned int nBinsX, const double x_min, const double x_max,
                                  const unsigned int nBinsY, const double y_min, const double y_max );
    virtual void init_hist( const std::string& name, 
                                  const unsigned int nBinsX, const double *xbins,
                                  const unsigned int nBinsY, const double *ybins );
    /* initialize histograms (3D) */
    virtual void init_hist( const std::string& name, const unsigned int nBinsX, const double x_min, const double x_max,
                              const unsigned int nBinsY, const double y_min, const double y_max,
                              const unsigned int nBinsZ, const double z_min, const double z_max );
    virtual void init_hist( const std::string& name, const unsigned int nBinsX, const double *xbins,
                              const unsigned int nBinsY, const double *ybins,  
                              const unsigned int nBinsZ, const double *zbins );
    virtual void bookHists( const std::string name );


    /* fill histograms */
    virtual void fill( const std::string& name, const edm::Event& event, double event_weight );
    virtual void fill( const std::string& name, const double& value, const double& weight );
    virtual void fill( const std::string& name, const double& xvalue, const double& yvalue, const double& weight );
    virtual void fill( const std::string& name, const double& xvalue, const double& yvalue, const double& zvalue, const double& weight );

    /* Put over/underflow in last/first bins.  Called from outside macro */
    virtual void overUnderFlow();
    virtual void overFlow();
    virtual void underFlow();

  private:
    virtual void beginJob() override;
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;

    edm::Service<TFileService> m_fs;
    edm::InputTag m_src;

    std::string m_name;
    bool m_isMC;
    bool m_useSystWeights;
    bool m_putOverflowInLastBin;
    bool m_putUnderflowInFirstBin;

    bool m_useJets;
    bool m_useLeptons;
    bool m_useNeutrinos;

    std::vector<std::string> m_listOfWeightSystematics;
    std::map<std::string,unsigned int> m_mapOfWeightVectorSystematics;

    std::map<std::string, TH1D*> m_map_histograms1D;
    std::map<std::string, TH2D*> m_map_histograms2D;
    std::map<std::string, TH3D*> m_map_histograms3D;
};

#endif

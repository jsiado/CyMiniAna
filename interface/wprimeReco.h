#ifndef WPRIMERECO_H
#define WPRIMERECO_H

#include <string>
#include <map>
#include <vector>

#include "Analysis/CyMiniAna/interface/tools.h"
#include "Analysis/CyMiniAna/interface/configuration.h"
#include "Analysis/CyMiniAna/interface/physicsObjects.h"


class WprimeReco {
  public:
    WprimeReco( configuration& cmaConfig );

    ~WprimeReco();

    void initialize();
    Wprime execute();
    void getWprime( const Jet& j0, const Jet& j1 );

    void setLepton(Lepton& lepton);
    void setBtagJets(std::vector<int> bjets);
    void setJets(std::vector<Jet>& jets);
    void setNeutrino(Neutrino& nu);

  protected:

    configuration *m_config;

    Lepton m_lepton;
    Neutrino m_neutrino;
    std::vector<Jet> m_jets;
    std::vector<int> m_btag_jets;
    Wprime m_wprime;
};

#endif

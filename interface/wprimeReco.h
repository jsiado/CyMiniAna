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
    void execute();

  protected:

    configuration *m_config;
};

#endif


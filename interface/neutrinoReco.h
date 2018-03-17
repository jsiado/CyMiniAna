#ifndef NEUTRINORECO_H
#define NEUTRINORECO_H

#include <string>
#include <map>
#include <vector>

#include "Analysis/CyMiniAna/interface/tools.h"
#include "Analysis/CyMiniAna/interface/configuration.h"
#include "Analysis/CyMiniAna/interface/physicsObjects.h"


class NeutrinoReco {
  public:
    NeutrinoReco( configuration& cmaConfig );

    ~NeutrinoReco();

    void initialize();
    void execute();

  protected:

    configuration *m_config;
};

#endif


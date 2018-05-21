#ifndef NEUTRINORECO_H
#define NEUTRINORECO_H

#include <string>
#include <vector>
#include <cmath> 

#include "TH1D.h"
#include "TRandom3.h"

#include "Analysis/CyMiniAna/interface/tools.h"
#include "Analysis/CyMiniAna/interface/configuration.h"
#include "Analysis/CyMiniAna/interface/physicsObjects.h"


class NeutrinoReco {
  public:
    NeutrinoReco( configuration& cmaConfig );

    ~NeutrinoReco();

    void setObjects(Lepton& lepton, MET& met);
    void setLepton(Lepton& lepton);
    void setMET(MET& met);

    float execute(const bool standard, const float wmass=80.4);   // build the neutrino assuming W mass [GeV]
    void nu_pz(float wmass);              // calculate the pz  
    void sampling();                      // wrapper around nu_pz() to build pz with different wmass values

    bool isImaginary(){ return m_isImaginary;}

    std::vector<float> pzSolutions();

  protected:

    configuration *m_config;

    Neutrino m_nu;
    Lepton m_lepton;
    MET m_met;

    TRandom3* r;
    TH1D* m_wmass_hist;
    unsigned int m_sampling;

    bool m_isImaginary;

    std::vector<float> m_pz_solutions;
};

#endif

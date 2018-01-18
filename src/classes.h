/*
Allow custom structs to be stored in the Event
*/
#include "DataFormats/Common/interface/Wrapper.h"
#include "Analysis/CyMiniAna/interface/physicsObjects.h"

#include <vector>

namespace {
  struct dictionary {

edm::Wrapper<JRAEvent> dummy1;

    edm::Wrapper<Electron> el_dummy;
    std::vector<Electron> v_el_dummy1;
    edm::Wrapper<std::vector<Electron>> v_el_dummy2;

    edm::Wrapper<Muon> mu_dummy;
    std::vector<Muon> v_mu_dummy1;
    edm::Wrapper<std::vector<Muon>> v_mu_dummy2;

    edm::Wrapper<Neutrino> nu_dummy;
    std::vector<Neutrino> v_nu_dummy1;
    edm::Wrapper<std::vector<Neutrino>> v_nu_dummy2;

    edm::Wrapper<Jet> jet_dummy;
    std::vector<Jet> v_jet_dummy1;
    edm::Wrapper<std::vector<Jet>> v_jet_dummy2;

    edm::Wrapper<Ljet> ljet_dummy;
    std::vector<Ljet> v_ljet_dummy1;
    edm::Wrapper<std::vector<Ljet>> v_ljet_dummy1;

    edm::Wrapper<MET> met_dummy;

  };
}

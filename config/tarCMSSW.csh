# Created: 9 April 2018
#
# Dan Marley
# daniel.edison.marley@cernSPAMNOT.ch
# Texas A&M University
# ---
# Tar the current CMSSW directory for running condor jobs


# set path for where to put the tarball
set eos_path = "/store/group/lpctlbsm/wprime-qQ/flatNtuples/viper/"


echo " Make tarball ..."

tar --exclude-caches-all --exclude-vcs -zcf $CMSSW_VERSION.tgz -C $CMSSW_BASE/.. $CMSSW_VERSION \
    --exclude=$CMSSW_VERSION/src/Analysis/CyMiniAna/data \
    --verbose


echo " Copy tarball to new location on EOS "$eos_path
xrdcp $CMSSW_VERSION.tgz root://cmseos.fnal.gov/"$eos_path"

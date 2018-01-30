21 January 2018  
Dan Marley  


# CyMiniAna

#### C++/Python Analysis Framework

This class is derived from the [CyMiniAna](https://gitlab.cern.ch/dmarley/CyMiniAna) base class.  
Dependencies (available in CMSSW):

    - ROOT6
    - python2.7 (matplotlib & numpy)


## Overview

CyMiniAna is built to process both EDM  
(from the [B2GAnaFW](https://github.com/cmsb2g/B2GAnaFW/tree/CMSSW_8_0_X_V3)) and flat ntuples.  
Flat ntuples are generally much easier to work with, so the framework is designed
to process the EDM ntuples into flat ntuples, which can then be used for analysis studies.

  1. EDMNtuples
     - Generate flat ntuples based on an event/object selection with histograms
  2. Flat ntuples
     - Generate histograms, efficiency curves, and skimmed/slimmed flat ntuples
     - Perform variaous analysis studies

Additionally, it is possible to generate publication-quality figures using python:

 - Make stacked histogram plots that compare the prediction with Data
 - Plot generic histograms (with support for ratio plots, including significance)
 - Plot general efficiency curves (with support for drawing the underlying distribution)

The CyMiniAna Analysis Framework is structured as follows:

Directory  | About
---------  | ---------
bin/       | steering macros (for flat ntuples)
config/    | configuration files (running options, lists of files/histograms, etc.)
python/    | plotting, neural network, CMSSW configuration, and running scripts
plugins/   | EDAnalyzers, EDProducers, and EDFilters
src/       | `*.cxx` files (classes)
interface/ | `*.h` files
examples/  | Example scripts
BuildFile.xml | File used by `scram` to compile software in CMSSW
Makefile   | compiles c++ code -- not needed in CMSSW framework!
README.md  | This file
setup.sh   | Setup script for cmssw environment



## Getting Started

Before running an analysis, check out the relevant tag for your studies or make a *new* branch to
perform your work.

**DO NOT USE THE MASTER BRANCH**  
_The master branch is only used to make new tags!_

To get started, you first need a proper CMSSW release environment and other packages to work with CyMiniAna.  
This package has been developed within an environment that includes the most recent CMSSW80X 
[B2GAnaFW](https://github.com/cmsb2g/B2GAnaFW/tree/CMSSW_8_0_X_V3).  
It is not completely necessary to include it, but it is recommended.

This package depends on the [EventCounter](https://github.com/dmajumder/EventCounter) 
and [lwtnn](https://github.com/lwtnn/lwtnn) packages.  
To include the `lwtnn` package, please follow the steps
[here](https://github.com/demarley/lwtnn/tree/CMSSW_8_0_X-compatible#cmssw-compatibility).
The `lwtnn` package is not currently used, but the dependency exists for future use.

```shell
## setup CMSSW (slc6_amd64_gcc530)
cmsrel CMSSW_8_0_26_patch1
cd CMSSW_8_0_26_patch1/src/
cmsenv
git cms-init

## add necessary packages
# 1. Clone the lwtnn package 
# 2. Add the "EventCounter" package
mkdir Analysis
cd Analysis/
git clone https://github.com/dmajumder/EventCounter.git
cd ..

## add our code
cd Analysis/
git clone https://github.com/b2g-wprime-qQ/CyMiniAna.git
```

Once everything is checked out, compile it all!

```
scram b -j8
```

When opening a new shell, you can easily setup the environment by running:

```shell
source Analysis/CyMiniAna/setup.csh   # ALWAYS DO THIS FIRST! (initializes everything)
```

And anytime you modify `*.cxx` or `*.h` code:
```shell
scram b -j8
```


# Contact

If there are questions, concerns, or bugs found in this package, please contact the author.  
If you would like contribute to the project, it is recommended to follow the 
[feature branch workflow](https://www.atlassian.com/git/tutorials/comparing-workflows/feature-branch-workflow).  
See `CONTRIBUTING.md` for more information.

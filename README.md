7 June 2018  
Dan Marley  


# CyMiniAna

#### C++/Python Analysis Framework

This class is derived from the [CyMiniAna](https://gitlab.cern.ch/dmarley/CyMiniAna) base class.  
Dependencies (available in CMSSW):

    - ROOT6
    - python2.7 (matplotlib & numpy)


## Overview

CyMiniAna is built to process flat ntuples.  
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

This package depends on the  [lwtnn](https://github.com/lwtnn/lwtnn) package.  
To include the `lwtnn` package, please follow the steps
[here](https://github.com/demarley/lwtnn/tree/CMSSW_8_0_X-compatible#cmssw-compatibility).
The `lwtnn` package is only used for testing purposes.

```shell
## setup CMSSW (slc6_amd64_gcc530)
cmsrel CMSSW_9_4_4
cd CMSSW_9_4_4/src/
cmsenv
git cms-init

## add necessary packages
# 1. Clone the lwtnn package 
# 2. Add our code
mkdir Analysis
git clone https://github.com/b2g-wprime-qQ/CyMiniAna.git Analysis/CyMiniAna
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

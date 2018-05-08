/*
Created:        --
Last Updated:    7 May 2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University
-----

Basic steering macro for running CyMiniAna
 - Make flat ntuples for machine learning
 - Make histograms to compare features
*/
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TSystem.h"
#include "TMath.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <map>
#include <fstream>
#include <string>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <boost/algorithm/string/join.hpp>

#include "Analysis/CyMiniAna/interface/configuration.h"
#include "Analysis/CyMiniAna/interface/Event.h"
#include "Analysis/CyMiniAna/interface/eventSelection.h"
#include "Analysis/CyMiniAna/interface/flatTree4ML.h"
#include "Analysis/CyMiniAna/interface/tools.h"
#include "Analysis/CyMiniAna/interface/histogrammer4ML.h"


int main(int argc, char** argv) {
    /* Steering macro for CyMiniAna */
    if (argc < 2) {
        cma::HELP();
        return -1;
    }

    unsigned long long maxEntriesToRun(0);     // maximum number of entries in TTree
    unsigned int numberOfEventsToRun(0);       // number of events to run

    // configuration
    configuration config(argv[1]);                         // configuration file
    config.initialize();

    long long nEvents(config.nEventsToProcess());                    // requested number of events to run
    std::string outpath(config.outputFilePath());                    // directory for output files
    std::string outpathBase(config.outputFilePath());                // directory for output files (base name to modify)
    unsigned long long firstEvent(config.firstEvent());              // first event to begin running over
    std::vector<std::string> filenames  = config.filesToProcess();
    std::vector<std::string> treenames  = config.treeNames();
    std::vector<std::string> selections = config.selections();
    std::vector<std::string> cutfiles   = config.cutsfiles();
    std::string selection = boost::algorithm::join(selections, "-");

    bool generateCutsFiles = (cutfiles.size()!=selections.size());   // user did not provide different cuts files
    std::string treename( treenames.at(0) );

    std::string customDirectory( config.customDirectory() );
    if (customDirectory.length()>0  && customDirectory.substr(0,1).compare("_")!=0){
        customDirectory = "_"+customDirectory; // add '_' to beginning of string, if needed
    }

    // event selection(s) -- support for multiple event selections simulataneously
    std::vector<eventSelection> evtSels;
    std::vector<unsigned int> ncuts;                         // number of cuts in selection
    std::vector< std::vector<std::string> > namesOfCuts;     // names of cuts in selection
    for (unsigned int ss=0, size=selections.size(); ss<size; ss++) {
        std::string sel      = selections.at(ss);
        std::string cutsfile = (generateCutsFiles) ? "config/cuts_"+sel+".txt" : cutfiles.at(ss);

        eventSelection evtSel_tmp( config );
        evtSel_tmp.initialize( sel, cutsfile );

        evtSels.push_back(evtSel_tmp);
        ncuts.push_back(evtSel_tmp.numberOfCuts());
        namesOfCuts.push_back( evtSel_tmp.cutNames() );
    }


    // --------------- //
    // -- File loop -- //
    // --------------- //
    unsigned int numberOfFiles(filenames.size());
    unsigned int currentFileNumber(0);
    cma::INFO("RUNML : *** Starting file loop *** ");
    for (const auto& filename : filenames) {

        ++currentFileNumber;
        cma::INFO("RUNML :   Opening "+filename+"   ("+std::to_string(currentFileNumber)+"/"+std::to_string(numberOfFiles)+")");

        auto file = TFile::Open(filename.c_str());
        if (!file || file->IsZombie()){
            cma::WARNING("RUNML :  -- File: "+filename);
            cma::WARNING("RUNML :     does not exist or it is a Zombie. ");
            cma::WARNING("RUNML :     Continuing to next file. ");
            continue;
        }

        // Inspecting the file
        std::vector<std::string> fileKeys;
        cma::getListOfKeys(file,fileKeys);                  // keep track of ttrees in file

        std::string metadata_treename("tree/metadata");     // hard-coded for now
        std::vector<std::string> metadata_names;
        cma::split(metadata_treename, '/', metadata_names);
        if (std::find(fileKeys.begin(), fileKeys.end(), metadata_treename) == fileKeys.end())
            metadata_treename = "";  // metadata TTree doesn't exist, set this so "config" won't look for it
        config.inspectFile( *file,metadata_treename );      // check the type of file being processed
        Sample s = config.sample();

        // -- Output file -- //
        // CMS doesn't use 'mcChannelNumber', need to keep the same file names
        // therefore, make new directories for the different selections
        struct stat dirBuffer;
        std::string outpath = outpathBase+"/"+selection+customDirectory;
        if ( !(stat((outpath).c_str(),&dirBuffer)==0 && S_ISDIR(dirBuffer.st_mode)) ){
            cma::DEBUG("RUNML : Creating directory for storing output: "+outpath);
            system( ("mkdir "+outpath).c_str() );  // make the directory so the files are grouped together
        }

        std::size_t pos   = filename.find_last_of(".");     // the last ".", i.e., ".root"
        std::size_t found = filename.find_last_of("/");     // the last "/"
        std::string outputFilename = filename.substr(found+1,pos-1-found); // betwee "/" and "."
        // hopefully this returns: "diboson_WW_361082" given something like:
        // "/some/path/to/file/diboson_WW_361082.root"

        std::string fullOutputFilename = outpath+"/"+outputFilename+".root";
        std::unique_ptr<TFile> outputFile(TFile::Open( fullOutputFilename.c_str(), "RECREATE"));
        cma::INFO("RUNML :   >> Saving to "+fullOutputFilename);

        histogrammer4ML histMaker(config,"ML");      // initialize histogrammer
        histMaker.initialize( *outputFile );

        for (auto& x : evtSels) x.setCutflowHistograms( *outputFile );  // setup cutflow histograms

        // check that the ttree exists in this file before proceeding
        if (std::find(fileKeys.begin(), fileKeys.end(), treename) == fileKeys.end()){
            cma::INFO("RUNML : TTree "+treename+" is not present in this file, continuing to next TTree");
            continue;
        }

        // -- Load TTree to loop over
        cma::INFO("RUNML :      TTree "+treename);
        TTreeReader myReader(treename.c_str(), file);

        // -- Make new Tree in Root file
        flatTree4ML miniTTree(config);          // initialize TTree for new file
        miniTTree.initialize( *outputFile );

        // -- Number of Entries to Process -- //
        maxEntriesToRun = myReader.GetEntries(true);
        if (maxEntriesToRun<1) // skip files with no entries
            continue;

        if (nEvents < 0 || ((unsigned int)nEvents+firstEvent) > maxEntriesToRun)
            numberOfEventsToRun = maxEntriesToRun - firstEvent;
        else
            numberOfEventsToRun = nEvents;

        // ---------------- //
        // -- Event Loop -- //
        // ---------------- //
        Long64_t imod = 1;                     // print to the terminal
        Event event = Event(myReader, config);

        Long64_t eventCounter = 0;    // counting the events processed
        Long64_t entry = firstEvent;  // start at a different event!
        while (myReader.Next()) {

            if (eventCounter+1 > numberOfEventsToRun){
                cma::INFO("RUNML : Processed the desired number of events: "+std::to_string(eventCounter)+"/"+std::to_string(numberOfEventsToRun));
                break;
            }

            if (entry%imod==0){
                cma::INFO("RUNML :       Processing event "+std::to_string(entry) );
                if(imod<2e4) imod *=10;
            }

            // -- Build Event -- //
            cma::DEBUG("RUNML : Execute event");
            event.execute(entry);
            // now we have event object that has the event-level objects in it
            // pass this to the selection tools

            // -- Event Selection -- //
            // can do separate cutflows by creating multiple instances of eventSelection()
            cma::DEBUG("RUNML : Apply event selection");
            std::vector<unsigned int> passEvents;
            unsigned int passedEvents(0);
            for (unsigned int ss=0,size=selections.size();ss<size;ss++){
                bool passEvent = evtSels.at(ss).applySelection(event);
                cma::DEBUG("RUNML : Event selection "+selections.at(ss)+" = "+std::to_string(passEvent));
                passEvents.push_back( passEvent );
                passedEvents += passEvent;
            }

            if (passedEvents>0){
                cma::DEBUG("RUNML : Passed selection, now save information");
                std::map<std::string,double> features2save;        // save features related to neutrino pz


                std::map<std::string,double> features = event.deepLearningFeatures();
                if (features.size()>0) { // only save information if we have features to save!

                    for (const auto& x : features )
                        features2save[x.first] = x.second;

                    features2save["xsection"] = s.XSection;
                    features2save["kfactor"]  = s.KFactor;
                    features2save["sumOfWeights"]   = s.sumOfWeights;
                    features2save["nominal_weight"] = event.nominal_weight();
                    features2save["weight"] = 1.;  // weight the entries in the network in some way

                    miniTTree.saveEvent(features2save);
                    histMaker.fill(features2save);
                }
            }

            // iterate the entry and number of events processed
            ++entry;
            ++eventCounter;
        } // end event loop

        event.finalize();
        miniTTree.finalize();

        // put overflow/underflow content into the first and last bins
        histMaker.overUnderFlow();

        cma::INFO("RUNML :   END Running  "+filename);
        cma::INFO("RUNML :   >> Output at "+fullOutputFilename);

        outputFile->Write();
        outputFile->Close();

        // -- Clean-up stuff
        delete file;          // free up some memory 
        file = ((TFile *)0);  // (no errors for too many root files open)
    } // end file loop

    cma::INFO("RUNML : *** End of file loop *** ");
    cma::INFO("RUNML : Program finished. ");
}

// THE END


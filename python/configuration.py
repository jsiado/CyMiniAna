"""
Created:        2 August  2017
Last Updated:  14 January 2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University
-----

Configuration class for getting/setting parameters
to use when running CMSSW over EDM Ntuples
"""
import os
import sys
import util

class Configuration(object):
    """Configuration object that handles the setup"""
    def __init__(self,configfile,script='cma_cfg.py',verbose_level="INFO"):
        self.filename = configfile

        self.vb = util.VERBOSE()
        self.vb.level = verbose_level
        self.vb.name  = "CONFIG"

        self.configuration = {}      # hold all values in dictionary (as strings)
        self.script        = script  # the script that calls this class (e..g, createJobs.py)

        self.files    = []
        self.defaults = {}

        self.set_defaults()


    def initialize(self):
        """
           Initialize the configuration
           Read the configuration file and set arguments 
           (overwrite default values)
        """
        file = open(self.filename,'r').readlines()
        line_skip = ["#"," ","\n"]
        for line in file:
            if any( [line.startswith(i) for i in line_skip] ): continue

            param,value = line.split(' ')
            value       = value.rstrip('\n')

            self.configuration[param] = value

        # Set some configurations
        files = self.get("inputfile")
        files = open(files,"r").readlines()
        self.configuration["inputfile"] = [i.rstrip("\n") for i in files]

        trees  = self.get("treenames")
        trees = open(trees,"r").readlines()
        self.configuration["treenames"] = [i.rstrip("\n") for i in trees]

        self.configuration["NEvents"] = int( self.configuration["NEvents"] )

        # Change boolean config options to bools, not strings
        for key in self.defaults.keys():
            value = self.defaults[key]
            if type( value )==bool:
                self.configuration[key] = util.str2bool( self.configuration[key] )

        return


    def get(self,param):
        """Return the value of a configuration parameter to the user"""
        value = None

        try:
            value = self.configuration[param]
        except KeyError:
            self.vb.WARNING("The configuration file does not contain {0}".format(param))
            self.vb.WARNING("Using default value.")
            try:
                value = self.defaults[param]
            except KeyError:
                raise KeyError("There is no default value for {0}.\n"
                               "Please set this parameter in the configuration.".format(param))

        return value


    def configuration(self):
        """Return the full configuration dictionary to the user"""
        return self.configuration


    def set_defaults(self):
        """Set default values for configurations"""
        self.defaults = {
            'verbose_level':"INFO",
            'isMC':False,
            'NEvents':-1,
            'outputFileName':'output.root',
            'dataFilePath':os.environ["CMSSW_BASE"]+"/src/Analysis/CyMiniAna/data",
            'jerShift':None,
            'jecShift':None,
            'inputfile':[],
            'useJets':False,
            'useLargeRJets':False,
            'useLeptons':False,
            'useNeutrinos':False,
            'buildNeutrinos':False,
            'kinematicReco':False,
            'metadataFile':"config/sampleMetaData.txt",
            'LUMI':35900.,
            'useTruth':False,
            'getDNN':False,
            'jet_btag_wkpt':'medium',
            'treenames':"config/treenames_nominal.txt",
            'input_selection':'b2g',
            'makeNewFile':True,
            'makeHistograms':True,
            'makeEfficiencies':False,
            'selection':None,
            'output_path': "./",
            'cutsfile':"config/cuts_none.txt",
            'useSystWeights':False,
            'weightSystematicsFile':"config/weightSystematics.txt",
            'weightVectorSystematicsFile':"config/weightVectorSystematics.txt",
        }
        # copy defaults into the configuration
        self.configuration = dict( (k,self.defaults[k]) for k in self.defaults.keys() )

        return


    ## Configuration attributes
    def verbose_level(self):
        """Verbosity of output"""
        return self.get("verbose_level")
    def NEvents(self):
        """Maximum number of events"""
        return self.get("NEvents")
    def dataFilePath(self):
        """Path to 'data/' directory"""
        return self.get("dataFilePath")

    ## SAMPLE INFORMATION
    def isMC(self,filename=None):
        """Determine if a file is MC or Data given a filename (or not)"""
        datanames = ["/SingleElectron","/SingleMuon","/JetHT"]
        if filename is not None:
            if any( [i in filename for i in datanames] ):
                self.configuration["isMC"] = False
            else:
                self.configuration["isMC"] = True

        is_MC = self.get("isMC")
        if not is_MC and self.get("useTruth"):
            print " CONFIG : WARNING: 'isMC' is False and 'useTruth' is True!"
            print " CONFIG : WARNING: Setting 'useTruth' to False "
            self.configuration["useTruth"] = False

        return is_MC


    def filenames(self):
        """List of filenames to process"""
        return self.get("inputfile")
    def treenames(self):
        """Treenames to process"""
        return self.get("treenames")

    def outputFileName(self):
        """Naming for the output file"""
        return self.get("outputFileName")
    def output_path(self):
        """Destination to save output file"""
        return self.get("makeNewFile")

    def getDNN(self):
        """Calculate the DNN score"""
        return self.get("getDNN")

    def jet_btag(self):
        """B-tag working point"""
        return self.get("jet_btag")

    def input_selection(self):
        """Selection used to process the input"""
        return self.get("input_selection")

    def makeNewFile(self):
        """Save TTree information to output file"""
        return self.get("makeNewFile")

    def makeHistograms(self):
        """Save Histograms to output file"""
        return self.get("makeHistograms")

    def makeEffciency(self):
        """Save TEfficiencies to output file"""
        return self.get("makeEfficiencies")

    def selection(self):
        """Selection to apply to input files"""
        return self.get("selection")

    def cutsfile(self):
        """File that lists selection to apply"""
        return self.get("cutsfile")

    def useJets(self):
        """Use jets in analysis"""
        return self.get("useJets")

    def useLargeRJets(self):
        """Use large-R jets in analysis"""
        return self.get("useLargeRJets")

    def useLeptons(self):
        """Use leptons in analysis"""
        return self.get("useLeptons")

    def useNeutrinos(self):
        """Use neutrinos in analysis"""
        return self.get("useNeutrinos")

    def buildNeutrinos(self):
        """Reconstruct neutrinos in analysis"""
        return self.get("buildNeutrinos")

    def kinematicReco(self):
        """Kinematic reconstruction in analysis"""
        return self.get("kinematicReco")

    def metadataFile(self):
        """Metadata for samples in analysis"""
        return self.get("metadataFile")

    def LUMI(self):
        """LUMI in analysis"""
        return self.get("LUMI")

    def useTruth(self):
        """Use truth (generator) information in analysis"""
        return self.get("useTruth")

    def useSystWeights(self):
        """Use systematic weights in histograms"""
        return self.get("useSystWeights")

    def weightSystematicsFile(self):
        """Name of file that contains names of systematics stored as weights"""
        return self.get("weightSystematicsFile")

    def weightVectorSystematicsFile(self):
        """Name of file that contains names of systematics stored as vectors of weights"""
        return self.get("weightVectorSystematicsFile")


    ## Functions for just this class
    def __str__(self):
        """Specialized print statement for this class"""
        command = """ CyMiniAna : Configuration

cmsRun %(prog)s <configuration>

Use CyMiniAna in CMSSW to process EDM Ntuples.
""" % {'prog': self.source}

        keys = self.configuration.keys()
        keys.sort()
        max_len = max( len(i) for i in keys )+2


        for i in keys:
            neededlength = max_len-len(i)
            whitespace   = ' '*neededlength

            try:
                command+="   ** {0}{1}= {2:.4f}\n".format(i,whitespace,self.__dict__[i])
            except ValueError:
                continue

        return command


## THE END ##

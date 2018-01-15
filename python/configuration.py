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

        self.set_defaults()


    def initialize(self):
        """Initialize the configuration"""
        self.setConfigurations()     # set the configuration options

        return


    def setConfigurations(self):
        """Read the configuration file and set arguments (overwrite default values)"""
        file = open(self.filename,'r').readlines()
        for line in file:
            param,value = line.split(' ')
            value       = value.rstrip('\n')

            self.configuration[param] = value

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
            'nEventsToProcess':-1,
            'outputFileName':'output.root',
            'dataFilePath':os.environ["$CMSSW_BASE]+"/src/Analysis/CyMiniAna/data",
            'jerShift':None,
            'jecShift':None,
            'filenames':[],
        }

        # copy defaults into the configuration
        self.configuration = dict( (k,self.defaults[k]) for k in self.defaults.keys() )

        return


    ## Configuration attributes
    def verbose_level(self):
        """Verbosity of output"""
        return self.get("verbose_level")
    def nEventsToProcess(self):
        """Maximum number of events"""
        return self.get("nEventsToProcess")
    def dataFilePath(self):
        """Path to 'data/' directory"""
        return self.get("dataFilePath")

    ## SAMPLE INFORMATION
    def isMC(self):
        """Use if running on MC"""
        return util.str2bool( self.get("isMC") )
    def filenames(self):
        """List of filenames to process"""
        return self.get("filenames") )
    def outputFileName(self):
        """Naming for the output file"""
        return self.get("outputFileName")

    ## LEPTONS
    ## MET
    ## JETS
    def jerShift(self):
        """Shift from JER"""
        return self.get("jerShift")
    def jecShift(self):
        """Shift from JEC"""
        return self.get("jecShift")


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

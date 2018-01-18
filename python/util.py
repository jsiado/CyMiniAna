"""
Created:         1 August  2017
Last Updated:   14 January 2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University
-----

File that holds any and all misc. functions 
to be called from other python scripts.
(All information in one file => one location to update!)
"""
import ROOT


def deltaR(tlvA,tlvB,dR=0.75):
    """Determine if two objects are matched in eta-phi space"""
    result = False
    try:
        result = (tlvA.p4.DeltaR(tlvB.p4)<dR)
    except:
        return False # something that isn't a LorentzVector

    return result


def str2bool(param):
    """Convert a string to a boolean"""
    if param in ['true','True','1']:
        return True
    else:
        return False



def file2list(filename):
    """Load text file and dump contents into a list"""
    listOfFiles = open( filename,'r').readlines()
    listOfFiles = [i.rstrip('\n') for i in listOfFiles]

    return listOfFiles



class VERBOSE(object):
    """Object for handling output"""
    def __init__(self):
        self.verboseMap = {"DEBUG":0,
                           "INFO": 1,
                           "WARNING":2,
                           "ERROR":  3};
        self.level = "WARNING"

    def DEBUG(self,message):
        """Debug level - most verbose"""
        self.verbose("DEBUG",message)
        return

    def INFO(self,message):
        """Info level - standard output"""
        self.verbose("INFO",message)
        return

    def WARNING(self,message):
        """Warning level - if something seems wrong but code can continue"""
        self.verbose("WARNING",message)
        return

    def ERROR(self,message):
        """Error level - something is wrong"""
        self.verbose("ERROR",message)
        return

    def verbose(self,level,message):
        if self.verboseMap[level] >= self.verboseMap[self.level]:
            print " {0} :: {1}".format(level,message)
        return


## THEN ##

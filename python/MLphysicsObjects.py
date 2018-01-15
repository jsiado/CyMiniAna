"""
Created:        14 January 2018
Last Updated:   14 January 2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University
-----

Collection of physics objects used in 
deep learning training/testing.
"""

class LorentzVector(object):
    """Simple class that extends TLorentzVectors"""
    def __init__(self):
        self.p4 = ROOT.TLorentzVector()
        self.charge = 0
        self.index = -1


class Jet(LorentzVector):
    """Jet objects"""
    def __init__(self):
        LorentzVector.__init__(self)
        self.CSVv2   = -1
        self.deepCSV = -1
        self.cMVAv2  = -1

    def __str__(self):
        """Print properties for debugging"""
        command  = " > Jet "
        command += "\n   - Target = {0}".format(self.target)
        command += "\n   - Index  = {0}".format(self.index)
        command += "\n   - Charge = {0}".format(self.charge)

        return command


class Ljet(Jet):
    """
       Large-R jet top quark container 
       for DL training/testing.
       Use this in the Data I/O step (ROOT->Pandas)
    """
    def __init__(self):
        Jet.__init__(self)

        self.target  = -1               # target value for NN training
        # substructure
        self.tau1    = -999             # n-subjettiness
        self.tau2    = -999
        self.tau3    = -999
        self.tau21   = -999
        self.tau32   = -999
        # subjets
        self.subjets = [Jet,Jet]       # list of soft drop subjets
        # event-level
        self.reco_mttbar  = 0          # Reconstructed m_ttbar
        self.truth_mttbar = 0          # Generator-level m_ttbar

    def __str__(self):
        """Print properties for debugging"""
        command  = " > Top Quark Jet "
        command += "\n   - Target            = {0}".format(self.target)
        command += "\n   - Index             = {0}".format(self.index)
        command += "\n   - Charge            = {0}".format(self.charge)
        command += "\n   - CSVv2             = {0}".format(self.CSVv2)
        command += "\n   - Substructure "
        command += "\n     - Tau1            = {0}".format(self.tau1)
        command += "\n     - Tau2            = {0}".format(self.tau2)
        command += "\n     - Tau3            = {0}".format(self.tau3)
        command += "\n     - Tau21           = {0}".format(self.tau21)
        command += "\n     - Tau32           = {0}".format(self.tau32)

        return command



## THE END ##

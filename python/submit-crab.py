"""
Script to submit CRAB jobs
 https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookCRAB3Tutorial#1_CRAB_configuration_file_to_run
"""
import os


def main():

    from CRABClient.UserUtilities import config
    config = config()

    from CRABAPI.RawCommand import crabCommand
    from httplib import HTTPException

    git_tag        = os.environ['GIT_CMA_TAG']
    input_datasets = open( "config/crab-datasets.txt","r" ).readlines()


    def submit(config):
        try:
            crabCommand('submit', config = config)
        except HTTPException, hte:
            print 'Cannot execute commend'
            print hte.headers


    for id in input_datasets:
        id = id.rstrip('\n')

        id_split = id.split("/")
        if (not id or id.startswith('#')): continue

        print '  --> added '+id

        requestName = id[1:].replace('/','-').rstrip('-USER')[:75]

        config.General.workArea    = 'crab-cyminiana-'+git_tag
        config.General.transferOutputs = True
        config.General.transferLogs    = False

        config.JobType.pluginName = 'Analysis'
        config.JobType.psetName   = 'python/cma_cfg.py'

        # just put the directory as 'inputFiles' value
        # https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3FAQ#How_are_the_inputFiles_handled_i
        config.JobType.inputFiles = 'data'
                                    #['data/CSVv2_Moriond17_B_H.csv',
                                    # 'data/subjet_CSVv2_Moriond17_B_H.csv',
                                    # 'data/cuts_pre.txt',
                                    # 'data/weightSystematics.txt',
                                    # 'data/weightVectorSystematics.txt']

        config.Data.inputDBS      = 'phys03'
        config.Data.splitting     = 'FileBased'
        config.Data.unitsPerJob   = 10
        config.Data.outLFNDirBase = '/store/user/dmarley/B2G/wprime-qQ/samples-v0.1/'
        config.Data.publication   = False #True

        config.Site.storageSite    = "T3_US_FNALLPC"
        config.General.requestName = requestName
        config.Data.inputDataset   = id

        print 'Configuration :'
        print config
        try :
            from multiprocessing import Process
            p = Process(target=submit, args=(config,))
            p.start()
            p.join()
            submit(config)
        except :
            print 'Not submitted.'


if __name__=='__main__':
    main()

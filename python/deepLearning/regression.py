
"""
Created:        11 November  2016
Last Updated:   15 February  2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University
-----

Derived class for performing deep learning via regression
"""
import json
import util
import datetime

import ROOT
import uproot
import numpy as np
import pandas as pd

import keras
from keras.models import Sequential,model_from_json,load_model
from keras.layers import Dense, Activation
from keras.callbacks import EarlyStopping
from keras.utils.np_utils import to_categorical
from sklearn.model_selection import train_test_split,KFold,cross_val_score
from sklearn.metrics import roc_curve, auc

from deepLearning import DeepLearning
from deepLearningPlotter import DeepLearningPlotter


# fix random seed for reproducibility
seed = 2018
np.random.seed(seed)


class Regression(DeepLearning):
    """Deep Learning base class"""
    def __init__(self):
        DeepLearning.__init__(self)
        self.dnn_method = 'regression'          # DNN method applied: classification/regression: ['binary','multi','regression']


    ## Single functions to run all of the necessary pieces
    def runTraining(self,extra_branches=[]):
        """Train NN model"""
        self.load_hep_data(extra_branches)
        self.build_model()

        self.plotter.initialize(self.df,'nu_pz')

        if self.runDiagnostics:
            self.diagnostics(preTraining=True)     # save plots of the features and model architecture

        self.train()

        self.msg_svc.INFO(" SAVE MODEL")
        self.save_model(self.useLWTNN)

        if self.runDiagnostics:
            self.diagnostics(postTraining=True)    # save plots of the performance in training/testing

        return



    def train(self):
        """Setup for training the model using k-fold cross-validation"""
        self.msg_svc.INFO("DL : Train the model!")

        callbacks_list = []
        if self.earlystopping:
            earlystop = EarlyStopping(**self.earlystopping)
            callbacks_list = [earlystop]

        X = self.df[self.features].values  # self.df[self.features].values
        Y = self.df['target'].values       # self.df['target'].values

        kfold   = KFold(n_splits=self.kfold_splits, shuffle=True, random_state=seed)
        nsplits = kfold.get_n_splits(X,Y)
        cvpredictions = []                 # compare outputs from each cross-validation

        self.msg_svc.INFO("DL :   Fitting K-Fold cross validation".format(self.kfold_splits))
        for ind,(train,test) in enumerate(kfold.split(X)):
            self.msg_svc.DEBUG("DL :   - Fitting K-Fold {0}".format(ind))

            # Fit the model to training data & save the history
            Y_train = Y[train]
            Y_test  = Y[test]
            if self.dnn_method=='multi' and not np.array_equal(Y_train,(Y_train[0],self.output_dim)):
                train_shape = Y_train.shape[0]
                train_total_array = []
                test_shape = Y_test.shape[0]
                test_total_array = []
                for a in range(self.output_dim):
                    dummy_train = np.zeros(train_shape)
                    dummy_train[Y[train][0]==a] = 1
                    train_total_array.append( dummy_train.tolist() )

                    dummy_test = np.zeros(test_shape)
                    dummy_test[Y[test][0]==a] = 1
                    test_total_array.append( dummy_test.tolist() )
                Y_train = np.array(train_total_array).T
                Y_test  = np.array(test_total_array).T

            history = self.model.fit(X[train],Y_train,epochs=self.epochs,\
                                     callbacks=callbacks_list,batch_size=self.batch_size,verbose=self.verbose)
            self.histories.append(history)

            # evaluate the model
            self.msg_svc.DEBUG("DL :     + Evaluate the model: ")
            predictions = self.model.evaluate(X[test], Y_test,verbose=self.verbose,batch_size=self.batch_size)
            cvpredictions.append(predictions[1] * 100)
            self.msg_svc.DEBUG("DL :       {0}: {1:.2f}%".format(self.model.metrics_names[1], predictions[1]*100))

            # Evaluate training sample
            #self.train_predictions.append( train_predictions ) # memory error
            train_predictions = self.predict(X[train])
            h_train_predictions = ROOT.TH1D("train_predictions","train_predictions",200,-10,10)
            h_train_predictions.SetDirectory(0)
            h_train_target = ROOT.TH1D("train_target","train_target",200,-10,10)
            h_train_target.SetDirectory(0)
            h_train_resolution = ROOT.TH1D("train_resolution","train_resolution",200,-10,10)
            h_train_resolution.SetDirectory(0)
            for tp,tt in zip(train_predictions,Y[train]):
                h_train_predictions.Fill(tp)
                h_train_target.Fill(tt)
                h_train_resolution.Fill(tp-tt)

            # Evaluate test sample
            #self.test_predictions.append( test_predictions )   # memory error
            test_predictions = self.predict(X[test])
            h_test_predictions = ROOT.TH1D("test_predictions","test_predictions",200,-10,10)
            h_test_predictions.SetDirectory(0)
            h_test_target = ROOT.TH1D("test_target","test_target",200,-10,10)
            h_test_target.SetDirectory(0)
            h_test_resolution = ROOT.TH1D("test_resolution","test_resolution",200,-10,10)
            h_test_resolution.SetDirectory(0)
            for tp,tt in zip(test_predictions,Y[test]):
                h_test_predictions.Fill(tp)
                h_test_target.Fill(tt)
                h_test_resolution.Fill(tp-tt)

            # Compare training and testing in plots
            self.msg_svc.INFO("DL : -- post-training :: PREDICTIONS {0}".format(ind))
            train = {'prediction':h_train_predictions,'target':h_train_target,'resolution':h_train_resolution}
            test  = {'prediction':h_test_predictions, 'target':h_test_target, 'resolution':h_test_resolution}
            self.plotter.prediction(train,test,ind)   # compare DNN prediction for different targets

        self.msg_svc.INFO("DL :   Finished K-Fold cross-validation: ")
        self.accuracy = {'mean':np.mean(cvpredictions),'std':np.std(cvpredictions)}
        self.msg_svc.INFO("DL :   - Accuracy: {0:.2f}% (+/- {1:.2f}%)".format(np.mean(cvpredictions), np.std(cvpredictions)))

        return



    def diagnostics(self,preTraining=False,postTraining=False):
        """Diagnostic tests of the NN"""

        self.msg_svc.INFO("DL : Diagnostics")

        # Diagnostics before the training
        if preTraining:
            self.msg_svc.INFO("DL : -- pre-training")
            self.plotter.features()                        # compare features for different targets
            self.plotter.correlations()                    # correlations of features
            #self.plotter.separations()                    # correlations of features
            self.plotter.model(self.model,self.model_name) # Keras plot of the model architecture

        # post training/testing
        if postTraining:
            self.msg_svc.INFO("DL : -- post-training")

            self.msg_svc.INFO("DL : -- post-training :: History")
            self.plotter.loss(self.histories)     # loss as a function of epoch
            self.plotter.metrics(self.histories)  # metrics as a function of epoch (e.g., 'mean-squared-error')

        return


## THE END ##

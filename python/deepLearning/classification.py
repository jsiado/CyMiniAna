
"""
Created:        11 November  2016
Last Updated:   15 February  2018

Dan Marley
daniel.edison.marley@cernSPAMNOT.ch
Texas A&M University
-----

Inherits from Base class for performing classification
"""
import json
import util
import datetime

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


class Classification(DeepLearning):
    """Deep Learning via classification"""
    def __init__(self):
        DeepLearning.__init__(self)
        self.dnn_method = 'classification'


    ## Single functions to run all of the necessary pieces
    def runTraining(self,extra_branches=[]):
        """Train NN model"""
        self.load_hep_data(extra_branches)
        self.build_model()

        self.plotter.initialize(self.df)

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

            # store test/train data from each k-fold to compare later
            self.test_data['X'].append(X[test])
            self.test_data['Y'].append(Y[test])
            self.train_data['X'].append(X[train])
            self.train_data['Y'].append(Y[train])

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
            train_predictions = self.predict(X[train])
            self.train_predictions.append( train_predictions )

            # Evaluate test sample
            test_predictions  = self.predict(X[test])
            self.test_predictions.append( test_predictions )

            # Make ROC curve from test sample
            fpr,tpr,_ = roc_curve( Y[test], test_predictions )
            self.fpr.append(fpr)
            self.tpr.append(tpr)

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
            self.plotter.features()                        # compare features
            self.plotter.correlations()                    # correlations of features
            #self.plotter.separations()                    # separation of features between classes
            self.plotter.model(self.model,self.model_name) # Keras plot of the model architecture

        # post training/testing
        if postTraining:
            self.msg_svc.INFO("DL : -- post-training")

            self.msg_svc.INFO("DL : -- post-training :: PREDICTIONS ")
            train = {'prediction':self.train_predictions,'target':self.train_data['Y']}
            test  = {'prediction':self.test_predictions, 'target':self.test_data['Y']}
            self.plotter.prediction(train,test)                   # compare DNN prediction for different targets

            self.msg_svc.INFO("DL : -- post-training :: ROC")
            self.plotter.ROC(self.fpr,self.tpr,self.accuracy)     # ROC curve for signal vs background

            self.msg_svc.INFO("DL : -- post-training :: History")
            self.plotter.loss(self.histories)                     # loss as a function of epoch

        return


## THE END ##


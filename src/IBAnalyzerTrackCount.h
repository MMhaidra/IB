/*//////////////////////////////////////////////////////////////////////////////
// CMT Cosmic Muon Tomography project //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  Copyright (c) 2014, Universita' degli Studi di Padova, INFN sez. di Padova

  Coordinators: Prof. Gianni Zumerle < gianni.zumerle@pd.infn.it >
                Paolo Checchia       < paolo.checchia@pd.infn.it >

  Authors: Andrea Rigoni Garola < andrea.rigoni@pd.infn.it >
           Matteo Furlan        < nuright@gmail.com >
           Sara Vanini          < sara.vanini@pd.infn.it >

  All rights reserved
  ------------------------------------------------------------------

  This file can not be copied and/or distributed without the express
  permission of  Prof. Gianni Zumerle  < gianni.zumerle@pd.infn.it >

//////////////////////////////////////////////////////////////////////////////*/



#ifndef IBANALYZERTRACKCOUNT_H
#define IBANALYZERTRACKCOUNT_H


#include "IBAnalyzer.h"
#include "IBVoxRaytracer.h"
#include "IBPocaEvaluator.h"

using namespace uLib;


class IBAnalyzerTrackCount : public IBAnalyzer {    
    uLibTypeMacro(IBAnalyzerTrackCount,IBAnalyzer)
public:

    IBAnalyzerTrackCount();
    ~IBAnalyzerTrackCount();

    bool AddMuon(const MuonScatterData &muon);

    void Run(unsigned int iterations = 1, float muons_ratio = 1);

    void SetRayAlgorithm(IBVoxRaytracer *raytracer);

    void SetPocaAlgorithm(IBPocaEvaluator *evaluator);

    void SetDetectorZSelection(int selectZ);

    void Clear();

    unsigned int Size() const;

    void SetMuonCollection(IBMuonCollection *muons);


private:
    class IBAnalyzerTrackCountPimpl *d;
};



#endif // IBANALYZERTRACKCOUNT_H

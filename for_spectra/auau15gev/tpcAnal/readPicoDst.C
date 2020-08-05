// This is needed for calling standalone classes (not needed on RACF)
#define _VANILLA_ROOT_

// C++ headers
#include <iostream>

// ROOT headers
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TVector3.h"



// PicoDst headers
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoDstReader.h"
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoDst.h"
#include "/home/manukhov/DUBNA/STAR/StPicoDstMaker/StPicoDstMaker.h"
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoEvent.h"
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoTrack.h"
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoBTofHit.h"
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoBTowHit.h"
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoEmcTrigger.h"
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoBTofPidTraits.h"
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoTrackCovMatrix.h"

// Load libraries (for ROOT_VERSTION_CODE >= 393215)
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0) 
R__LOAD_LIBRARY(/home/manukhov/DUBNA/STAR/StPicoEvent/libStPicoDst.so)
#endif

void readPicoDst(TString InputFileList, TString OutputDir) 
{

  //
  Int_t nFiles = 1e+4;
    
  // Load libraries
  gROOT ->Macro("loadPicoDst.C");
  gSystem->Load("MyAnalysisMaker") ;
  gSystem->Load("StRefMultCorr");
  //gSystem->Load("StBTofUtil");          //Added                                                                                               
  //gSystem->Load("StVpdCalibMaker");     //Adeed                                                                                               
  //gSystem->Load("StBTofCalibMaker");    //Added   
  
  
  // List of member links in the chain
  StChain*                    chain  =  new StChain ;
  StPicoDstMaker*          picoDstMaker  =  new StPicoDstMaker(0,0,"",InputFileList,"PicoDst",nFiles) ;
  MyAnalysisMaker*    AnalysisCode   =  new MyAnalysisMaker(picoDstMaker) ;

  // Turn off everything but Primary tracks in order to speed up the analysis and eliminate IO
  picoDstMaker -> SetStatus("*",0) ;                // Turn off all branches
  picoDstMaker -> SetStatus("PicoEvent*",1) ;          // Turn on the Event data (esp. Event number)
  picoDstMaker -> SetStatus("PrimaryTracks*",1) ;    // Turn on the primary track data
  picoDstMaker -> SetStatus("GlobalTracks*",1) ;    // Turn on the Global track data
  picoDstMaker -> SetStatus("PrimaryVertices*",1) ;    // Turn on the primary track data
  picoDstMaker -> SetStatus("BTofHeader*",1) ;    // Turn on the BtofHeader 
  
//AnalysisCode -> SetChargeFlag(chargeFlag);
  AnalysisCode -> SetOutputFileName(OutputDir) ;  

  Int_t nEvents = 1e5;

  nEvents = picoDstMaker->chain()->GetEntries();
  //nEvents = 10000;

  cout<<"\n############################ Total Event in chain = "<<nEvents<<" ############################\n "<<endl;

  // Loop over the links in the chain
  chain -> Init() ;
  int iret;
  for(int i=0;i<nEvents;i++)
   {
   iret = chain->Make(i);

     if(i%1000==0)
        cout<<" => Processing event# = "<<i<<" with return code = "<<iret<<endl;

   if(iret){
     cout<<"....File Ends, Analysis DONE...."<<endl; break;
   }

  }

  chain -> Finish() ;
  

  // Cleanup
  delete chain ;

}


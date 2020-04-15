#ifndef MyAnalysisMaker_def
#define MyAnalysisMaker_def

#include "StPicoDSTMaker/COMMON/StPicoTrack.h"
#include "StPicoDSTMaker/COMMON/StPicoEvent.h"
#include "StPicoDSTMaker/COMMON/StPicoDst.h"
//#include "StEvent/StBTofCollection.h"
#include "StBTofHeader.h"
#include "StMaker.h"
#include "TString.h"
#include "TH1.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TFile.h"

class StPicoDstMaker  ;
class StRefMultCorr ;
class TFile         ;
class TH1F          ;
class TH2F          ;
class TH3F          ;
class TH1D;
class TH2D;
class TString       ;
class TProfile      ;
class TProfile2D    ;
class TLorentzVector;


#define MaxNumberOfCentrality		9
#define MaxNumberOfnharmo		2
#define MaxNumberOfCorrection		15	



class MyAnalysisMaker : public StMaker
{
 public:
  MyAnalysisMaker(StPicoDstMaker* maker) ;           //  Constructor

  virtual          ~MyAnalysisMaker( ) ;           //  Destructor
  
  Int_t Init    ( ) ;                              //  Initiliaze the analysis tools ... done once
  Int_t Make    ( ) ;                              //  The main analysis that is done on each event
  Int_t Finish  ( ) ;                              //  Finish the analysis, close files, and clean up.

  Bool_t IsBadEvent(StPicoEvent *picoEvent);
  Bool_t IsBadTrack(StPicoTrack * track);
  Int_t EventCentrality(unsigned short picoMult);
  void SetOutputFileName(TString name) 
  {OutputFileName = name;}          // Make name available to member functions
  
  
  
 private:
  StPicoDstMaker* mPicoDstMaker ;                      //  Make PicoDst pointer available to member functions
  //StPicoDst * mPicoDst;
  StRefMultCorr* refmultCorrUtil ;
  TFile*        histogram_output ;                 //  Histograms outputfile pointer
  UInt_t        mEventsProcessed ;                 //  Number of Events read and processed
  TString  OutputFileName ;                   //  Name of the histogram output file 
  int      ChargeFlag;
  Double_t VertexZPos;
  Double_t reweight;
  Double_t zdc_cons;
  Double_t bbc_cons;
  Double_t Qxsum;
  Double_t Qysum;
  Float_t  VpdVzPos;
  Float_t  Pi;
  Float_t  twoPi;
  
  Int_t    trigger;
  Int_t    runnumber;
  Int_t    runnumber_flag;
  Int_t    runnumberPointer;
  Int_t    CurrentEvent_centrality; 
  Int_t    cent_flag;
  Int_t    cent; 
  
  //---------------nTuple pointers------------
  //TTree                       *t1;
  
  Int_t                Centrality;
  Int_t                TPCRefmult;
  Int_t                ToFRefmult;
  Int_t                    nTrack;
  Int_t                      ntof;
  Double_t                VertexZ;
  Double_t                VertexX;
  Double_t                VertexY;
  Float_t                  mvpdVz;
  Double_t                   dEdx;
  
  Int_t             gtracks;
  Float_t              kppt;
  Float_t              kmpt;
  Float_t              pppt;
  Float_t              pmpt;
  Double_t             mphi;
  Float_t              meta;
  Float_t         eta[5000];
  Double_t        phi[5000];
  double         dEdx_trunc;
  Float_t          pt[5000];
  Float_t        kapt[5000];
  Float_t        prpt[5000];
  Float_t      tof_pt[5000];
  Float_t    tof_kapt[5000];
  Float_t    tof_prpt[5000];
  Float_t         mom[5000];
  float                  pp;
  Float_t            primPx;
  Float_t            primPy;
  Float_t            primPz;
  Float_t             mHits;
  Double_t              dca;
  UShort_t            nHits;
  UShort_t         nHitsFit;
  Float_t          mHitsFit;
  Float_t         mHitsPoss;
  Float_t         mHitsdEdx;
  Float_t         mdEdx_pts;
  Float_t              tNse;
  Float_t             tNspi;
  Float_t              tNsk;
  Float_t             tNspr;
  Float_t   tNspi_tof[5000];
  Float_t    tNsk_tof[5000];
  Float_t   tNspr_tof[5000];
  short                flag;
  short        charge[5000];
  Double_t            mdEdx;
  Float_t         rpi[5000];
  Float_t         rka[5000];
  Float_t         rpr[5000];
  
  Float_t     tof_rpi[5000];
  Float_t     tof_rka[5000];
  Float_t     tof_rpr[5000];
  
  Float_t        Beta[5000];
  float                beta;
  Float_t  Pathlength[5000];
  Float_t         Tof[5000];
  short     matchflag[5000];
  
  Double_t  dEdx_expected1;
  Double_t dEdx_expected11;
  Double_t  dEdx_expected2;
  Double_t dEdx_expected22;
  Double_t  dEdx_expected3;
  Double_t dedx_th1[12000];
  Double_t dedx_th2[12000];
  Double_t dedx_th3[12000];
  Double_t Zpi;
  Double_t Zpi1;
  Double_t Zk;
  Double_t Zk1;
  Double_t Zpr;
  
  //-----------------histograms pointer------------
  TH1F *EventCount;
  TH1F *CentCount;
  TH1D *hrunid;
  
  TH1D *hVz;
  TH1D *href;
  TH1D *hrefm[9];
  TH2D *hVxVy;
  TH1D *hvpdvz;

  TH1D *hpt;
  TH2D *hdEdxp;
  TH2D *hetaphi;
  TH2D *hmass2p;
  TH1D *heta;
  TH1D *hphi;
  TH2D *hptphi;
  TH1D *hdca;
  TH2D *hptdca;
  TH2D *hptdca_prp[9];
  TString hptdca_prp_names[9];
  TH2D *hptdca_prm[9];
  TString hptdca_prm_names[9];  
  TH1D *hnhits;
  TH2D *hpthits;
  TH1D *hndedx;
  TH2D *hptndedx;
  TH2D *hbeta;
  TH2D *hZpipos[9];
  TH2D *hZpineg[9];
  //-------------------------
  TH2D *hZpipos1[9];
  TH2D *hZpineg1[9];
  //---------------------
  TH2D *hZkpos[9];
  TH2D *hZkneg[9];
  //--------------------------

  TH2D *hZkpos1[9];
  TH2D *hZkneg1[9];
  //--------------------------
  TH2D *hZprpos[9];
  TH2D *hZprneg[9];
  TH2D *hrap_pip;
  TH2D *hrap_pim;
  TH2D *hrap_piptof;
  TH2D *hrap_pimtof;
  TH2D *hrap_kap;
  TH2D *hrap_kam;
  TH2D *hrap_kaptof;
  TH2D *hrap_kamtof;
  TH2D *hrap_prp;
  TH2D *hrap_prm;
  TH2D *hrap_prptof;
  TH2D *hrap_prmtof;

  // method (a simple track filter)                                                                                                            
  //bool accept(StPicoTrack*);            // and this is used to select tracks  

 protected:


  ClassDef(MyAnalysisMaker,1)                       //Macro for CINT compatability
    
};

#endif
















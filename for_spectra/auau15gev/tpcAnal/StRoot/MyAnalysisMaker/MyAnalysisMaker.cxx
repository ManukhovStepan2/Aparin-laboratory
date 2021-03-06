#include "MyAnalysisMaker.h"
#include <iostream>
#include "StPicoDSTMaker/COMMON/StPicoTrack.h"
#include "StPicoDSTMaker/COMMON/StPicoEvent.h"
#include "StPicoDSTMaker/COMMON/StPicoDst.h"
#include "StBichsel/Bichsel.h"
#include "StBTofHeader.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TFile.h"
#include "TMath.h"
#include "TRandom.h"
#include "TRandom3.h"
#include "TProfile.h"
#include "TProfile2D.h"
//#include "StThreeVecor.h"
#include "TLorentzVector.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TObjArray.h"
#include "TComplex.h"
#include "StRoot/StRefMultCorr/StRefMultCorr.h"
#include "StRefMultCorr/CentralityMaker.h"


 using namespace std;


ClassImp(MyAnalysisMaker)                       // Macro for CINT compatibility
MyAnalysisMaker::MyAnalysisMaker(StPicoDstMaker* maker ) 
: StMaker("MyAnalysisMaker")
{                                               // Initialize data members here.
  mPicoDstMaker      = maker ;                    // Pass PicoDst pointer to DstAnlysisMaker Class member functions
  histogram_output = NULL  ;                    // Zero the Pointer to histogram output file
  mEventsProcessed = 0     ;                    // Zero the Number of Events processed by the maker 
  OutputFileName = "" ;                         // Output File Name( will be set inside the "readPicoDst".C )
}


MyAnalysisMaker::~MyAnalysisMaker() 
{/* */}


Int_t MyAnalysisMaker::Init()
{  
  //----------------------------------------
  runnumber        = -11000001;  //just a no.
  runnumberPointer =         0;
  cent_flag        =        10;
  Pi               =   3.14159;
  twoPi            =   6.28318;
  
  histogram_output = new TFile(OutputFileName,"RECREATE") ;      // Name is set in readPicoDst.C
  //refmultCorrUtil  = new StRefMultCorr();
  refmultCorrUtil  = CentralityMaker::instance()->getRefMultCorr();
  //--------------------------------------------------------------------------
  //change here and in header file:
  
  //----- Define masses ------//
  
  t1 = new TTree("t1","UU ntuple");
  t1->Branch("Centrality",&Centrality, "Centrality/I");
  t1->Branch("ToFRefmult",&ToFRefmult, "ToFRefmult/I");
  //t1->Branch("Vz",        &Vz,                 "Vz/D");
  t1->Branch("nTrack",    &nTrack,         "nTrack/I");
  t1->Branch("w",         &w,                   "w/D");
  t1->Branch("dca",       &dca,               "dca/F");
  t1->Branch("nHitsFit",  &nHitsFit,              "nHitsFit/F");
  t1->Branch("nHitsdEdx", &nHitsdEdx,            "nHitsdEdx/F");
  t1->Branch("tNspi_tof", &tNspi_tof,    "tNspi_tof[nTrack]/F");
  t1->Branch("tNsk_tof",  &tNsk_tof,      "tNsk_tof[nTrack]/F");
  t1->Branch("tNspr_tof", &tNspr_tof,    "tNspr_tof[nTrack]/F");
  t1->Branch("mom",       &mom,                "mom[nTrack]/F");
  t1->Branch("tof_pt",    &tof_pt,          "tof_pt[nTrack]/F");
  t1->Branch("tof_kapt",  &tof_kapt,      "tof_kapt[nTrack]/F");
  t1->Branch("tof_prpt",  &tof_prpt,      "tof_prpt[nTrack]/F");
  t1->Branch("tof_rpi",   &tof_rpi,        "tof_rpi[nTrack]/F");
  t1->Branch("tof_rka",   &tof_rka,        "tof_rka[nTrack]/F");
  t1->Branch("tof_rpr",   &tof_rpr,        "tof_rpr[nTrack]/F");
  t1->Branch("charge",    &charge,          "charge[nTrack]/s");
  t1->Branch("Beta",      &Beta,              "Beta[nTrack]/F");
  t1->Branch("Pathlength",&Pathlength,  "Pathlength[nTrack]/F");
  t1->Branch("matchflag", &matchflag,    "matchflag[nTrack]/s");
  t1->Branch("Tof",       &Tof,                "Tof[nTrack]/F");


  //histograms: Add/remove as u need. See the header file. Declare there.
  EventCount = new TH1F("EventCount","EventCount",10,0,10);  //1
  CentCount  = new TH1F("CentCount",  "CentCount",10,0,10);  //2
  hrunid     = new TH1D("hrunid","AuAu Run Id",8000,15050000,15080000);
  
  hVz_raw    = new TH1D("VertexZ_pre", "VertexZ ",400,-200.,200.); //3
  href_raw   = new TH1D("href_pre", "RefMult ",400,0,400);  //4
  hVr_raw    = new TH1D("Vr_pre", "Vr ",400, 0 ,4 ); //3
  hVxVy_raw  = new TH2D("hVxVy_pre","VertexXY",150,-3.,3.,150,-3.,3.);//6
  
  hVz        = new TH1D("VertexZ", "VertexZ ",100,-50.,50.); //3
  href       = new TH1F("href", "RefMult ",500,0,500);  //4
  hcorr_ref  = new TH1F("hcorr_ref", "Corrected RefMult ",1000,0,500);  //4
  hVr        = new TH1D("Vr", "Vr ",110, 0, 1.1); //3
  hVxVy      = new TH2D("hVxVy","VertexXY",150,-3.,3.,150,-3.,3.);//6
  for(Int_t i = 0, i < 9, i++)
  {
  	hrefm_names[i] 	= "Refm_cent_" + to_string(i+1);
  	hrefm[i] 		= new TH1D(hrefm_names[i],"Refmult",500, 0, 500);
  };

  //track histograms:
  TrackCount = new TH1F("TrackCount","TrackCount",20,0,20);  //1  
  hnhits_pre =  new TH1D("hnhits_pre","NHits distribution",52,0,52);
  hnfit_nfitposs_pre =  new TH1D("hnfit_nfitposs_pre","Nfit_nfitposs distribution", 100, 0, 1.0);
  hndedx_pre =  new TH1D("hndedx_pre","NhitsdEdx distribution",52,0,52);
  hdca_pre   =  new TH1D("hdca_pre","DCA distribution",1000, 0, 100.);
  heta_pre   =  new TH1D("heta_pre","#eta distribution",300,-1.5,1.5);
  hrapidity_pre   =  new TH1D("hrapidity_pre","Rapidity distribution",300,- 1.5,1.5);
  
  hdEdxp    =  new TH2D("hdEdxp","  dEdx-vs-p   ",1200, -4,  4, 600,  0 , 30);
  hmass2p   =  new TH2D("hmass2p","mass2-vs-p   ", 1000,  0,  5, 1000,-0.5,1.5);
  hbeta     =  new TH2D("hbeta","1/#beta vs p*q",1200, -4, 4, 1000 ,  -1,  4);
  
  hetaphi   =  new TH2D("hetaphi","eta-phi dist.", 500, -5,  5, 500,  -5,  5);
  hpt       =  new TH1D("hpt","pt dist", 200, 0, 5);
  
  for(Int_t i = 0, i < 9, i++)
  {
  	hpt_names[i] 	= "hpt_" + to_string(i+1);
  	hpt_titles[i]	= "pt dist_" + to_string(i+1);
  	hpt[i] 			= new TH1D(hpt_names[i],hpt_titles[i],200, -5, 5);
  };

  
  heta      =  new TH1D("heta","#eta distribution",300,-1.5,1.5);
  hrapidity =  new TH1D("hrapidity","Rapidity distribution",300,-1.5,1.5);
  hphi      =  new TH1D("hphi","#phi distribution",200, -5. ,5.);
  hptphi    =  new TH2D("hptphi","phi Distribution",200,0,10,100,-4,4);
  hdca      =  new TH1D("hdca","DCA distribution",140, 0, 3.5);
  hptdca    =  new TH2D("hptdca","DCA Distribution",200,0,10,100,0,10);  
  hnhits    =  new TH1D("hnhits","NHits distribution",52,0,52);
  hnfit_nfitposs =  new TH1D("hnfit_nfitposs","Nfit_nfitposs distribution", 100, 0, 1.0);
  hpthits   =  new TH2D("hpthits","nHits Distribution",200,0,10,50,0,50);
  hndedx    =  new TH1D("hndedx","NhitsdEdx distribution",52,0,52);
  hptndedx  =  new TH2D("hptndedx","nHitsdEdx Distribution",200,0,10,50,0,50);

  //====================================================================
  hzpipos  =  new TH2D("hpipos","zpi_pos distribution",240,0,12,500,-5,5);
  hzpineg  =  new TH2D("hpineg","zpi_neg distribution",240,0,12,500,-5,5);
  hzkapos  =  new TH2D("hkapos","zka_pos distribution",240,0,12,500,-5,5);
  hzkaneg  =  new TH2D("hkaneg","zka_neg distribution",240,0,12,500,-5,5);
  hzprpos  =  new TH2D("hprpos","zpr_pos distribution",240,0,12,500,-5,5);
  hzprneg  =  new TH2D("hprneg","zpr_neg distribution",240,0,12,500,-5,5);

  //===================================================================
  for(Int_t i = 0, i < 9, i++)
  {
  	hZpipos_names[i] = "hZpipos_cent_" + to_string(i+1);
  	hZpipos[i]		 = new TH2D(hZpipos_names[i],"zpi_pos distribution",240,0,12,500,-5,5);
  };

  for(Int_t i = 0, i < 9, i++)
  {
  	hZpineg_names[i] = "hZpineg_cent_" + to_string(i+1);
  	hZpineg[i]	 	 =  new TH2D(hZpineg_names[i],"zpi_neg distribution",240,0,12,500,-5,5);
  };

  for(Int_t i = 0, i < 9, i++)
  {
  	hZkpos_names[i]	 = "hZkpos_cent_" + to_string(i+1);
  	hZkpos[i]		 =  new TH2D(hZkpos_names[i],"zka_pos distribution",240,0,12,500,-5,5);
  };
 
  for(Int_t i = 0, i < 9, i++)
  {
  	hZkneg_names[i]	 = "hZkneg_cent_" + to_string(i+1);
  	hZkneg[i] 		 =  new TH2D(hZkneg_names[i],"zka_neg distribution",240,0,12,500,-5,5);
  };
  
  for(Int_t i = 0, i < 9, i++)
  {
  	hZprpos_names[i] = "hZprpos_cent_" + to_string(i+1);
  	hZprpos[i] 		 =  new TH2D(hZprpos_names[i], "zpr_pos distribution",240,0,12,500,-5,5);
  };
  
  for(Int_t i = 0, i < 9, i++)
  {
  	hZprneg_names[i] = "hZprneg_cent_" + to_string(i+1);
  	hZprneg[i] 		 =  new TH2D(hZprneg_names[i], "zpr_neg distribution",240,0,12,500,-5,5);
  };

  //=========matching efficiency===============
  hrap_pip    = new TH2D("hrap_pip","",10,0,10,100,0,10);
  hrap_pim    = new TH2D("hrap_pim","",10,0,10,100,0,10);
  hrap_piptof = new TH2D("hrap_piptof","",10,0,10,100,0,10);
  hrap_pimtof = new TH2D("hrap_pimtof","",10,0,10,100,0,10);
  
  hrap_kap    = new TH2D("hrap_kap","",10,0,10,100,0,10);
  hrap_kam    = new TH2D("hrap_kam","",10,0,10,100,0,10);
  hrap_kaptof = new TH2D("hrap_kaptof","",10,0,10,100,0,10);
  hrap_kamtof = new TH2D("hrap_kamtof","",10,0,10,100,0,10);
  
  hrap_prp    = new TH2D("hrap_prp","",10,0,10,100,0,10);
  hrap_prm    = new TH2D("hrap_prm","",10,0,10,100,0,10);
  hrap_prptof = new TH2D("hrap_prptof","",10,0,10,100,0,10);
  hrap_prmtof = new TH2D("hrap_prmtof","",10,0,10,100,0,10);

//---------------------------------------------------------------------------

  VertexZ = -100.0;
  mvpdVz  = -100.0;
  

  return kStOK ; 
}



Bool_t MyAnalysisMaker::IsBadEvent(StPicoEvent *picoEvent)        // <==== I am using this function. **Please Change your trigger**
{

  if(!picoEvent)
    return kTRUE;

  if(!((picoEvent->triggerIdCollection().nominal().isTrigger(440005)) ||   //     change it for different data.
       (picoEvent->triggerIdCollection().nominal().isTrigger(440015))
       ) )
    return kTRUE; 
  
  
  return kFALSE;
}


Bool_t MyAnalysisMaker::IsBadTrack(StPicoTrack * track)          // <==== I am not using this function. I used my cuts in trackloop in Make()
{
  unsigned short     nHits = track->nHits();
  unsigned short nHitsPoss = track->nHitsPoss();
  double            eta    = track->eta();
  double            dcaMag = track->dca().mag();
  short            charge  = track->charge();	
  if(fabs(dcaMag)>2 || nHits<20 || (1.0*nHits/nHitsPoss)<0.51 || TMath::Abs(eta)>1) return kTRUE;
  if(!( ChargeFlag==0 || (ChargeFlag ==1 && charge==1)||(ChargeFlag == -1 && charge== -1))  ) return kTRUE;
  return kFALSE;
}

Int_t MyAnalysisMaker::nTofMatchedTracks(){
  
  // ToF matched points cut                                                                                                                
  Int_t nMatchedToF = 0;
  Int_t nN_prim = 0;
  Int_t nN_non_prim = 0;
  
  StPicoDst *mPicoDst = mPicoDstMaker->picoDst();

  const Int_t nTracks = mPicoDst->numberOfPrimaryTracks();
  for(Int_t i = 0; i < nTracks; i++)
    {
      StPicoTrack *track = (StPicoTrack*)mPicoDst->primaryTracks(i);
      if(!track){
        continue;
      }

      if(track->dcaGlobal().mag() > 3) // non primary track                                                                                            
        {
          nN_non_prim++;
        }
      else
        {
          nN_prim++;
          if(track->btofPidTraits().matchFlag() > 0 && track->btofPidTraits().timeOfFlight() != 0 && track->btofPidTraits().beta() != 0)
            {
              nMatchedToF++;
            }
        }
    }

  return nMatchedToF;
}



Int_t MyAnalysisMaker::Make()
{
  StPicoDst *mPicoDst = mPicoDstMaker->picoDst();
  
  StPicoEvent* picoEvent  =  mPicoDstMaker->picoDst()->event() ;
  EventCount-> Fill(0.2);
  
  if(IsBadEvent(picoEvent))                                       // Nominal Event cuts and trigger (change trigger as ur data)
    return           kStOK;

  runnumber = picoEvent->runId();

  Int_t badrun[253] = {15046073, 15046089, 15046094, 15046096, 15046102, 15046103, 15046104, 15046105, 15046106, 15046107, 15046108, 15046109, 15046110, 15046111, 15047004, 15047015, 15047016, 15047019, 15047021, 15047023, 15047024, 15047026, 15047027, 15047028, 15047029, 15047030, 15047039, 15047040, 15047041, 15047044, 15047047, 15047050, 15047052, 15047053, 15047056, 15047057, 15047061, 15047062, 15047063, 15047064, 15047065, 15047068, 15047069, 15047070, 15047071, 15047072, 15047074, 15047075, 15047082, 15047085, 15047086, 15047087, 15047093, 15047096, 15047097, 15047098, 15047100, 15047102, 15047104, 15047106, 15048003, 15048004, 15048012, 15048013, 15048014, 15048016, 15048017, 15048018, 15048019, 15048020, 15048021, 15048023, 15048024, 15048025, 15048026, 15048028, 15048029, 15048030, 15048031, 15048033, 15048034, 15048074, 15048075, 15048076, 15048077, 15048078, 15048079, 15048080, 15048081, 15048082, 15048083, 15048084, 15048085, 15048086, 15048087, 15048088, 15048089, 15048091, 15048092, 15048093, 15048094, 15048095, 15048096, 15048097, 15048098, 15049002, 15049003, 15049009, 15049013, 15049014, 15049015, 15049016, 15049017, 15049018, 15049019, 15049020, 15049021, 15049022, 15049023, 15049025, 15049026, 15049027, 15049028, 15049030, 15049031, 15049032, 15049033, 15049037, 15049038, 15049039, 15049040, 15049041, 15049074, 15049077, 15049083, 15049084, 15049085, 15049086, 15049087, 15049088, 15049089, 15049090, 15049091, 15049092, 15049093, 15049094, 15049096, 15049097, 15049098, 15049099, 15050001, 15050002, 15050003, 15050004, 15050005, 15050006, 15050010, 15050011, 15050012, 15050013, 15050014, 15050015, 15050016, 15051131, 15051132, 15051133, 15051134, 15051137, 15051141, 15051144, 15051146, 15051147, 15051148, 15051149, 15051156, 15051157, 15051159, 15051160, 15052001, 15052004, 15052005, 15052006, 15052007, 15052008, 15052009, 15052010, 15052011, 15052014, 15052015, 15052016, 15052017, 15052018, 15052019, 15052020, 15052021, 15052022, 15052023, 15052024, 15052025, 15052026, 15052040, 15052041, 15052042, 15052043, 15052060, 15052061, 15052062, 15052063, 15052064, 15052065, 15052066, 15052067, 15052068, 15052069, 15052070, 15052073, 15052074, 15052075, 15053027, 15053028, 15053029, 15053034, 15053035, 15053052, 15053054, 15053055, 15054053, 15054054, 15055018, 15055137, 15056117, 15057055, 15057059, 15058006, 15058011, 15058021, 15059057, 15059058, 15061001, 15061009, 15062006, 15062069, 15065012, 15065014, 15066070, 15068013, 15068014, 15068016, 15068018, 15069036, 15070008, 15070009, 15070010};

  //removing bad runs  
  int badflag=0; ///bad run from Shusu code     
  for(int irun=0; irun<253; irun++)
    {
      if(runnumber == badrun[irun])
        {
          badflag = 1;//cout<<"Bad run found --->"<<runnumber<<endl;
          return kStOk;
        }
    }
  if(badflag==1) cout<<"why not skip this bad run event"<<endl;

  //if(runnumber < 15053000 && runnumber > 15070021)
  //return kStOK;
  
  //-----------------------------------------------------
  
  EventCount-> Fill(1.2);
  
  
  //-----------------All about Vertex-------------------- 
  VertexX = picoEvent->primaryVertexPosition().x();
  VertexY = picoEvent->primaryVertexPosition().y();
  VertexZ = picoEvent->primaryVertexPosition().z();
  if( (VertexX < 1.e-5 && VertexX > -1.e-5) &&       
      (VertexY < 1.e-5 && VertexY > -1.e-5) &&
      (VertexZ < 1.e-5 && VertexZ > -1.e-5) )  
    return kStOK;

  double Vr = TMath::Sqrt(VertexX*VertexX+pow(VertexY+0.89,2));

  unsigned short tofMult = picoEvent->btofTrayMultiplicity();
  unsigned short refMult =              picoEvent->refMult();
  
  hVz_raw   -> Fill(VertexZ);
  hVr_raw   -> Fill(Vr);
  hVxVy_raw -> Fill(VertexX,VertexY);
  href_raw  -> Fill(refMult);
  

  if(fabs(VertexZ)>30.0)              // <== Change this cut. for AuAu19GeV it is +/-70cm
    return kStOK;     
  
  if(sqrt(VertexX*VertexX+pow(VertexY+0.89,2))>1.0)
    return kStOK;

  Int_t nTofMatch = nTofMatchedTracks();
  if(nTofMatch < 3)   // use to reject event
    return kStOk;
  //cout<<nTofMatch<<endl;


  //reweight  =              1.0;
  
  //--------call Hiroshi's RefmultCorr class-----------------                     
  zdc_cons  = picoEvent->runInfo().zdcCoincidenceRate();
  bbc_cons  = picoEvent->runInfo().bbcCoincidenceRate();
  refmultCorrUtil->init(runnumber);
  refmultCorrUtil->initEvent(refMult, VertexZ);            // Use this for All Au+Au < 200 GeV data.
  //refmultCorrUtil->initEvent(refMult, VertexZ, zdc_cons);  // Use ZDC conincident rate for AuAu200GeV run10 and run11. 
  
  if(refmultCorrUtil->isBadRun(runnumber))                    // If Hiroshi says it is bad run, remove it.
    { cout<<"!!!! Bad run found, skipped this Event...!!!"<<endl; 
      return kStOK;
    }
    cent = refmultCorrUtil->getCentralityBin9() ;            // you got centrality
    reweight = refmultCorrUtil->getWeight() ;              // you got reweight factor.
    Corr_refMult = refmultCorrUtil->getRefMultCorr();
    
    w = reweight;

    if(cent < 0) 
      return kStOK;
  //---------------------------------------------------------
    hVz     ->Fill(VertexZ);
    hVr     ->Fill(Vr);
    hVxVy   ->Fill(VertexX,VertexY);
    href    ->Fill(refMult);
    
    EventCount->Fill(2.2);
    
    hrunid    ->Fill(runnumber);
    hcorr_ref -> Fill(Corr_refMult,w);
    
    //cout<<"I am in Event Loop"<<endl;
  
  
  //==============Centrality selection==================
  /*  if(refMult > 5        && refMult <= 11) cent = 0;
  else if(refMult > 11  && refMult <= 20) cent = 1;
  else if(refMult > 20  && refMult <= 36) cent = 2;
  else if(refMult > 36  && refMult <= 59) cent = 3;
  else if(refMult > 59  && refMult <= 93) cent = 4;
  else if(refMult > 93  && refMult <= 138)cent = 5;
  else if(refMult > 138 && refMult <= 200)cent = 6;
  else if(refMult > 200 && refMult <= 239)cent = 7;
  else if(refMult > 239)                  cent = 8;*/
  //====================================================

      
    hrefm[i]->Fill(refMult);
    
    EventCount    ->        Fill(3.2);
    CentCount     ->     Fill(cent+1);
    
    double beta        =         -999;
    double pt1         =         -999;
    float Tof1         =         -999;
    int ntrack         =            0;
    int ntof           =            0;
    
    TObjArray* tracks = mPicoDstMaker->picoDst()->primaryTracks() ;      // Create a TObject array containing the primary tracks
    TObjArrayIter  GetTracks(tracks) ;                        // Create an iterator to step through the tracks
    StPicoTrack*                 track ;                        // Pointer to a track
    Qxsum              =           0.;
    Qysum              =           0.;
    CurrentEvent_centrality  = cent+1;
    
    
    
    while((track = (StPicoTrack*)GetTracks.Next()))   //this is track loop
      {
	//----------------track loop started------------------------
	
	//---------------------------------
	
	//======== Do your analysis, fill track property here======================
	
	Float_t Mpion = 0.1395700;
	Float_t Mkaon = 0.493677;
	Float_t Mproton = 0.93827231;
	Float_t Melectron = 0.0005109990;
	
	double a0 =  2.55501e-05;
	double a1 = -1.16103e-05;
	double a2 =  9.47638e+01;
	double a3 =  9.96323e-01;
	
	double b0 =  2.12770e-03;
	double b1 = -9.10954e-05;
	double b2 =  9.29762e+01;
	double b3 =  9.11240e-01;
	
	TLorentzVector vpi, vka, vpr;
	
	//================Getting Track Variables====================//
	
	mHitsdEdx          = track->nHitsDedx();
	//dcas                = track->dca().mag();
	float pt1          = track->pt();
	short q            = track->charge();
	float ddca         = track->dcaGlobal().mag();
	mHitsFit           = track->nHitsFit(kTpcId);
	//mHitsFit           = track->nHitsFit();
	mHitsPoss          = track->nHitsPoss(kTpcId);
	//mHitsPoss          = track->nHitsPoss();
	float ratio        = mHitsFit/mHitsPoss;
	float eta1         = track->eta();  
	mphi               = track->phi();
	flag               = track->flag();
	float Pathlength1  = track->btofPidTraits().pathLength();
	short matchflag1   = track->btofPidTraits().matchFlag();
	tNse               = track->nSigmaElectron();
	tNspi              = track->nSigmaPion();
	tNsk               = track->nSigmaKaon();
	tNspr              = track->nSigmaProton();
	dEdx_trunc         = track->probPidTraits().dEdxTruncated();
	double dEdx        = 1.e6*track->dEdx();
	float pp           = track->p().mag();
	beta               = track->btofPidTraits().beta();
 	Tof1               = track->btofPidTraits().timeOfFlight();
	
	//=======pT correction====================
	float kapt1 = pt1 - (a0 + a1*pow(1+a2/(pt1*pt1),a3));
	float prpt1 = pt1 -(b0 + b1*pow(1+b2/(pt1*pt1),b3));

	//=========Rapidity=================
	
	vpi.SetPtEtaPhiM(pt1,eta1,mphi,Mpion);
	vka.SetPtEtaPhiM(kapt1,eta1,mphi,Mkaon);
	vpr.SetPtEtaPhiM(prpt1,eta1,mphi,Mproton);
	float rpi1 = vpi.Rapidity();
	float rka1 = vka.Rapidity();
	float rpr1 = vpr.Rapidity();

	//--------pre QA plots--------------------
	hnhits_pre ->Fill(mHitsFit,w);
	hnfit_nfitposs_pre ->Fill(ratio,w);
	hndedx_pre ->Fill(mHitsdEdx,w);
	hdca_pre   ->Fill(ddca,w);
	heta_pre   ->Fill(eta1,w);
	hrapidity_pre->Fill(rpi1,w);
	
	TrackCount-> Fill(0.2);

	//=========Track Cuts==============//

	if(fabs(eta1) > 1.0)         continue;
	TrackCount-> Fill(1.2);
	if(flag < 0)                continue;
	TrackCount-> Fill(2.2);
	if(fabs(ddca) > 1.0)         continue;
	TrackCount-> Fill(3.2);
	if(mHitsFit < 16)           continue;
	TrackCount-> Fill(4.2);
	if(mHitsdEdx < 11)          continue;
	TrackCount-> Fill(5.2);
	if(ratio  < 0.52)           continue;
	TrackCount-> Fill(6.2);
	if(pt1 <=0.2)               continue;
	//if(kapt1 <=0.2)             continue;
	//if(prpt1 <=0.2)             continue;
	TrackCount-> Fill(7.2);
	if(pt1 >10.0)               continue;
	TrackCount-> Fill(8.2);
	
		
	//--------QA plots--------------------
	hpt       ->Fill(pt1,w); 
	heta      ->Fill(eta1,w);
	hrapidity ->Fill(rpi1,w);
	hphi      ->Fill(mphi,w);
	hptphi    ->Fill(pt1,mphi,w);
	hetaphi   ->Fill(eta1,mphi,w);
	hdca      ->Fill(ddca,w);
	hptdca    ->Fill(pt1,ddca,w);
	hnhits    ->Fill(mHitsFit,w);
	hnfit_nfitposs ->Fill(ratio,w);
	hpthits   ->Fill(pt1,mHitsFit,w);
	hndedx    ->Fill(mHitsdEdx,w);
	hptndedx  ->Fill(pt1,mHitsdEdx,w);
	hdEdxp    ->Fill(q*pp,dEdx,w);
	hbeta     ->Fill(q*pp,1/beta,w);
	
	float mass_sq =          -999;
	if (beta>0)
	  {
	    mass_sq  = (1/(beta*beta)-1)*pp*pp;
	    hmass2p  ->       Fill(pp,mass_sq,w);
	  }
	if(fabs(rpi1) < 0.25)
	hpt_[i]->Fill(q*pt1,w);
	
	dEdx_expected1 = q*q*1.e-6*TMath::Exp(Bichsel::Instance()->GetMostProbableZ(TMath::Log10(pp*TMath::Abs(q)/Mpion)));   
	dEdx_expected11 = q*q*1.e-6*(Bichsel::Instance()->GetI70M(TMath::Log10(pp*TMath::Abs(q)/Mpion)));
	
	Zpi = TMath::Log(dEdx_trunc/dEdx_expected1);
	Zpi1 = TMath::Log(dEdx_trunc/dEdx_expected11);
	
	dEdx_expected2 = q*q*1.e-6*TMath::Exp(Bichsel::Instance()->GetMostProbableZ(TMath::Log10(pp*TMath::Abs(q)/Mkaon)));
	dEdx_expected22 = q*q*1.e-6*(Bichsel::Instance()->GetI70M(TMath::Log10(pp*TMath::Abs(q)/Mkaon)));
	
	Zk = TMath::Log(dEdx_trunc/dEdx_expected2);
	Zk1 = TMath::Log(dEdx_trunc/dEdx_expected22);
	
	dEdx_expected3 = q*q*1.e-6*TMath::Exp(Bichsel::Instance()->GetMostProbableZ(TMath::Log10(pp*TMath::Abs(q)/Mproton)));
      
	//dEdx_expected3 = q*q*1.e-6*(Bichsel::Instance()->GetI70M(TMath::Log10(pp*TMath::Abs(q)/Mproton)));
	
	Zpr = TMath::Log(dEdx_trunc/dEdx_expected3);
	
	//=====================mb Events===============================
	if(fabs(rpi1) < 0.1)
	  {
	    if(q>0){
	      hzpipos->Fill(pt1,Zpi,w);
	    }
	    
	    if(q<0){
	      hzpineg->Fill(pt1,Zpi,w);
	    }
	  }
	if(fabs(rka1) < 0.1)
	  {
	    if(q>0){
	      hzkapos->Fill(kapt1,Zk,w);
	    }
	    
	    if(q<0){
	      hzkaneg->Fill(kapt1,Zk,w);
	    }
	  }
	if(fabs(rpr1) <0.1)
	  {
	    if(q>0){
	      hzprpos->Fill(prpt1,Zpr,w);
	    }
	    
	  if(q<0){
	    hzprneg->Fill(prpt1,Zpr,w);
	  }
	}
	//============================================================
	//-------------------------pion
	if(fabs(rpi1)<0.25){
	  if(q>0)
	  hZpipos[i]->Fill(pt1,Zpi,w);
	  
	  if(q<0)
	  hZpineg[i]->Fill(pt1,Zpi,w);
	}
	//---------------------------kaon
	if(fabs(rka1)<0.25){
	  if(q>0)
	  hZkpos[i]->Fill(kapt1,Zk,w);
	  
	  if(q<0)
	  hZkneg[i]->Fill(kapt1,Zk,w);
	}
	//---------------------------proton
	if(fabs(rpr1)<0.25){
	  if(q>0)
	  hZprpos[i]->Fill(prpt1,Zpr,w);
	  
	  if(q<0)
	  hZprneg[i]->Fill(prpt1,Zpr,w);
	
	//====================Filling Matching efficiency===============
	if(TMath::Abs(rpi1) < 0.25 && TMath::Abs(tNspi) < 2.0) //TPC PION
	{
	  if(q > 0)hrap_pip->Fill(cent,pt1,w);
	  if(q < 0)hrap_pim->Fill(cent,pt1,w);
	}
	
	if(TMath::Abs(rka1) < 0.25 && TMath::Abs(tNsk) < 2.0) //TPC Kaon
	  {
	    if(q > 0)hrap_kap->Fill(cent,kapt1,w);
	    if(q < 0)hrap_kam->Fill(cent,kapt1,w);
	  }
	
	if(TMath::Abs(rpr1) < 0.25 && TMath::Abs(tNspr) < 2.0) //TPC Proton
	  {
	    if(q > 0)hrap_prp->Fill(cent,prpt1,w);
	    if(q < 0)hrap_prm->Fill(cent,prpt1,w);
	  }
	//==============ToF Loop======================================
	if(beta > 0 && Tof1 > 0 && matchflag1 > 0)
	//if(beta > 0 && matchflag1 > 0)
	  {
	    if(TMath::Abs(rpi1) < 0.25 && TMath::Abs(tNspi) < 2.0)
	      {
		if(q > 0)hrap_piptof->Fill(cent,pt1,w);
		if(q < 0)hrap_pimtof->Fill(cent,pt1,w);
	      }
	    
	    if(TMath::Abs(rka1) < 0.25 && TMath::Abs(tNsk) < 2.0)
	      {
		if(q > 0)hrap_kaptof->Fill(cent,kapt1,w);
		if(q < 0)hrap_kamtof->Fill(cent,kapt1,w);
	      }
	    
	    if(TMath::Abs(rpr1) < 0.25 && TMath::Abs(tNspr) < 2.0)
	      {
		if(q > 0)hrap_prptof->Fill(cent,prpt1,w);
		if(q < 0)hrap_prmtof->Fill(cent,prpt1,w);
	      }

	    dca[ntof]       = ddca;
	    nHitsFit[ntof]  = mHitsFit;
	    nHitsdEdx[ntof] = mHitsdEdx;
	    mom[ntof]       = pp;
	    tof_pt[ntof]    = pt1;
	    tof_kapt[ntof]  = kapt1;
	    tof_prpt[ntof]  = prpt1;
	    
	    tof_rpi[ntof]   = rpi1;
	    tof_rka[ntof]   = rka1;
	    tof_rpr[ntof]   = rpr1;
	    
	    tNspi_tof[ntof]  = tNspi;
	    tNsk_tof[ntof]   = tNsk;
	    tNspr_tof[ntof]  = tNspr;
	    
	    charge[ntof]     = q;
	    Beta[ntof]       = beta;
	    Pathlength[ntof] = Pathlength1;
	    Tof[ntof]        = Tof1;
	    matchflag[ntof]  = matchflag1;
	    
	    ntof++;
	    
	  }
	
	//cout<<"I am in Track Loop"<<endl;
	ntrack++;
 	
      }//--------------track loop ends----------------------
    
    
	  //do your eventwise clculation here:
    //fill event variable:
    
    nTrack     =      ntof;
    Centrality =    cent+1;
    ToFRefmult =   refMult;  
    w          =  reweight;
	  
    EventCount-> Fill(5.2);
    
    t1->Fill();
    
    mEventsProcessed++ ;
    EventCount->Fill(6.2);
    
    return kStOK ;
    
}//--------------- Make (Event) loop ends--------------------------


Int_t MyAnalysisMaker::Finish()
{ 
  histogram_output -> Write() ;          // Write all histograms to disk 
  cout <<"\n ======> All done <======"<<endl;
  cout<<" Acutal #Events Processed = " <<mEventsProcessed<<"\n###### Thank You ######\n"<< endl ;
  histogram_output->Close();
  return kStOk ;  
}



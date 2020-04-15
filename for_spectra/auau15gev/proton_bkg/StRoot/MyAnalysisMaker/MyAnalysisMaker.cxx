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
  refmultCorrUtil  = new StRefMultCorr();
  //--------------------------------------------------------------------------
  //change here and in header file:
  
  //----- Define masses ------//
  /*
  t1 = new TTree("t1","UU ntuple");
  t1->Branch("Centrality",&Centrality,"Centrality/I");
  t1->Branch("ToFRefmult",&ToFRefmult,"ToFRefmult/I");   
  t1->Branch("nTrack",       &nTrack,     "nTrack/I");
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
  */

  //histograms: Add/remove as u need. See the header file. Declare there.
  EventCount = new TH1F("EventCount","EventCount",10,0,10);  //1
  CentCount  = new TH1F("CentCount",  "CentCount",10,0,10);  //2
  hrunid     = new TH1D("hrunid","UU Run id",500,1300000,1700000);
  hVz        = new TH1D("VertexZ", "VertexZ ",500,-50.,50.); //3
  href       = new TH1D("href", "RefMult ",1000,0,1000);  //4
  hVxVy      = new TH2D("hVxVy","VertexXY",200,-5.,5.,200,-5.,5.);//6
  //hvpdvz     = new TH1D("hvpdVz","vpdVz",500,-50,50);

  hdca      =  new TH1D("hdca","DCA distribution",100, 0,10);

  for( Int_t i = 0; i < 9; i++)
  {
  	hptdca_prp_names[i] = "hptdca_prp_" + to_string(i+1);
  	hptdca_prp[i] = new TH2D(hptdca_prp_names[i],"DCA Distribution(p)",200,0,10,200,0,10);
  }
  for( Int_t i = 0; i < 9; i++)
  {
  	hptdca_prm_names[i] = "hptdca_prp_" + to_string(i+1);
  	hptdca_prm[i] = new TH2D(hptdca_prp_names[i],"DCA Distribution(p)",200,0,10,200,0,10);
  }

//---------------------------------------------------------------------------

  VertexZ = -100.0;
  mvpdVz  = -100.0;
  

  return kStOK ; 
}



Bool_t MyAnalysisMaker::IsBadEvent(StPicoEvent *picoEvent)        // <==== I am using this function. **Please Change your trigger**
{

  if(!picoEvent)
    return kTRUE;
  /*
    double vx = picoEvent->primaryVertexPosition().x();
    double vy = picoEvent->primaryVertexPosition().y();
    double vz = picoEvent->primaryVertexPosition().z();
    if( (vx < 1.e-5 && vx > -1.e-5) &&       
    (vy < 1.e-5 && vy > -1.e-5) &&
    (vz < 1.e-5 && vz > -1.e-5) )  
    return kTRUE;
  */
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



Int_t MyAnalysisMaker::Make()
{
  StPicoDst *pico = mPicoDstMaker->picoDst();
  /*StPicoDstMaker * picoDstMaker = (StPicoDstMaker*)GetMaker("PicoDst");

  if(!picoDstMaker)
    {
      LOG_ERROR << "ERROR: cannot find PicoDstMaker"<<endm;
      return kStOk;
    }

    StPicoDst * pico = picoDstMaker->picoDst();*/

  
  StPicoEvent* picoEvent  =  mPicoDstMaker->picoDst()->event() ;
  EventCount-> Fill(0.2);
  
  if(IsBadEvent(picoEvent))                                       // Nominal Event cuts and trigger (change trigger as ur data)
    return           kStOK;
  EventCount-> Fill(1.2);
  
  runnumber = picoEvent->runId();

    Int_t badrun[253] = {15046073, 15046089, 15046094, 15046096, 15046102, 15046103, 15046104, 15046105, 15046106, 15046107, 15046108, 15046109, 15046110, 15046111, 15047004, 15047015, 15047016, 15047019, 15047021, 15047023, 15047024, 15047026, 15047027, 15047028, 15047029, 15047030, 15047039, 15047040, 15047041, 15047044, 15047047, 15047050, 15047052, 15047053, 15047056, 15047057, 15047061, 15047062, 15047063, 15047064, 15047065, 15047068, 15047069, 15047070, 15047071, 15047072, 15047074, 15047075, 15047082, 15047085, 15047086, 15047087, 15047093, 15047096, 15047097, 15047098, 15047100, 15047102, 15047104, 15047106, 15048003, 15048004, 15048012, 15048013, 15048014, 15048016, 15048017, 15048018, 15048019, 15048020, 15048021, 15048023, 15048024, 15048025, 15048026, 15048028, 15048029, 15048030, 15048031, 15048033, 15048034, 15048074, 15048075, 15048076, 15048077, 15048078, 15048079, 15048080, 15048081, 15048082, 15048083, 15048084, 15048085, 15048086, 15048087, 15048088, 15048089, 15048091, 15048092, 15048093, 15048094, 15048095, 15048096, 15048097, 15048098, 15049002, 15049003, 15049009, 15049013, 15049014, 15049015, 15049016, 15049017, 15049018, 15049019, 15049020, 15049021, 15049022, 15049023, 15049025, 15049026, 15049027, 15049028, 15049030, 15049031, 15049032, 15049033, 15049037, 15049038, 15049039, 15049040, 15049041, 15049074, 15049077, 15049083, 15049084, 15049085, 15049086, 15049087, 15049088, 15049089, 15049090, 15049091, 15049092, 15049093, 15049094, 15049096, 15049097, 15049098, 15049099, 15050001, 15050002, 15050003, 15050004, 15050005, 15050006, 15050010, 15050011, 15050012, 15050013, 15050014, 15050015, 15050016, 15051131, 15051132, 15051133, 15051134, 15051137, 15051141, 15051144, 15051146, 15051147, 15051148, 15051149, 15051156, 15051157, 15051159, 15051160, 15052001, 15052004, 15052005, 15052006, 15052007, 15052008, 15052009, 15052010, 15052011, 15052014, 15052015, 15052016, 15052017, 15052018, 15052019, 15052020, 15052021, 15052022, 15052023, 15052024, 15052025, 15052026, 15052040, 15052041, 15052042, 15052043, 15052060, 15052061, 15052062, 15052063, 15052064, 15052065, 15052066, 15052067, 15052068, 15052069, 15052070, 15052073, 15052074, 15052075, 15053027, 15053028, 15053029, 15053034, 15053035, 15053052, 15053054, 15053055, 15054053, 15054054, 15055018, 15055137, 15056117, 15057055, 15057059, 15058006, 15058011, 15058021, 15059057, 15059058, 15061001, 15061009, 15062006, 15062069, 15065012, 15065014, 15066070, 15068013, 15068014, 15068016, 15068018, 15069036, 15070008, 15070009, 15070010};

  //removing bad runs                                                                                                                             
  int badflag=0; ///bad run from Shusu code                                                                                                       
  for(int irun=0; irun<253; irun++)
    {
      if(runnumber == badrun[irun])
        {
          badflag = 1;//cout<<"Bad run found ---->"<<runnumber<<endl;                                                                             
          return kStOk;
        }
    }
  if(badflag==1) cout<<"why not skip this bad run event"<<endl;
  
  //-----------------All about Vertex-------------------- 
  double_t MagF = picoEvent->magneticField();

  StThreeVectorF Vertex_v = picoEvent->primaryVertexPosition();


  VertexX = picoEvent->primaryVertexPosition().x();
  VertexY = picoEvent->primaryVertexPosition().y();
  VertexZ = picoEvent->primaryVertexPosition().z();
  if( (VertexX < 1.e-5 && VertexX > -1.e-5) &&       
      (VertexY < 1.e-5 && VertexY > -1.e-5) &&
      (VertexZ < 1.e-5 && VertexZ > -1.e-5) )  
    return kStOK;
  
  if(fabs(VertexZ)>30.0)              // <== Change this cut. for AuAu19GeV it is +/-70cm
    return kStOK;     
  
  if(sqrt(VertexX*VertexX+pow(VertexY+0.89,2))>1.0)
    return kStOK;

  //-----------------------------------------------------
  hVz     ->Fill(VertexZ);
  hVxVy   ->Fill(VertexX,VertexY);
  //hvpdvz  ->Fill(mvpdVz);
  EventCount->Fill(2.2);
  
  unsigned short tofMult = picoEvent->btofTrayMultiplicity();
  unsigned short refMult =              picoEvent->refMult();
  if(refMult <= 0)
    return kStOK;
                        
  hrunid    ->Fill(runnumber);
  //reweight  =              1.0;
  
  //--------call Hiroshi's RefmultCorr class-----------------                     
  zdc_cons  = picoEvent->runInfo().zdcCoincidenceRate();
  bbc_cons  = picoEvent->runInfo().bbcCoincidenceRate();
  refmultCorrUtil->init(runnumber);
  refmultCorrUtil->initEvent(refMult, VertexZPos);            // Use this for All Au+Au < 200 GeV data.
  //refmultCorrUtil->initEvent(refMult, VertexZPos, zdc_cons);  // Use ZDC conincident rate for AuAu200GeV run10 and run11. 
  
  if(refmultCorrUtil->isBadRun(runnumber))                    // If Hiroshi says it is bad run, remove it.
    { cout<<"!!!! Bad run found, skipped this Event...!!!"<<endl; 
      return kStOK;
    }
  cent = refmultCorrUtil->getCentralityBin9() ;              // you got centrality
  //cout<<"cent = "<<cent<<endl;
  reweight = refmultCorrUtil->getWeight() ;                      // you got reweight factor.
  double w = reweight;
  //---------------------------------------------------------

  href         ->    Fill(refMult);
  //hrefm[cent]  ->    Fill(refMult);
  EventCount   ->        Fill(3.2);
  CentCount    ->     Fill(cent+1);
 
  double beta        =         -999;
  double pt1         =         -999;
  int ntrack         =            0;						 				 
  int ntof           =            0;						 				 



  cout<<"I am in event loop"<<endl;  


  gtracks = pico->numberOfGlobalTracks();
  StPicoTrack*  track;

  //cout<<"No. of Events "<<mEventsProcessed<<" No of Tracks "<<ntrack<<endl;
  cout<<" Event# "<<mEventsProcessed<<" No of Tracks "<<gtracks<<endl;


      //----------------track loop started------------------------


  for (int ig=0; ig<gtracks; ig++) {
    track =  (StPicoTrack*) pico->globalTracks(ig);


//======== Do your analysis, fill track property here======================

      Float_t Mpion = 0.1395700;
      Float_t Mkaon = 0.493677;
      Float_t Mproton = 0.93827231;
      Float_t Melectron = 0.0005109990;
      
      double a0 = 0.0009153;
      double a1 = -4.157e-05;
      double a2 = 96.27;
      double a3 = 0.8292;
      
      double b0 = 1.52112e-03;
      double b1 = -8.15108e-05;
      double b2 = 8.16250e+01;
      double b3 = 9.45283e-01;
	
      //================Getting Track Variables====================//

      StPhysicalHelixD helix = track->helix();
      StThreeVectorD phelx = helix.momentum(MagF*kilogauss);
      StThreeVectorF ohelx = helix.origin();

      short qhel = helix.charge(MagF*kilogauss);

      StPhysicalHelix *vhelx = new StPhysicalHelix(phelx,ohelx,MagF*kilogauss,qhel);
      double s = vhelx->pathLength(Vertex_v);
      StThreeVector<double> dcaPoint = vhelx->at(s);
      StThreeVector<double> ghdca1 = dcaPoint - Vertex_v;
      double ghdca = ghdca1.mag();
      //cout <<"Vertex: "<< *Vx1 <<"  " << 


      mHitsdEdx          = track->nHitsDedx();
      dca                = track->dca().mag();
      float pt1          = track->pt();
      //cout<<"pt = "<<pt1<<endl;

      short q            = track->charge();
      //float ddca         = track->dcaGlobal().mag();
      StThreeVectorF dcaGlobal = track->dcaGlobal();
      Float_t    dcaGlobal_vx=dcaGlobal.x();
      Float_t    dcaGlobal_vy=dcaGlobal.y();
      Float_t    dcaGlobal_vz=dcaGlobal.z();
      float ddca = TMath::Sqrt((dcaGlobal_vx * dcaGlobal_vx) + (dcaGlobal_vy * dcaGlobal_vy) + (dcaGlobal_vz * dcaGlobal_vz));

      //cout<<"dca = "<<ddca<<endl;
      mHitsFit           = track->nHitsFit();
      mHitsPoss          = track->nHitsPoss(); 
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
      float Tof1         = track->btofPidTraits().timeOfFlight();

      //=======pT correction====================
      float kapt1 = pt1 - (a0 + a1*pow(1+a2/(pt1*pt1),a3));
      float prpt1 = pt1 -(b0 + b1*pow(1+b2/(pt1*pt1),b3));
		    
      //=========Track Cuts==============//
      if(fabs(eta1) >1.0)         continue;
      if(mHitsFit < 25)            continue;
      if(mHitsdEdx < 15)          continue;
      float  ratio  = mHitsFit / mHitsPoss;
      if(ratio  < 0.52)           continue;
      if(pt1 <0.1)                continue;
      if(kapt1 <0.1)              continue;
      if(prpt1 <0.1)              continue;
      if(pt1 >10.0)               continue;
      if(flag < 0)                continue;

      //=========Rapidity=================

      //hdca->Fill(ddca);
      hdca->Fill(ghdca);


      TLorentzVector vpi, vka, vpr;
      vpi.SetPtEtaPhiM(pt1,eta1,mphi,Mpion);
      vka.SetPtEtaPhiM(kapt1,eta1,mphi,Mkaon);
      vpr.SetPtEtaPhiM(prpt1,eta1,mphi,Mproton);
      float rpi1 = vpi.Rapidity();
      float rka1 = vka.Rapidity();
      float rpr1 = vpr.Rapidity();
		    
      //--------QA plots--------------------
      //if(TMath::Abs(tNspr)<2 && TMath::Abs(rpr1)<0.1)
      if(TMath::Abs(tNspr)<2)
	{
	  if(qhel>0){
	    hptdca_prp[cent]->Fill(prpt1,ghdca,w);	    
	      }
	  if(qhel<0){
	    hptdca_prm[cent]->Fill(prpt1,ghdca,w);   
	      }
	}
      if(fabs(ddca) >3.0)         continue;
      
      /*
      hpt       ->Fill(pt1); 
      heta      ->Fill(eta1);
      hphi      ->Fill(mphi);
      hptphi    ->Fill(pt1,mphi);
      hetaphi   ->Fill(eta1,mphi);
      hdca      ->Fill(ddca);
      hptdca    ->Fill(pt1,ddca);
      hnhits    ->Fill(mHitsFit);
      hpthits   ->Fill(pt1,mHitsFit);
      hndedx    ->Fill(mHitsdEdx);
      hptndedx  ->Fill(pt1,mHitsdEdx);
      hdEdxp    ->Fill(q*pp,dEdx);
      hbeta     ->Fill(q*pp,1/beta);
      double mass_sq =             -1000.0;
      if(beta>0 && nHitsFit>25 && ddca<1.0)
	{
	  mass_sq    = (1/(beta*beta)-1)*pp*pp;
	  hmass2p    ->       Fill(pp,mass_sq);
	}		    
      */

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
      /*
      //-------------------------pion
      if(fabs(rpi1)<0.1){
	if(q>0){
	  hZpipos[cent]->Fill(pt1,Zpi);
	}
	
	if(q<0){
	  hZpineg[cent]->Fill(pt1,Zpi);
	}
      }
      //.................................
      if(fabs(rpi1)<0.1){
	if(q>0){
	  hZpipos1[cent]->Fill(pt1,Zpi1);
	}
	
	if(q<0){
	  hZpineg1[cent]->Fill(pt1,Zpi1);
	}
      }
      //---------------------------kaon
      if(fabs(rka1)<0.1){
	if(q>0){
	  hZkpos[cent]->Fill(kapt1,Zk);
	}
	
	if(q<0){
	  hZkneg[cent]->Fill(kapt1,Zk);
	}
      }
      //---------------------------------
      if(fabs(rka1)<0.1){
	if(q>0){
	  hZkpos1[cent]->Fill(kapt1,Zk1);
	}
	
	if(q<0){
	  hZkneg1[cent]->Fill(kapt1,Zk1);
	}
      }
      //---------------------------proton
      if(fabs(rpr1)<0.1){
	if(q>0){
	  hZprpos[cent]->Fill(prpt1,Zpr);
	}
	
	if(q<0){
	  hZprneg[cent]->Fill(prpt1,Zpr);
	}
      }

      //====================Filling Matching efficiency===============
      if(TMath::Abs(rpi1) < 0.1 && TMath::Abs(tNspi) < 2.0) //TPC PION
	{
	  if(q > 0)hrap_pip->Fill(cent,pt1);
	  if(q < 0)hrap_pim->Fill(cent,pt1);
	}
      
      if(TMath::Abs(rka1) < 0.1 && TMath::Abs(tNsk) < 2.0) //TPC Kaon
	{
	  if(q > 0)hrap_kap->Fill(cent,kapt1);
	  if(q < 0)hrap_kam->Fill(cent,kapt1);
	}
      
      if(TMath::Abs(rpr1) < 0.1 && TMath::Abs(tNspr) < 2.0) //TPC Proton
	{
	  if(q > 0)hrap_prp->Fill(cent,prpt1);
	  if(q < 0)hrap_prm->Fill(cent,prpt1);
	}
      //==============ToF Loop======================================
      if(beta != -999 && matchflag1 > 0)
	{
	  if(TMath::Abs(rpi1) < 0.1 && TMath::Abs(tNspi) < 2.0)
	    {
	      if(q > 0)hrap_piptof->Fill(cent,pt1);
	      if(q < 0)hrap_pimtof->Fill(cent,pt1);
	    }
	  
	  if(TMath::Abs(rka1) < 0.1 && TMath::Abs(tNsk) < 2.0)
	    {
	      if(q > 0)hrap_kaptof->Fill(cent,kapt1);
	      if(q < 0)hrap_kamtof->Fill(cent,kapt1);
	    }
	  
	  if(TMath::Abs(rpr1) < 0.1 && TMath::Abs(tNspr) < 2.0)
	    {
	      if(q > 0)hrap_prptof->Fill(cent,prpt1);
	      if(q < 0)hrap_prmtof->Fill(cent,prpt1);
	    }
	  mom[ntof]      = pp;
	  tof_pt[ntof]   = pt1;
	  tof_kapt[ntof] = kapt1;
	  tof_prpt[ntof] = prpt1;
	  
	  tof_rpi[ntof]  = rpi1;
	  tof_rka[ntof]  = rka1;
	  tof_rpr[ntof]  = rpr1;
	  
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
      */
      ntrack++;
 		    
    }//--------------track loop ends----------------------
	  
	  
	  //do your eventwise clculation here:
  //fill event variable:
  nTrack     =      ntof;
  Centrality =    cent+1;
  ToFRefmult =   refMult;  
	  
  EventCount->        Fill(5.2);

  //t1->Fill();
	
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



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

// The way libraries are called below is done in order to allow
// to call TrackCut(StPicoTrack* track) method. One cannot simply
// include STAR headers in macro

// Load STAR libraries
Int_t starLibs = gROOT->Macro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
// Load StPicoEvent library
Int_t picoLib = gSystem->Load("StPicoEvent");

// Example of the track cut
Bool_t TrackCut(StPicoTrack* track) {
  return true;
}

//_________________
void runPicoDstAnalysisWithMacro(const char* inFileName = "/star/rcf/test/dev/daq_sl302.stica/Sun/year_2019/production_19GeV_2019/st_physics_20069002_raw_1500008.picoDst.root") {

  std::cout << "Lets run the StPicoDstAnalysisMaker, Master" << std::endl;

  // Initialize picoDst reader with input file (list)
  StPicoDstReader *picoReader = new StPicoDstReader(inFileName);
  picoReader->Init();

  // This is a way if you want to spead up I/O
  std::cout << "Explicit read status for some branches" << std::endl;
  picoReader->SetStatus("*",0);
  picoReader->SetStatus("Event*", 1);
  picoReader->SetStatus("Track*", 1);
  picoReader->SetStatus("BTofPidTraits*", 1);
  picoReader->SetStatus("BTowHit*", 1);
  //picoReader->SetStatus("EmcTrigger",0);
  //picoReader->SetStatus("TrackCovMatrix",1);
  std::cout << "Status has been set" << std::endl;

  std::cout << "Now I know what to read, Master!" << std::endl;

  if( !picoReader->chain() ) {
    std::cout << "No chain has been found." << std::endl;
  }

  // Retrieve number of events in chain
  Long64_t events2read = picoReader->chain()->GetEntries();
  std::cout << "Number of events to read: " << events2read << std::endl;

  // Create file where to store histograms with results
  TFile *oFile = new TFile("oPicoAnaMacro_3.root","recreate");
  // Create some histograms
  TH1F *hVtxZ = new TH1F("hVtxZ","hVtxZ", 140, -70., 70.);
  TH2F *hPrimaryDedxVsPt = new TH2F("hPrimaryDedxVsPt","Primary track dE/dx vs. p_{T};charge #cross p_{T} (GeV/c);dE/dx (keV/cm)",
				    300, -1.5, 1.5, 120, 0., 12.);
  TH2F *hPrimaryInvBetaVsP = new TH2F("hPrimaryInvBetaVsP","Primary track 1/#beta vs. p;charge #cross p (GeV/c);1/#beta",
				      420, -2.1, 2.1, 200, 0.8, 2.8);
  TH1D *hBemcTowerAdc = new TH1D("hBemcTowerAdc","BEMC tower ADC;ADC;Entries", 500, 0., 3000.);

  // Loop over events
  for(Long64_t iEvent=0; iEvent<events2read; iEvent++) {

    std::cout << "Working on event #[" << (iEvent+1)
	      << "/" << events2read << "]" << std::endl;

    Bool_t readEvent = picoReader->readPicoEvent(iEvent);
    if( !readEvent ) {
      std::cout << "Something went wrong, Master! Nothing to analyze..."
		<< std::endl;
      break;
    }

    // Retrieve picoDst
    StPicoDst *dst = picoReader->picoDst();
    
    // Retrieve event information
    StPicoEvent *event = dst->event();
    if( !event ) {
      std::cout << "Something went wrong, Master! Event is hiding from me..."
		<< std::endl;
      break;
    }

    // Retrieve primary vertex position
    TVector3 pVtx = event->primaryVertex();
    // Fill event histograms
    hVtxZ->Fill( pVtx.Z() );

    // Track loop
    for(Int_t iTrk=0; iTrk<dst->numberOfTracks(); iTrk++) {

      // Retrieve i-th pico track
      StPicoTrack *picoTrack = dst->track(iTrk);
      if(!picoTrack) continue;

      // Primary track analysis
      if( picoTrack->isPrimary() ) {
	hPrimaryDedxVsPt->Fill( picoTrack->charge() * picoTrack->pMom().Perp(), picoTrack->dEdx() );

	// Accessing TOF PID traits information.
	// One has to remember that TOF information is valid for primary tracks ONLY.
	// For global tracks the path length and time-of-flight have to be
	// recalculated by hands.
	if ( picoTrack->isTofTrack() ) {
	  StPicoBTofPidTraits *trait = (StPicoBTofPidTraits*)dst->btofPidTraits( picoTrack->bTofPidTraitsIndex() );
	  if (!trait) continue;

	  // Fill primary track TOF information
	  hPrimaryInvBetaVsP->Fill( picoTrack->charge() * picoTrack->pMom().Mag(),
				    1./trait->btofBeta() );
	} // if ( picoTrack->isTofTrack() )	
      } // if( picoTrack->isPrimary() )

    } // for(Int_t iTrk=0; iTrk<nTracks; iTrk++)

    // Get number of BTOW hits (make sure that SetStatus("BTowHit*",1) is set to 1)
    // If it is set to 0, then mPicoDst will return 0 hits all the time
    // Loop over BTOW hits
    for (UInt_t iHit=0; iHit<dst->numberOfBTowHits(); iHit++) {
      // Retrieve i-th BTOW hit
      StPicoBTowHit *btowHit = (StPicoBTowHit*)dst->btowHit(iHit);
      // Hit must exist
      if (!btowHit) continue;
      // Fill tower ADC
      hBemcTowerAdc->Fill( btowHit->adc() );
    } // for (UInt_t iHit=0; iHit<dst->numberOfBTowHits(); iHit++)

  } // for(Long64_t iEvent=0; iEvent<events2read; iEvent++)

  // Finish reader
  picoReader->Finish();

  // Write and close output file
  oFile->Write();
  oFile->Close();

  // Free memory
  delete picoReader;
  
  std::cout << "I'm done with analysis. We'll have a Nobel Prize, Master!" << std::endl;
}

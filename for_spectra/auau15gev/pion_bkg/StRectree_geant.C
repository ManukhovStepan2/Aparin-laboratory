//Modified from StAssociator.C
//////////////////////////////////////////////////////////////////////

class StChain;
StChain *chain=0;

//void StRectree_geant(Int_t nevents=1000,const char *MainFile="/star/institutions/vecc/lokesh/FD_AuAu9GeV/fd_simu_data/rcf1503_140_1000evts.geant.root",char* out="test.root")

void StRectree_geant(Int_t nevents=100,const char *MainFile="/star/data99/reco/AuAu15/hijing_382/b0_20/minbias/y2014a/gheisha_on/p14ii_Aug_11/rcf16010_7_50evts.geant.root",char* out="test.root")
{

  // Dynamically link needed shared libs
  gSystem->Load("St_base");
  gSystem->Load("StChain");

  //gSystem->Load("/afs/rhic.bnl.gov/star/packages/SL14i/.sl64_gcc482/lib");


  gSystem->Load("St_Tables");
  gSystem->Load("StUtilities");
  gSystem->Load("StIOMaker");
  gSystem->Load("StarClassLibrary");
  gSystem->Load("StDetectorDbMaker");

  //gSystem->Load("COMMON");
  gSystem->Load("StRoot/StTpcDb");
  gSystem->Load("StEvent");
  //   gSystem->Load("StEventMaker"); //not needed if event.root branch present
  gSystem->Load("StEmcUtil"); 
  gSystem->Load("StEEmcUtil");
  
  gSystem->Load("StMcEvent");
  gSystem->Load("StMcEventMaker");
  //gSystem->Load("StRoot/StAssociationMaker");
  gSystem->Load("StAssociationMaker");
  gSystem->Load("StRecTreeMaker");
    
  chain = new StChain("StChain"); 
  chain->SetDebug();
   
  // Now we add Makers to the chain...

  StIOMaker* ioMaker = new StIOMaker("IO","r",MainFile,"bfcTree");
  ioMaker->SetDebug();
  ioMaker->SetIOMode("r");
  ioMaker->SetBranch("*",0,"0");                 //deactivate all branches
  ioMaker->SetBranch("geantBranch",0,"r"); //activate geant Branch
  ioMaker->SetBranch("eventBranch",0,"r"); //activate geant Branch

//   ioMaker->SetBranch("dstBranch",0,"r"); //activate Event Branch
//   ioMaker->SetBranch("runcoBranch",0,"r"); //activate runco Branch

  // Note, the title "events" is used in the Association Maker, so don't change it.
  // StEventMaker is not needed for event.root files
//   StEventMaker*       eventReader   = new StEventMaker("events","title");
//   eventReader->doPrintMemoryInfo = kFALSE;

  StMcEventMaker*     mcEventReader = new StMcEventMaker; // Make an instance...
  StAssociationMaker* associator    = new StAssociationMaker;
  cout<<"created new StAssociationMaker"<<endl;

  //associator->useInTracker();

  cout<<"creating new StAnalysisMaker"<<endl;

  StRecTreeMaker*  examples      = new StRecTreeMaker(out);
  cout<<"created Maker for reconstructed tree!"<<endl;

  // Define the cuts for the Associations

  /*StMcParameterDB* parameterDB = StMcParameterDB::instance();  
  // TPC
  parameterDB->setXCutTpc(.6); // 6 mm
  parameterDB->setYCutTpc(.6); // 6 mm
  parameterDB->setZCutTpc(.6); // 6 mm
  parameterDB->setReqCommonHitsTpc(3); // Require 3 hits in common for tracks to be associated
  // FTPC
  parameterDB->setRCutFtpc(.3); // 3 mm
  parameterDB->setPhiCutFtpc(5*(3.1415927/180.0)); // 5 degrees
  parameterDB->setReqCommonHitsFtpc(3); // Require 3 hits in common for tracks to be associated
  // SVT
  parameterDB->setXCutSvt(.08); // 800 um
  parameterDB->setYCutSvt(.08); // 800 um
  parameterDB->setZCutSvt(.08); // 800 um
  parameterDB->setReqCommonHitsSvt(1); // Require 1 hits in common for tracks to be associated
  */
    
  // now execute the chain member functions

  chain->PrintInfo();
  Int_t initStat = chain->Init(); // This should call the Init() method in ALL makers
  if (initStat) chain->Fatal(initStat, "during Init()");
    
  int istat;
  cout<<"begin event loop"<<endl;
  for(int iev=1;iev<=nevents;iev++){// Event Loop
   cout<<"istat: "<<istat<<endl;
   //if(istat!=2) continue;
   chain->Clear();
   cout << "---------------------- Processing Event : " << iev << " ----------------------" << endl;
   istat = chain->Make(iev); // This should call the Make() method in ALL makers
   if(istat){
     cout<<"bad return code!"<<endl;
     break;
   }
   //if (istat == 2) { cout << "Last  Event Processed. Status = " << istat << endl; }
   //if (istat == 3) { cout << "Error Event Processed. Status = " << istat << endl; }
  }

  chain->Finish(); 
}


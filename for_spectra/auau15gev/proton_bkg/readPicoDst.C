
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


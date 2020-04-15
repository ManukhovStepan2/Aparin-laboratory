
#include <fstream>
#include <iostream>

#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TMath.h"
#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "ReadPico.h"

using namespace std ;

ClassImp(ReadPico)


  void ReadPico::LoopAll(const Char_t* filelist, const Char_t* outfile, const Char_t* outpi, const Char_t* outka, const Char_t* outpr)
{
  fpw      = new TFile(outfile,"recreate");
  
  hrefmult = new TH1F("hrefmult","Reference Multiplicity",1000, 0, 1000);
  hcentmult = new TH1F("hcentmult","Reference Multiplicity",1000, 0, 1000);
  
  //Measured m^2 vs pT for all pi, k, and p
  hmsq_all_pos = new TH2D("hmsq_all_pos","M-sq all positive",120, 0., 12., 600, -1.0, 2.0 );
  hmsq_all_neg = new TH2D("hmsq_all_neg","M-sq all negative",120, 0., 12., 600, -1.0, 2.0 );
  hmsq_all_pos_check = new TH2D("hmsq_all_pos_check","M-sq all positive",120, 0., 12., 600, -1.0, 2.0 ); //To check in 2nd event loop
  
  hdeltapiplus = new TH2D("hdeltapiplus","#Deltat [#pi^{+}]",120, 0., 12., 400,-1.0,1.0);
  hdeltakplus  = new TH2D("hdeltakplus","#Deltat [K^{+}]",120, 0., 12., 400,-1.0,1.0);
  hdeltaprplus = new TH2D("hdeltaprplus","#Deltat [p]",120, 0., 12., 400,-1.0,1.0);
  hdeltapineg  = new TH2D("hdeltapineg","#Deltat [#pi^{-}]",120, 0., 12., 400,-1.0,1.0);
  hdeltakneg   = new TH2D("hdeltakneg","#Deltat [K^{-}]",120, 0., 12., 400,-1.0,1.0);
  hdeltaprneg  = new TH2D("hdeltaprneg","#Deltat [#bar{p}]",120, 0., 12., 400,-1.0,1.0);
  hdeltacheck = new TH1D("hdeltacheck","",400,-1.0,1.0);
  
  
  hdeltapiplus_check = new TH2D("hdeltapiplus_check","#Deltat [#pi^{+}]",120, 0., 12., 400,-1.0,1.0);
  
  
  //Define file to fill predicted m^2 for pions
  fpion      = new TFile(outpi,"recreate");
  //Predicted m^2 pion positive

  for(Int_t i = 4; i < 30; i++)
  {
  	hpreg_msqpipos_names[i] = "hpred_msqpipos" + to_string(i+1);
  	hpred_msqpipos[i] = new TH1D(hpreg_msqpipos_names[i],"",600, -1.0, 2.0 ); //pT: 0.i-0.(i+1) GeV/c
  }; 
  //hpred_msqpipos_highpt = new TH2D("hpred_msqpipos_highpt","",120, 0., 12., 1000, -2.0, 3.0 ); 
  hpred_msqpipos_highpt = new TH2D("hpred_msqpipos_highpt","",120, 0., 12., 600, -1.0, 2.0 );



  //Predicted m^2 pion negative
  for(Int_t i = 4; i < 30; i++)
  {
  	hpreg_msqpineg_names[i] = "hpred_msqpineg" + to_string(i+1);
  	hpred_msqpineg[i] = new TH1D(hpreg_msqpineg_names[i],"",600, -1.0, 2.0 ); //pT: 0.i-0.(i+1) GeV/c
  }; 
  hpred_msqpineg_highpt = new TH2D("hpred_msqpineg_highpt","",120, 0., 12., 600, -1.0, 2.0 );



  //Define file to fill predicted m^2 for pions
  fkaon      = new TFile(outka,"recreate");
  //Predicted m^2 kaon positive
  for(Int_t i = 4; i < 30; i++)
  {
  	hpreg_msqkapos_names[i] = "hpred_msqkapos" + to_string(i+1);
  	hpred_msqkapos[i] = new TH1D(hpreg_msqkapos_names[i],"",600, -1.0, 2.0 ); //pT: 0.i-0.(i+1) GeV/c
  }; 
  hpred_msqkapos_highpt = new TH2D("hpred_msqkapos_highpt","",120, 0., 12., 600, -1.0, 2.0 );


  //Predicted m^2 kaon negative
  for(Int_t i = 4; i < 30; i++)
  {
  	hpreg_msqkaneg_names[i] = "hpred_msqkaneg" + to_string(i+1);
  	hpred_msqkaneg[i] = new TH1D(hpreg_msqkaneg_names[i],"",600, -1.0, 2.0 ); //pT: 0.i-0.(i+1) GeV/c
  };   
  hpred_msqkaneg_highpt = new TH2D("hpred_msqkaneg_highpt","",120, 0., 12., 600, -1.0, 2.0 );



  //Define file to fill predicted m^2 for pions
  fproton      = new TFile(outpr,"recreate");
  //Predicted m^2 pron positive
  for(Int_t i = 4; i < 30; i++)
  {
  	hpreg_msqprpos_names[i] = "hpred_msqprpos" + to_string(i+1);
  	hpred_msqprpos[i] = new TH1D(hpreg_msqprpos_names[i],"",600, -1.0, 2.0 ); //pT: 0.i-0.(i+1) GeV/c
  };    
  hpred_msqprpos_highpt = new TH2D("hpred_msqprpos_highpt","",120, 0., 12., 600, -1.0, 2.0 );

  //Predicted m^2 proton negative
  for(Int_t i = 4; i < 30; i++)
  {
  	hpreg_msqprneg_names[i] = "hpred_msqprneg" + to_string(i+1);
  	hpred_msqprneg[i] = new TH1D(hpreg_msqprneg_names[i],"",600, -1.0, 2.0 ); //pT: 0.i-0.(i+1) GeV/c
  };    
  hpred_msqprneg_highpt = new TH2D("hpred_msqprneg_highpt","",120, 0., 12., 600, -1.0, 2.0 );
  
  //------- Event counter and breaks -------//
  EvCount = 0;
  EvCount2nd = 0;
  EventBreak =  40000000; 
  //EventBreak =  2000; 
  //------- Event counter and breaks -------//
  
  //-------- constants  --------//
  c_light = 29.9792458; //speed of light in [cm/ns]
  pimass = 0.13957;
  kmass = 0.493677;
  pmass = 0.938272;
  nfiles = 3000;  //nfiles in input file list 
  //-------- constants  --------//




  //============  All cuts ==========// 
  rapcut   = 0.25;
  //vzcut    = 30.0;
  dcacut   = 1.0;
  nHitscut = 16;
  ndEdxcut = 11;
  lowpTcut = 0.2;
  //vzcut = 70.0;

  //---- Centrality cut from Hiroshi ------------// 
  ////////////////////////////////////////////
  //0-5%   ::::  > 535          cent1=8
  //5-10%  ::::  460 - 535      cent1=7
  //10-20% ::::  338 - 460      cent1=6
  //20-30% ::::  233 - 338      cent1=5
  //30-40% ::::  150 - 233      cent1=4
  //40-50% ::::   90 - 150      cent1=3
  //50-60% ::::   50 - 90       cent1=2
  //60-70% ::::   24 - 50       cent1=1
  //70-80% ::::   10 - 24       cent1=0
  ////////////////////////////////////////////
  
  //minbias (0-5)%
  RefLowCut=239;
  RefHighCut=800;
  CentCut=9;
  //-----------  Centrality cut from Hiroshi ------------//  

  //============  All cuts ==========//


  //cout <<" I am within the main Loopall() ========= " << endl;

  //     TString fNameList = TString("pico_39gev_new.list");
     TString fNameList = TString(filelist);
     cout <<" Load files from file: " << fNameList << endl;


     //     ifstream fList((char*)fNameList);
     ifstream fList(fNameList.Data());


     if (!fList)
       {
         cout << "!!! Can't open file " << fNameList << endl;
         return;
       }
     
     
     // Reading each file starts here
     char lineFromFile[255];
     //string lineFromFile;
     Int_t j=1;


     while(fList.getline(lineFromFile, 250))
       {
	 
	 
	 TString fileName = lineFromFile;
	 fileName = "" + fileName;
	 
	 //cout<<j<< " Filename== " << fileName <<"   " << lineFromFile << endl;
	 
	 //cout<<j<< " Filename== "  << lineFromFile << endl; 	     
	 
	 
	 //getline(lineFromFile, 250)
	 
	 if(EvCount > EventBreak) 
	   {
	     cout <<"//------- All events requested for 1st event loop have been processed ---------//" << endl;
	     break;
	   }
	 
	 //cout <<" events " << ev << endl;
	 //if(EvCount > ev) break;
	 
	 ///cout<<j << " Getting file from the list withiin while loop  ======== " << endl;
	 
	 
	 
	 if(j>nfiles) 
	   {
	     cout <<"<<------- All input files requested for 1st event loop have been processed --------->>" << endl;
	     //break;
	   }
         // Open this loop only when you need the information about loading files  
	 
	 //if(chain->Add((char*)fileName)){};
         //Loop(ev,fileName);
	 //cout<<j<< " Filename== " << fileName << endl;
	 //if(fList.eof())break;
	 Loop(fileName);
	 //Loop(lineFromFile);
	 j++;
       }

     cout <<" I come out of the loop =========== " << endl;
     fList.close();
     
     //Again open the flist
     ifstream fList2nd(fNameList.Data());
     char lineFromFile2nd[255];
     cout <<" opened  list for 2nd loop === " << endl;
     Int_t k=1;
     while(!(fList2nd.getline(lineFromFile2nd, 250)).eof())
       {
	 cout <<" I am within the 2nd flist llop ====== " << endl;
	 if(EvCount2nd > EventBreak)
           {
             cout <<"//------- All events requested for 2nd event loop have been processed ---------//" << endl;
             break;
           }

         //cout <<" events " << ev << endl;  
         //if(EvCount > ev) break;  

         TString fileName2nd = lineFromFile2nd;

         fileName2nd = "" + fileName2nd;
         // Open this loop only when you need the information about loading files 
	 //cout <<k <<" reading file in 2nd while loop ==== " <<fileName2nd <<  endl;
         //if(chain->Add((char*)fileName)){};         
         //Loop(ev,fileName);  
	 if(k>nfiles)
	   {
	     cout <<"<<------- All input files requested for 2nd event loop have been processed --------->>" << endl;
	     //break;
	   }
         Loop2(fileName2nd);

	 k++;
       }
     
     fList2nd.close();
     
//     tree = chain;

  Finish();
}

//void ReadPico::Loop(const int ev, const Char_t* filename)
void ReadPico::Loop(const Char_t* filename)
{
//   In a ROOT session, you can do:
//      Root > .L ReadPico.C
//      Root > ReadPico t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
  
  //cout<<" Input file ::: " << filename << endl;

  //TFile* input = TFile::Open(filename);
  TFile* input = new TFile(filename,"r");

  //----- check if zombie() ------//
  if(input->IsZombie()) {
    cout << filename <<" **** zombie *** skipping *****" << endl; 
    return;
  }


  TTree* tree = (TTree*) input->Get("t1"); //change here
  if(!tree)
    {
      //cout <<" File: ====> " << filename;
      cout<<"   ********* No tree found ---- skipping ********* " << endl;
      return;
    }
  Init(tree);

  if (fChain == 0) return;


   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

   //Float_t rapcut = 0.1;

   //cout<<" I am starting event loop, total events  == " << nentries << endl;

   //Event Loop starts
   for (Long64_t jentry=0; jentry<nentries;jentry++) {


     //if(EvCount > EventBreak) break;

      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      //if(jentry==5000)break;

      //if(jentry%1000==0)
      //cout <<" Processing Event Number # " << jentry << endl;
      


     //cout <<"Npvrtx: " << Npvrtx << endl;

      //Int_t RefMult = NPos+NNeg;
      //Float_t vrad = TMath::Sqrt(Vx*Vx + Vy*Vy);
     
     
     //----- Cuts for my analysis -----//
     //if(TMath::Abs(Vz) > 75.0) continue;
      //if(TMath::Abs(Vz) > vzcut) continue;
      if(ToFRefmult <= 0) continue;                            //==========check
     //if(TMath::Abs(vrad)>2.0) continue;
     //----- Cuts for my analysis -----//

      
      //hgrefmult->Fill(gRefMult);
      //hpplusnmult->Fill(PosPlusNegMul);

      hrefmult->Fill(ToFRefmult);      


      
      ////////////////////////////////////////////
      //-----------  Centrality cut from Hiroshi ------------//  
      // ************** cuts ********************//
      ////////////////////////////////////////////  
      //0-5%   ::::  > 185
      //5-10%  ::::  154 - 185
      //10-20% ::::  106 - 154
      //20-30% ::::   72 - 106
      //30-40% ::::   46 - 72 
      //40-50% ::::   28 - 46 
      //50-60% ::::   16 - 28 
      //60-70% ::::    8 - 16 
      //60-70% ::::    4 - 8  
      //if(RefMult <= RefLowCut  ||  RefMult >  RefHighCut) continue; 
      if(Centrality!=CentCut) continue;  //39 GeV (5-10%)
      //----------------------------------------// 
      
      
      if(EvCount > EventBreak) break;
      if(EvCount%500==0)
	
	//	cout <<" Processing Event Number # " << EvCount << endl; 
	cout <<" Process Event No. # " << EvCount 
	     << " jentry = " << jentry
             << " input = " << filename
             << " Centrality = " << Centrality
             << " RefM = " << ToFRefmult << endl; 
      
      hcentmult->Fill(ToFRefmult);   
           
      // Loop over primary tracks (TOF tracks)                                                                
      for(Int_t tr = 0; tr < nTrack; tr++)
         {
	   
	   Float_t pirap = tof_rpi[tr];
	   Float_t karap = tof_rka[tr];
	   Float_t prrap = tof_rpr[tr];

	   if(dca[tr] > dcacut)         continue;
	   if(nHitsFit[tr] > nHitscut)  continue;
	   if(nHitsdEdx[tr] > ndEdxcut) continue;
	   if(tof_pt[tr] <= lowpTcut)   continue;
	   if(tof_kapt[tr] <= lowpTcut) continue;
	   if(tof_prpt[tr] <= lowpTcut) continue;

	   //Working for TOF
	   //  if(Beta[tr]!=-999  && Match[tr]>0)
	       {
		 
		 
		 Float_t msq = mom[tr]*mom[tr] * ( 1.0 / (Beta[tr]*Beta[tr]) - 1);
		 
         	 //cout <<P[tr]<<"   " << Beta[tr] <<" mass-sq :: " << msq_pipos << endl;
		 
	       
	       // M^2 for pi, k, p in |y| < 0.5
	       //if(TMath::Abs(pirap) < rapcut || TMath::Abs(karap) < rapcut || TMath::Abs(prrap) < rapcut)
        	 //if(TMath::Abs(prrap) < rapcut)
		 if(TMath::Abs(karap) < rapcut) //change here
		 {
		   if(charge[tr] > 0)
		     {
		       hmsq_all_pos->Fill(tof_pt[tr],msq,w);
		       //hmsq_all_pos->Fill(Tof_Pt[tr],msq,w);
		     }
		   if(charge[tr] < 0)
		     {
		       hmsq_all_neg->Fill(tof_pt[tr],msq,w);
		     }
		 }


	    //---------- Calculating tof (exp) ---------------//
            Float_t tofpi_exp = TMath::Sqrt((Pathlength[tr]*Pathlength[tr])/(c_light*c_light)*(1+(pimass*pimass)/(mom[tr]*mom[tr])));
	    Float_t tofk_exp = TMath::Sqrt((Pathlength[tr]*Pathlength[tr])/(c_light*c_light)*(1+(kmass*kmass)/(mom[tr]*mom[tr])));
	    Float_t tofp_exp = TMath::Sqrt((Pathlength[tr]*Pathlength[tr])/(c_light*c_light)*(1+(pmass*pmass)/(mom[tr]*mom[tr])));


	   if(charge[tr] > 0)
	     {
	       //cout<<pirap <<"   " << SigmaPi[tr] << endl;

	       if(TMath::Abs(pirap) < rapcut && TMath::Abs(tNspi_tof[tr]) < 2.0)
		 {
		   //cout<<pirap <<"   " << SigmaPi[tr] << endl;
		   //Float_t msq_pipos = mom[tr]*mom[tr] * ( 1.0 / (Beta[tr]*Beta[tr]) - 1);
		   //hmsq_pipos->Fill(Pt[tr],msq_pipos);
		   //hmsq_pipos->Fill(Pt[tr],msq);

		    Float_t delta_piplus = Tof[tr] - tofpi_exp;
		    hdeltapiplus->Fill(tof_pt[tr], delta_piplus,w);


		 }
		   
	       if(TMath::Abs(karap) < rapcut && TMath::Abs(tNsk_tof[tr]) < 2.0)
		 {
		   //Float_t msq_kapos = mom[tr]*mom[tr] * ( 1.0 / (Beta[tr]*Beta[tr]) - 1);
		   //hmsq_kapos->Fill(tof_pt[tr],msq);

		   Float_t delta_kplus = Tof[tr] - tofk_exp;
		   hdeltakplus->Fill(tof_kapt[tr], delta_kplus,w);

		 }
	       
	       if(TMath::Abs(prrap) < rapcut && TMath::Abs(tNspr_tof[tr]) < 2.0)
		 {
		   //Float_t msq_prpos = mom[tr]*mom[tr] * ( 1.0 / (Beta[tr]*Beta[tr]) - 1);
		   //hmsq_prpos->Fill(pt[tr],msq);

		   Float_t delta_prplus = Tof[tr] - tofp_exp;
		   hdeltaprplus->Fill(tof_prpt[tr], delta_prplus,w);
		   
		 }
	     }
		 
	   
	   if(charge[tr] < 0)
	     {
	       if(TMath::Abs(pirap) < rapcut && TMath::Abs(tNspi_tof[tr]) < 2.0)
		 {
		   //Float_t msq_pineg = mom[tr]*mom[tr] * ( 1.0 / (Beta[tr]*Beta[tr]) - 1);
		   //hmsq_pineg->Fill(pt[tr],msq);

		    Float_t delta_pineg = Tof[tr] - tofpi_exp;
		    hdeltapineg->Fill(tof_pt[tr], delta_pineg,w);

		 }
		   
	       if(TMath::Abs(karap) < rapcut && TMath::Abs(tNsk_tof[tr]) < 2.0)
		 {
		   //Float_t msq_kaneg = mom[tr]*mom[tr] * ( 1.0 / (Beta[tr]*Beta[tr]) - 1);
		   //hmsq_kaneg->Fill(pt[tr],msq);

		   Float_t delta_kneg = Tof[tr] - tofk_exp;
		   hdeltakneg->Fill(tof_kapt[tr], delta_kneg,w);
		   
		 }
	       
	       if(TMath::Abs(prrap) < rapcut && TMath::Abs(tNspr_tof[tr]) < 2.0)
		 {
		   //Float_t msq_prneg = mom[tr]*mom[tr] * ( 1.0 / (Beta[tr]*Beta[tr]) - 1);
		   //hmsq_prneg->Fill(pt[tr],msq);

		   Float_t delta_prneg = Tof[tr] - tofp_exp;
		   hdeltaprneg->Fill(tof_prpt[tr], delta_prneg,w);
		 }

	     } //Negative charge particle loop
	    
	       }//Beta 
	
	 }// loop over primary particles

      EvCount++;

   } // Event loop ends

   input->Close();

}//Loop closed




  //**********************************************************//
  //For getting predicted m^2 distribution
  //Again get the event loop, particle loop and the tof loop
  //2nd Event Loop starts from here
  //**********************************************************//
   void ReadPico::Loop2(const Char_t* filename2nd)
   {

     //cout <<"  I am in the Loop2, reading file  ==== " << filename2nd << endl;

     //cout<<" Input file for 2nd loop::: " << filename2nd << endl;

     //TFile* input2nd = TFile::Open(filename2nd);
     TFile* input2nd = new TFile(filename2nd,"r");

   //----- check if zombie() ------// 
   if(input2nd->IsZombie()) {
     cout << filename2nd <<" **** zombie *** skipping *****" << endl;
     return;
   }




   TTree* tree2nd = (TTree*) input2nd->Get("t1");
   
   if(!tree2nd)
     {
       //cout <<" ====> " << filename2nd;
       cout<<" ********* No tree found in Loop2 ------ skipping  ********* " << endl;
       //Finish();
       return;
     }

   Init(tree2nd);

  if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;

   //cout <<" Found total events in loop2 === " << nentries << endl;

  for (Long64_t jentry=0; jentry<nentries;jentry++) {


    //if(EvCount2nd > EventBreak) break;

    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    
    //if(jentry==1e5)break;
    //if(jentry==1e5)break;
    
    //if(jentry%500==0)
    //if(jentry%1000==0)
    //cout <<" Processing (in Loop2) Event Number # " << jentry << endl;

    Int_t RefMult1 = ToFRefmult;

    
    //*********  Event Cuts for 2nd time ************//
    //if(TMath::Abs(Vz) > vzcut) continue;
    if(RefMult1 <= 0) continue;                             //=========check
    //if(TMath::Abs(vrad1)>2.0) continue;
    
    //centrality 
    //0-5%   ::::  > 299
    //5-10%  ::::  247 - 299
    //10-20% ::::  167 - 247
    //20-30% ::::  112 - 167
    //30-40% ::::  72  - 112 
    //40-50% ::::  44  - 72
    //50-60% ::::  25  - 44
    //60-70% ::::  13  - 25
    //70-80% ::::  6  - 13
    //if(RefMult1 <= 167  ||  RefMult1 >  247) continue;  //39 GeV (5-10%)
    //if(RefMult1 <= RefLowCut  ||  RefMult1 >  RefHighCut) continue;  //39 GeV (5-10%)
    if(Centrality!=CentCut) continue;  //39 GeV (5-10%)
    //*********  Event Cuts for 2nd time ************//

    if(EvCount2nd > EventBreak) break;
    if(EvCount2nd%500==0)
      cout <<" Process Event No. (2nd loop) # " << EvCount2nd 
	 << " jentry = " << jentry
	 << " input = " << filename2nd
	 << " RefM = " << RefMult1 << endl; 
    


    Float_t P1[10000];
    for(Int_t i=0; i < 10000; i++)
      {
	P1[i]=0.;
      }


    //Float_t pirap1 =0.0;
    //Float_t tof_rka1 =0.0;
    //Float_t prrap1 =0.0;

    TLorentzVector vpi1, vka1, vpr1;

    //2nd Loop over primary tracks
    for(Int_t tr1 = 0; tr1 < nTrack; tr1++)
      { 
	
	//------ Energy Loss correction for 2nd loop ------//
	//pt[tr1] = pt[tr1] - a0 - a1*TMath::Power(1 + (a2/(pt[tr1]*pt[tr1])),a3);
	//-------------------------------------------------//


	Float_t pirap1 = tof_rpi[tr1];
	Float_t karap1 = tof_rka[tr1];
	Float_t prrap1 = tof_rpr[tr1];

	if(dca[tr1] > dcacut)         continue;
	if(nHitsFit[tr1] > nHitscut)  continue;
	if(nHitsdEdx[tr1] > ndEdxcut) continue;
	if(tof_pt[tr1] <= lowpTcut)   continue;
	if(tof_kapt[tr1] <= lowpTcut) continue;
	if(tof_prpt[tr1] <= lowpTcut) continue;

	//------ To get rapidities for pi, k and P --------//

	//P1[tr1] = TMath::Sqrt(pt[tr1]*pt[tr1] + Pz[tr1]*Pz[tr1]);
	//Float_t fit_ratio1 = (float) NFit[tr1]/NFitPos[tr1];

	P1[tr1] = mom[tr1];



	//----  2nd TOF Loop ------------//
	//if(Beta[tr1]!=-999  && Match[tr1]>0)
	  {

	    //----- Calculating tof (exp) 2nd time
	    Float_t tofpi_exp1 = TMath::Sqrt((Pathlength[tr1]*Pathlength[tr1])/(c_light*c_light)*(1+(pimass*pimass)/(P1[tr1]*P1[tr1])));
	    Float_t tofka_exp1 = TMath::Sqrt((Pathlength[tr1]*Pathlength[tr1])/(c_light*c_light)*(1+(kmass*kmass)/(P1[tr1]*P1[tr1])));
	    Float_t tofpr_exp1 = TMath::Sqrt((Pathlength[tr1]*Pathlength[tr1])/(c_light*c_light)*(1+(pmass*pmass)/(P1[tr1]*P1[tr1])));
	    

	    //------ Check mass^2 and delta T distributions ------//
	    Float_t msq1 = P1[tr1]*P1[tr1] * ( 1.0 / (Beta[tr1]*Beta[tr1]) - 1);
	    if(TMath::Abs(pirap1) < rapcut || TMath::Abs(karap1) < rapcut || TMath::Abs(prrap1) < rapcut)
	      {
		if(charge[tr1] > 0) hmsq_all_pos_check->Fill(tof_pt[tr1],msq1,w);
	      }

	    
	    if(TMath::Abs(pirap1) < rapcut && TMath::Abs(tNspi_tof[tr1]) < 2.0)
	      {
		if(charge[tr1] > 0) 
		  {
		    Float_t delta_piplus1 = Tof[tr1] - tofpi_exp1;
		    hdeltapiplus_check->Fill(tof_pt[tr1], delta_piplus1,w);
		  }
	      }

	    //------ Check mass^2 and delta T distributions ------//
	    



	    //---- Predict m^2 calculation for positively charged particels ------ //
	    if(charge[tr1] > 0)
	      {
		
		//-- Get predicted m^2 for Piplus for all pt bins --//
		if(TMath::Abs(pirap1) < rapcut && TMath::Abs(tNspi_tof[tr1]) < 2.0)
		  {
		    for(Int_t i = 4; i<30; i++)
		    {
		    	hdpipos_names[i] = "hdpipos" + to_string(i+1) + to_string(i+1);
		    	hdpipos[i] = hdeltapiplus->ProjectionY(hdpipos_names[i],i+1,i+1);
		    	if(tof_pt[tr1] >= (i/10) && tof_pt[tr1] < ((i+1)/10) )
		    	  {
					ran_pipos[i] = hdpipos[i]->GetRandom();
					tofpiplus_new[i] = tofpi_exp1 + ran_pipos[i];
					pred_msqpipos[i] = P1[tr1]*P1[tr1] * ( ((tofpiplus_new[i]*tofpiplus_new[i]*c_light*c_light)/(Pathlength[tr1]*Pathlength[tr1])) - 1);
					hpred_msqpipos[i]->Fill(pred_msqpipos[i]);
		    	  }
		    }
		  

		    //------ use random number of 0.7-0.8 for all higher pT bins for pions -----//
		    //if(pt[tr1] >= 0.7 && pt[tr1] < 0.8)
		      {
			double ran_pipos_highpt = hdpipos[7]->GetRandom(); //ran_pipos88
			Float_t tofpiplus_newhighpt = tofpi_exp1 + ran_pipos_highpt;
			Float_t pred_msqpipos_highpt = P1[tr1]*P1[tr1] * ( ((tofpiplus_newhighpt*tofpiplus_newhighpt*c_light*c_light)/(Pathlength[tr1]*Pathlength[tr1])) - 1);		    
			hpred_msqpipos_highpt->Fill(tof_pt[tr1],pred_msqpipos_highpt);
		      }
		    //--------------------------------------------------------------------------//


		    
		  } // All piplus ends
		
		// ----------- pion plus ends ------------- //
		


		//-- Get predicted m^2 for Kplus for all pt bins --//
		if(TMath::Abs(karap1) < rapcut && TMath::Abs(tNsk_tof[tr1]) < 2.0)
		  {
		    for(Int_t i = 4; i<30; i++)
		    {
		    	hdkapos_names[i] = "hdkapos" + to_string(i+1) + to_string(i+1);
		    	hdkapos[i] = hdeltakaplus->ProjectionY(hdkapos_names[i],i+1,i+1);
		    	if(tof_kapt[tr1] >= (i/10) && tof_kapt[tr1] < ((i+1)/10) )
		    	  {
					ran_kapos[i] = hdkapos[i]->GetRandom();
					tofkaplus_new[i] = tofka_exp1 + ran_kapos[i];
					pred_msqkapos[i] = P1[tr1]*P1[tr1] * ( ((tofkaplus_new[i]*tofkaplus_new[i]*c_light*c_light)/(Pathlength[tr1]*Pathlength[tr1])) - 1);
					hpred_msqkapos[i]->Fill(pred_msqkapos[i]);
		    	  }
		    }		  

		    //------ use random number of 0.6-0.7 for all higher pT bins for kaons -----//
		    //if(tof_pt[tr1] >= 0.6 && tof_pt[tr1] < 0.7)
		      {
			double ran_kapos_highpt = hdkapos[6]->GetRandom(); //ran_kapos77 
			Float_t tofkaplus_newhighpt = tofka_exp1 + ran_kapos_highpt;
			Float_t pred_msqkapos_highpt = P1[tr1]*P1[tr1] * ( ((tofkaplus_newhighpt*tofkaplus_newhighpt*c_light*c_light)/(Pathlength[tr1]*Pathlength[tr1])) - 1);		    
			hpred_msqkapos_highpt->Fill(tof_kapt[tr1],pred_msqkapos_highpt);
		      }
		    //--------------------------------------------------------------------------//




		    
		  } // kplus rapidity and nsigma cut

		// ----------- kplus ends ------------- //
		





		//-- Get predicted m^2 for proton plus for all pt bins --//
		if(TMath::Abs(prrap1) < rapcut && TMath::Abs(tNspr_tof[tr1]) < 2.0)
		  {
		  
		    for(Int_t i = 4; i<30; i++)
		    {
		    	hdprpos_names[i] = "hdprpos" + to_string(i+1) + to_string(i+1);
		    	hdprpos[i] = hdeltaprplus->ProjectionY(hdprpos_names[i],i+1,i+1);
		    	if(tof_prpt[tr1] >= (i/10) && tof_prpt[tr1] < ((i+1)/10) )
		    	  {
					ran_prpos[i] = hdprpos[i]->GetRandom();
					tofprplus_new[i] = tofpr_exp1 + ran_prpos[i];
					pred_msqprpos[i] = P1[tr1]*P1[tr1] * ( ((tofprplus_new[i]*tofprplus_new[i]*c_light*c_light)/(Pathlength[tr1]*Pathlength[tr1])) - 1);
					hpred_msqprpos[i]->Fill(pred_msqprpos[i]);
		    	  }
		    }			  
		  

		    
		    //------ use random number of 1.2-1.3 for all higher pT bins for protons -----//
		    //if(pt[tr1] >= 1.2 && pt[tr1] < 1.3)
		      {
			double ran_prpos_highpt = hdprpos[12]->GetRandom(); //ran_prpos1313
			Float_t tofprplus_newhighpt = tofpr_exp1 + ran_prpos_highpt;
			Float_t pred_msqprpos_highpt = P1[tr1]*P1[tr1] * ( ((tofprplus_newhighpt*tofprplus_newhighpt*c_light*c_light)/(Pathlength[tr1]*Pathlength[tr1])) - 1);		    
			hpred_msqprpos_highpt->Fill(tof_prpt[tr1],pred_msqprpos_highpt);
		      }
		    //--------------------------------------------------------------------------//

		    
		  } //Prrap < 0.5 and |Nsigma| < 2
		

	      }//Positively charged particles end







	    //Negative charge particles
	    //---- Predict m^2 for piplus in bin 5 to 5 -----//
	    if(charge[tr1] < 0)
	      {

		//-- Get predicted m^2 for Pineg for all pt bins --//
		if(TMath::Abs(pirap1) < rapcut && TMath::Abs(tNspi_tof[tr1]) < 2.0)
		  {
		  
		  
		    for(Int_t i = 4; i<30; i++)
		    {
		    	hdpineg_names[i] = "hdpineg" + to_string(i+1) + to_string(i+1);
		    	hdpineg[i] = hdeltapineg->ProjectionY(hdpineg_names[i],i+1,i+1);
		    	if(tof_pt[tr1] >= (i/10) && tof_pt[tr1] < ((i+1)/10) )
		    	  {
					ran_pineg[i] = hdpineg[i]->GetRandom();
					tofpineg_new[i] = tofpi_exp1 + ran_pineg[i];
					pred_msqpineg[i] = P1[tr1]*P1[tr1] * ( ((tofpineg_new[i]*tofpineg_new[i]*c_light*c_light)/(Pathlength[tr1]*Pathlength[tr1])) - 1);
					hpred_msqpineg[i]->Fill(pred_msqpineg[i]);
		    	  }
		    }	


		    //------ use random number of 0.7-0.8 for all higher pT bins for pions -----//
		    //if(pt[tr1] >= 0.7 && pt[tr1] < 0.8)
		      {
			double ran_pineg_highpt = hdpineg[7]->GetRandom(); //ran_pineg88
			Float_t tofpineg_newhighpt = tofpi_exp1 + ran_pineg_highpt;
			Float_t pred_msqpineg_highpt = P1[tr1]*P1[tr1] * ( ((tofpineg_newhighpt*tofpineg_newhighpt*c_light*c_light)/(Pathlength[tr1]*Pathlength[tr1])) - 1);		    
			hpred_msqpineg_highpt->Fill(tof_pt[tr1],pred_msqpineg_highpt);
		      }
		    //--------------------------------------------------------------------------//
		    
		    
		  } // All pineg ends
		
		// ----------- pion neg ends ------------- //
		


		
		//-- Get predicted m^2 for Kneg for all pt bins --//
		if(TMath::Abs(karap1) < rapcut && TMath::Abs(tNsk_tof[tr1]) < 2.0)
		  {
		  
		  
		  
		  
		    for(Int_t i = 4; i<30; i++)
		    {
		    	hdkaneg_names[i] = "hdkaneg" + to_string(i+1) + to_string(i+1);
		    	if (i<8)
		    	hdkaneg[i] = hdeltakaneg->ProjectionY(hdkaneg_names[i],i+1,i+1);
		    	else hdkaneg[i] = hdeltakaneg->ProjectionY(hdkaneg_names[i],7,7);
		    	if(tof_ptka[tr1] >= (i/10) && tof_ptka[tr1] < ((i+1)/10) )
		    	  {
					ran_kaneg[i] = hdkaneg[i]->GetRandom();
					tofkaneg_new[i] = tofka_exp1 + ran_kaneg[i];
					pred_msqkaneg[i] = P1[tr1]*P1[tr1] * ( ((tofkaneg_new[i]*tofkaneg_new[i]*c_light*c_light)/(Pathlength[tr1]*Pathlength[tr1])) - 1);
					hpred_msqkaneg[i]->Fill(pred_msqkaneg[i]);
		    	  }
		    }
		    

		    
		    //------ use random number of 0.6-0.7 for all higher pT bins for kaons -----//
		    //if(pt[tr1] >= 0.6 && pt[tr1] < 0.7)
		      {
			double ran_kaneg_highpt = hdkaneg[6]->GetRandom(); //ran_kaneg77 			
			Float_t tofkaneg_newhighpt = tofka_exp1 + ran_kaneg_highpt;
			Float_t pred_msqkaneg_highpt = P1[tr1]*P1[tr1] * ( ((tofkaneg_newhighpt*tofkaneg_newhighpt*c_light*c_light)/(Pathlength[tr1]*Pathlength[tr1])) - 1);		    
			hpred_msqkaneg_highpt->Fill(tof_kapt[tr1],pred_msqkaneg_highpt);
		      }
		    //--------------------------------------------------------------------------//


		  } // kneg rapidity and nsigma cut

		// ----------- kneg ends ------------- //
		




		//-- Get predicted m^2 for proton neg for all pt bins --//
		if(TMath::Abs(prrap1) < rapcut && TMath::Abs(tNspr_tof[tr1]) < 2.0)
		  {
		  
		    for(Int_t i = 4; i<30; i++)
		    {
		    	hdprneg_names[i] = "hdprneg" + to_string(i+1) + to_string(i+1);
		    	if (i<14)
		    	hdprneg[i] = hdeltaprneg->ProjectionY(hdprneg_names[i],i+1,i+1);
		    	else hdprneg[i] = hdeltaprneg->ProjectionY(hdprneg_names[i],13,13);
		    	if(tof_ptpr[tr1] >= (i/10) && tof_ptpr[tr1] < ((i+1)/10) )
		    	  {
					ran_prneg[i] = hdprneg[i]->GetRandom();
					tofprneg_new[i] = tofpr_exp1 + ran_prneg[i];
					pred_msqprneg[i] = P1[tr1]*P1[tr1] * ( ((tofprneg_new[i]*tofprneg_new[i]*c_light*c_light)/(Pathlength[tr1]*Pathlength[tr1])) - 1);
					hpred_msqprneg[i]->Fill(pred_msqprneg[i]);
		    	  }
		    }		  

		    

		    //------ use random number of 1.2-1.3 for all higher pT bins for protons -----//
		    //if(pt[tr1] >= 1.2 && pt[tr1] < 1.3)
		      {
			double ran_prneg_highpt = hdprneg[12]->GetRandom(); //ran_prneg1313			
			Float_t tofprneg_newhighpt = tofpr_exp1 + ran_prneg_highpt;
			Float_t pred_msqprneg_highpt = P1[tr1]*P1[tr1] * ( ((tofprneg_newhighpt*tofprneg_newhighpt*c_light*c_light)/(Pathlength[tr1]*Pathlength[tr1])) - 1);		    
			hpred_msqprneg_highpt->Fill(tof_prpt[tr1],pred_msqprneg_highpt);
		      }
		    //--------------------------------------------------------------------------//



		    
		    // ----------- proton neg ends ------------- //

		    
		  } //Prrap < 0.5 and |Nsigma| < 2
		
	      }//Negatively charged particles
	    
	  } // 2nd tof loop ends
	
      }//2nd loop over primary track ends
    

    EvCount2nd++;



  } //2nd event loop ends
  
  
   
  input2nd->Close();
} //Loop function

void ReadPico::Finish()
{
  fpw->Write();
  fpion->Write(); 
  fkaon->Write(); 
  fproton->Write(); 

}


ReadPico::ReadPico(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
     /*TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("day_nfitdedx_10k.root");
      if (!f) {
         f = new TFile("day_nfitdedx_10k.root");
      }
      tree = (TTree*)gDirectory->Get("t1");*/



   }
//   Init(tree);
}

ReadPico::~ReadPico()
{
  //No need if you are not using TChain
  //This try to delete the memory of the input files (which we already did for this macro)
  //if (!fChain) return;
  //delete fChain->GetCurrentFile();
}

Int_t ReadPico::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t ReadPico::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void ReadPico::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
  //if (!tree) {cout <<" I am returing from init == " << endl; return;}
  if (!tree) return; //lokesh
  fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   //fChain->SetBranchAddress("Vz", &Vz, &b_Vz);
   fChain->SetBranchAddress("nTrack", &nTrack, &b_nTrack);
   fChain->SetBranchAddress("ToFRefmult", &ToFRefmult, &b_ToFRefmult);
   fChain->SetBranchAddress("Centrality", &Centrality, &b_Centrality);
   fChain->SetBranchAddress("w", &w, &b_w);
   fChain->SetBranchAddress("dca", dca, &b_dca);
   fChain->SetBranchAddress("nHitsFit", nHitsFit, &b_nHitsFit);
   fChain->SetBranchAddress("nHitsdEdx", nHitsdEdx, &b_nHitsdEdx);
   fChain->SetBranchAddress("tof_pt", tof_pt, &b_tof_pt);
   fChain->SetBranchAddress("tof_kapt", tof_kapt, &b_tof_kapt);
   fChain->SetBranchAddress("tof_prpt", tof_prpt, &b_tof_prpt);
   fChain->SetBranchAddress("mom", mom, &b_mom);
   fChain->SetBranchAddress("tNspi_tof", tNspi_tof, &b_tNspi_tof);
   fChain->SetBranchAddress("tNsk_tof", tNsk_tof, &b_tNsk_tof);
   fChain->SetBranchAddress("tNspr_tof", tNspr_tof, &b_tNspr_tof);
   fChain->SetBranchAddress("charge", charge, &b_charge);
   fChain->SetBranchAddress("Tof", Tof, &b_Tof);
   fChain->SetBranchAddress("Pathlength", Pathlength, &b_Pathlength);
   fChain->SetBranchAddress("tof_rpi", tof_rpi, &b_tof_rpi);
   fChain->SetBranchAddress("tof_rka", tof_rka, &b_tof_rka);
   fChain->SetBranchAddress("tof_rpr", tof_rpr, &b_tof_rpr);
   fChain->SetBranchAddress("Beta", Beta, &b_Beta);

   Notify();
}

Bool_t ReadPico::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void ReadPico::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t ReadPico::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

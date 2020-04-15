#define FillHistPt_cxx
#include "FillHistPt.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void FillHistPt::Loop()
{

  // March 23, 2011: Added "hptnchmatchnodedx" histogram
  // to be filled before we put ratio>0.52 and dedxpt > 15 cuts
  



//   In a ROOT session, you can do:
//      Root > .L FillHistPt.C
//      Root > FillHistPt t
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
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;

   fpw = new TFile("EffCalHist.root","RECREATE");

   //------------tree------------------------//

   //Tree variables
   Float_t tvzmctrk,tvzrec;
   Int_t tmult;
   Int_t tcountmctrk,tcountmatch, tcountghost;

   const int maxtrack = 30000;
   Float_t tptmctrk[maxtrack],tptmcmatch[maxtrack],tptprmatch[maxtrack];
   Float_t trapmctrk[maxtrack],trapptmcmatch[maxtrack],trapptprmatch[maxtrack];
   Int_t tgeantidmctrk[maxtrack],tgeantidmatch[maxtrack];
   Float_t tdcamatch[maxtrack];
   Int_t tnfitmatch[maxtrack],tcommonhitmatch[maxtrack];
   Int_t tnfitposs[maxtrack], tdedxpts[maxtrack];
   //ghost 
   Float_t tptghost[maxtrack], tdcaghost[maxtrack];
   Int_t tnfitghost[maxtrack];

   //Branches
   minitree = new TTree("minitree","Variables");
   
   //Event variable has " & "
   minitree->Branch("tvzmctrk",&tvzmctrk,"tvzmctrk/F");
   minitree->Branch("tvzrec",&tvzrec,"tvzrec/F");
   minitree->Branch("tmult",&tmult,"tmult/I");

   // Event variable
   minitree->Branch("tcountmctrk",&tcountmctrk,"tcountmctrk/I");

   // Particle variable have  NO " & " but arrary at last ONLY 
   minitree->Branch("tptmctrk", tptmctrk,"tptmctrk[tcountmctrk]/F");
   minitree->Branch("trapmctrk",trapmctrk,"trapmctrk[tcountmctrk]/F");
   minitree->Branch("tgeantidmctrk",tgeantidmctrk,"tgeantidmctrk[tcountmctrk]/I");

   // Event variable  
   minitree->Branch("tcountmatch",&tcountmatch,"tcountmatch/I");

   // Particle variable
   minitree->Branch("tptmcmatch",tptmcmatch,"tptmcmatch[tcountmatch]/F");
   minitree->Branch("tptprmatch",tptprmatch,"tptprmatch[tcountmatch]/F");
   minitree->Branch("tdcamatch",tdcamatch,"tdcamatch[tcountmatch]/F");
   minitree->Branch("tnfitmatch",tnfitmatch,"tnfitmatch[tcountmatch]/I");
   minitree->Branch("tcommonhitmatch",tcommonhitmatch,"tcommonhitmatch[tcountmatch]/I");
   minitree->Branch("trapptmcmatch",trapptmcmatch,"trapptmcmatch[tcountmatch]/F");
   minitree->Branch("trapptprmatch",trapptprmatch,"trapptprmatch[tcountmatch]/F");
   minitree->Branch("tgeantidmatch",tgeantidmatch,"tgeantidmatch[tcountmatch]/I");

   // Event variable (ghost)
   minitree->Branch("tcountghost",&tcountghost,"tcountghost/I");

   // Particle variable (ghost)
   minitree->Branch("tptghost",tptghost,"tptghost[tcountghost]/F");
   minitree->Branch("tdcaghost",tdcaghost,"tdcaghost[tcountghost]/F");
   minitree->Branch("tnfitghost",tnfitghost,"tnfitghost[tcountghost]/I");


   //Vertex-z 
   TH1F *hrefmult = new TH1F("hrefmult","RefMult " , 800, 0., 800.);
   TH2F *hvxvy = new TH2F("hvxvy","Vr Distribution " , 200, -2. , 2. , 200., -2. , 2.);
   TH1F *hvzmctrk = new TH1F("hvzmctrk","Vz MC track " ,200, -100., 100.);
   TH1F *hvzmatch = new TH1F("hvzmatch","Vz Matched Track " ,200, -100., 100.);
   TH1F *hvzdiff = new TH1F("hvzdiff","Vz Difference " ,1000, -500., 500.);
   TH2F *hvz2d = new TH2F("hvz2d","Vz MC vs. Matched Track " ,200, -100., 100.,200, -100., 100.);
   
   //Histograms for Mc track
   TH1F *hptmctrk = new TH1F("hptmctrk","pT MC Track" ,200, 0., 20.);
   TH2D *hptymctrk = new TH2D("hptymctrk"," Rapidity vs pT MC tracks ",100, -0.8, 0.8, 200, 0, 20); //pT vs y for mc tracks
   TH1F *hptmctrksum = new TH1F("hptmctrksum","pT MC Track" ,200, 0., 20.);
   TH1F *hmommctrk = new TH1F("hmommctrk","p MC Track" ,200, 0., 20.);
   //TH2D *hptnchmctrk = new TH2D("hptnchmctrk","Nch Vs. pT for MC Track", 800, 0, 800, 200, 0., 20.);
   TH2D *hptnchmctrk = new TH2D("hptnchmctrk","Nch Vs. pT for MC Track", 800, 0, 800, 200, 0., 10.);
   TH2D *hptnchmctrk_check = new TH2D("hptnchmctrk_check","Nch Vs. pT for MC Track", 800, 0, 800, 500, 0., 50.);
   TH1F *hphimctrk = new TH1F("hphimctrk", "Phi Mc Track", 100, -4., 4.);
   TH1F *hetamctrk = new TH1F("hetamctrk"," Eta Mc Track ", 100, -3.0, 3.0);
   TH1F *hrapmctrk = new TH1F("hrapmctrk","Rapidity MC Track", 100, -0.8, 0.8);

   //Histograms for Matched track
   TH1F *hptmatch = new TH1F("hptmatch", " pT Match", 200, 0., 20.);
   TH1F *hmommatch = new TH1F("hmommatch", " p Match", 200, 0., 20.);
   //TH2D *hptnchmatch = new TH2D("hptnchmatch","Nch Vs. pT for match track",  800, 0, 800, 200, 0., 20.);
   TH2D *hptnchmatch = new TH2D("hptnchmatch","Nch Vs. pT for match track",  800, 0, 800, 200, 0., 10.);
   TH1F *hdcamatch = new TH1F("hdcamatch"," DCA Match ", 100, 0., 10.);
   //TH2F *hdcaptmatch = new TH2F("hdcaptmatch"," DCA Match ", 100, 0.0, 10.0, 100, 0., 10.);
   TH2F *hdcaptmatch = new TH2F("hdcaptmatch"," DCA Match ", 200, 0.0, 10.0, 100, 0., 10.);
   TH1F *hnfitmatch =  new TH1F("hnfitmatch"," nFit Poits", 50, 0, 50);
   //TH2F *hnfitptmatch =  new TH2F("hnfitptmatch"," nFit Poits", 100, 0.0, 10.0, 50, 0, 50);
   TH2F *hnfitptmatch =  new TH2F("hnfitptmatch"," nFit Poits", 200, 0.0, 10.0, 50, 0, 50);
   TH1F *hrapmatch = new TH1F("hrapmatch"," Rapidity Match ",100, -0.8, 0.8);
   TH2F *hptdiff = new TH2F("hptdiff"," pT Diff. Vs. pT Rec. ", 20, 0., 2.5., 80, -2., 2.);
   TH1F *hptdiff1D = new TH1F("hptdiff1D"," pT Diff. Vs. pT Rec. ", 20, 0., 2.5);

   TH2F *hpadrowphi = new TH2F("hpadrowphi","Padrow Vs. phi " ,100, -4., 4.,500, 0, 50);

   //Other plots for Match
   TH1F *hphimatch = new TH1F("hphimatch", "Phi Match Track", 100, -4., 4.);
   TH1F *hetamatch = new TH1F("hetamatch"," Eta Match ", 100, -3.0, 3.0);
   TH1F *hphimatch1 = new TH1F("hphimatch1", "Phi Match1 Track", 100, -4., 4.);
   TH1F *hetamatch1 = new TH1F("hetamatch1"," Eta Match ", 100, -3.0, 3.0);
   //TH2F *hdedxmatch = new TH2F("hdedxmatch"," dedx  matched ", 100, 0., 10., 500, 0., .0001);
   //TH2F *hdedxmatch = new TH2F("hdedxmatch"," dedx  matched ", 500, 0., 10., 500, 0., .0001); //p bin width = 0.002
   //TH2F *hdedxmatch = new TH2F("hdedxmatch"," dedx  matched ", 500, 0., 10., 1000, 0.0000001, 0.00014); //same as in data
   TH2F *hdedxmatch = new TH2F("hdedxmatch"," dedx  matched ", 200, 0., 10., 1000, 0.0000001, 0.00014); //same as in data
   

   //Histogram for nHitsDedx
   TH1D *hndedxhits_match = new TH1D("hndedxhits_match","",50,0,50);
   TH2D *hptndedxhits_match = new TH2D("hptndedxhits_match","",200, 0., 10., 50, 0, 50);

   //Matched and Reconstructed tracks
   TH1F *hptmatch1 = new TH1F("hptmatch1", " pT Match", 200, 0., 20.);
   TH2D *hptymatch1 = new TH2D("hptymatch1"," Rapidity vs pT Match ",100, -0.8, 0.8, 200, 0, 20); //pT vs y for matched track
   TH1F *hptmatch1sum = new TH1F("hptmatch1sum", " pT Match", 200, 0., 20.);
   TH1F *hmommatch1 = new TH1F("hmommatch1", " p Match", 200, 0., 20.);
   //TH2D *hptnchmatch1 = new TH2D("hptnchmatch1","Nch Vs. pT for match track",  800, 0, 800, 200, 0., 20.);
   TH2D *hptnchmatch1 = new TH2D("hptnchmatch1","Nch Vs. pT for match track",  800, 0, 800, 200, 0., 10.); // For efficiency
   TH2D *hptnchmatchnodedx = new TH2D("hptnchmatchnodedx","Nch Vs. pT for match track (no dedx cuts)",  800, 0, 800, 200, 0., 10.); // For efficiency
   TH1F *hrapmatch1 = new TH1F("hrapmatch1"," Rapidity Match ",100, -0.8, 0.8);
   //TH2F *hptdiff1 = new TH2F("hptdiff1"," pT Diff. Vs. pT Rec. ", 20, 0., 2.5., 80, -2., 2.);  //Used for eloss
   //TH2F *hptdiff1 = new TH2F("hptdiff1"," pT Diff. Vs. pT Rec. ", 100, 0., 10, 80, -2., 2.);  //Used for eloss
   TProfile *hptdiff1 = new TProfile("hptdiff1"," pT Diff. Vs. pT Rec. ", 200, 0., 10, -2., 2.);  //Used for eloss
   TH1F *hptdiff1D1 = new TH1F("hptdiff1D1"," pT Diff. Vs. pT Rec. ", 20, 0., 2.5);
   TH2F *hptdiff1sum = new TH2F("hptdiff1sum"," pT Diff. Vs. pT Rec. ", 20, 0., 2.5., 80, -2., 2.);
   TH1F *hptdiff1D1sum = new TH1F("hptdiff1D1sum"," pT Diff. Vs. pT Rec. ", 20, 0., 2.5);

   // Ghost Tracks
   TH1F *hptghost = new TH1F("hptghost", " pT Ghost", 200, 0., 20.);
   TH1F *hdcaghost = new TH1F("hdcaghost"," DCA Match ", 100, 0., 10.);
   TH1F *hnfitghost =  new TH1F("hnfitghost"," nFit Poits", 50, 0, 50);
   //TH2F *hdedxghost = new TH2F("hdedxghost"," dedx  Ghost ", 100, 0., 10.,  500, 0., .0001);
   TH2F *hdedxghost = new TH2F("hdedxghost"," dedx  Ghost ", 500, 0., 10.,  500, 0., .0001);

   //dEdx Global
   //TH2F *hdedxglob = new TH2F("hdedxglob"," dedx  globed ", 100, 0., 10., 500, 0., .0001);
   TH2F *hdedxglob = new TH2F("hdedxglob"," dedx  globed ", 500, 0., 10., 500, 0., .0001);
   
   //2D Histograms for check for MC tracks
   //TH2F *hptphi = new TH2F("hptphi"," pT Vs. Phi" , 200, 0., 20., 800, -4., 4.);
   TH2F *hptphi = new TH2F("hptphi"," pT Vs. Phi" , 200, 0., 10., 100, -4., 4.);
   TH2F *hpteta = new TH2F("hpteta"," pT Vs. Eta" , 200, 0., 10., 600, -3., 3.);
   //TH2F *hetaphi = new TH2F("hetaphi"," Eta Vs. Phi" , 100, -3., 3., 800, -4., 4.);
   TH2F *hetaphi = new TH2F("hetaphi"," Eta Vs. Phi" , 100, -3., 3., 100, -4., 4.);
   TH2F *hpty = new TH2F("hpty"," pT Vs. Y" , 200, 0., 10., 600, -3., 3.);
   TH2F *hptym1 = new TH2F("hptym1"," pT Vs. Y" , 200, 0., 10., 600, -3., 3.); //matched
   //TH2F *hyphi = new TH2F("hyphi"," Y Vs. Phi" , 100, -3., 3., 800, -4., 4.);
   TH2F *hyphi = new TH2F("hyphi"," Y Vs. Phi" , 100, -3., 3., 100, -4., 4.);


   //---- pT vs phi for match tracks ----//
   //TH2F *hptphimatch = new TH2F("hptphimatch"," pT Vs. Phi Matched" , 200, 0., 10., 800, -4., 4.);
   TH2F *hptphimatch = new TH2F("hptphimatch"," pT(Mc) Vs. Phi(Mc) Matched " , 200, 0., 10., 100, -4., 4.); //For PtPr Vs PhiMc
   TH2F *hptphimatch_etapos = new TH2F("hptphimatch_etapos"," pT(Mc) Vs. Phi(Mc) Matched (#eta>0)" , 200, 0., 10., 100, -4., 4.); //For PtPr Vs PhiMc
   TH2F *hptphimatch_etaneg = new TH2F("hptphimatch_etaneg"," pT(Mc) Vs. Phi(Mc) Matched (#eta<0)" , 200, 0., 10., 100, -4., 4.); //For PtPr Vs PhiMc
   TH2F *hptphimatch1 = new TH2F("hptphimatch1"," pT(Mc) Vs. Phi(Mc) Matched (No dedxpt cut)" , 200, 0., 10., 100, -4., 4.); //For PtMc Vs PhiMc
   TH2F *hptprphimcmatch = new TH2F("hptprphimcmatch"," pT(Rc) Vs. Phi(Mc) Matched" , 200, 0., 10., 100, -4., 4.); //For PtMc Vs PhiRc
   TH2F *hptmcphiprmatch = new TH2F("hptmcphiprmatch"," pT(Mc) Vs. Phi(Rc) Matched" , 200, 0., 10., 100, -4., 4.); //For PtMc Vs PhiRc

   //------- Various cuts and constants used for the analysis ----------//
   Float_t vzcut = 30.0;
   Float_t vradcut = 1.0;
   Int_t RefLowCut = 4;
   Int_t RefHighCut = 1000;
   //------ masses -------//
   Float_t Mpion = 0.1395700;
   Float_t Mkaon = 0.493677;
   Float_t Mproton = 0.93827231;
   Float_t Melectron = 0.0005109990;
   //---------------------//
   Float_t mp = Mkaon; //change mass of particle here
   //----- Geant ID ------//
   //piplus = 8,  piminus = 9
   //kplus  = 11, kminus = 12
   //proton = 14, pbar = 15
   //--------------------//
   Int_t Gid = 12; //change Gid here
   Int_t ch = -1; //change charge here
   Float_t LowPtCut = 0.15;
   //Float_t rapcut = 0.1;
   Float_t rapcut = 0.1;
   Float_t dcacut = 3.0;
   Int_t Nfit = 25;
   Float_t NfRatio = 0.52;
   //-------------------------------------------------------------------//

   cout <<" PRINT ALL CUTS ############ " << endl;
   cout <<"|Vz|< " << vzcut <<" RefLowCut: " << RefLowCut
	<<" Partcle mass: " << mp <<" Gid: " << Gid
	<<" Charge: " << ch <<" LowpTcut: " << LowPtCut
	<<" |y|< " << rapcut <<" Nfit > " << Nfit <<" dca < " << dcacut
	<< endl;

     
   Int_t gevt = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) 
     {
       Long64_t ientry = LoadTree(jentry);
       if (ientry < 0) break;
       nb = fChain->GetEntry(jentry);   nbytes += nb;
       //if (Cut(ientry) < 0) continue;
       //cout << " kMaxmMatchedPairs " << mMatchedPairs_ << " kMaxmGhostPairs " << mGhostPairs_ << endl;
       
       //if(jentry == 5000) break;

       if(jentry%1000 ==0) 
	 cout << " Processing event # " << jentry << endl; 

       //*********** Bad runnumber rejection********************//

       Int_t badrun[253] = {15046073, 15046089, 15046094, 15046096, 15046102, 15046103, 15046104, 15046105, 15046106, 15046107, 15046108, 15046109, 15046110, 15046111, 15047004, 15047015, 15047016, 15047019, 15047021, 15047023, 15047024, 15047026, 15047027, 15047028, 15047029, 15047030, 15047039, 15047040, 15047041, 15047044, 15047047, 15047050, 15047052, 15047053, 15047056, 15047057, 15047061, 15047062, 15047063, 15047064, 15047065, 15047068, 15047069, 15047070, 15047071, 15047072, 15047074, 15047075, 15047082, 15047085, 15047086, 15047087, 15047093, 15047096, 15047097, 15047098, 15047100, 15047102, 15047104, 15047106, 15048003, 15048004, 15048012, 15048013, 15048014, 15048016, 15048017, 15048018, 15048019, 15048020, 15048021, 15048023, 15048024, 15048025, 15048026, 15048028, 15048029, 15048030, 15048031, 15048033, 15048034, 15048074, 15048075, 15048076, 15048077, 15048078, 15048079, 15048080, 15048081, 15048082, 15048083, 15048084, 15048085, 15048086, 15048087, 15048088, 15048089, 15048091, 15048092, 15048093, 15048094, 15048095, 15048096, 15048097, 15048098, 15049002, 15049003, 15049009, 15049013, 15049014, 15049015, 15049016, 15049017, 15049018, 15049019, 15049020, 15049021, 15049022, 15049023, 15049025, 15049026, 15049027, 15049028, 15049030, 15049031, 15049032, 15049033, 15049037, 15049038, 15049039, 15049040, 15049041, 15049074, 15049077, 15049083, 15049084, 15049085, 15049086, 15049087, 15049088, 15049089, 15049090, 15049091, 15049092, 15049093, 15049094, 15049096, 15049097, 15049098, 15049099, 15050001, 15050002, 15050003, 15050004, 15050005, 15050006, 15050010, 15050011, 15050012, 15050013, 15050014, 15050015, 15050016, 15051131, 15051132, 15051133, 15051134, 15051137, 15051141, 15051144, 15051146, 15051147, 15051148, 15051149, 15051156, 15051157, 15051159, 15051160, 15052001, 15052004, 15052005, 15052006, 15052007, 15052008, 15052009, 15052010, 15052011, 15052014, 15052015, 15052016, 15052017, 15052018, 15052019, 15052020, 15052021, 15052022, 15052023, 15052024, 15052025, 15052026, 15052040, 15052041, 15052042, 15052043, 15052060, 15052061, 15052062, 15052063, 15052064, 15052065, 15052066, 15052067, 15052068, 15052069, 15052070, 15052073, 15052074, 15052075, 15053027, 15053028, 15053029, 15053034, 15053035, 15053052, 15053054, 15053055, 15054053, 15054054, 15055018, 15055137, 15056117, 15057055, 15057059, 15058006, 15058011, 15058021, 15059057, 15059058, 15061001, 15061009, 15062006, 15062069, 15065012, 15065014, 15066070, 15068013, 15068014, 15068016, 15068018, 15069036, 15070008, 15070009, 15070010};

       for(int irun=0; irun<253; irun++)
	 {
	   if(mRunId == badrun[irun]) continue;
	 }

       //if(mRunId < 15053000 && mRunId >15070021) continue;
       
       //*********** Event Variable cuts **********************//
       //----- Analysis vertex cut ----//
       if(TMath::Abs(mVertexZ) > vzcut) continue;
       if(mVertexX == 0 && mVertexY == 0 && mVertexZ == 0) continue; 
       //if(TMath::Abs(mVertexZ-mMcVertexZ) > 0.01)continue;
       
       //---- Vertex radius cuts  ------//
       Float_t vrad = TMath::Sqrt( mVertexX*mVertexX + pow(mVertexY+0.89,2));
       if(TMath::Abs(vrad) > vradcut) continue;


       //----- Reference multiplicity cuts (0-70%) ----//
       if(mNUncorrectedPrimaries <= 0) continue;
       if(mNUncorrectedPrimaries <= RefLowCut || mNUncorrectedPrimaries > RefHighCut) continue;
       //*********** Event Variable cuts **********************//


       //Event variabl filling in tree
       tvzmctrk = mMcVertexZ;
       tvzrec = mVertexZ;
       tmult = mNUncorrectedPrimaries;
       
       hrefmult->Fill(mNUncorrectedPrimaries);
       hvxvy->Fill(mVertexX,mVertexY);
       hvzmctrk->Fill(mMcVertexZ);
       hvzmatch->Fill(mVertexZ);
       hvzdiff->Fill(mMcVertexZ-mVertexZ);
       hvz2d->Fill(mMcVertexZ,mVertexZ);     
       

       //This is the loop on the embedded tracks. 
       //This will go into the denominator of efficiency calculations
	
	Int_t countmc = 0;
	tcountmctrk = 0;
       //MC tracks
       for(Int_t mc = 0; mc < mMcTracks_; mc++)
	 {

	   hptnchmctrk_check->Fill(mNUncorrectedPrimaries,mMcTracks_mPtMc[mc]);

	   //---- Check Charge of MC tracks ---//


           //----- Check PID -------// 
           //if(mMcTracks_mGeantId[mc] != Gid  && mMcTracks_mParentGeantId[mc]!=0)
	   if(mMcTracks_mGeantId[mc] != Gid  || mMcTracks_mParentGeantId[mc]!=0)
             {
               //cout<< "MC id = " << mMcTracks_mGeantId[mc] << " ";// << endl;  
               //cout<< "MC parent id = " << mMcTracks_mParentGeantId[mc] << ", skipping..... " << endl; 
               continue;
             }

	   //cout<< "MC id = " << mMcTracks_mGeantId[mc] ;
	   //cout<<" MC parent id = " << mMcTracks_mParentGeantId[mc] << endl; 

	   //if(mMcTracks_mChargeMc[mc] != 1) 
	   if(mMcTracks_mChargeMc[mc] != ch) 
	     {
	       //cout <<"MC charge = " << mMcTracks_mChargeMc[mc] <<", skipping..." << endl; 
	       continue; 
	     } 

	   
	   /*
	   //---- Select Only primary tracks ---//
	   if(mMcTracks_mIsPrimary[mc] != 1)
	     {
	       //cout <<" IsPrimary (mctrk):: " << mMcTracks_mIsPrimary[mc] <<", skipping... " << endl;
	       continue;
	     }
	   */




	   /*	   
	   //----- Check PID -------// 
	   if(mMcTracks_mGeantId[mc] != Gid) 
	     { 
	       cout<< "MC id = " << mMcTracks_mGeantId[mc] << ", skipping..... " << endl; 
	       continue; 
	     }
	   */


	   //----- Check if everything is OK for this particle embedding -----//
	   //if(mMcTracks_mChargeMc[mc] != 1 || mMcTracks_mIsPrimary[mc] != 1 || mMcTracks_mGeantId[mc] != Gid) { cout <<" Problem in MC loop help.... " << endl;}
	   //if(mMcTracks_mChargeMc[mc] != 1 || mMcTracks_mGeantId[mc] != Gid) { cout <<" Problem in MC loop help.... " << endl;}


	   //cout <<" MC tracks:: " <<" charge: " <<  mMcTracks_mChargeMc[mc] <<" Isprimary: " <<  mMcTracks_mIsPrimary[mc] <<" Gid: " << mMcTracks_mGeantId[mc] << endl;
	   //cout <<" MC track :: " <<" pT: " <<  mMcTracks_mPtMc[mc] << endl; //" Isprimary: " <<  mMcTracks_mIsPrimary[mc] <<" Gid: " << mMcTracks_mGeantId[mc] << endl;


	   //--------- Now start with good tracks we have for this embedding ---------//

	   //----- Momentum Calculation -----//
	   Float_t mommctrk = TMath::Sqrt(mMcTracks_mPtMc[mc]*mMcTracks_mPtMc[mc] + mMcTracks_mPzMc[mc]*mMcTracks_mPzMc[mc]);
	   
	   //----- Rapidity calculation -----//
	   Float_t enmctrk = TMath::Sqrt(mommctrk*mommctrk + mp*mp);
	   Float_t rapmctrk = 0.5*TMath::Log((enmctrk + mMcTracks_mPzMc[mc])/(enmctrk - mMcTracks_mPzMc[mc]));
	   
	   //---- Filling in tree -------//
	   tptmctrk[countmc] = mMcTracks_mPtMc[mc];
	   trapmctrk[countmc] = rapmctrk;
	   tgeantidmctrk[countmc] = mMcTracks_mGeantId[mc];
	   countmc++;


	   //*************** MC Track cuts ********************//
	   //---- pT cut -----//
	   if(mMcTracks_mPtMc[mc] < LowPtCut) continue;

	   //----- Rapidity Cut  ------//
	   if(TMath::Abs(rapmctrk) > rapcut) continue;
	   //*************** Mc Track cuts ********************//

	   //-------- Histogram filling -----------//
	   hphimctrk->Fill(mMcTracks_mPhiMc[mc]);
	   hetamctrk->Fill(mMcTracks_mEtaMc[mc]);
	   hrapmctrk->Fill(rapmctrk);
	   hptmctrk->Fill(mMcTracks_mPtMc[mc]);
	   hptmctrksum->Fill(mMcTracks_mPtMc[mc]); //same plot
	   hmommctrk->Fill(mommctrk);
	   hptnchmctrk->Fill(mNUncorrectedPrimaries,mMcTracks_mPtMc[mc]);

	   //2D plots
	   hptphi->Fill(mMcTracks_mPtMc[mc],mMcTracks_mPhiMc[mc]);
	   hpteta->Fill(mMcTracks_mPtMc[mc],mMcTracks_mEtaMc[mc]); 
	   hetaphi->Fill(mMcTracks_mEtaMc[mc],mMcTracks_mPhiMc[mc]); 
	   hptymctrk->Fill(rapmctrk,mMcTracks_mPtMc[mc]); //pT vs y for mc tracks 
	   hpty->Fill(mMcTracks_mPtMc[mc],rapmctrk);
	   hyphi->Fill(rapmctrk,mMcTracks_mPhiMc[mc]);


	 }// Loop over MC track ends

       tcountmctrk = countmc;


       //These are matched pairs, and it goes into denominator
       //of efficiency calculations.
        
       Int_t countmatch = 0;
       tcountmatch = 0;
       //-------- Loop over Matched pairs  ---------//
       for(Int_t i = 0; i < mMatchedPairs_ ; i++)
	 {

	   //----- Check Flag -------//
	   if(mMatchedPairs_mFlag[i] <= 0 || mMatchedPairs_mFlag[i] >= 1000)continue;

	   
	   //---Check charge ------//
	   if(mMatchedPairs_mChargeMc[i] != ch) 
	     {
	       cout <<"Matched track charge = " << mMatchedPairs_mChargeMc[i] <<", skipping..." << endl; 
	       continue; 
	     } 

	   /*
	   //----- Select only primary tracks -----//
	   if(mMatchedPairs_mIsPrimary[i] != 1)
	     {
	       //cout <<" IsPrimary (matched):: " << mMatchedPairs_mIsPrimary[i] <<", skipping... " << endl;
	       continue;
	     }
	   */

	   //cout<< "Matched track id = " << mMatchedPairs_mGeantId[i] << " ";
	   //cout<< "Matched track Parent id = " << mMatchedPairs_mParentGeantId[i] << endl;

	   //------ Check Geant Id ----------//
	   if(mMatchedPairs_mGeantId[i] != Gid || mMatchedPairs_mParentGeantId[mc]!=0)
	     {
	       cout<< "Matched track id = " << mMatchedPairs_mGeantId[i] << ", skipping..... " << endl;
	       continue;
	     }

	   //cout<< "Matched track id = " << mMatchedPairs_mGeantId[i] << " ";
	   //cout<< "Matched track Parent id = " << mMatchedPairs_mParentGeantId[i] << endl;

	   //-------- Check if everything is ok for this particle embedding ----------//
	   //if(mMatchedPairs_mGeantId[i] != Gid || mMatchedPairs_mChargeMc[i] != 1 || mMatchedPairs_mIsPrimary[i] != 1) {cout <<" Problem in matched track loop help... " << endl;}
	   //if(mMatchedPairs_mGeantId[i] != Gid || mMatchedPairs_mChargeMc[i] != 1 ) {cout <<" Problem in matched track loop help... " << endl;}


	   //cout <<" Matched pairs: " <<" Gid: " << mMatchedPairs_mGeantId[i] <<"  charge:    " <<  mMatchedPairs_mChargeMc[i] <<"  isprimary:   " <<  mMatchedPairs_mIsPrimary[i] << endl;
	   

	   //-------- Histogram Filling Starts ----------//
	   hpadrowphi->Fill(mMatchedPairs_mPhiPr[i],mMatchedPairs_mFirstPadrow[i]);
	   
	   //======== Calculation for matched pairs =========//
	   //------- Momentum calculation ------------//
	   Float_t mommatch=TMath::Sqrt(mMatchedPairs_mPtPr[i]*mMatchedPairs_mPtPr[i] + mMatchedPairs_mPzPr[i]*mMatchedPairs_mPzPr[i]);
	   
	   //------- Rapidity calculation ------------//
           Float_t enmatch = TMath::Sqrt(mommatch*mommatch + mp*mp);
           Float_t rapmatch = 0.5*TMath::Log((enmatch + mMatchedPairs_mPzPr[i])/(enmatch - mMatchedPairs_mPzPr[i]));

	   
	   //======== Calculation for matched and recontsructed pairs ========//
	   //------- Momentum calculation ------------//
	   Float_t mommatch1=TMath::Sqrt(mMatchedPairs_mPtMc[i]*mMatchedPairs_mPtMc[i] + mMatchedPairs_mPzMc[i]*mMatchedPairs_mPzMc[i]);
	   
	   //------- Rapidity calculation ------------//
           Float_t enmatch1 = TMath::Sqrt(mommatch1*mommatch1 + mp*mp);
           Float_t rapmatch1 = 0.5*TMath::Log((enmatch1 + mMatchedPairs_mPzMc[i])/(enmatch1 - mMatchedPairs_mPzMc[i]));

	   //-------- Filling in tree -----------//
	   tgeantidmatch[countmatch] = mMatchedPairs_mGeantId[i];
	   tptprmatch[countmatch] = mMatchedPairs_mPtPr[i];
	   tptmcmatch[countmatch] = mMatchedPairs_mPtMc[i];
	   trapptprmatch[countmatch] = rapmatch;
	   trapptmcmatch[countmatch] = rapmatch1; 
	   tdcamatch[countmatch] = mMatchedPairs_mDcaGl[i];
	   tnfitmatch[countmatch] = mMatchedPairs_mFitPts[i];
	   tcommonhitmatch[countmatch] = mMatchedPairs_mNCommonHit[i];
	   //New variables added for tree
	   tnfitposs[countmatch] = mMatchedPairs_mNPossible[i];
	   tdedxpts[countmatch] = mMatchedPairs_mDedxPts[i];
	   countmatch++;

	   //*************** Matched Track cuts ********************//
	   //---------- dca cut ------------//
	   if(mMatchedPairs_mDcaGl[i] < 0.) continue;
	   if(mMatchedPairs_mDcaGl[i] > dcacut) continue;

	   
 	   //----------- Rapidity cut --------//
	   if(TMath::Abs(rapmatch) > rapcut) continue;
	   if(TMath::Abs(rapmatch1) > rapcut) continue;


	   //-------- pT cut ---------//
	   if(mMatchedPairs_mPtPr[i] < LowPtCut) continue;
	   if(mMatchedPairs_mPtMc[i] < LowPtCut) continue;

	   
	   //--------- Nfit point and common hits cut ----------//
	   //if(mMatchedPairs_mFitPts[i] < Nfit || mMatchedPairs_mNCommonHit[i] <10 ) continue;
	   if(mMatchedPairs_mFitPts[i] < Nfit) continue;
	   if(mMatchedPairs_mNCommonHit[i] <15 ) continue;


	   /////////////////////////////////////////////////////////////////////////////////////////
	   //------- Filling phi vs pT for matched track before nfit ratio and nhitsdedx cut -----//
	   //hptphimatch->Fill(mMatchedPairs_mPtPr[i],mMatchedPairs_mPhiMc[i]);
	   hptphimatch1->Fill(mMatchedPairs_mPtMc[i],mMatchedPairs_mPhiMc[i]);
	   hptnchmatchnodedx->Fill(mNUncorrectedPrimaries,mMatchedPairs_mPtMc[i]);
	   //hptphimatch2->Fill(mMatchedPairs_mPtMc[i],mMatchedPairs_mPhiPr[i]);
	   //-------------------------------------------------------------------------------------//


	   //------- Nfit/Nposs > 0.52 cuts ---------//
	   Float_t ratio = (float) mMatchedPairs_mFitPts[i]/mMatchedPairs_mNPossible[i];
	   //if(ratio < 0.52) continue;
	   if(ratio < NfRatio) continue;
	   //------- DeDx points cut ------------//
	   if(mMatchedPairs_mDedxPts[i] < 11) continue;
	   //*************** Matched Track cuts ********************//


	   //---- pT vs phi matched ------//
	   hptphimatch->Fill(mMatchedPairs_mPtMc[i],mMatchedPairs_mPhiMc[i]);
	   hptprphimcmatch->Fill(mMatchedPairs_mPtPr[i],mMatchedPairs_mPhiMc[i]);
	   hptmcphiprmatch->Fill(mMatchedPairs_mPtMc[i],mMatchedPairs_mPhiPr[i]);

	   if(mMatchedPairs_mEtaMc[i]>0.)
	     hptphimatch_etapos->Fill(mMatchedPairs_mPtMc[i],mMatchedPairs_mPhiMc[i]);
	   if(mMatchedPairs_mEtaMc[i]<0.)
	     hptphimatch_etaneg->Fill(mMatchedPairs_mPtMc[i],mMatchedPairs_mPhiMc[i]);
	   //-----------------------------//

	   hndedxhits_match->Fill(mMatchedPairs_mDedxPts[i]);
	   hptndedxhits_match->Fill(mMatchedPairs_mPtMc[i],mMatchedPairs_mDedxPts[i]);


	   // Other plots 
	   hphimatch->Fill(mMatchedPairs_mPhiPr[i]);
	   hetamatch->Fill(mMatchedPairs_mEtaPr[i]);

	   if(mMatchedPairs_mDedx[i] > 0.) hdedxmatch->Fill(mommatch,mMatchedPairs_mDedx[i]);
	   
	   hdcamatch->Fill(mMatchedPairs_mDcaGl[i]);
	   hdcaptmatch->Fill(mMatchedPairs_mPtMc[i],mMatchedPairs_mDcaGl[i]);
           hnfitmatch->Fill(mMatchedPairs_mFitPts[i]);
           hnfitptmatch->Fill(mMatchedPairs_mPtMc[i],mMatchedPairs_mFitPts[i]);
           hrapmatch->Fill(rapmatch);
           hptmatch->Fill(mMatchedPairs_mPtPr[i]);
           hmommatch->Fill(mommatch);
           hptnchmatch->Fill(mNUncorrectedPrimaries,mMatchedPairs_mPtPr[i]);
           //hptdiff->Fill(mMatchedPairs_mPtMc[i],mMatchedPairs_mPtMc[i]-mMatchedPairs_mPtPr[i]);
           //hptdiff1D->Fill(mMatchedPairs_mPtMc[i],mMatchedPairs_mPtMc[i]-mMatchedPairs_mPtPr[i]);

	   hrapmatch1->Fill(rapmatch1);
	   hphimatch1->Fill(mMatchedPairs_mPhiMc[i]);
	   hetamatch1->Fill(mMatchedPairs_mEtaMc[i]);
	   hptmatch1->Fill(mMatchedPairs_mPtMc[i]);
	   hptymatch1->Fill(rapmatch1,mMatchedPairs_mPtMc[i]); //pT vs y for matched tracks
	   hptym1->Fill(mMatchedPairs_mPtMc[i],rapmatch1); //pT vs y for matched tracks
	   hptmatch1sum->Fill(mMatchedPairs_mPtMc[i]); // same plot
	   hmommatch1->Fill(mommatch1);
	   hptnchmatch1->Fill(mNUncorrectedPrimaries,mMatchedPairs_mPtMc[i]);
	   hptdiff1->Fill(mMatchedPairs_mPtPr[i],mMatchedPairs_mPtPr[i]-mMatchedPairs_mPtMc[i]);
	   hptdiff1D1->Fill(mMatchedPairs_mPtPr[i],mMatchedPairs_mPtPr[i]-mMatchedPairs_mPtMc[i]);
	   hptdiff1sum->Fill(mMatchedPairs_mPtPr[i],mMatchedPairs_mPtPr[i]-mMatchedPairs_mPtMc[i]);
	   hptdiff1D1sum->Fill(mMatchedPairs_mPtPr[i],mMatchedPairs_mPtPr[i]-mMatchedPairs_mPtMc[i]);
	   
	 }// Loop over Matched pairs ends
       
       tcountmatch = countmatch;


       
       //Tree filling event wise
       //minitree->Fill(); // No need to fill the tree
       
       gevt++;

     } // Event loop ends

   
   cout <<" Total good events :: " << gevt << endl;


   fpw->cd();
   
   minitree->Write(); 

   hrefmult->Write();
   hvzmctrk->Write();
   hvzmatch->Write();
   hvzdiff->Write();
   hvz2d->Write();
   hvxvy->Write();

   hptmctrk->Write();
   //sumw2
   //   hptmctrksum->Sumw2();
   //   hptmctrksum->Write();
   hmommctrk->Write();
   hptnchmctrk->Write();
   //hptnchmctrk_check->Write();
   hphimctrk->Write();
   hetamctrk->Write();
   hrapmctrk->Write();

   hptmatch->Write();
   hmommatch->Write();
   hptnchmatch->Write();
   /*
   hphimatch->Write();
   hetamatch->Write();
   hrapmatch->Write();
   */
   hphimatch->Write();


   //---- Phi vs Pt match ------//
   hptphimatch->Write();
   hptphimatch_etapos->Write();
   hptphimatch_etaneg->Write();
   hptphimatch1->Write();
   hptprphimcmatch->Write();
   hptmcphiprmatch->Write();
   //---------------------------//


   hdcamatch->Write();
   hnfitmatch->Write();
   hdcaptmatch->Write();
   hnfitptmatch->Write();
   hndedxhits_match->Write();
   hptndedxhits_match->Write();

   //sumw2
   //   hptmatch1sum->Sumw2();
   //   hptmatch1sum->Write();


   hptmatch1->Write();
   hrapmatch1->Write();
   hphimatch1->Write();
   hetamatch1->Write();   
   hmommatch1->Write();
   hptnchmatch1->Write();
   hptnchmatchnodedx->Write();


   
   hptdiff1->Write(); // eloss

   //   hptdiff1D1->Write();
   //sumw2()
   /*
   hptdiff1sum->Sumw2();
   hptdiff1D1sum->Sumw2();
   hptdiff1sum->Write();
   hptdiff1D1sum->Write();
   */

   hdedxmatch->Write();
   hdedxghost->Write();

   //   hdedxglob->Write();

   hdcaghost->Write();
   hnfitghost->Write();


   hptphi->Write();
   hpteta->Write();
   hetaphi->Write();
   hpty->Write();
   hptym1->Write();
   hyphi->Write();

   hptghost->Write();


   //2D plots pT vs Y for mc track and match track
   hptymctrk->Write();
   hptymatch1->Write();

   fpw->Close();
   

}

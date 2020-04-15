//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed May 12 03:43:47 2010 by ROOT version 5.19/04
// from TTree t1/Jet Tree 2004 AuAu MB
// found on file: day_nfitdedx_10k.root
//////////////////////////////////////////////////////////

#ifndef ReadPico_h
#define ReadPico_h

class TBranch ;
class TH1 ;
class TH2 ;
class TH3 ;
class TFile ;
class TTree ;
 
class ReadPico {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   //Double_t        Vz;
   Int_t           nTrack;
   Int_t           ToFRefmult;
   Int_t           Centrality;
   Double_t        w;
   Float_t         dca[5000];
   Float_t         nHitsFit[5000];
   Float_t         nHitsdEdx[5000];
   Float_t         tof_pt[5000];   //[Ntrk]
   Float_t         tof_kapt[5000];   //[Ntrk]
   Float_t         tof_prpt[5000];   //[Ntrk]
   Float_t         mom[5000];   //[Ntrk]
   Float_t         tNspi_tof[5000];   //[Ntrk]
   Float_t         tNsk_tof[5000];   //[Ntrk]
   Float_t         tNspr_tof[5000];   //[Ntrk]
   short           charge[5000];   //[Ntrk]
   Float_t         Tof[5000];   //[Ntrk]
   Float_t         Pathlength[5000];   //[Ntrk]
   Float_t         Beta[5000];   //[Ntrk]
   Float_t         tof_rpi[5000];   //[Ntrk] 
   Float_t         tof_rka[5000];   //[Ntrk] 
   Float_t         tof_rpr[5000];   //[Ntrk]   

   Int_t           EvCount;
   Int_t           EvCount2nd;
   Int_t           EventBreak;
   Float_t         c_light;
   Float_t         pimass;
   Float_t         kmass;
   Float_t         pmass;
   Float_t         rapcut;
   Int_t           RefLowCut;
   Int_t           RefHighCut;
   Int_t           CentCut;
   //Float_t         vzcut;
   Float_t         dcacut;
   Float_t         nHitscut;
   Float_t         ndEdxcut;
   Float_t         lowpTcut; 
   Int_t           nfiles;
   //Float_t         a0,a1,a2,a3;
   //TLorentzVector vpi, vka, vpr;


   // List of branches
   //TBranch        *b_Vz;   //! 
   TBranch        *b_nTrack;   //! 
   TBranch        *b_ToFRefmult;   //! 
   TBranch        *b_Centrality;   //! 
   TBranch        *b_w;   //!
   TBranch        *b_dca;   //!
   TBranch        *b_nHitsFit;   //!
   TBranch        *b_nHitsdEdx;   //! 
   TBranch        *b_tof_pt;   //!  
   TBranch        *b_tof_kapt;   //!  
   TBranch        *b_tof_prpt;   //!      
   TBranch        *b_mom;   //!   
   TBranch        *b_tNspi_tof;   //!    
   TBranch        *b_tNsk_tof;   //!     
   TBranch        *b_tNspr_tof;   //! 
   TBranch        *b_charge;   //! 
   TBranch        *b_Tof;   //!   
   TBranch        *b_Pathlength;   //! 
   TBranch        *b_tof_rpi;   //!  
   TBranch        *b_tof_rka;   //! 
   TBranch        *b_tof_rpr;   //!
   TBranch        *b_Beta;   //! 

   ReadPico(TTree *tree=0);
   virtual ~ReadPico();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   //virtual void     LoopAll(const Char_t* filelist, const Char_t *outfile);  // defined by Hiroshi
   //virtual void     LoopAll(const int ev, const Char_t* filelist, const Char_t *outfile);  // defined by Hiroshi
   virtual void     LoopAll(const Char_t* filelist, const Char_t *outfile, const Char_t *outpi, const Char_t *outka, const Char_t *outpr); 
   //virtual void     LoopAll(const Char_t* filelist, const Char_t *outfile, const Char_t *outpi, const Char_t *outka, const Char_t *outpr, const Char_t *outlum); 
   virtual void     Loop(const Char_t* filename); //defined by Hiroshi
   virtual void     Loop2(const Char_t* filename); //defined by Hiroshi
   //virtual void     Loop(const int ev, const Char_t* filename); //defined by Hiroshi
   //virtual void     Loop();
   virtual void     Finish();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   TFile* fpw ;

   TH1* hrefmult ;
   TH1* hcentmult ;

   TH2* hmsq_all_pos;
   TH2* hmsq_all_neg;
   TH2* hmsq_all_pos_check;

   TH2* hdeltapiplus ; 
   TH2* hdeltakplus  ; 
   TH2* hdeltaprplus ; 
   TH2* hdeltapineg  ; 
   TH2* hdeltakneg   ; 
   TH2* hdeltaprneg  ; 

   TH1* hdeltacheck  ; 
   
   //TH2* hdeltapiplus1 ;
   TH2* hdeltapiplus_check ;
   
   
   TFile *fpion;
   
   TH1* hpred_msqpipos[30] ; 
   TString hpreg_msqpipos_names[30];
   //For filling - next 5
   TH1D* hdpipos[30];   
   TString hdpipos_names[30];   
   Float_t tofpiplus_new[30];
   Float_t pred_msqpipos[30];
   Double_t ran_pipos[30] ;
   
   TH2* hpred_msqpipos_highpt ;
   
   
   //Predicted m^2 pion negative
   TH1* hpred_msqpineg[30] ; 
   TString hpreg_msqpineg_names[30];
   //For filling - next 5
   TH1D* hdpineg[30];   
   TString hdpineg_names[30];   
   Float_t tofpineg_new[30];
   Float_t pred_msqpineg[30];
   Double_t ran_pineg[30] ;   
      
   TH2* hpred_msqpineg_highpt ;
   
   //Define file to fill predicted m^2 for pions
   TFile *fkaon;
  //Predicted m^2 kaon positive
   TH1* hpred_msqkapos[30] ; 
   TString hpreg_msqkapos_names[30] ;
   //For filling - next 5
   TH1D* hdkapos[30];   
   TString hdkapos_names[30] ;   
   Float_t tofkaplus_new[30] ;
   Float_t pred_msqkapos[30];
   Double_t ran_kapos[30] ;
     
   TH2* hpred_msqkapos_highpt ;
   
   //Predicted m^2 kaon negative
   TH1* hpred_msqkaneg[30] ; 
   TString hpreg_msqkaneg_names[30] ; 
   //For filling - next 5
   TH1D* hdkaneg[30];   
   TString hdkaneg_names[30];   
   Float_t tofkaneg_new[30];
   Float_t pred_msqkaneg[30];
   Double_t ran_kaneg[30] ;
      
   TH2* hpred_msqkaneg_highpt ;
   
   //Define file to fill predicted m^2 for pions
   TFile *fproton;
   //Predicted m^2 proton positive
   TH1* hpred_msqprpos[30] ; 
   TString hpreg_msqprpos_names[30] ;   
   //For filling - next 5
   TH1D* hdprpos[30];   
   TString hdprpos_names[30] ;   
   Float_t tofprplus_new[30] ;
   Float_t pred_msqprpos[30];
   Double_t ran_prpos[30] ;
   TH2* hpred_msqprpos_highpt ;
   
   
   //Predicted m^2 proton negative
   TH1* hpred_msqprneg[30] ; 
   TString hpreg_msqprneg_names[30] ;  
   //For filling - next 5
   TH1D* hdprneg[30];   
   TString hdprneg_names[30];   
   Float_t tofprneg_new[30];
   Float_t pred_msqprneg[30];
   Double_t ran_prneg[30] ; 
    
   TH2* hpred_msqprneg_highpt ;
   
   
   ClassDef(ReadPico, 0)
};

#endif


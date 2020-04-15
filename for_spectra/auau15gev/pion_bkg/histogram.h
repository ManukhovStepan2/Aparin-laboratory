TH1D *mMult = new TH1D("Mult","Mult",1000,0.,1000.);
TH2D *mVzCorr = new TH2D("VzCorr","VzCorr",200,-100.,100.,200,-100.,100.);
TH1D *mVxRes = new TH1D("VxRes","VxRes",1000,-1.,1.);
TH1D *mVyRes = new TH1D("VyRes","VyRes",1000,-1.,1.);
TH1D *mVzRes = new TH1D("VzRes","VzRes",1000,-1.,1.);

//pions
TH2D *mPiParentGId = new TH2D("PiParentGId","PiParentGId",100,0.,10.,100,0.,100.);
TH1D *mPiPtMc = new TH1D("PiPtMc","PiPtMc",100,0.,5.);
TH1D *mKsLPtMc = new TH1D("KsLPtMc","KsLPtMc",100,0.,5.);      //<=== Added
TH1D *mPiPtMcPrim = new TH1D("PiPtMcPrim","PiPtMcPrim",100,0.,5.);
TH1D *mPiPtRc = new TH1D("PiPtRc","PiPtRc",100,0.,5.);
TH1D *mPiPtRcPrim = new TH1D("PiPtRcPrim","PiPtRcPrim",100,0.,5.);
TH1D *mPiPtKs = new TH1D("PiPtKs","PiPtKs",100,0.,5.);
TH1D *mPiPtOther = new TH1D("PiPtOther","PiPtOther",100,0.,5.);
TH2D *mPiPrimParentGId = new TH2D("PiPrimParentGId","PiPrimParentGId",100,0.,5.,100,0.,100.);
TH2D *mPiNonPrimParentGId = new TH2D("PiNonPrimParentGId","PiNonPrimParentGId",100,0.,5.,100,0.,100.);

TH2D *mPiOtherXY = new TH2D("PiOtherXY","PiOtherXY",500,-50.,50.,500,-50.,50.);
TH1D *mPiTotDecay = new TH1D("PiTotDecay","PiTotDecay",100,0.,5.);


//muons
TH2D *mMuParentGId = new TH2D("MuParentGId","MuParentGId",100,0.,10.,100,0.,100.);
TH1D *mMuPtRc = new TH1D("MuPtRc","MuPtRc",100,0.,5.);
TH1D *mMuPtRcPrim = new TH1D("MuPtRcPrim","MuPtRcPrim",100,0.,5.);
TH2D *mMuPrimParentGId = new TH2D("MuPrimParentGId","MuPrimParentGId",100,0.,5.,100,0.,100.);



//electrons
//SDCA vs pt
TH2D *mSDCA = new TH2D("SDCA","SDCA",100,0.,5.0,200,-4.,16.);
TH2D *mSDCA_Prim = new TH2D("SDCA_Prim","SDCA_Prim",100,0.,5.0,200,-4.,16.);
TH2D *mSDCA_PrimPi = new TH2D("SDCA_PrimPi","SDCA_PrimPi",100,0.,5.0,200,-4.,16.); 
TH2D *mSDCA_BP = new TH2D("SDCA_BP","SDCA_BP",100,0.,5.0,200,-4.,16.);
TH2D *mSDCA_Air = new TH2D("SDCA_Air","SDCA_Air",100,0.,5.0,200,-4.,16.);
TH2D *mSDCA_IFC = new TH2D("SDCA_IFC","SDCA_IFC",100,0.,5.0,200,-4.,16.);

TH2D *mSDCACut = new TH2D("SDCACut","SDCACut",100,0.,5.0,200,-4.,16.);
TH2D *mSDCACut_Prim = new TH2D("SDCACut_Prim","SDCACut_Prim",100,0.,5.0,200,-4.,16.);
TH2D *mSDCACut_PrimPi = new TH2D("SDCACut_PrimPi","SDCACut_PrimPi",100,0.,5.0,200,-4.,16.);
TH2D *mSDCACut_BP = new TH2D("SDCACut_BP","SDCACut_BP",100,0.,5.0,200,-4.,16.);
TH2D *mSDCACut_Air = new TH2D("SDCACut_Air","SDCACut_Air",100,0.,5.0,200,-4.,16.);
TH2D *mSDCACut_IFC = new TH2D("SDCACut_IFC","SDCACut_IFC",100,0.,5.0,200,-4.,16.);

TH2D *mSDCAWt = new TH2D("SDCAWt","SDCAWt",100,0.,5.0,200,-4.,16.);
TH2D *mSDCAWt_BP = new TH2D("SDCAWt_BP","SDCAWt_BP",100,0.,5.0,200,-4.,16.);
TH2D *mSDCAWt_Air = new TH2D("SDCAWt_Air","SDCAWt_Air",100,0.,5.0,200,-4.,16.);
TH2D *mSDCAWt_IFC = new TH2D("SDCAWt_IFC","SDCAWt_IFC",100,0.,5.0,200,-4.,16.);

TH2D *mSDCAWtCut = new TH2D("SDCAWtCut","SDCAWtCut",100,0.,5.0,200,-4.,16.);
TH2D *mSDCAWtCut_BP = new TH2D("SDCAWtCut_BP","SDCAWtCut_BP",100,0.,5.0,200,-4.,16.);
TH2D *mSDCAWtCut_Air = new TH2D("SDCAWtCut_Air","SDCAWtCut_Air",100,0.,5.0,200,-4.,16.);
TH2D *mSDCAWtCut_IFC = new TH2D("SDCAWtCut_IFC","SDCAWtCut_IFC",100,0.,5.0,200,-4.,16.);

//electron check
TH2D *mEPtEta = new TH2D("EPtEta","EPtEta",100,0.,5.0,20,-1.,1.);
TH2D *mEPtPhi = new TH2D("EPtPhi","EPtPhi",100,0.,5.0,180,-180.,180.);
TH1D *mENHits = new TH1D("ENHits","ENHits",50,0.,50.);

// conversion check
TH2D *mConvRZ = new TH2D("ConvRZ","ConvRZ",160,-80.,80.,2000,0.,200.);
TH2D *mConvRPhi = new TH2D("ConvRPhi","ConvRPhi",180,-180.,180.,2000,0.,200.);
TH2D *mMcRcR = new TH2D("McRcR","McRcR",500,0.,50.,500,0.,50.);
TH2D *mMcRcRWt = new TH2D("McRcRWt","McRcRWt",500,0.,50.,500,0.,50.);

// input photon
TH2D *mMcGammaPtEta = new TH2D("McGammaPtEta","McGammaPtEta",1000,0.,10.,40,-2.,2.);
TH1D *mNGamma = new TH1D("NGamma","NGamma",100,0.,100.);

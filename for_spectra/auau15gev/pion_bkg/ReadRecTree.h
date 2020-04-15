//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue May 24 16:59:38 2016 by ROOT version 5.34/30
// from TTree mTrackEvent/trackTree
// found on file: test.rectree.root
//////////////////////////////////////////////////////////

#ifndef ReadRecTree_h
#define ReadRecTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class ReadRecTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           mIEvt;
   Int_t           OriginMult;
   Int_t           McMult;
   Float_t         mRcVertexX;
   Float_t         mRcVertexY;
   Float_t         mRcVertexZ;
   Float_t         mMcVertexX;
   Float_t         mMcVertexY;
   Float_t         mMcVertexZ;
   Float_t         mMcGVertexX;
   Float_t         mMcGVertexY;
   Float_t         mMcGVertexZ;
   Float_t         MagField;
   Int_t           mNMcTrk;
   Int_t           mMcId[5179];   //[mNMcTrk]
   Int_t           mGeantId[5179];   //[mNMcTrk]
   Int_t           mParentGeantId[5179];   //[mNMcTrk]
   Int_t           mParentId[5179];   //[mNMcTrk]
   Float_t         mMcPt[5179];   //[mNMcTrk]
   Float_t         mMcEta[5179];   //[mNMcTrk]
   Float_t         mMcPhi[5179];   //[mNMcTrk]
   Float_t         mMcParentPt[5179];   //[mNMcTrk]
   Float_t         mMcParentEta[5179];   //[mNMcTrk]
   Float_t         mMcParentPhi[5179];   //[mNMcTrk]
   Int_t           mMcParentO[5179];   //[mNMcTrk]
   Float_t         mMcMass[5179];   //[mNMcTrk]
   Float_t         mMcStartX[5179];   //[mNMcTrk]
   Float_t         mMcStartY[5179];   //[mNMcTrk]
   Float_t         mMcStartZ[5179];   //[mNMcTrk]
   Int_t           mMcNhits[5179];   //[mNMcTrk]
   Int_t           mNRcTrk;
   Int_t           mRcId[872];   //[mNRcTrk]
   Int_t           mRcAssoId[872];   //[mNRcTrk]
   Int_t           mRcAssoOffset[872];   //[mNRcTrk]
   Float_t         mRcPt[872];   //[mNRcTrk]
   Float_t         mRcEta[872];   //[mNRcTrk]
   Float_t         mRcPhi[872];   //[mNRcTrk]
   Int_t           mRcNhits[872];   //[mNRcTrk]
   Int_t           TPCrightTrack[872];   //[mNRcTrk]
   Int_t           sharedTpcHits[872];   //[mNRcTrk]
   Float_t         percentSharedTpcHits[872];   //[mNRcTrk]
   Float_t         mHelixCurv[872];   //[mNRcTrk]
   Float_t         mHelixDip[872];   //[mNRcTrk]
   Float_t         mHelixPhase[872];   //[mNRcTrk]
   Float_t         mHelixX[872];   //[mNRcTrk]
   Float_t         mHelixY[872];   //[mNRcTrk]
   Float_t         mHelixZ[872];   //[mNRcTrk]
   Float_t         mArc[872];   //[mNRcTrk]
   Float_t         mSDCA[872];   //[mNRcTrk]
   Float_t         mSDCA1[872];   //[mNRcTrk]
   Float_t         mSDCA2[872];   //[mNRcTrk]

   // List of branches
   TBranch        *b_mIEvt;   //!
   TBranch        *b_OriginMult;   //!
   TBranch        *b_McMult;   //!
   TBranch        *b_mRcVertexX;   //!
   TBranch        *b_mRcVertexY;   //!
   TBranch        *b_mRcVertexZ;   //!
   TBranch        *b_mMcVertexX;   //!
   TBranch        *b_mMcVertexY;   //!
   TBranch        *b_mMcVertexZ;   //!
   TBranch        *b_mMcGVertexX;   //!
   TBranch        *b_mMcGVertexY;   //!
   TBranch        *b_mMcGVertexZ;   //!
   TBranch        *b_MagField;   //!
   TBranch        *b_mNMcTrk;   //!
   TBranch        *b_mMcId;   //!
   TBranch        *b_mGeantId;   //!
   TBranch        *b_mParentGeantId;   //!
   TBranch        *b_mParentId;   //!
   TBranch        *b_mMcPt;   //!
   TBranch        *b_mMcEta;   //!
   TBranch        *b_mMcPhi;   //!
   TBranch        *b_mMcParentPt;   //!
   TBranch        *b_mMcParentEta;   //!
   TBranch        *b_mMcParentPhi;   //!
   TBranch        *b_mMcParentO;   //!
   TBranch        *b_mMcMass;   //!
   TBranch        *b_mMcStartX;   //!
   TBranch        *b_mMcStartY;   //!
   TBranch        *b_mMcStartZ;   //!
   TBranch        *b_mMcNhits;   //!
   TBranch        *b_mNRcTrk;   //!
   TBranch        *b_mRcId;   //!
   TBranch        *b_mRcAssoId;   //!
   TBranch        *b_mRcAssoOffset;   //!
   TBranch        *b_mRcPt;   //!
   TBranch        *b_mRcEta;   //!
   TBranch        *b_mRcPhi;   //!
   TBranch        *b_mRcNhits;   //!
   TBranch        *b_TPCrightTrack;   //!
   TBranch        *b_sharedTpcHits;   //!
   TBranch        *b_percentSharedTpcHits;   //!
   TBranch        *b_mHelixCurv;   //!
   TBranch        *b_mHelixDip;   //!
   TBranch        *b_mHelixPhase;   //!
   TBranch        *b_mHelixX;   //!
   TBranch        *b_mHelixY;   //!
   TBranch        *b_mHelixZ;   //!
   TBranch        *b_mArc;   //!
   TBranch        *b_mSDCA;   //!
   TBranch        *b_mSDCA1;   //!
   TBranch        *b_mSDCA2;   //!

   ReadRecTree(TTree *tree=0);
   virtual ~ReadRecTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef ReadRecTree_cxx
ReadRecTree::ReadRecTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("test.rectree.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("test.rectree.root");
      }
      f->GetObject("mTrackEvent",tree);

   }
   Init(tree);
}

ReadRecTree::~ReadRecTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t ReadRecTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t ReadRecTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void ReadRecTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("mIEvt", &mIEvt, &b_mIEvt);
   fChain->SetBranchAddress("OriginMult", &OriginMult, &b_OriginMult);
   fChain->SetBranchAddress("McMult", &McMult, &b_McMult);
   fChain->SetBranchAddress("mRcVertexX", &mRcVertexX, &b_mRcVertexX);
   fChain->SetBranchAddress("mRcVertexY", &mRcVertexY, &b_mRcVertexY);
   fChain->SetBranchAddress("mRcVertexZ", &mRcVertexZ, &b_mRcVertexZ);
   fChain->SetBranchAddress("mMcVertexX", &mMcVertexX, &b_mMcVertexX);
   fChain->SetBranchAddress("mMcVertexY", &mMcVertexY, &b_mMcVertexY);
   fChain->SetBranchAddress("mMcVertexZ", &mMcVertexZ, &b_mMcVertexZ);
   fChain->SetBranchAddress("mMcGVertexX", &mMcGVertexX, &b_mMcGVertexX);
   fChain->SetBranchAddress("mMcGVertexY", &mMcGVertexY, &b_mMcGVertexY);
   fChain->SetBranchAddress("mMcGVertexZ", &mMcGVertexZ, &b_mMcGVertexZ);
   fChain->SetBranchAddress("MagField", &MagField, &b_MagField);
   fChain->SetBranchAddress("mNMcTrk", &mNMcTrk, &b_mNMcTrk);
   fChain->SetBranchAddress("mMcId", mMcId, &b_mMcId);
   fChain->SetBranchAddress("mGeantId", mGeantId, &b_mGeantId);
   fChain->SetBranchAddress("mParentGeantId", mParentGeantId, &b_mParentGeantId);
   fChain->SetBranchAddress("mParentId", mParentId, &b_mParentId);
   fChain->SetBranchAddress("mMcPt", mMcPt, &b_mMcPt);
   fChain->SetBranchAddress("mMcEta", mMcEta, &b_mMcEta);
   fChain->SetBranchAddress("mMcPhi", mMcPhi, &b_mMcPhi);
   fChain->SetBranchAddress("mMcParentPt", mMcParentPt, &b_mMcParentPt);
   fChain->SetBranchAddress("mMcParentEta", mMcParentEta, &b_mMcParentEta);
   fChain->SetBranchAddress("mMcParentPhi", mMcParentPhi, &b_mMcParentPhi);
   fChain->SetBranchAddress("mMcParentO", mMcParentO, &b_mMcParentO);
   fChain->SetBranchAddress("mMcMass", mMcMass, &b_mMcMass);
   fChain->SetBranchAddress("mMcStartX", mMcStartX, &b_mMcStartX);
   fChain->SetBranchAddress("mMcStartY", mMcStartY, &b_mMcStartY);
   fChain->SetBranchAddress("mMcStartZ", mMcStartZ, &b_mMcStartZ);
   fChain->SetBranchAddress("mMcNhits", mMcNhits, &b_mMcNhits);
   fChain->SetBranchAddress("mNRcTrk", &mNRcTrk, &b_mNRcTrk);
   fChain->SetBranchAddress("mRcId", mRcId, &b_mRcId);
   fChain->SetBranchAddress("mRcAssoId", mRcAssoId, &b_mRcAssoId);
   fChain->SetBranchAddress("mRcAssoOffset", mRcAssoOffset, &b_mRcAssoOffset);
   fChain->SetBranchAddress("mRcPt", mRcPt, &b_mRcPt);
   fChain->SetBranchAddress("mRcEta", mRcEta, &b_mRcEta);
   fChain->SetBranchAddress("mRcPhi", mRcPhi, &b_mRcPhi);
   fChain->SetBranchAddress("mRcNhits", mRcNhits, &b_mRcNhits);
   fChain->SetBranchAddress("TPCrightTrack", TPCrightTrack, &b_TPCrightTrack);
   fChain->SetBranchAddress("sharedTpcHits", sharedTpcHits, &b_sharedTpcHits);
   fChain->SetBranchAddress("percentSharedTpcHits", percentSharedTpcHits, &b_percentSharedTpcHits);
   fChain->SetBranchAddress("mHelixCurv", mHelixCurv, &b_mHelixCurv);
   fChain->SetBranchAddress("mHelixDip", mHelixDip, &b_mHelixDip);
   fChain->SetBranchAddress("mHelixPhase", mHelixPhase, &b_mHelixPhase);
   fChain->SetBranchAddress("mHelixX", mHelixX, &b_mHelixX);
   fChain->SetBranchAddress("mHelixY", mHelixY, &b_mHelixY);
   fChain->SetBranchAddress("mHelixZ", mHelixZ, &b_mHelixZ);
   fChain->SetBranchAddress("mArc", mArc, &b_mArc);
   fChain->SetBranchAddress("mSDCA", mSDCA, &b_mSDCA);
   fChain->SetBranchAddress("mSDCA1", mSDCA1, &b_mSDCA1);
   fChain->SetBranchAddress("mSDCA2", mSDCA2, &b_mSDCA2);
   Notify();
}

Bool_t ReadRecTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void ReadRecTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t ReadRecTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef ReadRecTree_cxx

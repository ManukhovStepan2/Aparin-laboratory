#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class PlotFile;
#endif

#ifndef __CINT__
//#include <stdio.h>
#include <stdlib.h>
//#include "iostream.h"
//#include "iomanip.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "math.h"
#include "string.h"

#include "TROOT.h"
#include "TFile.h"

#include "TChain.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TRandom.h"
#include "TMath.h"
#include "TVector2.h"
#include "TVector3.h"
//#include "StThreeVectorF.hh"
#include "TLorentzVector.h"
#include "TSystem.h"
#include "TUnixSystem.h"
#include "TRandom3.h"
#endif

#include <stdio.h>
#include <iostream>
#include <fstream>
//#include "iomanip.h"
#include <iomanip>
#include "mTrackEvent.h"
#include "PhysicalConstants.h"
#include "SystemOfUnits.h" 
#include "StHelixD.h"
#include "StPhysicalHelixD.h"
#include "array.h"
#include "histogram.h"
using namespace std;

Double_t pplevyFun(Double_t *x, Double_t *par);

//int main(int argc, char **argv)
int main()
{

  /*
//  int argc = 2;
//  char *argv[2];


  if(argc != 2) exit(1);
  FileOutPut1 = argv[1];
  FileOutPut1 += ".hist.root";
  FileOutPut2 = argv[1];
  FileOutPut2 += ".tree.root";
  */

  const Double_t Pi = 3.1415927;

  FileOutPut1 = "output_Daniel_cut.root";

  //----------------------------------
  // Open files and add to the chain
  //----------------------------------
  TChain *chain = new TChain("mTrackEvent");
  
  /*
  int fileNumber = 0;
  void* dir = gSystem->OpenDirectory(gSystem->ExpandPathName(argv[1]));

  int nruns=0;
  char *file_name;
  TString Tname;
  char file_list[2500][256];
  
  cout << " execute : " << argv[0] << " " << argv[1] << endl;
  do {
    file_name = (char*)gSystem->GetDirEntry(dir);
    Tname=file_name;
    if (file_name && Tname.Contains("rectree.root")) {
      sprintf(file_list[nruns],"%s/%s",argv[1],file_name);
      TFile *ftmp = new TFile(file_list[nruns]); 
      if(!ftmp||!(ftmp->IsOpen())||!(ftmp->GetNkeys())) {
        printf(" file %s error in opening!!!\n",file_list[nruns]);
      } else { 
        printf(" read in file %s\n",file_list[nruns]);
        chain->Add(file_list[nruns]); 
        fileNumber++;
      } 
      nruns++;
      delete ftmp;
    }
  } while (file_name);

  printf("  %d files read in\n",fileNumber);
  */


  TString fNameList = TString("filelist_Aug16.list");
  //TString fNameList = TString("onefile.list");                                                                                                    
  cout <<" Load files from file: " << fNameList << endl;

  //ifstream fList((char*)fNameList);                                                                                                               
  ifstream fList(fNameList);
  if (!fList)
    {
      cout << "!!! Can't open file " << fNameList << endl;
      //return;                                                                                                                                     
    }

  char lineFromFile[255];
  while(fList.getline(lineFromFile, 250))
    {
      if(fList.eof())break;
      TString fileName = lineFromFile;

      fileName = "" + fileName;
      // Open this loop only when you need the information about loading files                                                                      

      cout <<" File Added::: " << fileName << endl;

      //if(chain->Add((char*)fileName)){};                                                                                                          
      if(chain->Add(fileName)){};
    }
  fList.close();


 
  //--------------------
  // loop the chain
  //--------------------
  //  TTree *tree = (TTree *)chain->GetTree();
  int nevents = (int)chain->GetEntries();
  cout << "== total entries : " << nevents << endl;
  
  mTrackEvent *t = new mTrackEvent(chain);

  TF1 *convPt = new TF1("convPt","sqrt([0]*[0]+x*x/0.0015/0.0015)-x/0.0015",0.,5.0);

  TF1 *pplevy = new TF1("pplevy",pplevyFun, 0.,10.,3);
  pplevy->SetParameters(46.417,9.408,0.1047);


  // EVent loop
  for(int i=0;i<nevents;i++){
    t->GetEntry(i);

    //event level    

    // Reference multiplicity
    int rmult = (int) t->McMult;
    CurrentEvent_RefMult = rmult;
    mMult->Fill(rmult);

    // centralities
    /*
    // 0-10% (> 162)
    if(rmult < 162) continue;
    */


    /*
    // 10-30% (74 - 162)
    if(rmult < 74) continue;
    if(rmult > 162) continue;

    */


    // 30-60% (17 - 74)
    //if(rmult < 17) continue;
    //if(rmult > 74) continue;


    CurrentEvent_B = 0.1*tesla*t->MagField;

    CurrentEvent_Vx = t->mRcVertexX;
    CurrentEvent_Vy = t->mRcVertexY;
    CurrentEvent_Vz = t->mRcVertexZ;

    CurrentEvent_McPrimVx = t->mMcVertexX;
    CurrentEvent_McPrimVy = t->mMcVertexY;
    CurrentEvent_McPrimVz = t->mMcVertexZ;

    mVzCorr->Fill(CurrentEvent_McPrimVz, CurrentEvent_Vz);
    mVxRes->Fill(CurrentEvent_Vx - CurrentEvent_McPrimVx);
    mVyRes->Fill(CurrentEvent_Vy - CurrentEvent_McPrimVy);
    mVzRes->Fill(CurrentEvent_Vz - CurrentEvent_McPrimVz);

    CurrentEvent_McPrimGVx = t->mMcGVertexX;
    CurrentEvent_McPrimGVy = t->mMcGVertexY;
    CurrentEvent_McPrimGVz = t->mMcGVertexZ;

    CurrentEvent_nTracks = t->mNRcTrk;
    CurrentEvent_nMcTracks = t->mNMcTrk;

    if(i%1000==0) {
      cout << "begin " << i << "th entry...." << endl;
      cout << "# of tracks = " << CurrentEvent_nTracks << endl;
      cout << "# of Mc tracks = " << CurrentEvent_nMcTracks << endl;
    }

    int ng = 0;

    //========= input mc loop for photon embedding starts=========//
    for(int imc = 0; imc < CurrentEvent_nMcTracks; imc++) {
      int gId = t->mGeantId[imc];

      //vipul
      //int index = t->mRcAssoOffset[imc];
      int pgId = t->mParentGeantId[imc];
      //if(pgId==0){
      //cout<<"Mc Geantid: "<<gId<<"\t Mc Paraent Gid: "<<pgId<< "mass of paticle:"<<t->mMcMass[imc]<<endl;
      //}

      if(gId!=1) continue;

      float mcStartX = t->mMcStartX[imc];
      float mcStartY = t->mMcStartY[imc];
      float mcStartZ = t->mMcStartZ[imc];

      // photon
      // bug in photon embedding, vertex not the same
      bool g_primary = false;
      if( fabs(mcStartX-CurrentEvent_McPrimGVx)<1.e-3 &&
          fabs(mcStartY-CurrentEvent_McPrimGVy)<1.e-3 &&
          fabs(mcStartZ-CurrentEvent_McPrimGVz)<1.e-3 ) {
        g_primary = true;
      }

      if(g_primary) {
        mMcGammaPtEta->Fill(t->mMcPt[imc], t->mMcEta[imc]);
        ng++;
      }
    } //========= input mc loop for photon embedding ends =========// 
    mNGamma->Fill(ng);

   
    //========= input mc loop for pion simulation starts =========//
    for(int imc = 0; imc < CurrentEvent_nMcTracks; imc++) {
      int gId = t->mGeantId[imc];

      float mcStartX = t->mMcStartX[imc];
      float mcStartY = t->mMcStartY[imc];
      float mcStartZ = t->mMcStartZ[imc];
      
      bool primary = false;
      if(fabs(mcStartX-CurrentEvent_McPrimVx)<1.e-3 && 
         fabs(mcStartY-CurrentEvent_McPrimVy)<1.e-3 &&
         fabs(mcStartZ-CurrentEvent_McPrimVz)<1.e-3 ) primary = true;

      if(gId!=8 &&gId!=9) continue;
      TLorentzVector pion(0.,0.,0.,0.);
      pion.SetPtEtaPhiM(t->mMcPt[imc],t->mMcEta[imc],t->mMcPhi[imc],0.13957);      
      if(pion.Pt()>5.0) continue;
      //      if(pion.Eta()>0.0||pion.Eta()<-1.0) continue; // change (we do not need in auau 9.2 gev
      //      if(pion.Rapidity()>0.0||pion.Rapidity()<-0.6) continue; // change
      if(pion.Rapidity()>0.25||pion.Rapidity()<-0.25) continue; // change


      mPiPtMc->Fill(pion.Pt());
      if(primary) mPiPtMcPrim->Fill(pion.Pt());
      
    }  //========= input mc loop for pion simulation ends =========//

    
    //========= Reconstructed track loop starts =========//
    for(int irc = 0; irc < CurrentEvent_nTracks; irc++) {
      if(abs(t->mRcNhits[irc])<16) continue;
    
    
/*
      bool hitTof = false;
      int charge = t->mRcNhits[irc]>0 ? +1 : -1;
      int    helixH     = (charge*CurrentEvent_B < 0.) ? 1 : -1;
      double helixC     = t->mHelixCurv[irc];
      double helixDip   = t->mHelixDip[irc]; 
      double helixPhase = t->mHelixPhase[irc];
      double helixX     = t->mHelixX[irc];
      double helixY     = t->mHelixY[irc];
      double helixZ     = t->mHelixZ[irc];

      TVector3 helixO(helixX,helixY,helixZ);
      StPhysicalHelixD helix(helixC,helixDip,helixPhase,helixO,helixH);

      pairD sPair = helix.pathLength(215.0);  // tof plane
      TVector3 projPoint = helix.at(sPair.first);
      float projPhi = projPoint.Phi()*180./Pi;
      float projZ = projPoint.Z();
      if( projPhi>-165. && projPhi<-135. && projZ<0 && projZ>-220.) hitTof = true;

      projPoint = helix.at(sPair.second);
      projPhi = projPoint.Phi()*180./Pi;
      projZ = projPoint.Z();
      if( projPhi>-165. && projPhi<-135. && projZ<0 && projZ>-220.) hitTof = true;

      if(!hitTof) continue;
*/
    
      int index = t->mRcAssoOffset[irc];

      if(index<0||index>=CurrentEvent_nMcTracks) {
        cout << " Ghost track !!! " << endl;
        continue;
      }

      float mcStartX = t->mMcStartX[index];
      float mcStartY = t->mMcStartY[index];
      float mcStartZ = t->mMcStartZ[index];
      TVector3 convPoint(mcStartX, mcStartY, mcStartZ);
      float mcR = convPoint.Perp();
      float mcPhi = convPoint.Phi()*180./Pi;

      int gId = t->mGeantId[index];
      int pgId = t->mParentGeantId[index];
      int po = t->mMcParentO[index];   // origin or not
      //cout<<"RC gid: "<<gId<<" \t"<<"PgId: "<<pgId<<endl;   //vipul
      float ptMc = t->mMcPt[index];
      float etaMc = t->mMcEta[index];
      float phiMc = t->mMcPhi[index];
      float ptParent = t->mMcParentPt[index];

      //cout<<"pgId = "<<pgId<<"\t gId = "<<gId<<endl;

      float wt = pplevy->Eval(ptParent);
//      cout << " ptParent = " << ptParent << " wt = " << wt << endl;

      float pt = t->mRcPt[irc];
      float eta = t->mRcEta[irc];
      float phi = t->mRcPhi[irc];
      float sdca = t->mSDCA[irc];

      // real MC primary
      bool primary = false;
      if(fabs(mcStartX-CurrentEvent_McPrimVx)<1.e-3 && 
         fabs(mcStartY-CurrentEvent_McPrimVy)<1.e-3 &&
         fabs(mcStartZ-CurrentEvent_McPrimVz)<1.e-3 ) primary = true;

      // selection used in real data analysis
      bool dataPrim = false;
      convPt->SetParameter(0,30.);
      //      if(sdca>-0.5 && sdca<convPt->Eval(pt)) dataPrim = true;  // change (default)
      if(sdca>-3.5 && sdca<convPt->Eval(pt)) dataPrim = true;

      if(pgId==16 || pgId==18){
	mKsLPtMc->Fill(ptMc);
      }
      // pions
      //      if((gId==8||gId==9)&&fabs(CurrentEvent_McPrimVz)<40.0) {
      //if((gId==8||gId==9)&&fabs(CurrentEvent_McPrimVz)<30.0 && fabs(CurrentEvent_McPrimVx*CurrentEvent_McPrimVx + (CurrentEvent_McPrimVy+0.89)*(CurrentEvent_McPrimVy+0.89))<1.0) {
      if((gId==8||gId==9)&&fabs(CurrentEvent_McPrimVz)<30.0)
	{
	  TLorentzVector pionMc(0.,0.,0.,0.);
	  pionMc.SetPtEtaPhiM(ptMc, etaMc, phiMc, 0.13957);
	  float yMc = pionMc.Rapidity();
	  //	if(etaMc>-1. && etaMc<0. && yMc>-0.6 && yMc<0.) {  // change 
	  if(yMc>-0.25 && yMc<0.25) {
	    
	    mPiPtMc->Fill(ptMc);
	    if(primary) mPiPtMcPrim->Fill(ptMc);
	  }
	  
	  TLorentzVector pionRc(0.,0.,0.,0.);
	  pionRc.SetPtEtaPhiM(pt, eta, phi, 0.13957);
	  float y = pionRc.Rapidity();
	  //        if(eta>-1. && eta<0. && y>-0.6 && y<0. && primary) { // change
	  if(y>-0.25 && y<0.25 && primary) {
	    mSDCA_PrimPi->Fill(pt, sdca);
	    if(dataPrim) {
	      mSDCACut_PrimPi->Fill(pt, sdca);
	    }
	  }

	  // change eta --> rap
	  //	if(dataPrim && eta>-1. && eta<0. && y>-0.6 && y<0.) { // change
	  if(dataPrim && y>-0.25 && y<0.25) {
	    //cout<<"pgId = "<<pgId<<"\t gId = "<<gId<<endl;
	    mPiParentGId->Fill(pt,pgId);
	    mPiPtRc->Fill(pt);        
	    if(primary)
	      {   
		mPiPtRcPrim->Fill(pt); 
		mPiPrimParentGId->Fill(pt, pgId);
	      }
	    else if(pgId>0)
	      {
		if (gId==0){ 
		cout<<"Mc Geantid: "<<gId<<"\t Paraent Gid: "<<pgId<<endl; //"mass of paticle:"<<t->mMcMass[irc]<<endl;
		}
		mPiTotDecay->Fill(pt);  // all   ==========change 1
		mPiNonPrimParentGId->Fill(pt, pgId);
		if (pgId==16 || pgId==18) {
		  mPiPtKs->Fill(pt);
		}
		else
		  {
		    mPiPtOther->Fill(pt);
		  }
	      }
	  } // end if (dataPrim...
	} // end if (pion)

      // muon loop  (5||6) {muon mass = 0.10565} ====== change2
      //if((gId==5||gId==6)&&fabs(CurrentEvent_McPrimVz)<30.0 && fabs(CurrentEvent_McPrimVx*CurrentEvent_McPrimVx + (CurrentEvent_McPrimVy+0.89)*(CurrentEvent_McPrimVy+0.89))<1.0) {
      if((gId==5||gId==6)&&fabs(CurrentEvent_McPrimVz)<30.0) {
	TLorentzVector muonRc(0.,0.,0.,0.);
	muonRc.SetPtEtaPhiM(pt, eta, phi, 0.10565);
	float y = muonRc.Rapidity();	
	if(dataPrim && y>-0.25 && y<0.25) {
	  mMuParentGId->Fill(pt,pgId);
	  if(pgId ==8 || pgId ==9){
	    mMuPtRc->Fill(pt);
	  }
	  if(primary) {
            mMuPtRcPrim->Fill(pt);
	    mMuPrimParentGId->Fill(pt,pgId);
	  } //primary
	} // data prim condtion
      } // Muon loop ends


      // electrons
      //if((gId==2||gId==3)&&fabs(CurrentEvent_McPrimGVz)<30.0 && fabs(CurrentEvent_McPrimGVx*CurrentEvent_McPrimGVx + (CurrentEvent_McPrimGVy+0.89)*(CurrentEvent_McPrimGVy+0.89))<1.0) {
      if((gId==2||gId==3)&&fabs(CurrentEvent_McPrimGVz)<40.0) {
        TLorentzVector eRc(0.,0.,0.,0.);
        eRc.SetPtEtaPhiM(pt, eta, phi, 0.000511);
        float y = eRc.Rapidity();

        mEPtEta->Fill(pt, eta);
        mEPtPhi->Fill(pt, phi*180./Pi);
        mENHits->Fill(abs(t->mRcNhits[irc]));

        if(eta>-1.0 && eta<0. && y>-0.6 && y<0.) {
          mSDCA->Fill(pt, sdca);
          mSDCAWt->Fill(pt, sdca, wt); 
         if(primary) mSDCA_Prim->Fill(pt, sdca);
          if(pgId==1&&po==1) {   // conversion electrons from embedde photon
            if(fabs(mcStartZ)<80.) {
              mConvRZ->Fill(mcStartZ, mcR);
              mConvRPhi->Fill(mcPhi, mcR);
            }

            double rcRho = 1./t->mHelixCurv[irc];
            double ss = sdca + rcRho;
            double rcR = sqrt(ss*ss - rcRho*rcRho);
            mMcRcR->Fill(mcR, rcR);
            mMcRcR->Fill(mcR, rcR, wt);

            if(mcR>3.8&&mcR<4.2) {
              mSDCA_BP->Fill(pt, sdca);
              mSDCAWt_BP->Fill(pt, sdca, wt);
            } else if(mcR>46.&&mcR<48.) {
              mSDCA_IFC->Fill(pt, sdca);
              mSDCAWt_IFC->Fill(pt, sdca, wt);
            } else if(mcR<30.) {
              mSDCA_Air->Fill(pt, sdca);
              mSDCAWt_Air->Fill(pt, sdca, wt);
            }
          }

          if(dataPrim) {  // cut
            mSDCACut->Fill(pt, sdca);
            if(primary) mSDCACut_Prim->Fill(pt, sdca);
            if(pgId==1&&po==1) {
              if(mcR>3.8&&mcR<4.2) {
                mSDCACut_BP->Fill(pt, sdca);
                mSDCAWtCut_BP->Fill(pt, sdca, wt);
              } else if(mcR>46.&&mcR<48.) {
                mSDCACut_IFC->Fill(pt, sdca);
                mSDCAWtCut_IFC->Fill(pt, sdca, wt);
              } else {
                mSDCACut_Air->Fill(pt, sdca); 
                mSDCAWtCut_Air->Fill(pt, sdca, wt); 
              }
            }
          }         

        }


      } // end if (electron)

    } // end irc  //========= Reconstructed track loop ends =========//
    
  } // end event looping


  ///////////////////////
  // histograms output
  ///////////////////////
  TFile *f2 = new TFile(FileOutPut1,"recreate");
  f2->cd();
  mMult->Write();
  mVzCorr->Write();
  mVxRes->Write();
  mVyRes->Write();
  mVzRes->Write();

  mPiParentGId->Write();
  mPiPrimParentGId->Write();
  mPiNonPrimParentGId->Write();
  mPiPtMc->Write();
  mKsLPtMc->Write();
  mPiPtMcPrim->Write();
  mPiPtRc->Write();
  mPiPtRcPrim->Write();
  mPiPtKs->Write();
  mPiPtOther->Write();
  mPiTotDecay->Write();

  mMuParentGId->Write();
  mMuPtRc->Write();
  mMuPtRcPrim->Write();
  mMuPrimParentGId->Write();

  mSDCA->Write();
  mSDCA_Prim->Write();
  mSDCA_PrimPi->Write();
  mSDCA_BP->Write();
  mSDCA_Air->Write();
  mSDCA_IFC->Write();

  mSDCACut->Write();
  mSDCACut_Prim->Write();
  mSDCACut_PrimPi->Write();
  mSDCACut_BP->Write();
  mSDCACut_Air->Write();
  mSDCACut_IFC->Write();

  mSDCAWt->Write();
  mSDCAWt_BP->Write();
  mSDCAWt_Air->Write();
  mSDCAWt_IFC->Write();

  mSDCAWtCut->Write();
  mSDCAWtCut_BP->Write();
  mSDCAWtCut_Air->Write();
  mSDCAWtCut_IFC->Write();

  mEPtEta->Write();
  mEPtPhi->Write();
  mENHits->Write();

  mConvRZ->Write();
  mConvRPhi->Write();
  mMcRcR->Write();
  mMcRcRWt->Write();

  mMcGammaPtEta->Write();
  mNGamma->Write();

  mPiOtherXY->Write();

  f2->Close();
  cout<<"end of program"<<endl;
  return(0);

  exit(0);
}

Double_t pplevyFun(Double_t *x, Double_t *par)
{
  double A = par[0];
  double n = par[1];
  double c = par[2];
  double Pi = 3.1415927;
  double m = 0.13957;
  double mT = sqrt(x[0]*x[0]+m*m) - m;
  double co = A*(n-1)*(n-2)/(2.*Pi*n*c*(n*c+m*(n-2)));
  return co*pow((1+mT)/n/c,-n)*x[0];
}
  

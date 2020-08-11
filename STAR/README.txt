Инструкция к запуску библиотеки STAR и примеров Григория:
1) Сначала нужно скомпилировать библиотеки:
Для этого открываете в терминале директорию, в которой лежит папка StPicoEvent, затем набираете: 
make
(просто make, больше ничего не нужно)

Если успешной компиляции не произошло, можно написать мне в социальную сеть вк (https://vk.com/s.manukhov00) или на почту (manukhov.2000@mail.ru). 

2) Для запуска второго примера нужно в начале файла runPicoDstAnalysisWithMacro.C написать от руки все используемые в дальнейшем библиотеки:
(начало файла может выглядить так):


// This is needed for calling standalone classes (not needed on RACF)
#define _VANILLA_ROOT_

// C++ headers
#include <iostream>

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



// PicoDst headers
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoDstReader.h"
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoDst.h"
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoEvent.h"
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoTrack.h"
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoBTofHit.h"
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoBTowHit.h"
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoEmcTrigger.h"
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoBTofPidTraits.h"
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoTrackCovMatrix.h"

// Load libraries (for ROOT_VERSTION_CODE >= 393215)
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0) 
R__LOAD_LIBRARY(/home/manukhov/DUBNA/STAR/StPicoEvent/libStPicoDst.so)
#endif



//_________________
void runPicoDstAnalysisWithMacro(const char* inFileName = "/home/manukhov/DUBNA/STAR/st_physics_19130071_raw_2000011.picoDst.root") {

  std::cout << "Lets run the StPicoDstAnalysisMaker, Master" << std::endl;


(далее идет остальной код программы)

Имейте в виду, путь до библиотек и до файла picoDst.root нужно будет поменять.

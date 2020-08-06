Как это робит:
1) Сначала нужно скомпилировать библиотеки:
Для этого открываете в вашем терминале директорию, в которой лежит папка StPicoEvent, затем вбиваете 
make
(просто make, больше ничего не нужно)

Если успешной компиляции не произошло, можно написать мне в вк (https://vk.com/s.manukhov00) или на почту (manukhov.2000@mail.ru), тут у меня возникали проблемсы на одном из двух компьютеров, на которых я запускал эту компиляцию, может помогу. 

2) Далее пробуем запускать примеры. Сразу говорю, первый и третий пример у меня не запускается, так как, видимо, я не подключен к серверу стара (сейчас я пытаюсь решить эту проблему, надеюсь к вечеру 6 августа получится). Поэтому начнем со второго. Там в начале кода нужно будет от руки писать подключаемые классы. У меня это выглябит так (начало файла runPicoDstAnalysisWithMacro.C):

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

3) Собственно сами picoDst файлы можно скачать, например, тут (выложил в майловское облако; три файла, можно использовать, в принципе, любой):
https://cloud.mail.ru/public/2X2e/ySGLaNfEJ
https://cloud.mail.ru/public/pNRT/2zz3smpdA
https://cloud.mail.ru/public/5wjg/RysiJBKv6

#include "ROOT/RMakeUnique.hxx"
#include "TString.h"
#include "TROOT.h"
#include "TChain.h"
#include "TFileCollection.h"
#include "TH1F.h"
#include "TTreeReader.h"
#include "ROOT/TTreeProcessorMP.hxx"
#include "processD.h"


int wrapper(){

TChain* T=new TChain("T");
T->Add("/global/scratch/rahmans/scratch/targetStudy/beamUpstreamR0.0/smallstat/t-750mmto750mm/moller.root");

/*
gROOT->SetBatch(kTRUE);
TString selectorPath="/home/rahmans/analysis/BeamSteeringAnalysis/colprocessD.C";
auto sel=TSelector::GetSelector(selectorPath);
sel->SetOption("/home/rahmans/analysis/BeamSteeringAnalysis/coldefault.txt,default");

ROOT::TTreeProcessorMP pool(4);

pool.Process(*T,*sel);
//sel->GetOutputList()->Delete();
*/
T->Process("/home/rahmans/analysis/TargetAnalysis/processD.C+","/global/scratch/rahmans/root/targetStudy,t-750mmto750mm,moller");

return 0;
}



#include "ROOT/RMakeUnique.hxx"
#include "TString.h"
#include "TROOT.h"
#include "TChain.h"
#include "TFileCollection.h"
#include "TH1F.h"
#include "TTreeReader.h"
#include "ROOT/TTreeProcessorMP.hxx"



int draw(TString source, TString out){

TChain* T=new TChain("T");
Int_t n=T->Add(source.Data());

TCut track= "hit.trid==1||hit.trid==2";
TCut w= Form("rate/1e9/70/%i",n);

TCut pDet= "hit.det==32 && hit.r>=885 && hit.r<990";
TCut segDet= "(hit.det>50000&&hit.det<50100)|| (hit.det>51000 && hit.det<51100)|| (hit.det>52000 &&hit.det<52100)";

std::vector<TCanvas *> c;
c.push_back(new TCanvas("c1", "c1", 800,600));
c.push_back(new TCanvas("c2", "c2", 800,600));


std::vector<TH1F *> h;
h.push_back(new TH1F("h1", "h1", 140, 600,1300));
h.push_back(new TH1F("h2", "h2", 140, 600,1300));

c[0]->cd();
T->Draw("hit.r>>h1", track*w*pDet);
c[1]->cd();
T->Draw("hit.r>>h2", track*w*segDet);

c[0]->Print(out+".png");
c[1]->Print(out+".png");


 



std::cout<<n <<std::endl;



return 0;
}



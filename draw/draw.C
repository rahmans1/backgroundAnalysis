#include "ROOT/RMakeUnique.hxx"
#include "TString.h"
#include "TROOT.h"
#include "TChain.h"
#include "TFileCollection.h"
#include "TH1F.h"
#include "TTreeReader.h"
#include "ROOT/TTreeProcessorMP.hxx"
#include <iostream>
#include <fstream>


int draw(TString source, TString out){

TChain* T=new TChain("T");
Int_t n=T->Add(source.Data());
ofstream outfile(out+".txt");
outfile<< "The following output was produced by "<< 5000*n<<"primary events."  << endl;
outfile<< "Detector\t\t\tIntegral\t\t\tError";


TCut track= "hit.trid==1||hit.trid==2";
TCut w= Form("rate/1e9/70/%i",n);

TCut pDet= "hit.det==32 && hit.r>=885 && hit.r<990";
TCut segDet="";




std::vector<TH1F *> h;
std::vector<TH1F *> h1;
std::vector<TH1F *> h2;
std::vector<TH1F *> h3;
std::vector<TH1F *> h4;
std::vector<TH2F *> h5o;
std::vector<TH1F *> h5t;
std::vector<TH1F *> h5c;
std::vector<TH1F *> h6; 

h.push_back(new TH1F("h1", "h1", 140, 600,1300));


TCanvas *c1=new TCanvas("c1","c1", 800,600);
for (int i=60001;i<=60028;i++){
h5o.push_back(new TH2F(Form("h%i",i), Form("h%i",i), 520,-1300,1300,520, -1300,1300));
segDet=Form("hit.det==%i",i);
T->Draw(Form("hit.y:hit.x>>h%i",i), track*w*segDet, "colzsame");
}
c1->Print(out+"1.png");










TCanvas *c=new TCanvas("c","c", 800,600);
T->Draw("hit.r>>h1", track*w*pDet);
c->Print(out+".png");






return 0;
}



#define processD_cxx
// The class definition in processD.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("processD.C")
// root> T->Process("processD.C","some options")
// root> T->Process("processD.C+")
//


#include "processD.h"
#include <TH2.h>
#include <TStyle.h>


TString folder;
TString tag;
TString generator;
TString particle;
TFile* rootfile;

std::map<TString,Int_t> pid;
std::map<Int_t,Int_t> det;
std::map<TString,Int_t> maxtr;

std::map<Int_t,Float_t > openmin;
std::map<Int_t, Float_t > transmin;
std::map<Int_t, Float_t> closedmin;
std::map<Int_t,Float_t > openmax;
std::map<Int_t, Float_t > transmax;
std::map<Int_t, Float_t> closedmax;


std::vector<TH1F*> r; 
std::vector<TH2F*> xy; 
std::vector<TH1F*> t; 
std::vector<TH1F*> tcom; 
std::vector<TH1F*> asy; 
//std::vector<TH1F*> ene;

std::vector<TH2F*> xy_all; 
std::vector<TH1F*> r_all; 

Int_t nFiles;
Int_t evPerFile;
Float_t eneCut;
Float_t weight;
Bool_t goodhit;

void processD::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   TObjArray *opt= option.Tokenize(",");
   folder=((TObjString *)(opt->At(0)))->String();
   tag= ((TObjString *)(opt->At(1)))->String();
   generator=((TObjString *)(opt->At(2)))->String();
   rootfile=new TFile(Form("%s/%s_%s.root",folder.Data(),tag.Data(),generator.Data()) , "RECREATE");
   
   particle="e-";
   nFiles=10;
   weight=1/(1e9*85*7*nFiles);
   eneCut=1;
   pid["e-"]=11;
   det[28]=0;
   det[32]=1;

   maxtr["moller"]=2;
   maxtr["elastic"]=1;
   maxtr["inelastic"]=1;

   openmin[28]=935; openmax[28]=1040;
   transmin[28]=960; transmax[28]=1075;
   closedmin[28]=960; closedmax[28]=1100;

   openmin[32]=885; openmax[32]=990;
   transmin[32]=885; transmax[32]=990;
   closedmin[32]=885; closedmax[32]=990;

   for(int i=0;i<2;i++){
     r_all.push_back(new TH1F(Form("r_all_%i",i),"rate (GHz/uA/sep/5mm) vs r(mm)",140,600,1300));
     xy_all.push_back(new TH2F(Form("xy_all_%i",i), "rate (GHz/uA/sep/(5mm)^2) vs xy(mm^2)",  140, -1300, -600, 240, -600, 600));
     for(int j=0;j<4;j++){
	r.push_back(new TH1F(Form("r_%i_%i",i,j),"rate (GHz/uA/sep/5mm) vs r(mm)",140,600,1300));
        xy.push_back(new TH2F(Form("xy_%i_%i",i,j), "rate (GHz/uA/sep/(5mm)^2) vs xy(mm^2)",  140, -1300, -600, 240, -600, 600));
        t.push_back(new TH1F(Form("t_%i_%i",i,j), "rate (GHz/uA/sep/rad) vs theta (rad)", 100,0, 0.03)); 
	if (generator=="moller") {
   		tcom.push_back(new TH1F(Form("tcom_%i_%i",i,j), "rate (GHz/uA/sep/deg) vs theta_com (deg)", 180,0, 180)); //moller only
  		asy.push_back(new TH1F(Form("asy_%i_%i",i,j), "rate (GHz/uA/sep/0.2ppb) vs asymmetry (ppb)",300, -60,0)); //moller
  	}else if (generator=="elastic") {
   		asy.push_back(new TH1F(Form("asy_%i_%i",i,j), "rate (GHz/uA/sep/3ppb)  vs asymmetry (ppb)",50, -150,0)); //elastic
 	}else{
   		asy.push_back(new TH1F(Form("asy_%i_%i",i,j), "rate (GHz/uA/sep/5ppb) vs asymmetry (ppb)",400, -2000,0)); //inelastic
   	}  
     }
   }




}

void processD::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   
  
}

Bool_t processD::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // When processing keyed objects with PROOF, the object is already loaded
   // and is available via the fObject pointer.
   //
   // This function should contain the \"body\" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   fReader.SetLocalEntry(entry);
   goodhit=false;
   for (auto i=0;i<hit_det.GetSize();i++){ 
   goodhit=(hit_pid[i]==pid[particle.Data()])&&(hit_det[i]==28||hit_det[i]==32) && hit_trid[i]<=maxtr[generator.Data()] ;
   if (!goodhit){continue;}
   
   Double_t sepdiv=2*TMath::Pi()/7.0;
   TVector2 XY(hit_x[i],hit_y[i]);
   Int_t sec=0;
   Double_t phi=atan2(hit_y[i],hit_x[i]);
   if (phi<0) {phi+=2*TMath::Pi();}
  
   Double_t secphi = fmod(phi, 2*TMath::Pi()/7);
   if (secphi<TMath::Pi()/28.0){sec=1;}           // closed 
   else if (secphi<3*TMath::Pi()/28.0){sec=2; }    // transition
   else if (secphi<5*TMath::Pi()/28.0) {sec=3;}   // open
   else if (secphi<7*TMath::Pi()/28.0) {sec=2;}   // transition
   else {sec=1;}    // closed
   
   /*----------------Rotating hits from other septant into one septant since field map mirrors. --------------------*/
    if(phi==0){XY=XY.Rotate(3*sepdiv);}
    else{XY=XY.Rotate(sepdiv*round((TMath::Pi()-phi)/sepdiv));}  
    

  /*----------------------------------------------------------------------------------------------------------------*/
   
   if(hit_r[i]>600){
   xy_all[det[hit_det[i]]]->Fill(XY.X(),XY.Y(), (*rate)*weight);
   }
   r_all[det[hit_det[i]]]->Fill(hit_r[i], (*rate)*weight);


   if( (sec==3 && (hit_r[i]<openmin[hit_det[i]] ||  hit_r[i]>=openmax[hit_det[i]]))
       || (sec==2 && (hit_r[i]<transmin[hit_det[i]] ||  hit_r[i]>=transmax[hit_det[i]]))
       || (sec==1 && (hit_r[i]<closedmin[hit_det[i]] || hit_r[i]>=closedmax[hit_det[i]]))){
   continue;
   }
   xy[4*det[hit_det[i]]]->Fill(XY.X(),XY.Y(), (*rate)*weight);
   xy[4*det[hit_det[i]]+sec]->Fill(XY.X(),XY.Y(), (*rate)*weight);
   r[4*det[hit_det[i]]]->Fill(hit_r[i],(*rate)*weight);
   r[4*det[hit_det[i]]+sec]->Fill(hit_r[i],(*rate)*weight);
   asy[4*det[hit_det[i]]]->Fill(*A,(*rate)*weight);
   asy[4*det[hit_det[i]]+sec]->Fill(*A,(*rate)*weight);
   t[4*det[hit_det[i]]]->Fill(part_th[hit_trid[i]-1],(*rate)*weight);
   t[4*det[hit_det[i]]+sec]->Fill(part_th[hit_trid[i]-1],(*rate)*weight);
   if (generator=="moller") {
         Double_t thetacom=0;
         if (hit_trid[i]==2){ thetacom=(TMath::Pi()-*thcom)*(180/TMath::Pi());}
         else { thetacom=(*thcom)*(180/TMath::Pi());}
	 tcom[4*det[hit_det[i]]]->Fill(thetacom,(*rate)*weight);
         tcom[4*det[hit_det[i]]+sec]->Fill(thetacom,(*rate)*weight);
   }
   
   }


   return kTRUE;
}

void processD::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void processD::Terminate()
{

   
   for(int i=0;i<2;i++){
     r_all[i]->Write("", TObject::kOverwrite); 
     xy_all[i]->Write("", TObject::kOverwrite);
     for(int j=0;j<4;j++){
	r[4*i+j]->Write("", TObject::kOverwrite);
        xy[4*i+j]->Write("", TObject::kOverwrite);  
        t[4*i+j]->Write("", TObject::kOverwrite);  
 	if (generator=="moller") {
   		tcom[4*i+j]->Write("", TObject::kOverwrite);  //moller only
  		asy[4*i+j]->Write("", TObject::kOverwrite);   //moller
  	}else if (generator=="elastic") {
   		asy[4*i+j]->Write("", TObject::kOverwrite);   //elastic
 	}else{
   		asy[4*i+j]->Write("", TObject::kOverwrite);   //inelastic
   	}  
     }
   }

 
   rootfile->Close();
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

}

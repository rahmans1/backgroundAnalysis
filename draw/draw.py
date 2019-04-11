import sys
import os
import ROOT as R

source = sys.argv[1]
output = sys.argv[2]

T=R.TChain("T")
T.Add(source)    ## Source Root File

h=[]
c=[]


c1=R.TCut("hit.det==32")
c2=R.TCut("hit.trid==1||hit.trid==2")
c3=R.TCut("hit.r>=885 && hit.r<990")
c4=R.TCut("(hit.det>50000&&hit.det<50100)&& (hit.det>51000 && hit.det<51100)&& (hit.det>52000 &&hit.det<52100)")


w=R.TCut("rate/(1e9*70)")  ## Weight with respect to rate and current GHz/uA



h.append(R.TH1F("h1", "h1", 140,600,1200))
h.append(R.TH1F("h2", "h2", 140,600,1200))

c.append(R.TCanvas("c1","c1",800,600))
c.append(R.TCanvas("c2","c2", 800,600))

c[0].cd()
T.Draw("hit.r>>h1", c1*c2*c3*w)
c[1].cd()
T.Draw("hit.r>>h2", c2*c4*w)


c[0].Print(output+"1.png")
c[1].Print(output+"2.png")

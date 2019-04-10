import sys
import os
import ROOT as R



T=R.TChain("T")

T.Add(sys.argv[1])    ## Source Root File
T.Process("/home/rahmans/analysis/TargetAnalysis/processD.C+",sys.argv[2]+","+sys.argv[3]+","+sys.argv[4])   ## Destination Root Folder, tag, generator

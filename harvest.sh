for((i=0;i<7;i++)) do
    echo "void cen$i() {" > cen$i.C
    echo  "thesis();" >> cen$i.C
    echo " // this code is automatically generated - you can change it, but better change the generator" >> cen$i.C
    cat ../systematics.log | grep cen$i >> cen$i.C
    tests=(v2_def v2_vtx12 v2_vtx5 v2_CL0cent v2_CL1cent v2_nopileup v2_esdDif700 v2_remCh46 v2_def_negEta v2_def_posEta v2_ncls60 v2_ncls80 v2_nclspid60 v2_nclspid90 v2_ncrfind07 v2_ncrfind09 v2_DCATrkPV005 v2_DCATrkPV03 v2_DCADght07 v2_DCADght03 v2_cosPA098 v2_minRad0 v2_minRad4 v2_minRad6 v2_maxRad50 v2_maxRad150 v2_armPodCut v2_nsig1 v2_nsig4 v2_looseV0 v2_tightV0 v2_ptCutDght v2_dEdxCutDght v2_minRad1 v2_minRad3 v2_minRad7 v2_minRad9 v2_minRad12 v2_minRad15 v2_tightV0Rad9DedxPt)
    echo "  TCanvas* c = new TCanvas();" >> cen$i.C
    echo "  c->Divide(2,1); " >> cen$i.C
    echo "  c->cd(1);" >> cen$i.C
    echo "  const Int_t nPtBins = 17; " >> cen$i.C
    echo "  Double_t binsPt[nPtBins+1] = {0.2, 0.5, 0.8, 1.1, 1.4, 1.7, 2.0, 2.5, 3.0, 3.5, 4.0, 5.0, 6.0, 8.0, 10.0, 13.0, 16.0, 20.0}; " >> cen$i.C     
    echo "  TH1D* hvnK0 = new TH1D(\"hv2K0_$i\", \"hv2K0; p_{T} (GeV/c); v_{2}{2, |#Delta#eta|>2}\", nPtBins, binsPt); " >> cen$i.C
    echo " for (int i = 0; i < nPtBins; i++) {">> cen$i.C
    echo "    hvnK0->SetBinContent(i+1, v2_def_cen$i[i]);">> cen$i.C
    echo " };">> cen$i.C
    for((j = 0; j < ${#tests[@]}; ++j)); do
       echo " TH1D* h${tests[$j]}_cen$i = new TH1D(\"${tests[$j]}_cen$i\", \"${tests[$j]}; p_{T} (GeV/c); v_{2}{2, |#Delta#eta|>2}\", nPtBins, binsPt); " >> cen$i.C
       echo " h${tests[$j]}_cen$i->SetLineColor((1+$j)*($i+1)); " >> cen$i.C 
       echo " for (int i = 0; i < nPtBins; i++) {">> cen$i.C
       echo "    h${tests[$j]}_cen$i->SetBinContent(i+1, ${tests[$j]}_cen$i[i]);">> cen$i.C
       echo " };">> cen$i.C 
    done
    echo "h${tests[1]}_cen$i->DrawCopy();">> cen$i.C
    for((j = 2; j < ${#tests[@]}; ++j)); do
        echo " h${tests[$j]}_cen$i->DrawCopy(\"same\");">> cen$i.C
    done
    echo "gPad->BuildLegend();">>cen$i.C
    echo "c->cd(2);">>cen$i.C


    echo " double syst[nPtBins];" >> cen$i.C
    echo " for (int i = 0; i < nPtBins; i++) syst[i] = 0,;" >> cen$i.C
    echo " for (int i = 0; i < nPtBins; i++) {">> cen$i.C
    for((j = 0; j < ${#tests[@]}; ++j)); do
        echo "    syst[i] += h${tests[$j]}_cen$i->GetBinContent(i+1)*h${tests[$j]}_cen$i->GetBinContent(i+1);">> cen$i.C
    done
    echo "};">> cen$i.C
    echo " for (int i = 0; i < nPtBins; i++) {">> cen$i.C
    echo " if(syst[i] > 0) syst[i] = TMath::Sqrt(syst[i]);">>cen$i.C
    echo "    hvnK0->SetBinError(i+1, v2_def_cen$i[i]*syst[i]);">> cen$i.C
    echo "};">>cen$i.C
    echo "hvnK0->GetYaxis()->SetRangeUser(-.05, .25);" >> cen$i.C
    echo "hvnK0->DrawCopy();">> cen$i.C

    echo "c->SaveAs(\"systs_cen$i.png\");">> cen$i.C
    echo "c->SaveAs(\"systs_cen$i.pdf\");">> cen$i.C
    echo "c->SaveAs(\"systs_cen$i.C\");">> cen$i.C
    echo " }"  >> cen$i.C
    root -l -b -q cen$i.C
done

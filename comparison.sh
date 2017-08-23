set -u
echo " void comparison() {"> comparison.C
for((i=0;i<8;i++)) do
    sed '1d;$d' vsysts_cen$i.C >> comparison.C
done
echo "gStyle->SetOptFit(1);">> comparison.C
echo "TCanvas* c = new TCanvas();" >> comparison.C
echo "c->Divide(3,3);">> comparison.C
echo "TFile f(\"/home/rbertens/Documents/dobrin/v2_pid_V0A.root\");">>comparison.C
echo "TGraphErrors* k[8];">> comparison.C
echo "TGraphErrors* p[8];">> comparison.C
echo "TGraphErrors* pi[8];">> comparison.C
echo "TSpline3* spline[8];">> comparison.C
echo "const Int_t nPtBins = 17; ">> comparison.C
echo "Double_t binsPt[nPtBins+1] = {0.2, 0.5, 0.8, 1.1, 1.4, 1.7, 2.0, 2.5, 3.0, 3.5, 4.0, 5.0, 6.0, 8.0, 10.0, 13.0, 16.0, 20.0}; ">> comparison.C
echo "TCanvas* s = new TCanvas();" >> comparison.C
echo "s->Divide(3,3);">> comparison.C
for((i=0;i<8;i++)) do
    echo "p[$i] = (TGraphErrors*)f.Get(\"grV2Pn_$i\");">> comparison.C
    echo "k[$i] = (TGraphErrors*)f.Get(\"grV2Kn_$i\");">> comparison.C
    echo "pi[$i] = (TGraphErrors*)f.Get(\"grV2Pin_$i\");">> comparison.C
    echo "spline[$i] = new TSpline3(\"kcharged_$i\", k[$i]->GetX(), k[$i]->GetY(), k[$i]->GetN());">> comparison.C
    echo " TH1D* splineHist$i = new TH1D(\"splineHist$i\", \"splineHist$i; p_{T} (GeV/c); v_{2}{2, |#Delta#eta|>2}\", nPtBins, binsPt); ">> comparison.C
    echo "for (int i = 0; i < nPtBins; i++) {">>comparison.C
    echo "     splineHist$i->SetBinContent(i+1, spline[$i]->Eval(splineHist$i->GetBinCenter(i+1)));">>comparison.C
    echo "}">> comparison.C
    echo "s->cd($i+1);">> comparison.C
    echo "splineHist$i->Divide(hv2K0_$i"_"_33);">> comparison.C
    echo "splineHist$i->GetXaxis()->SetRangeUser(0.5, 4);">> comparison.C
    echo "TF1* line$i = new TF1(\"line$i\", \"pol0\",.8, 4);">> comparison.C
    echo "splineHist$i->Fit(line$i);">> comparison.C
    echo "splineHist$i->GetYaxis()->SetTitle(\"KCH/K0\");">> comparison.C
    echo "splineHist$i->GetYaxis()->SetRangeUser(.95, 1.1);">> comparison.C
    echo "splineHist$i->DrawCopy(\"hist\");">> comparison.C
    echo "line$i->SetLineStyle(3);">>comparison.C
    echo "line$i->DrawCopy(\"same\");">> comparison.C
done
echo "s->SaveAs(\"spline_ratios.pdf\");">> comparison.C

for((i=0;i<8;i++)) do
    echo "c->cd($i+1);">> comparison.C
    echo "gPad->SetBottomMargin(.2);">>comparison.C
    echo "gPad->SetLeftMargin(.2);">>comparison.C
    echo "hv2K0_$i"_"_33->GetXaxis()->SetRangeUser(.5, 4);">>comparison.C
    echo "hv2K0_$i"_"_33->DrawCopy(\"E2\");">> comparison.C
    echo "class$i"_"_34->DrawCopy(\"same\");">>comparison.C
    echo "p[$i]->SetLineWidth(3);">>comparison.C
    echo "p[$i]->DrawClone(\"p same\");">> comparison.C
    echo "k[$i]->SetLineWidth(3);">>comparison.C
    echo "k[$i]->DrawClone(\"p same\");">> comparison.C
    echo "pi[$i]->SetLineWidth(3);">>comparison.C
    echo "pi[$i]->DrawClone(\"p same\");">> comparison.C
    echo "spline[$i]->SetLineColor(kBlack);">> comparison.C
    echo "spline[$i]->SetLineStyle(7);">> comparison.C
    echo "spline[$i]->SetLineWidth(3);">> comparison.C
    echo "spline[$i]->Draw(\"same\");">> comparison.C
done


echo "c->SaveAs(\"comparison.pdf\");">>comparison.C
echo "}">>comparison.C

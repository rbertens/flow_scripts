set -u
echo " void canvas() {"> canvas.C
for((i=0;i<8;i++)) do
    sed '1d;$d' vsysts_cen$i.C >> canvas.C
done
echo "TCanvas* c = new TCanvas();" >> canvas.C
echo "c->Divide(3,3);">> canvas.C
for((i=0;i<8;i++)) do
    echo "c->cd($i+1);">> canvas.C
    echo " gPad->SetBottomMargin(0.2);">> canvas.C
    echo " gPad->SetLeftMargin(0.2);">>canvas.C
    echo "hv2K0_$i"_"_33->SetMarkerSize(1.2);">>canvas.C
    echo "hv2K0_$i"_"_33->GetYaxis()->SetTitle(\"v_{2}{2, |#Delta#eta|>2}\");">> canvas.C
    echo "hv2K0_$i"_"_33->DrawCopy(\"E2\");">> canvas.C
    echo "hv2K0_$i"_"_33->DrawCopy(\"E2\");">> canvas.C
    echo "class$i"_"_34->DrawCopy(\"same\");">>canvas.C
done
echo "c->SaveAs(\"overview.pdf\");">>canvas.C
echo "}">>canvas.C

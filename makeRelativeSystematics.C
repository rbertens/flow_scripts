double passBarlow(TH1* a, TH1* b, int i, double limit = 1) {
    // return a syst if point is barlow significant
    double va = a->GetBinContent(i);
    double vb = b->GetBinContent(i);
    double ea = a->GetBinError(i);
    double eb = b->GetBinError(i);

    double temp = TMath::Abs(ea*ea-eb*eb);
    if(temp >0 ) temp = TMath::Sqrt(temp);
    else return 0;

    if((TMath::Abs(va-vb)/temp) > limit) {
        if (va/vb > 1) return va/vb - 1.;
        else return (TMath::Abs(1.-va/vb));
    }
    else return 0;

}








void makeRelativeSystematics(TString fileA, TString fileB) {

    TFile a(Form("%s/hist_%s.root", fileA.Data(), fileA.Data()));
    TFile b(Form("%s/hist_%s.root", fileB.Data(), fileB.Data()));

    TCanvas *c = new TCanvas("ratios", "ratios", 2088, 1416);
    c->Divide(3,3);


    a.ls();
    b.ls();

    for(Int_t i = 0; i < 7; i++) {
        c->cd(i+1);

        TH1D* ha = (TH1D*)a.Get(Form("hv2K0_%i", i));
        TH1D* hb = (TH1D*)b.Get(Form("hv2K0_%i", i));

        if(!ha || !hb) {
            ha = (TH1D*)a.Get(Form("hv3K0_%i", i));
            hb = (TH1D*)b.Get(Form("hv3K0_%i", i));
        }



        if(ha&&hb) { 
            if(fileA!=fileB) {
                for(int j = 1; j < ha->GetXaxis()->GetNbins()+1; j++) {
                    double syst = passBarlow(ha, hb, j);
                    ha->SetBinContent(j, syst);
                }
                ha->DrawCopy();
            }
        }
        cout << " double " << fileB.Data() << "_cen" << i << "[] = {";
        for(int j = 1; j < ha->GetXaxis()->GetNbins()+1; j++) {
            if (j < ha->GetXaxis()->GetNbins()) cout << ha->GetBinContent(j) << ", ";
            else cout << ha->GetBinContent(j) << "}; // AUTOGEN" << endl;
        }

    }
    c->SaveAs(Form("uncertainties/%s_over_%s.png", fileA.Data(), fileB.Data()));
}






void makeRatios(TString fileA, TString fileB) {

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
        if(!ha ||!hb) {
            ha = (TH1D*)a.Get(Form("hv3K0_%i", i));
            hb = (TH1D*)b.Get(Form("hv3K0_%i", i));
        }


        if(ha&&hb) {
            ha->GetYaxis()->SetRangeUser(.8, 1.2);
            ha->Divide(hb);
            ha->GetYaxis()->SetTitle(Form("%s/%s", fileA.Data(), fileB.Data()));
            ha->DrawCopy();
        }
    }
    c->SaveAs(Form("ratios/%s_over_%s.png", fileA.Data(), fileB.Data()));
}




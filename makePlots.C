void makePlots(TString fileA) {

    TFile a(Form("%s/hist_%s.root", fileA.Data(), fileA.Data()));

    TCanvas *c = new TCanvas("v_n", "v_n", 2088, 1416);
    c->Divide(3,3);


    a.ls();

    for(Int_t i = 0; i < 7; i++) {
        c->cd(i+1);

        TH1D* ha = (TH1D*)a.Get(Form("hv2K0_%i", i));
        if(!ha) ha = (TH1D*)a.Get(Form("hv3K0_%i", i));
 

        ha->GetYaxis()->SetTitle(Form("%s", fileA.Data()));
        ha->DrawCopy();
    }
    c->SaveAs(Form("vnplots/%s.png", fileA.Data()));
}




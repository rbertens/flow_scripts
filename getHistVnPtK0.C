#include <TFile.h>
#include <TH2.h>
#include <TH1.h>
#include <TProfile.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TMath.h>
#include <TList.h>  
#include <TDirectoryFile.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TMinuit.h>
#include <TSystem.h>

#include <iostream>
using namespace std;

// Quadratic background function
Double_t background(Double_t *x, Double_t *par) {
   return par[0] + par[1]*x[0] + par[2]*x[0]*x[0];
}


// Lorenzian Peak function
Double_t lorentzianPeak(Double_t *x, Double_t *par) {
  return (0.5*par[0]*par[1]/TMath::Pi()) / 
    TMath::Max( 1.e-10,(x[0]-par[2])*(x[0]-par[2]) 
   + .25*par[1]*par[1]);
}

// Sum of background and peak function
Double_t fitFunction(Double_t *x, Double_t *par) {
  return background(x,par) + lorentzianPeak(x,&par[3]);
}


void getHistVnPtK0(const char* fileName = "v2_SPV0AC_K0L_hirCombL13L4.root", const char* flagSyst = "v2_def", const char* flagSystOut = "v2_K0_def", Int_t flagVn = 0, Int_t kBW = 0)
{
    
    //fileName = "filesGrid/v2_SPV0AC_K0L_hirL13.root"
    
    //flagSyst = "v2_def", v2_cosPA098, v2_nsig1", "v2_ncls80
    
    //flagSystOut = "v2_K0_def", v2_K0_cosPA098, v2_K0_nsig1, v2_K0_ncls80
    
    //flagVn = 0(v2), 1(v3)
    
    // kBW 0 for gaussian, 1 for breit-wigner, 2 for lorentzian
    
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
    gStyle->SetPalette(1);
    gStyle->SetEndErrorSize(0);
    gStyle->SetNdivisions();
    gStyle->SetNdivisions(505, "Y");
    
    
    const Int_t nCentrBins = 8;
    const Char_t* centr[nCentrBins] = {"0-5%", "5-10%", "10-20%", "20-30%", "30-40%", "40-50%", "50-60%", "60-70%"};
 
    
    const Int_t nPtBins = 17;
    Double_t binsPt[nPtBins+1] = {0.2, 0.5, 0.8, 1.1, 1.4, 1.7, 2.0, 2.5, 3.0, 3.5, 4.0, 5.0, 6.0, 8.0, 10.0, 13.0, 16.0, 20.0};
    
    
    TLatex* lc = new TLatex();
    //lc->SetTextFont(132);
    lc->SetNDC();
    lc->SetTextSize(0.045);
    lc->SetTextColor(kBlack);
    
    
    TLatex* lpt = new TLatex();
    //lpt->SetTextFont(132);
    lpt->SetNDC();
    lpt->SetTextSize(0.045);
    lpt->SetTextColor(kBlack);
    
  
    
    TFile* in = TFile::Open(fileName);
    TDirectoryFile* dpbpb = (TDirectoryFile*)in->Get(Form("%s", flagSyst));
    TList* lpbpb = (TList*)dpbpb->Get(Form("output_%s", flagSyst));
    

    TProfile* hV0AV0Cvn = (TProfile*)lpbpb->FindObject("fV0AV0Cvn");
    TH1D* hV0AV0CvnPx = (TH1D*)hV0AV0Cvn->ProjectionX("hV0AV0CvnPx");
    
    TProfile* hV0ATPCvn = (TProfile*)lpbpb->FindObject("fV0ATPCvn");
    TH1D* hV0ATPCvnPx = (TH1D*)hV0ATPCvn->ProjectionX("hV0ATPCvnPx");
    
    TProfile* hV0CTPCvn = (TProfile*)lpbpb->FindObject("fV0CTPCvn");
    TH1D* hV0CTPCvnPx = (TH1D*)hV0CTPCvn->ProjectionX("hV0CTPCvnPx");
    
    TH1D* hResV0Avn = (TH1D*)hV0AV0CvnPx->Clone("hResV0Avn");
    hResV0Avn->Multiply(hV0ATPCvnPx);
    hResV0Avn->Divide(hV0CTPCvnPx);
    
    
    
    TFile* out = new TFile(Form("hist_%s.root", flagSystOut),  "RECREATE");

    
    for (Int_t j = 0; j < nCentrBins; j++){
        
        TH1D* hvnK0 = 0;
        if (flagVn == 0)
            hvnK0 = new TH1D(Form("hv2K0_%d", j), Form("Centrality %s; p_{T} (GeV/c); v_{2}{2, |#Delta#eta|>2}", centr[j]), nPtBins, binsPt);
        else if (flagVn == 1)
            hvnK0 = new TH1D(Form("hv3K0_%d", j), Form("Centrality %s; p_{T} (GeV/c); v_{3}{2, |#Delta#eta|>2}", centr[j]), nPtBins, binsPt);

     
        for (Int_t k = 2; k < nPtBins-1; k++){
        //for (Int_t k = 0; k < nPtBins-1; k++){

            TH1D* hInvMass = (TH1D*)lpbpb->FindObject(Form("fInvMassK0_%d_%d", k, j));
            
            TProfile* hVnK0 = (TProfile*)lpbpb->FindObject(Form("fVnK0A_%d_%d", k, j));
            TH1D* hVnK0Px = (TH1D*)hVnK0->ProjectionX(Form("hVnK0Px_%d_%d", k, j));
            hVnK0Px->Scale(1./TMath::Sqrt(hResV0Avn->GetBinContent(j+1)));
            

            TCanvas* cK0 = new TCanvas(Form("cK0_%d_%d", k, j), Form("cK0_%d_%d", k, j));
            cK0->cd();

            hInvMass->Sumw2();
            hInvMass->GetXaxis()->SetTitle("M_{K^{0}} (GeV/c^{2})");
            hInvMass->SetMarkerStyle(20);
            hInvMass->SetMinimum(0);
            hInvMass->DrawCopy();

            cout<<"   "<<endl;
            cout<<"!!!  Centrality and pT bins: "<<j<<"    "<<k<<"  !!!"<<endl;
            cout<<"   "<<endl;
            
            TF1* fit;
           if(kBW == 1 ) fit = new TF1(Form("fit_%d_%d", k, j), "[0] + [1]*(x-0.4) + [2]*(x-0.4)*sqrt(x-0.4) + [3]*(x-0.4)*(x-0.4) + [4]*(x-0.4)*(x-0.4)*sqrt(x-0.4) + [5]*(x-0.4)*(x-0.4)*(x-0.4) + [6]*TMath::BreitWigner(x, [7],[8])", 0.4, 0.6);
           else if (kBW == 0) fit = new TF1(Form("fit_%d_%d", k, j), "[0] + [1]*(x-0.4) + [2]*(x-0.4)*sqrt(x-0.4) + [3]*(x-0.4)*(x-0.4) + [4]*(x-0.4)*(x-0.4)*sqrt(x-0.4) + [5]*(x-0.4)*(x-0.4)*(x-0.4) + gaus(6)", 0.4, 0.6);

           else if (kBW == 2) fit = new TF1("fitFcn",fitFunction,0,3,6);

            if(kBW < 2) {
            fit->SetParameters(hInvMass->GetMaximum()/10., -hInvMass->GetMaximum()/50., hInvMass->GetMaximum()/30., hInvMass->GetMaximum()/10., hInvMass->GetMaximum()/20., hInvMass->GetMaximum()/20., hInvMass->GetMaximum(), 0.4976, 0.001);
            fit->SetParLimits(6, 0, hInvMass->GetMaximum()*100);
            //fit->FixParameter(7, 0.4976);
            fit->SetParLimits(7, 0.48, 0.52);
            fit->SetParLimits(8, 0, 1);
            } else fit->SetParameters(1,1,1,1,1,1);

            hInvMass->Fit(fit, "RNL");
            
            
            Int_t nfits = 1;
            TString status = gMinuit->fCstatu.Data();
            
            while ((!status.Contains("CONVERGED")) && (nfits < 10)){
                
                fit->SetParameters(fit->GetParameter(0), fit->GetParameter(1), fit->GetParameter(2), fit->GetParameter(3), fit->GetParameter(4), fit->GetParameter(5), fit->GetParameter(6), 0.4976, fit->GetParameter(8));
                //fit->FixParameter(7, 0.4976);
                fit->SetParLimits(7, 0.48, 0.52);
                fit->SetParLimits(8, 0, 1);
                hInvMass->Fit(fit, "RNL");
                
                status = gMinuit->fCstatu.Data();
                nfits++;
                
            }
            
            fit->DrawCopy("same");
            
            
            TF1* poli = new TF1(Form("poli_%d_%d", j, k), "[0] + [1]*(x-0.4) + [2]*(x-0.4)*sqrt(x-0.4) + [3]*(x-0.4)*(x-0.4) + [4]*(x-0.4)*(x-0.4)*sqrt(x-0.4) + [5]*(x-0.4)*(x-0.4)*(x-0.4)", 0.4, 0.6);
            poli->SetParameters(fit->GetParameter(0), fit->GetParameter(1), fit->GetParameter(2), fit->GetParameter(3), fit->GetParameter(4), fit->GetParameter(5));
            poli->SetLineColor(2);
            poli->SetLineStyle(2);
            poli->DrawCopy("same");
            
            TF1* g;
            if(kBW) g = new TF1(Form("gaus_%d_%d", j, k), "[0]*TMath::BreitWigner(x,[1],[2])", 0.4, 0.6);
            else g = new TF1(Form("gaus_%d_%d", j, k), "gaus(0)", 0.4, 0.6);

            g->SetParameters(fit->GetParameter(6), fit->GetParameter(7), fit->GetParameter(8));
            g->SetLineColor(4);
            g->SetLineStyle(2);
            g->DrawCopy("same");
            
            cout<<fit->GetParameter(6)<<"     "<<fit->GetParameter(7)<<"    "<<fit->GetParameter(8)<<endl;

            lc->DrawLatex(0.15, 0.85, Form("%s", centr[j]));
            lpt->DrawLatex(0.15, 0.79, Form("%0.2f<p_{T}<%0.2f GeV/c", binsPt[k], binsPt[k+1]));

            cK0->SaveAs(Form("MK0_cent_%d_pt_%d.png", k, j));
            
            
            TCanvas* cvnK0 = new TCanvas(Form("cvnK0_%d_%d", k, j), Form("cvnK0_%d_%d", k, j));
            cvnK0->cd();
            
            hVnK0Px->GetXaxis()->SetTitle("M_{K^{0}} (GeV/c^{2})");
            hVnK0Px->DrawCopy();
            
            
             TF1* fvna;
             if(kBW) fvna = new TF1(Form("fvna_%d_%d", j, k), "[0]*[1]*TMath::BreitWigner(x,[2],[3])/([4]*TMath::BreitWigner(x,[5],[6]) + [7] + [8]*(x-0.4) + [9]*(x-0.4)*sqrt(x-0.4) + [10]*(x-0.4)*(x-0.4) + [11]*(x-0.4)*(x-0.4)*sqrt(x-0.4) + [12]*(x-0.4)*(x-0.4)*(x-0.4)) + ([13] + [14]*(x-0.4) + [15]*(x-0.4)*sqrt(x-0.4) + [16]*(x-0.4)*(x-0.4) + [17]*(x-0.4)*(x-0.4)*sqrt(x-0.4) + [18]*(x-0.4)*(x-0.4)*(x-0.4)) / ( [19] + [20]*(x-0.4) + [21]*(x-0.4)*sqrt(x-0.4) + [22]*(x-0.4)*(x-0.4) + [23]*(x-0.4)*(x-0.4)*sqrt(x-0.4) + [24]*(x-0.4)*(x-0.4)*(x-0.4) + [25]*TMath::BreitWigner(x,[26],[27]))*([28]+[29]*x+[30]*x*x)", 0.4, 0.6);


             else fvna = new TF1(Form("fvna_%d_%d", j, k), "[0]*gaus(1)/(gaus(4) + [7] + [8]*(x-0.4) + [9]*(x-0.4)*sqrt(x-0.4) + [10]*(x-0.4)*(x-0.4) + [11]*(x-0.4)*(x-0.4)*sqrt(x-0.4) + [12]*(x-0.4)*(x-0.4)*(x-0.4)) + ([13] + [14]*(x-0.4) + [15]*(x-0.4)*sqrt(x-0.4) + [16]*(x-0.4)*(x-0.4) + [17]*(x-0.4)*(x-0.4)*sqrt(x-0.4) + [18]*(x-0.4)*(x-0.4)*(x-0.4)) / ( [19] + [20]*(x-0.4) + [21]*(x-0.4)*sqrt(x-0.4) + [22]*(x-0.4)*(x-0.4) + [23]*(x-0.4)*(x-0.4)*sqrt(x-0.4) + [24]*(x-0.4)*(x-0.4)*(x-0.4) + gaus(25))*([28]+[29]*x+[30]*x*x)", 0.4, 0.6);
            
             Double_t parama[32] = {0.1, fit->GetParameter(6), fit->GetParameter(7), fit->GetParameter(8), fit->GetParameter(6), fit->GetParameter(7), fit->GetParameter(8), fit->GetParameter(0), fit->GetParameter(1), fit->GetParameter(2), fit->GetParameter(3), fit->GetParameter(4), fit->GetParameter(5), fit->GetParameter(0), fit->GetParameter(1), fit->GetParameter(2), fit->GetParameter(3), fit->GetParameter(4), fit->GetParameter(5), fit->GetParameter(0), fit->GetParameter(1), fit->GetParameter(2), fit->GetParameter(3), fit->GetParameter(4), fit->GetParameter(5), fit->GetParameter(6), fit->GetParameter(7), fit->GetParameter(8), 4.65731e-01, -5.44560e+00, 0};
             
             
             fvna->SetParName(0, "v_{2}");
             //fvna->SetParLimits(0, 0.001, 0.4);
             fvna->SetLineColor(2);
             fvna->SetParameters(parama);
             
             fvna->FixParameter(1, fit->GetParameter(6));
             fvna->FixParameter(2, fit->GetParameter(7));
             fvna->FixParameter(3, fit->GetParameter(8));
             
             fvna->FixParameter(4, fit->GetParameter(6));
             fvna->FixParameter(5, fit->GetParameter(7));
             fvna->FixParameter(6, fit->GetParameter(8));
             
             fvna->FixParameter(7, fit->GetParameter(0));
             fvna->FixParameter(8, fit->GetParameter(1));
             fvna->FixParameter(9, fit->GetParameter(2));
             fvna->FixParameter(10, fit->GetParameter(3));
             fvna->FixParameter(11, fit->GetParameter(4));
             fvna->FixParameter(12, fit->GetParameter(5));
             
             fvna->FixParameter(13, fit->GetParameter(0));
             fvna->FixParameter(14, fit->GetParameter(1));
             fvna->FixParameter(15, fit->GetParameter(2));
             fvna->FixParameter(16, fit->GetParameter(3));
             fvna->FixParameter(17, fit->GetParameter(4));
             fvna->FixParameter(18, fit->GetParameter(5));
             
             fvna->FixParameter(19, fit->GetParameter(0));
             fvna->FixParameter(20, fit->GetParameter(1));
             fvna->FixParameter(21, fit->GetParameter(2));
             fvna->FixParameter(22, fit->GetParameter(3));
             fvna->FixParameter(23, fit->GetParameter(4));
             fvna->FixParameter(24, fit->GetParameter(5));
             
             fvna->FixParameter(25, fit->GetParameter(6));
             fvna->FixParameter(26, fit->GetParameter(7));
             fvna->FixParameter(27, fit->GetParameter(8));
             

             // scan through fit ranges
             double lowerEdge = .4;
             double upperEdge = .6;
             double stepsize = .01;
             int lowerDepth = 4;
             int upperDepth = 4;

             double prob = 0;
             double bestLE = .4;
             double bestUP = .6;

             for(int lb = 0; lb < lowerDepth; lb++) {
                 for(int hb = 0; hb < upperDepth; hb++) {
                     hVnK0Px->Fit(fvna, "RN", "", .4+stepsize*lb, .6-stepsize*hb);

                         cout << " probability " << fvna->GetProb() << endl;
                     if(fvna->GetProb() > prob) {
                         cout << " i'm contrubuting " << fvna->GetProb() << endl;
                         

                         prob = fvna->GetProb();
                         bestLE = .4+stepsize*lb;
                         bestUP = .6-stepsize*hb;
                     }
                 }
             }
             // redo the best one
             hVnK0Px->Fit(fvna, "RN", "", bestLE, bestUP);
             cout << " best upper edge " << bestLE << endl;
             cout << " best lower edge " << bestUP << endl;
             fvna->SetRange(bestLE, bestUP);

             fvna->DrawCopy("same");
            
            
            
            lc->DrawLatex(0.15, 0.85, Form("%s", centr[j]));
            lpt->DrawLatex(0.15, 0.79, Form("%0.2f<p_{T}<%0.2f GeV/c", binsPt[k], binsPt[k+1]));
            
            cvnK0->SaveAs(Form("v2c_cent_%d_pt_%d.png", k, j));
            
            
            hvnK0->SetBinContent(k+1, fvna->GetParameter(0));
            hvnK0->SetBinError(k+1, fvna->GetParError(0));
            
            /*
             for (Int_t np = 0; np <=29; np++){
             if (np < 9)
             fit->ReleaseParameter(np);
             fvna->ReleaseParameter(np);
             }
             
             fit->Delete();
             */
            
        }
       
        hvnK0->Write();
    
    }
    
    out->Close();


}

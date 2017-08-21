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


void getHistVnPtLambda(const char* fileName = "v2_SPV0AC_K0L_hirL13.root", const char* flagSyst = "v2_def", const char* flagSystOut = "v2_Lambda_def", Int_t flagVn = 0)
{
    
    //fileName = "vn_SPV0A_PID_FB32_hir.root" //"vn_SPV0A_PID_FB32_hir_lot13.root"
    
    //flagSyst = "v2_fb32", "v3_fb32", "v4_fb32"
    
    //flagSystOut = "v2_Lambda", "v3_Lambda", "v4_Lambda"
    
    //flagVn = 0(v2), 1(v3), 2(v4)
    
    
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
    gStyle->SetPalette(1);
    gStyle->SetEndErrorSize(0);
    gStyle->SetNdivisions();
    gStyle->SetNdivisions(505, "Y");
    
    
    const Int_t nCentrBins = 9;
    //Double_t binsCentr[nCentrBins+1] = {0., 5, 10, 20, 30, 40, 50, 60, 70, 80};
    const Char_t* centr[nCentrBins] = {"0-5%", "5-10%", "10-20%", "20-30%", "30-40%", "40-50%", "50-60%", "60-70%", "70-80%"};
 
    
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
    //for (Int_t j = 2; j < 3; j++){
        
        TH1D* hvnK0 = 0;
        if (flagVn == 0)
            hvnK0 = new TH1D(Form("hv2L_%d", j), Form("Centrality %s; p_{T} (GeV/c); v_{2}{2, |#Delta#eta|>2}", centr[j]), nPtBins, binsPt);
        else if (flagVn == 1)
            hvnK0 = new TH1D(Form("hv3L_%d", j), Form("Centrality %s; p_{T} (GeV/c); v_{3}{2, |#Delta#eta|>2}", centr[j]), nPtBins, binsPt);
        else if (flagVn == 2)
            hvnK0 = new TH1D(Form("hv4L_%d", j), Form("Centrality %s; p_{T} (GeV/c); v_{4}{2, |#Delta#eta|>2}", centr[j]), nPtBins, binsPt);

     
        //for (Int_t k = 2; k < nPtBins-3; k++){
        for (Int_t k = 2; k < nPtBins-1; k++){

            TH1D* hInvMass = (TH1D*)lpbpb->FindObject(Form("fInvMassL_%d_%d", k, j));
            
            TProfile* hVnK0 = (TProfile*)lpbpb->FindObject(Form("fVnLA_%d_%d", k, j));
            TH1D* hVnK0Px = (TH1D*)hVnK0->ProjectionX(Form("hVnK0Px_%d_%d", k, j));
            hVnK0Px->Scale(1./TMath::Sqrt(hResV0Avn->GetBinContent(j+1)));
            if (flagVn == 2)
                hVnK0Px->Scale(TMath::Sqrt(2));
            

            TCanvas* cK0 = new TCanvas(Form("cK0_%d_%d", k, j), Form("cK0_%d_%d", k, j));
            cK0->cd();

            hInvMass->Sumw2();
            hInvMass->GetXaxis()->SetTitle("M_{#Lambda} (GeV/c^{2})");
            hInvMass->SetMarkerStyle(20);
            hInvMass->SetMinimum(0);
            hInvMass->DrawCopy();

            cout<<"   "<<endl;
            cout<<"!!!  Centrality and pT bins: "<<j<<"    "<<k<<"  !!!"<<endl;
            cout<<"   "<<endl;
            
            TF1* fit = new TF1(Form("fit_%d_%d", k, j), "[0] + [1]*(x-1.08) + [2]*(x-1.08)*sqrt(x-1.08) + [3]*(x-1.08)*(x-1.08) + [4]*(x-1.08)*(x-1.08)*sqrt(x-1.08) + [5]*(x-1.08)*(x-1.08)*(x-1.08) + gaus(6)", 1.08, 1.17);
            fit->SetParameters(hInvMass->GetMaximum()/10., -hInvMass->GetMaximum()/50., hInvMass->GetMaximum()/30., hInvMass->GetMaximum()/10., hInvMass->GetMaximum()/20., hInvMass->GetMaximum()/20., hInvMass->GetMaximum(), 1.1156, 0.001);
            fit->SetParLimits(6, 0, hInvMass->GetMaximum()*100);
            //fit->FixParameter(7, 1.1156);
            fit->SetParLimits(7, 1.1, 1.3);
            fit->SetParLimits(8, 0, 1);
            hInvMass->Fit(fit, "RNL");
            fit->SetNpx(1000);
            
            
            Int_t nfits = 1;
            TString status = gMinuit->fCstatu.Data();
            
            while ((!status.Contains("CONVERGED")) && (nfits < 10)){
                
                fit->SetParameters(fit->GetParameter(0), fit->GetParameter(1), fit->GetParameter(2), fit->GetParameter(3), fit->GetParameter(4), fit->GetParameter(5), fit->GetParameter(6), 1.1156, fit->GetParameter(8));
                //fit->FixParameter(7, 1.1156);
                fit->SetParLimits(7, 1.1, 1.3);
                fit->SetParLimits(8, 0, 1);
                hInvMass->Fit(fit, "RNL");
                
                status = gMinuit->fCstatu.Data();
                nfits++;
                
            }
            
            fit->DrawCopy("same");
            
            
            TF1* poli = new TF1(Form("poli_%d_%d", j, k), "[0] + [1]*(x-1.08) + [2]*(x-1.08)*sqrt(x-1.08) + [3]*(x-1.08)*(x-1.08) + [4]*(x-1.08)*(x-1.08)*sqrt(x-1.08) + [5]*(x-1.08)*(x-1.08)*(x-1.08)", 1.08, 1.17);
            poli->SetNpx(1000);
            poli->SetParameters(fit->GetParameter(0), fit->GetParameter(1), fit->GetParameter(2), fit->GetParameter(3), fit->GetParameter(4), fit->GetParameter(5));
            poli->SetLineColor(2);
            poli->SetLineStyle(2);
            poli->DrawCopy("same");
            
            TF1* g = new TF1(Form("gaus_%d_%d", j, k), "gaus", 1.08, 1.17);
            g->SetNpx(1000);
            g->SetParameters(fit->GetParameter(6), fit->GetParameter(7), fit->GetParameter(8));
            g->SetLineColor(4);
            g->SetLineStyle(2);
            g->DrawCopy("same");
            
            cout<<fit->GetParameter(6)<<"     "<<fit->GetParameter(7)<<"    "<<fit->GetParameter(8)<<endl;

            lc->DrawLatex(0.15, 0.85, Form("%s", centr[j]));
            lpt->DrawLatex(0.15, 0.79, Form("%0.2f<p_{T}<%0.2f GeV/c", binsPt[k], binsPt[k+1]));

            //cK0->SaveAs(Form("ML_cent_%d_pt_%d.png", k, j));
            
            cK0->SaveAs(Form("cLambda_%d_%d.png", k, j));

            TCanvas* cvnK0 = new TCanvas(Form("cvnK0_%d_%d", k, j), Form("cvnK0_%d_%d", k, j));
            cvnK0->cd();
            
            hVnK0Px->GetXaxis()->SetTitle("M_{#Lambda} (GeV/c^{2})");
            hVnK0Px->SetMaximum(1.2*hVnK0Px->GetMaximum());
            hVnK0Px->DrawCopy();
            
            
             TF1* fvna = new TF1(Form("fvna_%d_%d", j, k), "[0]*gaus(1)/(gaus(4) + [7] + [8]*(x-1.08) + [9]*(x-1.08)*sqrt(x-1.08) + [10]*(x-1.08)*(x-1.08) + [11]*(x-1.08)*(x-1.08)*sqrt(x-1.08) + [12]*(x-1.08)*(x-1.08)*(x-1.08)) + ([13] + [14]*(x-1.08) + [15]*(x-1.08)*sqrt(x-1.08) + [16]*(x-1.08)*(x-1.08) + [17]*(x-1.08)*(x-1.08)*sqrt(x-1.08) + [18]*(x-1.08)*(x-1.08)*(x-1.08)) / ( [19] + [20]*(x-1.08) + [21]*(x-1.08)*sqrt(x-1.08) + [22]*(x-1.08)*(x-1.08) + [23]*(x-1.08)*(x-1.08)*sqrt(x-1.08) + [24]*(x-1.08)*(x-1.08)*(x-1.08) + gaus(25))*([28]+[29]*x+[30]*x*x)", 1.08, 1.17);
             fvna->SetNpx(1000);
            
             Double_t parama[31] = {0.1, fit->GetParameter(6), fit->GetParameter(7), fit->GetParameter(8), fit->GetParameter(6), fit->GetParameter(7), fit->GetParameter(8), fit->GetParameter(0), fit->GetParameter(1), fit->GetParameter(2), fit->GetParameter(3), fit->GetParameter(4), fit->GetParameter(5), fit->GetParameter(0), fit->GetParameter(1), fit->GetParameter(2), fit->GetParameter(3), fit->GetParameter(4), fit->GetParameter(5), fit->GetParameter(0), fit->GetParameter(1), fit->GetParameter(2), fit->GetParameter(3), fit->GetParameter(4), fit->GetParameter(5), fit->GetParameter(6), fit->GetParameter(7), fit->GetParameter(8), 4.65731e-01, -5.44560e+00, 0};
             
             
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
             






             double lowerEdge = 0.78;
             double upperEdge = 1.17;
             double stepSize = ...;



             hVnK0Px->Fit(fvna, "RN");
            fvna->DrawCopy("same");
            
            
            
            lc->DrawLatex(0.15, 0.85, Form("%s", centr[j]));
            lpt->DrawLatex(0.15, 0.79, Form("%0.2f<p_{T}<%0.2f GeV/c", binsPt[k], binsPt[k+1]));
            
            //cvnK0->SaveAs(Form("v2L_cent_%d_pt_%d.png", k, j));
            
            
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
             cvnK0->SaveAs(Form("cvnLambda_%d_%d.png", k, j));
           
        }
       
        hvnK0->Write();
    
    }
    
    out->Close();


}

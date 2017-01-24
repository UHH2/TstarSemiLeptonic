void setThesisStyle() {
  TStyle *thesisStyle = new TStyle("thesisStyle","Style for Thesis"); //hard
  

// For the canvas:
  thesisStyle->SetCanvasBorderMode(0);
  thesisStyle->SetCanvasColor(kWhite);
  thesisStyle->SetCanvasDefH(600); //Height of canvas
  thesisStyle->SetCanvasDefW(600); //Width of canvas
  thesisStyle->SetCanvasDefX(0);   //POsition on screen
  thesisStyle->SetCanvasDefY(0);

// For the Pad:
  thesisStyle->SetPadBorderMode(0);
  // thesisStyle->SetPadBorderSize(Width_t size = 1);
  thesisStyle->SetPadColor(kWhite);
  thesisStyle->SetPadGridX(false);
  thesisStyle->SetPadGridY(false);
  thesisStyle->SetGridColor(0);
  thesisStyle-> SetPadTickX(1);
  thesisStyle->   SetPadTickY(1);
  //thesisStyle->SetGridStyle(3);
  //thesisStyle->SetGridWidth(1);

// For the frame:
  thesisStyle->SetFrameBorderMode(0);
  thesisStyle->SetFrameBorderSize(3);
  thesisStyle->SetFrameFillColor(0);
  thesisStyle->SetFrameFillStyle(0);
  thesisStyle->SetFrameLineColor(1);
  thesisStyle->SetFrameLineStyle(1);
  thesisStyle->SetFrameLineWidth(3);

// For the histo:
  // thesisStyle->SetHistFillColor(1);
  // thesisStyle->SetHistFillStyle(0);
  thesisStyle->SetHistLineColor(1);
  thesisStyle->SetHistLineStyle(0);
  thesisStyle->SetHistLineWidth(3);
  // thesisStyle->SetLegoInnerR(Float_t rad = 0.5);
  // thesisStyle->SetNumberContours(Int_t number = 20);

  thesisStyle->SetEndErrorSize(2);
//  thesisStyle->SetErrorMarker(20);
  thesisStyle->SetErrorX(0.);
  
  thesisStyle->SetMarkerStyle(20);

//For the fit/function:
  thesisStyle->SetOptFit(0);
  thesisStyle->SetFitFormat("5.4g");
  thesisStyle->SetFuncColor(2);
  thesisStyle->SetFuncStyle(1);
  thesisStyle->SetFuncWidth(3);

//For the date:
  thesisStyle->SetOptDate(0);
  // thesisStyle->SetDateX(Float_t x = 0.01);
  // thesisStyle->SetDateY(Float_t y = 0.01);

// For the statistics box:
  thesisStyle->SetOptFile(0);
  thesisStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  thesisStyle->SetStatColor(kWhite);
  thesisStyle->SetStatFont(42);
  thesisStyle->SetStatFontSize(0.025);
  thesisStyle->SetStatTextColor(1);
  thesisStyle->SetStatFormat("6.4g");
  thesisStyle->SetStatBorderSize(1);
  thesisStyle->SetStatH(0.1);
  thesisStyle->SetStatW(0.15);
  // thesisStyle->SetStatStyle(Style_t style = 1001);
  // thesisStyle->SetStatX(Float_t x = 0);
  // thesisStyle->SetStatY(Float_t y = 0);

// Margins:
  thesisStyle->SetPadTopMargin(0.05);
  thesisStyle->SetPadBottomMargin(0.13);
  thesisStyle->SetPadLeftMargin(0.16);
  thesisStyle->SetPadRightMargin(0.06);

// For the Global title:

  thesisStyle->SetOptTitle(0);
  thesisStyle->SetTitleFont(42);
  thesisStyle->SetTitleColor(1);
  thesisStyle->SetTitleTextColor(1);
  thesisStyle->SetTitleFillColor(10);
  thesisStyle->SetTitleFontSize(0.05);
  // thesisStyle->SetTitleH(0); // Set the height of the title box
  // thesisStyle->SetTitleW(0); // Set the width of the title box
  // thesisStyle->SetTitleX(0); // Set the position of the title box
  // thesisStyle->SetTitleY(0.985); // Set the position of the title box
  // thesisStyle->SetTitleStyle(Style_t style = 1001);
  // thesisStyle->SetTitleBorderSize(2);

// For the axis titles:

  thesisStyle->SetTitleColor(1, "XYZ");
  thesisStyle->SetTitleFont(42, "XYZ");
  thesisStyle->SetTitleSize(0.06, "XYZ");
  // thesisStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // thesisStyle->SetTitleYSize(Float_t size = 0.02);
  thesisStyle->SetTitleXOffset(0.9);//0.9
  thesisStyle->SetTitleYOffset(1.25);//1.35
  // thesisStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

// For the axis labels:

  thesisStyle->SetLabelColor(1, "XYZ");
  thesisStyle->SetLabelFont(42, "XYZ");
  thesisStyle->SetLabelOffset(0.007, "XYZ");
  thesisStyle->SetLabelSize(0.05, "XYZ");

// For the axis:

  thesisStyle->SetAxisColor(1, "XYZ");
  thesisStyle->SetStripDecimals(kTRUE);
  thesisStyle->SetTickLength(0.03, "XYZ");
  thesisStyle->SetNdivisions(505, "XYZ");//510
  //thesisStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  //thesisStyle->SetPadTickY(1);
  thesisStyle->SetPadTickX(1); 
  thesisStyle->SetPadTickY(1);
  thesisStyle->SetNdivisions(510, "Z");//510

// Change for log plots:
  thesisStyle->SetOptLogx(0);
  thesisStyle->SetOptLogy(0);
  thesisStyle->SetOptLogz(0);

// Postscript options:
  thesisStyle->SetPaperSize(20.,20.);
  // thesisStyle->SetLineScalePS(Float_t scale = 3);
  // thesisStyle->SetLineStyleString(Int_t i, const char* text);
  // thesisStyle->SetHeaderPS(const char* header);
  // thesisStyle->SetTitlePS(const char* pstitle);

  // thesisStyle->SetBarOffset(Float_t baroff = 0.5);
  // thesisStyle->SetBarWidth(Float_t barwidth = 0.5);
  // thesisStyle->SetPaintTextFormat(const char* format = "g");
  // thesisStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // thesisStyle->SetTimeOffset(Double_t toffset);
  // thesisStyle->SetHistMinimumZero(kTRUE);

  thesisStyle->cd();

}

void limit()
{
  double x[10] = {700,800,900,1000,1100,1200,1300,1400,1500,1600};
 double x2[12]={700,700,800,900,1000,1100,1200,1300,1400,1500,1600,1600};
 double x3[12]={700,700,800,900,1000,1100,1200,1300,1400,1500,1600,1600};

 //Theta Top-Tag
 double central_Tag[10] = {0.78, 0.54, 0.37, 0.30, 0.24, 0.21, 0.17, 0.16, 0.15,  0.13};

//Theta no Top-Tag
double central_noTag[10] = {0.56, 0.49, 0.42, 0.32, 0.28, 0.27, 0.22, 0.20, 0.17,  0.15};
 
//Theta no Top-Tag
double central_comb[10] = {0.42, 0.34, 0.27, 0.21, 0.18, 0.16, 0.13, 0.12, 0.10,  0.09};

//Theta no kat
double central_noKat[10] = {0.46, 0.38, 0.31, 0.23, 0.2, 0.18, 0.15, 0.13, 0.11,  0.1};

//FINAL
/*
double central_FINAL[10] = {0.66, 0.57, 0.63, 0.53, 0.4, 0.33, 0.25, 0.22, 0.19,  0.16};
  double observed[10]    = {0.57, 0.46, 0.45, 0.37, 0.28, 0.26, 0.2, 0.17, 0.14, 0.12};
  double low_95[12]   = {0, 0.36, 0.29, 0.28, 0.23, 0.18, 0.16, 0.13, 0.11, 0.09, 0.07, 0};
  double up_95[12]    = {0, 0.94, 0.77, 0.75, 0.62, 0.47, 0.43, 0.33, 0.29, 0.24, 0.21, 0};
  double low_68[12]   = {0, 0.24, 0.19, 0.20, 0.16, 0.12, 0.12, 0.09, 0.08, 0.07, 0.05, 0};
 double up_68[12]     = {0, 1.58, 1.30, 1.20, 1.04, 0.78, 0.71, 0.56, 0.48, 0.41, 0.36, 0};
*/

double central_FINAL[10] = {0.28, 0.26, 0.3, 0.27, 0.24, 0.22, 0.29, 0.17, 0.16,  0.14};
  double observed[10]    = {0.59, 0.48, 0.47, 0.38, 0.30, 0.27, 0.21, 0.19, 0.15, 0.13};
  double low_95[12]   = {0, 0.37, 0.3, 0.30, 0.24, 0.19, 0.18, 0.14, 0.12, 0.09, 0.08, 0};
  double up_95[12]    = {0, 0.99, 0.80, 0.79, 0.64, 0.49, 0.45, 0.35, 0.3, 0.25, 0.22, 0};
  double low_68[12]   = {0, 0.26, 0.21, 0.21, 0.17, 0.14, 0.13, 0.1, 0.09, 0.07, 0.06, 0};
 double up_68[12]     = {0, 1.65, 1.36, 1.32, 1.09, 0.80, 0.75, 0.59, 0.50, 0.43, 0.37, 0};


 double theory[10] = {4.92281, 1.6752, 0.6357, 0.2623, 0.1156, 0.05372, 0.026058, 0.01309, 0.00677, 0.003588}; // Pythia
 //double theory[10] = {0.174E+02,0.526E+01,0.189E+01,0.769E+00,0.342E+00,0.163E+00,0.820E-01,0.431E-01,0.235E-01,0.132E-01};
 //double theory_up[10] = {0.150E+02,0.454E+01,0.163E+01,0.663E+00,0.295E+00,0.140E+00,0.705E-01,0.369E-01,0.201E-01,0.113E-01};
 //double theory_down[10] = {0.196E+02,0.594E+01,0.213E+01,0.865E+00,0.384E+00,0.183E+00,0.923E-01,0.484E-01,0.264E-01,0.149E-01};
 /*
 double rel_pdf_unc[17] = {0.05, 0.07, 0.09, 0.11, 0.13, 0.15, 0.17, 0.19, 0.21, 0.23, 0.25, 0.27, 0.29};
 for (int i=0; i<10; ++i){
   double pdfu = rel_pdf_unc[i];
   double rup = (theory_up[i] - theory[i])/theory[i];
   double up2 = pdfu*pdfu + rup*rup;
   theory_up[i] = (1+TMath::Sqrt(up2))*theory[i];

   double rdn = (theory[i] - theory_down[i])/theory[i];
   double dn2 = pdfu*pdfu + rdn*rdn;
   theory_down[i] = (1-TMath::Sqrt(dn2))*theory[i];   
 }
 */
 
 TGraph* g_low_95 = new TGraph(12,x2,low_95);
 TGraph* g_low_68 = new TGraph(12,x2,low_68);
 
 TGraph* g_up_95 = new TGraph(12,x2,up_95);
 TGraph* g_up_68 = new TGraph(12,x2,up_68);
 TGraph* g_observed = new TGraph(10,x,observed);
 
 
 TGraph* g_central_FINAL = new TGraph(10,x,central_FINAL);

 TGraph* g_central_Tag = new TGraph(10,x,central_Tag);
 TGraph* g_central_noTag = new TGraph(10,x,central_noTag);
 TGraph* g_central_comb = new TGraph(10,x,central_comb);
 TGraph* g_central_noKat = new TGraph(10,x,central_noKat);

 TGraph* g_theory = new TGraph(10,x,theory);
 //TGraph* g_theory_up = new TGraph(10,x,theory_up);
 //TGraph* g_theory_down = new TGraph(10,x,theory_down);
 
 g_up_95->UseCurrentStyle();
 g_low_95->UseCurrentStyle();
 g_up_68->UseCurrentStyle();
 g_low_68->UseCurrentStyle();
 g_up_95->SetLineColor(kYellow+2);
 g_up_95->SetFillColor(kGreen); 
 //g_up_95_dummy->SetFillColor(0); 
 g_up_68->SetLineColor(kYellow+2);
 g_up_68->SetFillColor(kYellow);
 g_low_95->SetLineColor(kGreen+2);
 g_low_95->SetFillColor(kYellow); 
 g_low_68->SetLineColor(kYellow+2);
 g_low_68->SetFillColor(kWhite);
 
  
 g_observed->SetFillColor(0);
 g_observed->SetLineStyle(9);
 g_observed->SetLineColor(kBlack);
 g_observed->SetLineWidth(3);
 //**************************************************
 g_central_Tag ->SetFillColor(0);
 g_central_Tag ->SetLineStyle(1);
 g_central_Tag ->SetLineColor(kRed);
 g_central_Tag ->SetLineWidth(3);

 g_central_noTag ->SetFillColor(0);
 g_central_noTag ->SetLineStyle(1);
 g_central_noTag ->SetLineColor(kBlue);
 g_central_noTag ->SetLineWidth(3);

 g_central_comb ->SetFillColor(0);
 g_central_comb ->SetLineStyle(1);
 g_central_comb ->SetLineColor(kGreen);
 g_central_comb ->SetLineWidth(3);

 g_central_noKat ->SetFillColor(0);
 g_central_noKat ->SetLineStyle(1);
 g_central_noKat ->SetLineColor(kBlack);
 g_central_noKat ->SetLineWidth(3);

 //*****************************************************

 g_central_FINAL ->SetFillColor(0);
 g_central_FINAL ->SetLineStyle(1);
 g_central_FINAL ->SetLineColor(kBlack);
 g_central_FINAL ->SetLineWidth(3);

 g_theory->SetFillColor(0);
 g_theory->SetLineStyle(10);
 g_theory->SetLineColor(kBlack);
 g_theory->SetLineWidth(2);
 /*
 g_theory_up->SetFillColor(0);
 g_theory_up->SetLineStyle(3);
 g_theory_up->SetLineColor(kBlack);
 g_theory_up->SetLineWidth(2);

 g_theory_down->SetFillColor(0);
 g_theory_down->SetLineStyle(3);
 g_theory_down->SetLineColor(kBlack);
 g_theory_down->SetLineWidth(2);
 */
 g_up_95->SetLineWidth(0);
 g_up_68->SetLineWidth(0);

 TCanvas * c = new TCanvas("c", "c", 800,600);
 c->SetLogy();
 //g_up_95_dummy->SetMinimum(0.001);
 //g_up_95_dummy->GetXaxis()->SetRangeUser(x[0],x[17]);
 //g_up_95_dummy->Draw("AF");
 double min = 10;
 
 g_central_FINAL->SetMinimum(min);
 g_central_FINAL->GetXaxis()->SetRangeUser(x[0],x[9]);
 

 //g_up_95_dummy->Draw("F SAME");
 //g_central_iso->Draw("L SAME");
 g_central_FINAL ->Draw("AF");
 /*
 g_up_68->Draw("F SAME");
 g_up_95->Draw("F SAME"); 
 g_low_95->Draw("F SAME"); 
 g_low_68 ->Draw("F SAME");
 g_observed->Draw("L SAME");
 g_central_FINAL ->Draw("L SAME");
 */
 
 g_central_Tag ->Draw("L SAME");
 g_central_noTag ->Draw("L SAME");
 g_central_comb ->Draw("L SAME");
 g_central_noKat ->Draw("L SAME");
 
 //g_theory->Draw("L SAME");
 //g_theory_up->Draw("L SAME");
 //g_theory_down->Draw("L SAME");
 TLegend *leg1;
 leg1 = new TLegend(0.37,0.7,0.85,0.9,"","brNDC");//x+0.1
 leg1->SetBorderSize(0);
 leg1->SetTextSize(0.035);
 leg1->SetFillColor(10);
 leg1->SetLineColor(1);
 leg1->SetTextFont(42);
 TLegendEntry* entries[4];
  
 //entries[0]=leg1->AddEntry(g_observed, "erwartete obere Grenze","L");
 // entries[1]=leg1->AddEntry(g_central_iso, "expected 95% C.L. upper limits; Isol.","L");
 //entries[0]=leg1->AddEntry( g_central_FINAL, "gemessene obere Grenze","L");
 

 //entries[3]=leg1->AddEntry(g_theory_up, "uncertainties on the theory cross section","L");
 //entries[4]=leg1->AddEntry(g_up_95, "central 68% expected upper limits","F");
 //entries[5]=leg1->AddEntry(g_up_68, "central 95% expected upper limits","F");
 /*
   entries[0]=leg1->AddEntry(g_theory, "theoretischer Wirkungsquerschnitt","L");
   entries[1]=leg1->AddEntry(g_observed, "erwartetes Limit","L");
   entries[2]=leg1->AddEntry(g_central_FINAL, "beobachtetes Limit","L");
   entries[3]=leg1->AddEntry(g_up_95, "zentrale 95% der Erwartung","F");
   entries[4]=leg1->AddEntry(g_up_68, "zentrale 68% der Erwartung","F");
 */ 

   entries[0]=leg1->AddEntry(g_central_noKat, "erwartetes Limit ohne Kategorisierung","L");
   entries[1]=leg1->AddEntry(g_central_Tag, "erwartetes Limit (Ereigniskategorie mit Top-Tag)","L");
   entries[2]=leg1->AddEntry(g_central_noTag, "erwartetes Limit (Ereigniskategorie ohne Top-Tag)","L");
   entries[3]=leg1->AddEntry(g_central_comb, "erwartetes Limit bei Kombination d. Kategorien","L");

 leg1->Draw();


  double m_lumi = 2.7;
  TString infotext = TString::Format("2.7 fb^{-1}, #sqrt{s} = 13 TeV");

   TLatex *text1 = new TLatex(3.5, 24, infotext);
  text1->SetNDC();
  text1->SetTextAlign(33);
  text1->SetX(0.95);
  text1->SetTextFont(42);
  text1->SetTextSize(0.04);
  text1->SetY(1.00);
  text1->Draw();


 TH1F* h = g_central_FINAL ->GetHistogram();
 h->UseCurrentStyle();
  h->UseCurrentStyle();
  h->GetXaxis()->SetRangeUser(700,1600);
  h->GetYaxis()->SetRangeUser(0.01,10);
  h->SetXTitle(" M_{T*} [GeV/c^{2}]");
 h->SetYTitle("#sigma(pp#rightarrow T*#rightarrow t+g) #times VV [pb]");
 h->Draw("AXIS SAME");
 c->GetFrame()->Draw("same");
 c->Print("limitplot.eps");
 c->Print("limitplot_HardSelection.pdf");
 c->Print("limitplot.png");


}


void LimitPlot() {



    setThesisStyle();

    limit();
 
}

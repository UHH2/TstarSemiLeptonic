//CorrMatchMacro.C


void CorrMatchMacro() {
 // open a file and get a histogram
 TFile *s1 = new TFile("/nfs/dust/cms/user/multh/RunII_76X_v1/Selection/FINAL/Match/uhh2.AnalysisModuleRunner.MC.Tstar_M700.root");
 TFile *s2 = new TFile("/nfs/dust/cms/user/multh/RunII_76X_v1/Selection/FINAL/Match/uhh2.AnalysisModuleRunner.MC.Tstar_M800.root");
 TFile *s3 = new TFile("/nfs/dust/cms/user/multh/RunII_76X_v1/Selection/FINAL/Match/uhh2.AnalysisModuleRunner.MC.Tstar_M900.root");
 TFile *s4 = new TFile("/nfs/dust/cms/user/multh/RunII_76X_v1/Selection/FINAL/Match/uhh2.AnalysisModuleRunner.MC.Tstar_M1000.root");
 TFile *s5 = new TFile("/nfs/dust/cms/user/multh/RunII_76X_v1/Selection/FINAL/Match/uhh2.AnalysisModuleRunner.MC.Tstar_M1100.root");
 TFile *s6 = new TFile("/nfs/dust/cms/user/multh/RunII_76X_v1/Selection/FINAL/Match/uhh2.AnalysisModuleRunner.MC.Tstar_M1200.root");
 TFile *s7 = new TFile("/nfs/dust/cms/user/multh/RunII_76X_v1/Selection/FINAL/Match/uhh2.AnalysisModuleRunner.MC.Tstar_M1300.root");
 TFile *s8 = new TFile("/nfs/dust/cms/user/multh/RunII_76X_v1/Selection/FINAL/Match/uhh2.AnalysisModuleRunner.MC.Tstar_M1400.root");
 TFile *s9 = new TFile("/nfs/dust/cms/user/multh/RunII_76X_v1/Selection/FINAL/Match/uhh2.AnalysisModuleRunner.MC.Tstar_M1500.root");
 TFile *s10 = new TFile("/nfs/dust/cms/user/multh/RunII_76X_v1/Selection/FINAL/Match/uhh2.AnalysisModuleRunner.MC.Tstar_M1600.root");

 
 TH1F *h1    = (TH1F*)s1->Get("corrmatch__HypHists/M_tophad");
 TH1F *h2    = (TH1F*)s2->Get("corrmatch__HypHists/M_tophad");
 TH1F *h3    = (TH1F*)s3->Get("corrmatch__HypHists/M_tophad");
 TH1F *h4    = (TH1F*)s4->Get("corrmatch__HypHists/M_tophad");
 TH1F *h5    = (TH1F*)s5->Get("corrmatch__HypHists/M_tophad");
 TH1F *h6    = (TH1F*)s6->Get("corrmatch__HypHists/M_tophad");
 TH1F *h7    = (TH1F*)s7->Get("corrmatch__HypHists/M_tophad");
 TH1F *h8    = (TH1F*)s8->Get("corrmatch__HypHists/M_tophad");
 TH1F *h9    = (TH1F*)s9->Get("corrmatch__HypHists/M_tophad");
 TH1F *h10   = (TH1F*)s10->Get("corrmatch__HypHists/M_tophad");

TList *list1 = new TList; 
list1->Add(h1); 
list1->Add(h2);
list1->Add(h3);
list1->Add(h4);
list1->Add(h5);
list1->Add(h6);
list1->Add(h7);
list1->Add(h8);
list1->Add(h9);
list1->Add(h10);
/*
TH1F* histos_2[10]={t1,t2,t3,t4,t5,t6,t7,t8,t9,t10};
for (int i=0;i<10;++i){
         histos_2[i]->Sumw2(); // *Very* Important
        }

TList *list2 = new TList; 
list2->Add(t1); 
list2->Add(t2);
list2->Add(t3);
list2->Add(t4);
list2->Add(t5);
list2->Add(t6);
list2->Add(t7);
list2->Add(t8);
list2->Add(t9);
list2->Add(t10);

TH1F* histos_3[10]={l1,l2,l3,l4,l5,l6,l7,l8,l9,l10};
for (int i=0;i<10;++i){
         histos_3[i]->Sumw2(); // *Very* Important
        }

TList *list3 = new TList; 
list3->Add(l1); 
list3->Add(l2);
list3->Add(l3);
list3->Add(l4);
list3->Add(l5);
list3->Add(l6);
list3->Add(l7);
list3->Add(l8);
list3->Add(l9);
list3->Add(l10);
*/
 gStyle->SetOptStat(0);
 gROOT->ForceStyle();
 int bin_up = 630;
  int bin_down = 510;

TCanvas* c1= new TCanvas("c1","c1",bin_up,bin_down);
TH1F *h = (TH1F*)h1->Clone("h"); 
h->Reset(); 
h->Merge(list1); 
 h->SetTitle("");
 h->GetXaxis()->SetTitle("M_{top}^{had} [GeV/c^{2}]");
 h->GetXaxis()->SetTitleSize(0.045);
 h->GetXaxis()->SetTitleOffset(1.1);
 h->GetYaxis()->SetTitle("Anzahl MC-Ereignisse");
 h->GetYaxis()->SetTitleSize(0.04);
 h->GetYaxis()->SetTitleOffset(1.1);
h->Draw();
}
void create_file(const char *dirname, TString filename, TString histoname, TString Prefix);
void limit(const char *dirname="/nfs/dust/cms/user/multh/RunII_76X_v1/Selection/FINAL/Nominal/"){

  //dirname2 = "/nfs/dust/cms/user/gonvaq/CMSSW/CMSSW_7_4_3/src/UHH2/VLQToTopAndLepton/config/";
  TString directories[] = {"/"};

  TString names[] = {"QCD","TTbar","SingleTop","WJets","ZJets","Diboson","Tstar_M700","Tstar_M800","Tstar_M900","Tstar_M1000","Tstar_M1100","Tstar_M1200","Tstar_M1300","Tstar_M1400","Tstar_M1500","Tstar_M1600"};
  
  TString histograms[] = {"chi2min_ttag_comb__HypHists/M_Tstar_comb"};
 
  TString Prefix[] = {"comb"};

  TFile* file2  = new TFile("/nfs/dust/cms/user/multh/RunII_76X_v1/Limit/Tstar_comb.root","RECREATE");
  file2->Close();
  
 
  for(int p =0; p < sizeof(names)/sizeof(TString); ++p){
    for(int m =0; m< sizeof(directories)/sizeof(TString); ++m){
      for(int i = 0; i< sizeof(histograms)/sizeof(TString); ++i){
	//if(p<3)
	create_file(dirname+directories[m], names[p],histograms[i],"Tstar"+Prefix[i]);
	//else
	//  create_file(dirname2+directories[m], names[p],histograms[i], "Tstar"+Prefix[i]);
      }
      cout<<names[p]<<" --> Done"<<endl;
    }
  }
}
 
void create_file(const char *dirname, TString filename, TString histoname, TString Prefix){
  
  TSystemDirectory dir(dirname, dirname);
  TList *fileslist = dir.GetListOfFiles();
  if (fileslist) {
    TSystemFile *systemfile;
    TString fname;
    TIter next(fileslist);
    while ((systemfile=(TSystemFile*)next())) {
      fname = systemfile->GetName();
      //cout<<fname<<endl;
      if (!systemfile->IsDirectory() && fname.Contains(filename+".root")) {
	cout<<Prefix<<endl;
	TFile* file  = new TFile(dirname+fname,"READ");
	TH1F* h1 = (TH1F*)file->Get(histoname);
	//if(!fname.Contains("Bpb_TW")) h1->Scale(0.25);
	//h1->Scale(15);
	TFile* file2  = new TFile("/nfs/dust/cms/user/multh/RunII_76X_v1/Limit/Tstar_comb.root","UPDATE");
	TString histname = h1->GetName();
	h1->SetName(Prefix+"__"+filename);
	h1->Write();
	file->Close();
	file2->Close();
      }
    }
  }
}

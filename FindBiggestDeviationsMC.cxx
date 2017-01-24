

{

  //Scale Variation
  //In each bin find the lowest and the highest entry of all 6 variations + nominal
  //lowest: down-variation for theta, highest: up-variation for theta, nominal: nominal for theta
  //don't care wether the nominal one is the lowest in some bins --> good luck for us



  //Files & histograms for other processes
  TFile* f_uu = new TFile("/nfs/dust/cms/user/multh/RunII_76X_v1/Selection/FINAL/ScaleVariation_UpUp/uhh2.AnalysisModuleRunner.MC.TTbar.root","READ");
  TFile* f_un = new TFile("/nfs/dust/cms/user/multh/RunII_76X_v1/Selection/FINAL/ScaleVariation_UpNone/uhh2.AnalysisModuleRunner.MC.TTbar.root","READ");
  TFile* f_nu = new TFile("/nfs/dust/cms/user/multh/RunII_76X_v1/Selection/FINAL/ScaleVariation_NoneUp/uhh2.AnalysisModuleRunner.MC.TTbar.root","READ");
  TFile* f_nd = new TFile("/nfs/dust/cms/user/multh/RunII_76X_v1/Selection/FINAL/ScaleVariation_NoneDown/uhh2.AnalysisModuleRunner.MC.TTbar.root","READ");
  TFile* f_dn = new TFile("/nfs/dust/cms/user/multh/RunII_76X_v1/Selection/FINAL/ScaleVariation_DownNone/uhh2.AnalysisModuleRunner.MC.TTbar.root","READ");
  TFile* f_dd = new TFile("/nfs/dust/cms/user/multh/RunII_76X_v1/Selection/FINAL/ScaleVariation_DownDown/uhh2.AnalysisModuleRunner.MC.TTbar.root","READ");
  TFile* f_nom = new TFile("/nfs/dust/cms/user/multh/RunII_76X_v1/Selection/FINAL/Nominal/uhh2.AnalysisModuleRunner.MC.TTbar.root","READ");

  TH1D* h_uu = (TH1D*)f_uu->Get("chi2min_ttag_comb__HypHists/M_Tstar_comb"); 
  TH1D* h_un = (TH1D*)f_un->Get("chi2min_ttag_comb__HypHists/M_Tstar_comb"); 
  TH1D* h_nu = (TH1D*)f_nu->Get("chi2min_ttag_comb__HypHists/M_Tstar_comb"); 
  TH1D* h_nd = (TH1D*)f_nd->Get("chi2min_ttag_comb__HypHists/M_Tstar_comb"); 
  TH1D* h_dn = (TH1D*)f_dn->Get("chi2min_ttag_comb__HypHists/M_Tstar_comb"); 
  TH1D* h_dd = (TH1D*)f_dd->Get("chi2min_ttag_comb__HypHists/M_Tstar_comb"); 
  TH1D* h_nom = (TH1D*)f_nom->Get("chi2min_ttag_comb__HypHists/M_Tstar_comb"); 
  

  const int nbins = h_nom->GetNbinsX();
  if(nbins != 30) throw runtime_error("adjust number of bins! not 30 as it seems...");
  double min_bins[30];
  double max_bins[30];
  double max_err[30];
  double min_err[30];

  for(int i=1; i<h_nom->GetNbinsX()+1; i++){
    double entries[7] = {h_uu->GetBinContent(i),h_un->GetBinContent(i),h_nu->GetBinContent(i),h_nd->GetBinContent(i),h_dn->GetBinContent(i),h_dd->GetBinContent(i),h_nom->GetBinContent(i)};
    double errors[7] = {h_uu->GetBinError(i),h_un->GetBinError(i),h_nu->GetBinError(i),h_nd->GetBinError(i),h_dn->GetBinError(i),h_dd->GetBinError(i),h_nom->GetBinError(i)};
    double min = 50000;
    double max_error = 0, min_error = 0;
    double max = 0;
    for(int j=0; j<7; j++){
      if(entries[j] > max) {max = entries[j]; max_error = errors[j];}
      if(entries[j] < min) {min = entries[j]; min_error = errors[j];}
    }
    min_bins[i-1] = min;
    max_bins[i-1] = max;
    min_err[i-1] = min_error;
    max_err[i-1] = max_error;
  }

  cout << "minimum entries: " << endl;
  for(int i=0; i<h_nom->GetNbinsX(); i++){
    cout << min_bins[i] << " ";
  }
  cout << endl;

  cout << "maximum entries: " << endl;
  for(int i=0; i<h_nom->GetNbinsX(); i++){
    cout << max_bins[i] << " ";
  }
  cout << endl;

  if(nbins+1 != 31)throw runtime_error("adjust number of bin low-edges! not 31 as it seems...");
  double bins_low[31];
  for(int i=1; i<h_nom->GetNbinsX()+2; i++){
    bins_low[i-1] = h_nom->GetBinLowEdge(i);
    cout << "bin " << i<< ", low edge: " << bins_low[i-1] << endl;
  }
  /*
    TH1D* H_T_rebin3_up = new TH1D("H_T_rebin3_up","HT",  nbins, bins_low);
    for(int i=1; i<h_nom->GetNbinsX()+1; i++){
    H_T_rebin3_up->SetBinContent(i,max_bins[i-1]);
    H_T_rebin3_up->SetBinError(i, max_err[i-1]);
    }

    cout << "test: " << H_T_rebin3_up->GetBinContent(3) << endl;
    H_T_rebin3_up->Draw();

    TH1D* H_T_rebin3_dn = new TH1D("H_T_rebin3_dn","HT",  nbins, bins_low);
    for(int i=1; i<h_nom->GetNbinsX()+1; i++){
    H_T_rebin3_dn->SetBinContent(i,min_bins[i-1]);
    H_T_rebin3_dn->SetBinError(i, min_err[i-1]);
    }

    cout << "test: " << H_T_rebin3_dn->GetBinContent(3) << endl;
    H_T_rebin3_dn->Draw("SAME");
  */

  TH1D* Mass_Tstar_up = new TH1D("Mass_Tstar_up","M_{T^{*}}",  nbins, bins_low);
  for(int i=1; i<h_nom->GetNbinsX()+1; i++){
    Mass_Tstar_up->SetBinContent(i,max_bins[i-1]);
    Mass_Tstar_up->SetBinError(i, max_err[i-1]);
  }

  cout << "test: " << Mass_Tstar_up->GetBinContent(3) << endl;
  Mass_Tstar_up->SetLineColor(2);
  Mass_Tstar_up->Draw();

  TH1D* Mass_Tstar_dn = new TH1D("Mass_TStar_dn","M_{T^{*}}",  nbins, bins_low);
  for(int i=1; i<h_nom->GetNbinsX()+1; i++){
    Mass_Tstar_dn->SetBinContent(i,min_bins[i-1]);
    Mass_Tstar_dn->SetBinError(i, min_err[i-1]);
  }

  cout << "test: " << Mass_Tstar_dn->GetBinContent(3) << endl;
  Mass_Tstar_dn->SetLineColor(2);
  Mass_Tstar_dn->Draw("SAME");

  h_nom->SetLineColor(kBlack);
  h_nom->Draw("SAME");


  //Paths for other processes
  TFile* f_out_up = new TFile("/nfs/dust/cms/user/multh/RunII_76X_v1/Selection/FINAL/ScaleVariation_Up/uhh2.AnalysisModuleRunner.MC.TTbar.root","RECREATE");
  Mass_Tstar_up->Write();
  f_out_up->Close();
  TFile* f_out_dn = new TFile("/nfs/dust/cms/user/multh/RunII_76X_v1/Selection/FINAL/ScaleVariation_Down/uhh2.AnalysisModuleRunner.MC.TTbar.root","RECREATE");
  Mass_Tstar_dn->Write();
  f_out_dn->Close();
  cout << "wrote files." << endl;

}

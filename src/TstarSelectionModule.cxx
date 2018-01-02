#include <iostream>
#include <memory>

#include <UHH2/core/include/AnalysisModule.h>
#include <UHH2/core/include/Event.h>
#include <UHH2/common/include/EventHists.h>
#include <UHH2/core/include/Selection.h>

#include <UHH2/common/include/MCWeight.h>
#include <UHH2/common/include/CommonModules.h>
#include <UHH2/common/include/CleaningModules.h>
#include <UHH2/common/include/NSelections.h>
#include <UHH2/common/include/LumiSelection.h>
#include <UHH2/common/include/LuminosityHists.h>
#include <UHH2/common/include/TriggerSelection.h>
#include <UHH2/common/include/JetCorrections.h>
#include <UHH2/common/include/ObjectIdUtils.h>
#include <UHH2/common/include/MuonIds.h>
#include <UHH2/common/include/MuonHists.h>
#include <UHH2/common/include/ElectronIds.h>
#include <UHH2/common/include/ElectronHists.h>
#include <UHH2/common/include/JetIds.h>
#include <UHH2/common/include/JetHists.h>
#include <UHH2/common/include/TopJetIds.h>
#include <UHH2/common/include/TTbarGen.h>
#include <UHH2/common/include/TTbarReconstruction.h>
#include <UHH2/common/include/ReconstructionHypothesis.h>
#include <UHH2/common/include/ReconstructionHypothesisDiscriminators.h>
#include <UHH2/common/include/HypothesisHists.h>
#include <UHH2/common/include/Utils.h>
#include "UHH2/common/include/PrintingModules.h"
#include "UHH2/common/include/TopPtReweight.h"

#include <UHH2/TstarSemiLeptonic/include/HypothesisHistsOwn.h>
#include <UHH2/TstarSemiLeptonic/include/TStarGen.h>
#include <UHH2/TstarSemiLeptonic/include/TStarGenHists.h>
#include "UHH2/TstarSemiLeptonic/include/TstarPDFHists.h"
#include <UHH2/TstarSemiLeptonic/include/TstarSelectionHists.h>
#include <UHH2/TstarSemiLeptonic/include/TstarReconstruction.h>
#include <UHH2/TstarSemiLeptonic/include/TstarReconstructionHypothesis.h>
#include <UHH2/TstarSemiLeptonic/include/TstarReconstructionHypothesisDiscriminators.h>



#include <UHH2/TstarSemiLeptonic/include/TstarSemiLeptonicSelections.h>
#include <UHH2/TstarSemiLeptonic/include/TstarSemiLeptonicUtils.h>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;


/** \brief module to produce "Selection" ntuples for theT*T*->tg tg semileptonic analysis
 *
 *  -- GOALS:
 *   * complete object reconstruction (pt/eta cuts, IDs, jet-lepton cleaning, JER smearing)
 *   * apply (most of) the kinematic cuts for the lepton+jets SR. current cutflow:

 *     * JER smearing for TopJet collection
 *     * Common Modules 
 *     * HLT
 *     * ==1 lepton (w/ pt+eta+ID cuts)
 *     * >=4 AK4 jets w/ pt> 50 |eta|<2.4
 *     * >=1 b-tag /CSVBTag::MEDIUM
 *     * HT > 500 GeV
 *     * lepton-2D-cut [DR>0.4 || pTrel>25 GeV] (wrt AK4 jets w/ pt>25 GeV)
 *     * (electron-only) triangular cuts
 *     * perform T*T*bar kinematical reconstruction (hyps stored in output ntuple)
 *     * Reconstruction without TopTag 
 *
 *   * JER smearing for TopJet collection
 *   * update 2D cut values (validation ongoing)
 *
 */


class TstarSelectionModule : public uhh2::AnalysisModule {

 public:
  explicit TstarSelectionModule(uhh2::Context&);
  virtual bool process(uhh2::Event&) override;

 private:
  enum lepton { muon, elec };
  lepton channel_;

  uhh2::Event::Handle<int>                     h_flag_toptagevent;

  // common modules
  std::unique_ptr<CommonModules>               common;
    std::unique_ptr<TopPtReweight>             ttbar_reweight;
  std::unique_ptr<AnalysisModule>              syst_module_scale, SF_muonID, SF_muonTrigger, SF_btag, SF_muonIso ;
   
  // cleaners
  std::unique_ptr<JetCleaner>                  jet_cleaner2;
  std::unique_ptr<TopJetCorrector>             topjet_corrector_MC;
  std::unique_ptr<TopJetCorrector>             topjet_corrector_BCD;
  std::unique_ptr<TopJetCorrector>             topjet_corrector_EFearly;
  std::unique_ptr<TopJetCorrector>             topjet_corrector_FlateG;
  std::unique_ptr<TopJetCorrector>             topjet_corrector_H;

  std::unique_ptr<SubJetCorrector>             subjet_corrector_MC;
  std::unique_ptr<SubJetCorrector>             subjet_corrector_BCD;
  std::unique_ptr<SubJetCorrector>             subjet_corrector_EFearly;
  std::unique_ptr<SubJetCorrector>             subjet_corrector_FlateG;
  std::unique_ptr<SubJetCorrector>             subjet_corrector_H;

  std::unique_ptr<TopJetLeptonDeltaRCleaner>   topjetlepton_cleaner;
  std::unique_ptr<TopJetCleaner>               topjet_cleaner;

  // selections
  std::unique_ptr<Selection>                   trigger_Mu50;
  std::unique_ptr<Selection>                   trigger_TkMu50;

  //ISO
  std::unique_ptr<Selection>                   trigger_sel1;
  std::unique_ptr<Selection>                   trigger_sel2;

  std::unique_ptr<uhh2::AndSelection>          lep1_sel;
  std::unique_ptr<uhh2::Selection>             jet2_sel;
  std::unique_ptr<uhh2::Selection>             jet1_sel;
  std::unique_ptr<uhh2::Selection>             b_tag_medium_sel;
  std::unique_ptr<uhh2::Selection>             twodcut_sel;
  std::unique_ptr<uhh2::Selection>             toptagevt_sel;
  std::unique_ptr<uhh2::Selection>             jet_top_sel;
  std::unique_ptr<uhh2::Selection>             ht_sel;
  std::unique_ptr<Selection>                   lumi_sel;
  std::unique_ptr<uhh2::Selection>             met_sel;

  // ttbar reconstruction
  std::unique_ptr<uhh2::AnalysisModule>        reco_primlep;
  std::unique_ptr<uhh2::AnalysisModule>        ttgenprod;

  //T*T*  reconstruction
  std::unique_ptr<uhh2::AnalysisModule>        tstargenprod;
  std::unique_ptr<uhh2::AnalysisModule>        tstar_reco;  
  std::unique_ptr<uhh2::AnalysisModule>        tstar_ttag_reco;
  std::unique_ptr<uhh2::AnalysisModule>        tstar_chi2;
  std::unique_ptr<uhh2::AnalysisModule>        tstar_ttag_chi2;
  std::unique_ptr<uhh2::AnalysisModule>        tstar_match;


 
  ////######################################   HISTS   ###################################################################################################################
   std::unique_ptr<Hists>      h_tstargenhists;
   std::unique_ptr<Hists>      h_tstargenhists_eta;
   std::unique_ptr<Hists>      h_tstargenhists_pt;

  std::unique_ptr<uhh2::Hists> chi2_before_h;
  std::unique_ptr<uhh2::Hists> chi2min_h;
  std::unique_ptr<uhh2::Hists> chi2min_ttag_h;
  std::unique_ptr<uhh2::Hists> chi2min_ttag_comb_h;

  std::unique_ptr<uhh2::Hists> corrmatch_h;

  std::unique_ptr<Hists>       input_h,       input_h_event,       input_h_bJet,       input_h_jet,       input_h_ele,       input_h_muo,       input_h_topjet,       input_h_lumi;
  std::unique_ptr<Hists>       h_trigger,     h_trigger_event,     h_trigger_bJet,     h_trigger_jet,     h_trigger_ele,     h_trigger_muon,    h_trigger_topjet,     h_trigger_lumi;
  std::unique_ptr<Hists>       cleaner_h,     cleaner_h_event,     cleaner_h_bJet,     cleaner_h_jet,     cleaner_h_ele,     cleaner_h_muo,     cleaner_h_topjet,     cleaner_h_lumi;
  std::unique_ptr<Hists>       lep1_h,        lep1_h_event,        lep1_h_bJet,        lep1_h_jet,        lep1_h_ele,        lep1_h_muo,        lep1_h_topjet,        lep1_h_lumi;
  std::unique_ptr<Hists>       jet_top_h,     jet_top_h_event,     jet_top_h_bJet,     jet_top_h_jet,     jet_top_h_ele,     jet_top_h_muo,     jet_top_h_topjet,     jet_top_h_lumi;
  std::unique_ptr<Hists>       jet2_h,        jet2_h_event,        jet2_h_bJet,        jet2_h_jet,        jet2_h_ele,        jet2_h_muo,        jet2_h_topjet,        jet2_h_lumi;
  std::unique_ptr<Hists>       jet_h,         jet_h_event,         jet_h_bJet,         jet_h_jet,         jet_h_ele,         jet_h_muo,         jet_h_topjet,         jet_h_lumi;
  std::unique_ptr<Hists>       bTag_h,        bTag_h_event,        bTag_h_bJet,        bTag_h_jet,        bTag_h_ele,        bTag_h_muo,        bTag_h_topjet,        bTag_h_lumi;
  std::unique_ptr<Hists>       twodcut_h,     twodcut_h_event,     twodcut_h_bJet,     twodcut_h_jet,     twodcut_h_ele,     twodcut_h_muo,     twodcut_h_topjet,     twodcut_h_lumi;
  std::unique_ptr<Hists>       met_h,         met_h_event,         met_h_bJet,         met_h_jet,         met_h_ele,         met_h_muo,         met_h_topjet,         met_h_lumi;
  std::unique_ptr<Hists>       ht_h,          ht_h_event,          ht_h_bJet,          ht_h_jet,          ht_h_ele,          ht_h_muo,          ht_h_topjet,          ht_h_lumi;
  std::unique_ptr<Hists>       toptagevt_h,   toptagevt_h_event,   toptagevt_h_bJet,   toptagevt_h_jet,   toptagevt_h_ele,   toptagevt_h_muo,   toptagevt_h_topjet,   toptagevt_h_lumi;
  std::unique_ptr<Hists>       notoptagevt_h, notoptagevt_h_event, notoptagevt_h_bJet, notoptagevt_h_jet, notoptagevt_h_ele, notoptagevt_h_muo, notoptagevt_h_topjet, notoptagevt_h_lumi;
  std::unique_ptr<Hists>       output_h,      output_h_event,      output_h_bJet,      output_h_jet,      output_h_ele,      output_h_muo,      output_h_topjet,      output_h_lumi;
  
  std::unique_ptr<Hists>       GenParticles_h_event,  GenParticles_h_bJet,      GenParticles_h_jet,      GenParticles_h_ele,  GenParticles_h_muo,  GenParticles_h_topjet;
  std::unique_ptr<Hists>       corrmatch_h_event,     corrmatch_h_bJet,         corrmatch_h_jet,         corrmatch_h_ele,     corrmatch_h_muo,     corrmatch_h_topjet;
  std::unique_ptr<Hists>       no_corrmatch_h_event,  no_corrmatch_h_bJet,      no_corrmatch_h_jet,      no_corrmatch_h_ele,  no_corrmatch_h_muo,  no_corrmatch_h_topjet;
  std::unique_ptr<Hists>       chi2_h_event,          chi2_h_bJet,              chi2_h_jet,              chi2_h_ele,          chi2_h_muo,          chi2_h_topjet, chi2_h_lumi;

 
  std::unique_ptr<Hists>                 h_PDF_variations;
  std::unique_ptr<BTagMCEfficiencyHists> BTagEffHists;

///###########################################################################################################################################################################


  MuonId     MuId;
  ElectronId EleId;
  JetId      Btag_medium;
  JetId      jetID;
  
  bool       do_scale_variation,  is_mc,  do_pdf_variations;
  string     sys_bTag,  sys_PU, sys_muonTrigger, sys_muonID;


  uhh2::Event::Handle<TStarGen> h_tstargen;

  uhh2::Event::Handle<std::vector<TstarReconstructionHypothesis>> h_hyps;
};

///********************************************************************************************************************

TstarSelectionModule::TstarSelectionModule(uhh2::Context& ctx){
  
  std::cout << "Hello World from TstarSelectionModule!" << std::endl;
  
  //#############################  Systematics Module  ############################################################
    do_scale_variation = (ctx.get("ScaleVariationMuR") == "up" || ctx.get("ScaleVariationMuR") == "down") || (ctx.get("ScaleVariationMuF") == "up" || ctx.get("ScaleVariationMuF") == "down");
    do_pdf_variations  =  ctx.get("b_PDFUncertainties") == "true";
    is_mc              = ctx.get("dataset_type") == "MC";
    sys_muonID         = ctx.get("sys_muonID");
    sys_muonTrigger    = ctx.get("sys_muonTrigger");
    sys_bTag           = ctx.get("sys_bTag");
    sys_PU             = ctx.get("sys_PU");
    //#############################################################################################################

 
 //############ ID's for Muons/Electrons and Jet/TopJets ##########################################################
    // MuId = AndId<Muon>(MuonIDTight(), PtEtaCut(30.0, 2.4),MuonIso(0.15)); 

    MuId  = AndId<Muon>    (MuonIDTight(),              PtEtaCut(53., 2.4));
    EleId = AndId<Electron>(ElectronID_Spring16_tight,  PtEtaSCCut(50., 2.4));
    jetID = AndId<Jet>     (JetPFID(JetPFID::WP_LOOSE), PtEtaCut(15., 2.4));

    const TopJetId topjetID = AndId<TopJet>(Type2TopTag(105,220,Type2TopTag::MassType::groomed), Tau32(0.67));
  
  ///####################  CommonModules ######################################################################
    common.reset(new CommonModules());
    // common->disable_lumisel();            //Disable LumiSelection   -> Done in PreSelection
    common->set_electron_id(EleId);      
    common->switch_jetlepcleaner(true);
    //common->disable_jec();                  //Disable JEC   -> Done in PreSelection        
    //common->disable_jersmear();             //Disable JER   -> Done in PreSelection
    common->set_muon_id(MuId);
    common->set_jet_id(jetID);
    common->switch_jetPtSorter(true);    
    common->init(ctx,sys_PU);             //Systematics for PileUpReweight in Common Modules
 
/// ###########################################################################################################



//######################### MC-ScaleVariation & B-Tag & T-Tag   ############################################################################
//MC-ScaleVariation 
 syst_module_scale  .reset(new MCScaleVariation(ctx));
 /*
//Muon Trigger/Scale Factor
 SF_muonID          .reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/multh/CMSSW_8_0_26_patch2/src/UHH2/common/data/MuonID_EfficienciesAndSF_average_RunBtoH.root", "MC_NUM_TightID_DEN_genTracks_PAR_pt_eta", 1)); 
 SF_muonTrigger.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/multh/CMSSW_8_0_26_patch2/src/UHH2/common/data/MuonTrigger_EfficienciesAndSF_average_RunBtoH.root", "IsoMu24_OR_IsoTkMu24_PtEtaBins", 0.5));
 SF_muonIso.reset(new MCMuonScaleFactor(ctx, "/nfs/dust/cms/user/multh/CMSSW_8_0_26_patch2/src/UHH2/common/data/MuonIso_EfficienciesAndSF_average_RunBtoH.root", "TightISO_TightID_pt_eta", 1));
 */

 SF_muonID          .reset(new MCMuonScaleFactor(ctx,"/nfs/dust/cms/user/multh/CMSSW_8_0_26_patch2/src/UHH2/common/data/MuonID_EfficienciesAndSF_average_RunBtoH.root", "MC_NUM_TightID_DEN_genTracks_PAR_pt_eta",1)); 
 SF_muonTrigger     .reset(new MCMuonScaleFactor(ctx,"/nfs/dust/cms/user/multh/CMSSW_8_0_26_patch2/src/UHH2/common/data/MuonTrigger_EfficienciesAndSF_average_RunBtoH.root", "IsoMu50_OR_IsoTkMu50_PtEtaBins",0.5));


// b-Tagging
//   BTagEffHists.reset(new BTagMCEfficiencyHists(ctx,"EffHists/BTag",CSVBTag::WP_MEDIUM));
 SF_btag            .reset(new MCBTagScaleFactor(ctx,CSVBTag::WP_MEDIUM,"jets",sys_bTag));  // "mujets","incl","MCBtagEfficienciesAK4","_AK4","BTagCalibration"
 syst_module_scale  .reset(new MCScaleVariation(ctx));

// top-tagging
const float minDR_topjet_jet(1.2);
 toptagevt_sel      .reset(new TopTagEventSelection(topjetID, minDR_topjet_jet));

 h_flag_toptagevent = ctx.declare_event_output<int>("flag_toptagevent");

//##########################################################################################################################################

/// ######################  Cannel and Jet/TopJet Cleaners  ###############################################################################
  const std::string& channel = ctx.get("channel", "");
   if     (channel == "muon") channel_ = muon;
   else if(channel == "elec") channel_ = elec;
   else throw std::runtime_error("TstarSelectionModule -- undefined argument for 'channel' key in xml file (must be 'muon' or 'elec'): "+channel);

   jet_cleaner2               .reset(new JetCleaner(ctx, 30., 2.4));

   topjetlepton_cleaner       .reset(new TopJetLeptonDeltaRCleaner(.8));
   topjet_cleaner             .reset(new TopJetCleaner(ctx, TopJetId(PtEtaCut(400., 2.4))));

  //TopJet Corector
  if(is_mc){
    topjet_corrector_MC       .reset(new TopJetCorrector(ctx,JERFiles::Summer16_23Sep2016_V4_L123_AK8PFchs_MC));   
    subjet_corrector_MC       .reset(new SubJetCorrector(ctx,JERFiles::Summer16_23Sep2016_V4_L123_AK4PFchs_MC));   //For MC
  }
  else {                                                                                                            //For Data ->
    topjet_corrector_BCD      .reset(new TopJetCorrector(ctx,JERFiles::Summer16_23Sep2016_V4_BCD_L123_AK8PFchs_DATA));    //            Run Period BCD
    topjet_corrector_EFearly  .reset(new TopJetCorrector(ctx,JERFiles::Summer16_23Sep2016_V4_EF_L123_AK8PFchs_DATA)); //            Run Period EF
    topjet_corrector_FlateG   .reset(new TopJetCorrector(ctx,JERFiles::Summer16_23Sep2016_V4_G_L123_AK8PFchs_DATA));   //            Run Period G
    topjet_corrector_H        .reset(new TopJetCorrector(ctx,JERFiles::Summer16_23Sep2016_V4_H_L123_AK8PFchs_DATA));        //            Run Period H

    subjet_corrector_BCD      .reset(new SubJetCorrector(ctx,JERFiles::Summer16_23Sep2016_V4_BCD_L123_AK4PFchs_DATA));    //            Run Period BCD
    subjet_corrector_EFearly  .reset(new SubJetCorrector(ctx,JERFiles::Summer16_23Sep2016_V4_EF_L123_AK4PFchs_DATA)); //            Run Period EF
    subjet_corrector_FlateG   .reset(new SubJetCorrector(ctx,JERFiles::Summer16_23Sep2016_V4_G_L123_AK4PFchs_DATA));   //            Run Period G
    subjet_corrector_H        .reset(new SubJetCorrector(ctx,JERFiles::Summer16_23Sep2016_V4_H_L123_AK4PFchs_DATA));        //            Run Period H

  }
///###################################################################################################################################



////############################  EVENT SELECTIONS #################################################################################

 //Trigger Selection

 trigger_sel1.reset(new TriggerSelection("HLT_IsoMu24_v*")); //original: IsoMu24
 trigger_sel2.reset(new TriggerSelection("HLT_IsoTkMu24_v*")); //original: IsoMu24


  trigger_Mu50.reset(new TriggerSelection("HLT_Mu50_v*"));              //As recommended on MuonTwiki for Non-iso Muons
  trigger_TkMu50.reset(new TriggerSelection("HLT_TkMu50_v*"));

//Lepton Selection
  lep1_sel.reset(new uhh2::AndSelection(ctx));
  if(channel_ == muon){
    lep1_sel->add<NMuonSelection>    ("muoN == 1", 1, 1);
    lep1_sel->add<NElectronSelection>("eleN == 0", 0, 0);
  }
  else if(channel_ == elec){
    lep1_sel->add<NMuonSelection>    ("muoN == 0", 0, 0);
    lep1_sel->add<NElectronSelection>("eleN == 1", 1, 1);
  }

  
  ///Jet Selection
  jet_top_sel         .reset(new NJetSelection(4, -1, JetId(PtEtaCut( 50., 2.4))));  //general Jet-Selection
  jet2_sel            .reset(new NJetSelection(5, -1, JetId(PtEtaCut( 50., 2.4))));  //Jet-Selection without Top-Tag
  ////

  ///b-Tag
  Btag_medium = CSVBTag(CSVBTag::WP_MEDIUM);                          //Workingpoint for b-Tag (Medium)  
  b_tag_medium_sel    .reset(new NJetSelection(1,-1, Btag_medium));  
  ///

  ///2D Cut Lepton-Jets
  twodcut_sel         .reset(new TwoDCut(.4, 25.));   //Still under investigation! QCD Problems
  ///

  ///MET Cut 
  met_sel             .reset(new METCut(80));     //Under investigation! QCD Reduction
  //

  ///HT Selection
  ht_sel              .reset(new HtSelection(500,-1));

 //Lumi Selection (For Data Only)
  lumi_sel            .reset(new LumiSelection(ctx));

 //###################################################################################################################################


////############################  TTBAR KINEMATICAL RECO ###############################################################################
  const std::string ttbar_gen_label ("ttbargen");
  const std::string tstar_gen_label ("tstargen");
  
  ttgenprod        .reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));
  tstargenprod     .reset(new TStarGenProducer(ctx, tstar_gen_label, false));
  h_tstargen = ctx.get_handle<TStarGen>("tstargen");

ttbar_reweight      .reset(new TopPtReweight(ctx, 0.0615, -0.0005, "", "weight_ttbar", true)); //13TeV

  // Lepton
  reco_primlep     .reset(new PrimaryLepton(ctx));

  // TStar Reco
  tstar_reco       .reset(new HighMassTstarReconstruction(ctx, TstarNeutrinoReconstruction, "TstarReconstruction"));
  tstar_ttag_reco  .reset(new TstarTopTagReconstruction(ctx, NeutrinoReconstruction, "TstarReconstruction", topjetID, 1.2));
  tstar_chi2       .reset(new TstarChi2Discriminator(ctx, "TstarReconstruction"));
  tstar_ttag_chi2  .reset(new TstarChi2DiscriminatorTTAG(ctx, "TstarReconstruction"));


  // Hypothesis Container
  h_hyps = ctx.get_handle<std::vector<TstarReconstructionHypothesis>>("TstarReconstruction");
  
////#########################################################################################################################################


////########################### Setup HISTS #################################################################################################
 
//Hists for Reconstruction
  chi2_before_h       .reset(new HypothesisHistsOwn(ctx, "chi2before__HypHists", "TstarReconstruction", "Chi2")); 
  chi2min_h           .reset(new HypothesisHistsOwn(ctx, "chi2min__HypHists", "TstarReconstruction", "Chi2")); 
  chi2min_ttag_h      .reset(new HypothesisHistsOwn(ctx, "chi2min_ttag__HypHists", "TstarReconstruction", "Chi2")); 
  chi2min_ttag_comb_h .reset(new HypothesisHistsOwn(ctx, "chi2min_ttag_comb__HypHists","TstarReconstruction", "Chi2"));


// GenHists 
  h_tstargenhists     .reset(new TStarGenHists(ctx, "tstargenhists"));
  h_tstargenhists_eta .reset(new TStarGenHists(ctx, "tstargenhists_eta"));
  h_tstargenhists_pt  .reset(new TStarGenHists(ctx, "tstargenhists_pt"));

//Hist after every Selection Step
  input_h             .reset(new TstarSelectionHists(ctx, "input"));
  input_h_event       .reset(new EventHists(ctx, "input_Event"));
  input_h_bJet        .reset(new TstarSelectionHists(ctx, "input_bJet"));
  input_h_jet         .reset(new JetHists(ctx, "input_Jets",6));
  input_h_ele         .reset(new ElectronHists(ctx, "input_Electrons"));
  input_h_muo         .reset(new MuonHists(ctx, "input_Muons"));
  input_h_topjet      .reset(new TopJetHists(ctx, "input_TopJets"));
  input_h_lumi        .reset(new LuminosityHists(ctx, "input_Lumi"));

  h_trigger           .reset(new TstarSelectionHists(ctx, "trigger"));
  h_trigger_event     .reset(new EventHists(ctx, "trigger_Event"));
  h_trigger_bJet      .reset(new TstarSelectionHists(ctx, "trigger_bJet"));
  h_trigger_jet       .reset(new JetHists(ctx, "trigger_Jets",6));
  h_trigger_ele       .reset(new ElectronHists(ctx, "trigger_Electrons"));
  h_trigger_muon      .reset(new MuonHists(ctx, "trigger_Muons"));
  h_trigger_topjet    .reset(new TopJetHists(ctx, "trigger_TopJets"));
  h_trigger_lumi      .reset(new LuminosityHists(ctx, "trigger_Lumi"));



  cleaner_h           .reset(new TstarSelectionHists(ctx, "cleaner"));
  cleaner_h_event     .reset(new EventHists(ctx, "cleaner_Event"));
  cleaner_h_bJet      .reset(new TstarSelectionHists(ctx, "cleaner_bJet"));
  cleaner_h_jet       .reset(new JetHists(ctx, "cleaner_Jets",6));
  cleaner_h_ele       .reset(new ElectronHists(ctx, "cleaner_Electrons"));
  cleaner_h_muo       .reset(new MuonHists(ctx, "cleaner_Muons"));
  cleaner_h_topjet    .reset(new TopJetHists(ctx, "cleaner_TopJets"));
  cleaner_h_lumi      .reset(new LuminosityHists(ctx, "cleaner_Lumi"));

  lep1_h              .reset(new TstarSelectionHists(ctx, "1Muon"));
  lep1_h_event        .reset(new EventHists(ctx, "1Muon_Event"));
  lep1_h_bJet         .reset(new TstarSelectionHists(ctx, "1Muon_bJet"));
  lep1_h_jet          .reset(new JetHists(ctx, "1Muon_Jets",6));
  lep1_h_ele          .reset(new ElectronHists(ctx, "1Muon_Electrons"));
  lep1_h_muo          .reset(new MuonHists(ctx, "1Muon_Muons"));
  lep1_h_topjet       .reset(new TopJetHists(ctx, "1Muon_TopJets")); 
  lep1_h_lumi         .reset(new LuminosityHists(ctx, "1Muon_Lumi"));

  jet_top_h           .reset(new TstarSelectionHists(ctx, "4Jets"));
  jet_top_h_event     .reset(new EventHists(ctx, "4Jets_Event"));
  jet_top_h_bJet      .reset(new TstarSelectionHists(ctx, "4Jets_bJet"));
  jet_top_h_jet       .reset(new JetHists(ctx, "4Jets_Jets",6));
  jet_top_h_ele       .reset(new ElectronHists(ctx, "4Jets_Electrons"));
  jet_top_h_muo       .reset(new MuonHists(ctx, "4Jets_Muons"));
  jet_top_h_topjet    .reset(new TopJetHists(ctx, "4Jets_TopJets")); 
  jet_top_h_lumi      .reset(new LuminosityHists(ctx, "4Jets_Lumi"));

  jet2_h              .reset(new TstarSelectionHists(ctx, "5Jets"));
  jet2_h_event        .reset(new EventHists(ctx, "5Jets_Event"));
  jet2_h_bJet         .reset(new TstarSelectionHists(ctx, "5Jets_bJet"));
  jet2_h_jet          .reset(new JetHists(ctx, "5Jets_Jets",6));
  jet2_h_ele          .reset(new ElectronHists(ctx, "5Jets_Electrons"));
  jet2_h_muo          .reset(new MuonHists(ctx, "5Jets_Muons"));
  jet2_h_topjet       .reset(new TopJetHists(ctx, "5Jets_TopJets")); 
  jet2_h_lumi         .reset(new LuminosityHists(ctx, "5Jets_Lumi"));

  jet_h              .reset(new TstarSelectionHists(ctx, "Jets"));
  jet_h_event        .reset(new EventHists(ctx, "Jets_Event"));
  jet_h_bJet         .reset(new TstarSelectionHists(ctx, "Jets_bJet"));
  jet_h_jet          .reset(new JetHists(ctx, "Jets_Jets",6));
  jet_h_ele          .reset(new ElectronHists(ctx, "Jets_Electrons"));
  jet_h_muo          .reset(new MuonHists(ctx, "Jets_Muons"));
  jet_h_topjet       .reset(new TopJetHists(ctx, "Jets_TopJets")); 
  jet_h_lumi         .reset(new LuminosityHists(ctx, "Jets_Lumi"));

  bTag_h              .reset(new TstarSelectionHists(ctx, "bTag"));
  bTag_h_event        .reset(new EventHists(ctx, "bTag_Event"));
  bTag_h_bJet         .reset(new TstarSelectionHists(ctx, "bTag_bJet"));
  bTag_h_jet          .reset(new JetHists(ctx, "bTag_Jets",6));
  bTag_h_ele          .reset(new ElectronHists(ctx, "bTag_Electrons"));
  bTag_h_muo          .reset(new MuonHists(ctx, "bTag_Muons"));
  bTag_h_topjet       .reset(new TopJetHists(ctx, "bTag_TopJets")); 
  bTag_h_lumi         .reset(new LuminosityHists(ctx, "bTag_Lumi"));

  twodcut_h           .reset(new TstarSelectionHists(ctx, "twodcut"));
  twodcut_h_event     .reset(new EventHists(ctx, "twodcut_Event"));
  twodcut_h_bJet      .reset(new TstarSelectionHists(ctx, "twodcut_bJet"));
  twodcut_h_jet       .reset(new JetHists(ctx, "twodcut_Jets",6));
  twodcut_h_ele       .reset(new ElectronHists(ctx, "twodcut_Electrons"));
  twodcut_h_muo       .reset(new MuonHists(ctx, "twodcut_Muons"));
  twodcut_h_topjet    .reset(new TopJetHists(ctx, "twodcut_TopJets")); 
  twodcut_h_lumi      .reset(new LuminosityHists(ctx, "twodcut_Lumi"));

  met_h               .reset(new TstarSelectionHists(ctx, "met"));
  met_h_event         .reset(new EventHists(ctx, "met_Event"));
  met_h_bJet          .reset(new TstarSelectionHists(ctx, "met_bJet"));
  met_h_jet           .reset(new JetHists(ctx, "met_Jets",6));
  met_h_ele           .reset(new ElectronHists(ctx, "met_Electrons"));
  met_h_muo           .reset(new MuonHists(ctx, "met_Muons"));
  met_h_topjet        .reset(new TopJetHists(ctx, "met_TopJets"));
  met_h_lumi          .reset(new LuminosityHists(ctx, "met_Lumi"));


  ht_h                .reset(new TstarSelectionHists(ctx, "HT500"));
  ht_h_event          .reset(new EventHists(ctx, "HT500_Event"));
  ht_h_bJet           .reset(new TstarSelectionHists(ctx, "HT500_bJet"));
  ht_h_jet            .reset(new JetHists(ctx, "HT500_Jets",6));
  ht_h_ele            .reset(new ElectronHists(ctx, "HT500_Electrons"));
  ht_h_muo            .reset(new MuonHists(ctx, "HT500_Muons"));
  ht_h_topjet         .reset(new TopJetHists(ctx, "HT500_TopJets")); 
  ht_h_lumi           .reset(new LuminosityHists(ctx, "HT500_Lumi"));


  toptagevt_h         .reset(new TstarSelectionHists(ctx, "toptagevt"));
  toptagevt_h_event   .reset(new EventHists(ctx, "toptagevt_Event"));
  toptagevt_h_bJet    .reset(new TstarSelectionHists(ctx, "toptagevt_bJet"));
  toptagevt_h_jet     .reset(new JetHists(ctx, "toptagevt_Jets",6));
  toptagevt_h_ele     .reset(new ElectronHists(ctx, "toptagevt_Electrons"));
  toptagevt_h_muo     .reset(new MuonHists(ctx, "toptagevt_Muons"));
  toptagevt_h_topjet  .reset(new TopJetHists(ctx, "toptagevt_TopJets")); 
  toptagevt_h_lumi    .reset(new LuminosityHists(ctx, "toptagevt_Lumi"));

  notoptagevt_h       .reset(new TstarSelectionHists(ctx, "notoptagevt"));
  notoptagevt_h_event .reset(new EventHists(ctx, "notoptagevt_Event"));
  notoptagevt_h_bJet  .reset(new TstarSelectionHists(ctx, "notoptagevt_bJet"));
  notoptagevt_h_jet   .reset(new JetHists(ctx, "notoptagevt_Jets",6));
  notoptagevt_h_ele   .reset(new ElectronHists(ctx, "notoptagevt_Electrons"));
  notoptagevt_h_muo   .reset(new MuonHists(ctx, "notoptagevt_Muons"));
  notoptagevt_h_topjet.reset(new TopJetHists(ctx, "notoptagevt_TopJets")); 
  notoptagevt_h_lumi  .reset(new LuminosityHists(ctx, "notoptagevt_Lumi"));


  chi2_h_event           .reset(new EventHists(ctx, "chi2_Event"));
  chi2_h_bJet            .reset(new TstarSelectionHists(ctx, "chi2_bJet"));
  chi2_h_jet             .reset(new JetHists(ctx, "chi2_Jets",6));
  chi2_h_ele             .reset(new ElectronHists(ctx, "chi2_Electrons"));
  chi2_h_muo             .reset(new MuonHists(ctx, "chi2_Muons"));
  chi2_h_topjet          .reset(new TopJetHists(ctx, "chi2_TopJets"));
  chi2_h_lumi            .reset(new LuminosityHists(ctx, "chi2_Lumi"));

  GenParticles_h_event   .reset(new EventHists(ctx, "GenParticles_Event"));
  GenParticles_h_bJet    .reset(new TstarSelectionHists(ctx, "GenParticles_bJet"));
  GenParticles_h_jet     .reset(new JetHists(ctx, "GenParticles_Jets",6));
  GenParticles_h_ele     .reset(new ElectronHists(ctx, "GenParticles_Electrons"));
  GenParticles_h_muo     .reset(new MuonHists(ctx, "GenParticles_Muons"));
  GenParticles_h_topjet  .reset(new TopJetHists(ctx, "GenParticles_TopJets"));

  output_h               .reset(new TstarSelectionHists(ctx, "output"));
  output_h_event         .reset(new EventHists(ctx, "output_Event"));
  output_h_bJet          .reset(new TstarSelectionHists(ctx, "output_bJet"));
  output_h_jet           .reset(new JetHists(ctx, "output_Jets",6));
  output_h_ele           .reset(new ElectronHists(ctx, "output_Electrons"));
  output_h_muo           .reset(new MuonHists(ctx, "output_Muons"));
  output_h_topjet        .reset(new TopJetHists(ctx, "output_TopJets"));
  output_h_lumi          .reset(new LuminosityHists(ctx, "output_Lumi"));


//Hist for PDF Variations
  h_PDF_variations      .reset(new TstarPDFHists(ctx, "PDF_variations", do_pdf_variations));

//################################################################################################
}


bool TstarSelectionModule::process(uhh2::Event& event){
 
//*****************************************************************************************************
//
// \brief module to produce "Selection" ntuples for the T*T*->ttbar gg semileptonic analysis
//  
//*****************************************************************************************************

//Only for Data: Lumi-Selections
if(!is_mc){
   if(!lumi_sel->passes(event)) return false;
    }

//Dump input content
  input_h         ->fill(event);
  input_h_event   ->fill(event);
  input_h_bJet    ->fill(event);
  input_h_jet     ->fill(event);
  input_h_ele     ->fill(event);
  input_h_muo     ->fill(event);
  input_h_topjet  ->fill(event);
  input_h_lumi    ->fill(event);


  ttbar_reweight->process(event);


 //// ############## Trigger Selection  ##################################################
if (!is_mc){

  //if(!(trigger_sel1->passes(event) || trigger_sel2->passes(event))) return false; 
  
    bool pass_Mu50 = trigger_Mu50->passes(event);
    if (event.run<274954) {
      if (!pass_Mu50) return false;
    }
    else {
      bool pass_TrkMu50 = trigger_TkMu50->passes(event);
      if (!(pass_Mu50||pass_TrkMu50)) return false;
      }
  }

  else{
    bool pass_Mu50 = trigger_Mu50->passes(event);
    bool pass_TrkMu50 = trigger_TkMu50->passes(event);
    if (!(pass_Mu50||pass_TrkMu50)) return false;
    }


// Scale Factors for MuonID/MuonTrigger
  SF_muonTrigger->process(event);
   SF_muonID->process(event);
   // SF_muonIso->process(event);

  h_trigger       ->fill(event);
  h_trigger_event ->fill(event);
  h_trigger_bJet  ->fill(event);
  h_trigger_jet   ->fill(event);
  h_trigger_ele   ->fill(event);
  h_trigger_muon  ->fill(event);
  h_trigger_topjet->fill(event);
  h_trigger_lumi  ->fill(event);

//// ####################################################################################


//Do MC-Scale Variations for Systematics
  if(do_scale_variation){
    syst_module_scale->process(event);
  }
// ############## Common Modules and Object Cleaning #################################### 

//Common Modules
  bool pass_common = common->process(event);
    if(!pass_common) return false;
 

 ////Object Cleaning for Jets/TopJets   
    const int runnr_BCD = 276811;
    const int runnr_EFearly = 278802;
    const int runnr_FlateG = 280385;

    if(is_mc){
      topjet_corrector_MC->process(event);
      subjet_corrector_MC->process(event);
    }
    else{
      /*
      if(event.run <= runnr_BCD)          topjet_corrector_BCD->process(event);
      else if(event.run < runnr_EFearly)  topjet_corrector_EFearly->process(event);
      else if(event.run <= runnr_FlateG)  topjet_corrector_FlateG->process(event);
      else if(event.run > runnr_FlateG)   topjet_corrector_H->process(event);
      else throw runtime_error("TstarPreSelectionModule: run number not covered");
      */
      if(event.run <= runnr_BCD)          subjet_corrector_BCD->process(event);
      else if(event.run < runnr_EFearly)  subjet_corrector_EFearly->process(event);
      else if(event.run <= runnr_FlateG)  subjet_corrector_FlateG->process(event);
      else if(event.run > runnr_FlateG)   subjet_corrector_H->process(event);
      else throw runtime_error("TstarPreSelectionModule: run number not covered");
    }

  topjetlepton_cleaner ->process(event);
  topjet_cleaner       ->process(event);

  
  sort_by_pt<Jet>(*event.jets);
  sort_by_pt<TopJet>(*event.topjets);

  cleaner_h         ->fill(event);
  cleaner_h_event   ->fill(event);
  cleaner_h_bJet    ->fill(event);
  cleaner_h_jet     ->fill(event);
  cleaner_h_ele     ->fill(event);
  cleaner_h_muo     ->fill(event);
  cleaner_h_topjet  ->fill(event);
  cleaner_h_lumi    ->fill(event);
//################################################################################### 

//// Lepton Selection
  const bool pass_lep1 = lep1_sel->passes(event);
     if(!pass_lep1) return false;

       lep1_h        ->fill(event);
       lep1_h_event  ->fill(event);
       lep1_h_bJet   ->fill(event);
       lep1_h_jet    ->fill(event);
       lep1_h_ele    ->fill(event);
       lep1_h_muo    ->fill(event);
       lep1_h_topjet ->fill(event);
       lep1_h_lumi   ->fill(event);

      
/// 2D-Cut for Muons
 const bool pass_twodcut = twodcut_sel->passes(event);
   if(!pass_twodcut) return false;
 
     twodcut_h        ->fill(event);
     twodcut_h_event  ->fill(event);
     twodcut_h_bJet   ->fill(event);
     twodcut_h_jet    ->fill(event);
     twodcut_h_ele    ->fill(event);
     twodcut_h_muo    ->fill(event);
     twodcut_h_topjet ->fill(event);
     twodcut_h_lumi   ->fill(event);
      
     //Need second Jet-Cleaner after 2D-Cut
 jet_cleaner2    ->process(event);


 //MET-Cut for further QCD Reduction
  if(!met_sel->passes(event)) return false;  
     met_h        ->fill(event);
     met_h_event  ->fill(event);
     met_h_bJet   ->fill(event);
     met_h_jet    ->fill(event);
     met_h_ele    ->fill(event);
     met_h_muo    ->fill(event);
     met_h_topjet ->fill(event);
     met_h_lumi   ->fill(event);


//################## B-Tag with Efficiency and SF #########################
 //BTagEffHists->fill(event);            //Run First Time without b-Tag and ScaleFactors (SF_btag)
  
  /// b-Tag 
 const bool pass_bTag = b_tag_medium_sel->passes(event);
      if(!pass_bTag) return false;
  
       SF_btag       ->process(event);     //Scale Factor for b-Tagging                        

        bTag_h        ->fill(event);
	bTag_h_event  ->fill(event);
	bTag_h_bJet   ->fill(event);
	bTag_h_jet    ->fill(event);
	bTag_h_ele    ->fill(event);
	bTag_h_muo    ->fill(event);
	bTag_h_topjet ->fill(event);
        bTag_h_lumi   ->fill(event);
 ////#######################################################################
	 
 
//// ##########  Top-Tagging with appropriate Jet-Selection #################### 
//Top-Tag
  const bool pass_ttagevt = toptagevt_sel->passes(event);

    if(pass_ttagevt){  
     toptagevt_h        ->fill(event);
     toptagevt_h_event  ->fill(event);
     toptagevt_h_bJet   ->fill(event);
     toptagevt_h_jet    ->fill(event);
     toptagevt_h_ele    ->fill(event);
     toptagevt_h_muo    ->fill(event);
     toptagevt_h_topjet ->fill(event);
     toptagevt_h_lumi   ->fill(event);

//JET selection
//  AK4 jet selection for TopTag Catogery  (#Jets >= 4) 
  const bool pass_jet_top = jet_top_sel->passes(event);
     if(!pass_jet_top) return false;

       jet_top_h        ->fill(event);
       jet_top_h_event  ->fill(event);
       jet_top_h_bJet   ->fill(event);
       jet_top_h_jet    ->fill(event);
       jet_top_h_ele    ->fill(event);
       jet_top_h_muo    ->fill(event);
       jet_top_h_topjet ->fill(event);
       jet_top_h_lumi   ->fill(event);
  }
   if(!pass_ttagevt){ 
     notoptagevt_h        ->fill(event);
     notoptagevt_h_event  ->fill(event);
     notoptagevt_h_bJet   ->fill(event);
     notoptagevt_h_jet    ->fill(event);
     notoptagevt_h_ele    ->fill(event);
     notoptagevt_h_muo    ->fill(event);
     notoptagevt_h_topjet ->fill(event);
     notoptagevt_h_lumi   ->fill(event);

 //Jet Selection without Top-Tag (#Jets >=5)
 const bool pass_jet2 = jet2_sel->passes(event);
   if(!pass_jet2) return false;
    jet2_h        ->fill(event);
    jet2_h_event  ->fill(event);
    jet2_h_bJet   ->fill(event);
    jet2_h_jet    ->fill(event);
    jet2_h_ele    ->fill(event);
    jet2_h_muo    ->fill(event);
    jet2_h_topjet ->fill(event);
    jet2_h_lumi   ->fill(event);
  }
 // add flag_toptagevent to output ntuple 
    event.set(h_flag_toptagevent, int(pass_ttagevt));
//##############################################################################
 
///HT Cut
 const bool pass_HTcut = ht_sel->passes(event);
   if(!pass_HTcut) return false;
 
     ht_h        ->fill(event);
     ht_h_event  ->fill(event);
     ht_h_bJet   ->fill(event);
     ht_h_jet    ->fill(event);
     ht_h_ele    ->fill(event);
     ht_h_muo    ->fill(event);
     ht_h_topjet ->fill(event);
     ht_h_lumi   ->fill(event);

//// ############################################  SYSTEM RECO  ######################################################################
 
//***********************************************************************************************************
//
//  Reconstruction of T*T* System for different Catogeries  (With/Without Top-Tag in Event) 
//  Fisrt Reconstruct Primary Lepton
//  Reconstruct the FourVector of Neutrino
//  
//  Reconstruct full Event with Chi2-Method  
//
//  Additional: Corect Matching between Reco and Gen Particles
//
//  ToDo: Change the Cut-Selection Scheme into boolian pass_chi2 return false; 
//
//***********************************************************************************************************

 ////PrimeLep Reco
  reco_primlep->process(event);

//For MC Only! Needs Generator Information
if(!event.isRealData){
  ttgenprod->process(event);
  tstargenprod->process(event);
 
  h_tstargenhists->fill(event);
 }

//######################  Reconstruction with/without TopTag & Chi2-Method  #######################################################

if(!pass_ttagevt){                                                              //Without Top-Tag
tstar_reco->process(event); tstar_chi2->process(event); }
  else { 
    const bool pass_reco = tstar_ttag_reco->process(event);                     //With Top-Tag
    if(!pass_reco) {       tstar_reco->process(event); tstar_chi2->process(event);}
    else{     tstar_ttag_chi2->process(event); }
  }
  std::vector<TstarReconstructionHypothesis>& hyps = event.get(h_hyps);

//##################################  Chi2 Hypothesis #################################################################

 const TstarReconstructionHypothesis* hyp = get_best_hypothesis(hyps, "Chi2");        //Get best Hypothesis

  if(!hyp) std::runtime_error("TstarSelectionModule::process -- best hypothesis for T*-Reconstruction not found");
  if(!hyp->discriminator("Chi2")) std::cout<<"Nullpointer!"<<std::endl;

  //Fill Reconstruction Hists BEFORE Discriminator Cut
  chi2_before_h ->fill(event);

  //Cut on Discriminator
  if(hyp->discriminator("Chi2")<50){

    if(!pass_ttagevt){ chi2min_h->fill(event); }
    else             { chi2min_ttag_h->fill(event); }
    chi2min_ttag_comb_h->fill(event);
 
    //Dump Events in Event Hists
    chi2_h_event  ->fill(event);
    chi2_h_bJet   ->fill(event);
    chi2_h_jet    ->fill(event);
    chi2_h_ele    ->fill(event);
    chi2_h_muo    ->fill(event);
    chi2_h_topjet ->fill(event);
    chi2_h_lumi   ->fill(event);
  }

// ##########################################################################################################################
 
//#######################################  End of SYSTEM RECO  ###########################################################################
 
 // Histogramms for PDF-Variations 
    // if(hyp->discriminator("Chi2")<50) h_PDF_variations->fill(event);

  //Clear Hypothesis Vector
  const TstarReconstructionHypothesis hyp_obj(*hyp);
  hyps.clear();
  hyps.push_back(hyp_obj);


  // dump output content
  output_h        ->fill(event);
  output_h_event  ->fill(event);
  output_h_bJet   ->fill(event);
  output_h_jet    ->fill(event);
  output_h_ele    ->fill(event);
  output_h_muo    ->fill(event);
  output_h_topjet ->fill(event);
  output_h_lumi   ->fill(event);

  return true;
}

UHH2_REGISTER_ANALYSIS_MODULE(TstarSelectionModule)

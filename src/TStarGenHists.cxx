#include "UHH2/TstarSemiLeptonic/include/TStarGenHists.h"
#include "TH1F.h"
#include "TH2F.h"

using namespace uhh2;

TStarGenHists::TStarGenHists(uhh2::Context & ctx, const std::string & dirname): Hists(ctx, dirname){

    M_TStar_gen =  book<TH1F>( "M_TStarTStar_gen", "M_{T^{*}#bar{T}^{*}} [GeV/c^{2}]", 1000, 0, 5000 ) ;
   
    Pt_TStarTStar_gen = book< TH1F>( "Pt_TStarTStar_gen", "P_{T,T^{*}#bar{T^{*}}} [GeV/c]", 10, -1, 1 ) ;
  
        
    M_tstar = book< TH1F>("M_TStar", "M_{T^{*}} [GeV/c^{2}]", 1000, 150, 2000) ;
    M_antitstar = book< TH1F>("M_AntiTStar", "M_{#bar{T}^{*}} [GeV/c^{2}]", 1000, 150, 2000) ;  

    Pt_tstar = book< TH1F>( "Pt_TStar", "P_{T,T^{*}} [GeV/c]", 1000, 0, 2000 ) ;
    Pt_antitstar = book< TH1F>( "Pt_AntiTStar", "P_{T,#bar{T}^{*}} [GeV/c]", 1000, 0, 2000 ) ;
    

    eta_TStar = book< TH1F>( "eta_TStar", "#eta_{T^{*}}", 1000, -5, 5 ) ;
    eta_AntiTStar = book< TH1F>( "eta_AntiTStar", "#eta_{#bar{T}^{*}}", 1000, -5, 5 ) ;
    
    y_tstar = book< TH1F>( "y_TStar", "y_{T^{*}}", 1000, -5, 5 ) ;
    y_antitstar = book< TH1F>( "y_AntiTStar", "y_{#bar{T}^{*}}", 1000, -5, 5 ) ;
    
    diffabseta = book<TH1F>( "diffabseta", "|#eta_{T^{*}}|-|#eta_{#bar{T}^{*}}|",1000,-5,5);
    diffabsy = book<TH1F>( "diffabsy", "|y_{T^{*}}|-|y_{#bar{T}^{*}}|",1000,-5,5);
       
    deltaR_tstar_decays = book<TH1F>( "deltaR_tstar_decays", "#DeltaR(T^{*} decay prod.)",1000,0,5);
    deltaR_antitstar_decays = book<TH1F>( "deltaR_antitstar_decays", "#DeltaR(#bar{T}^{*} decay prod.)",1000,0,5);  
    
    
     M_TStar_reco = book< TH1F>("M_TStar_reco", "M_{top g} [GeV/c^{2}]", 1000, 150, 2000) ;
     M_AntiTStar_reco = book< TH1F>("M_AntiTStar_reco", "M_{#bar{t} g} [GeV/c^{2}]", 1000, 150, 2000) ;

    deltaR_top_decays = book<TH1F>( "deltaR_top_decays", "#DeltaR(t decay prod.)",1000,0,5);
    deltaR_antitop_decays = book<TH1F>( "deltaR_antitop_decays", "#DeltaR(#bar{t} decay prod.)",1000,0,5);  
    
    
    Pt_top = book<TH1F>( "Pt_top","P_{T,t} [GeV/c]",1000,0,1000);
    Pt_antitop = book<TH1F>( "Pt_antitop","P_{T,#bar{t}} [GeV/c]",1000,0,1000);
    eta_top = book<TH1F>( "eta_top","#eta_{t}",1000,-5,5);
    eta_antitop = book<TH1F>( "eta_antitop","#eta_{#bar{t}}",1000,-5,5);

    Pt_gluon = book<TH1F>( "Pt_gluon_TStar","P_{T,g} [GeV/c]",1000,0,1000);
    Pt_antigluon = book<TH1F>( "Pt_gluon_AntiTStar","P_{T,g} [GeV/c]",1000,0,1000);
    eta_gluon = book<TH1F>( "eta_gluon_TStar","#eta_{g}",1000,-5,5);
    eta_antigluon = book<TH1F>( "eta_gluon_AntiTStar","#eta_{g}",1000,-5,5);

     M_tTStar_reco = book< TH1F>("M_tTStar_reco", "M_{W b} [GeV/c^{2}]", 1000, 150, 2000) ;
     M_tAntiTStar_reco = book< TH1F>("M_tAntiTStar", "M_{W #bar{b}} [GeV/c^{2}]", 1000, 150, 2000) ;

    
    h_tstargen = ctx.get_handle<TStarGen>("tstargen");
}


void TStarGenHists::fill(const uhh2::Event & e){
    //do not fill histograms if ttbargen information has not been filled
      if(!e.is_valid(h_tstargen)){
      return;
    }
    const auto & tstargen = e.get(h_tstargen);
    
    LorentzVector tstar = tstargen.TStar().v4();
    LorentzVector antitstar = tstargen.AntiTStar().v4();

 
    double mtstar_gen = (tstar+antitstar).M();

    double  pttstar_gen = ( tstar + antitstar ).Pt();

    M_TStar_gen->Fill( mtstar_gen,e.weight);
    Pt_TStarTStar_gen->Fill ( pttstar_gen, e.weight);

  
    M_tstar->Fill( tstargen.TStar().v4().M(), e.weight);
    M_antitstar->Fill(tstargen.AntiTStar().v4().M(), e.weight);
    Pt_tstar->Fill( tstar.Pt(), e.weight);
    Pt_antitstar->Fill(antitstar.Pt(), e.weight);


    eta_TStar->Fill( tstargen.TStar().eta(), e.weight);
    eta_AntiTStar->Fill(tstargen.AntiTStar().eta(), e.weight);
    
    y_tstar->Fill(tstargen.TStar().v4().Rapidity(), e.weight);
    y_antitstar->Fill(tstargen.AntiTStar().v4().Rapidity(), e.weight);

    double difabseta = fabs( tstargen.TStar().eta()) - fabs( tstargen.AntiTStar().eta());
    double difabsy = fabs( tstargen.TStar().v4().Rapidity()) - fabs( tstargen.AntiTStar().v4().Rapidity());

    diffabseta->Fill(difabseta, e.weight);
    diffabsy->Fill(difabsy, e.weight);
       
    double deltaR_tstar =  uhh2::deltaR(tstargen.gTStar(), tstargen.tTStar());
				 

    double deltaR_antitstar =  uhh2::deltaR(tstargen.gAntiTStar(), tstargen.tAntiTStar()); 
				      

    deltaR_tstar_decays->Fill(deltaR_tstar,e.weight);
    deltaR_antitstar_decays->Fill(deltaR_antitstar,e.weight);
   
    double deltaR_top = std::max(std::max( uhh2::deltaR(tstargen.wPlusDecay1(), tstargen.wPlusDecay2()),
					   uhh2::deltaR(tstargen.wPlusDecay1(), tstargen.bTStar())), 
				 uhh2::deltaR(tstargen.wPlusDecay2(), tstargen.bTStar() ));  
    
   double deltaR_antitop = std::max(std::max( uhh2::deltaR(tstargen.wMinusDecay1(), tstargen.wMinusDecay2()),
					   uhh2::deltaR(tstargen.wMinusDecay1(), tstargen.bAntiTStar())), 
				 uhh2::deltaR(tstargen.wMinusDecay2(), tstargen.bAntiTStar() ));  
    deltaR_top_decays->Fill(deltaR_top,e.weight);
    deltaR_antitop_decays->Fill(deltaR_antitop,e.weight);

    LorentzVector top = tstargen.tTStar().v4();
    LorentzVector gluon = tstargen.gTStar().v4();
 
    double mtstar_reco = (top+gluon).M();

    M_TStar_reco->Fill( mtstar_reco,e.weight);

    LorentzVector antitop = tstargen.tAntiTStar().v4();
    LorentzVector antigluon = tstargen.gAntiTStar().v4();
 
    double mantitstar_reco = (antitop+antigluon).M();
    M_AntiTStar_reco->Fill( mantitstar_reco,e.weight);

   
    Pt_top->Fill( tstargen.tTStar().pt(), e.weight);
    Pt_antitop->Fill( tstargen.tAntiTStar().pt(), e.weight);  
    eta_top->Fill( tstargen.tTStar().eta(), e.weight);
    eta_antitop->Fill( tstargen.tAntiTStar().eta(), e.weight); 
    
    Pt_gluon->Fill( tstargen.gTStar().pt(), e.weight);
    Pt_antigluon->Fill( tstargen.gAntiTStar().pt(), e.weight);  
    eta_gluon->Fill( tstargen.gTStar().eta(), e.weight);
    eta_antigluon->Fill( tstargen.gAntiTStar().eta(), e.weight); 

    LorentzVector wPlusDecay1 = tstargen.wPlusDecay1().v4();
    LorentzVector wPlusDecay2 = tstargen.wPlusDecay2().v4();
    LorentzVector bTStar = tstargen.bTStar().v4();
 
    double mttstar_reco = (wPlusDecay1+wPlusDecay2+bTStar).M();

    M_tTStar_reco->Fill( mttstar_reco,e.weight);

    LorentzVector wMinusDecay1 = tstargen.wMinusDecay1().v4();
    LorentzVector wMinusDecay2 = tstargen.wMinusDecay2().v4();
    LorentzVector bAntiTStar = tstargen.bAntiTStar().v4();
 
    double mtantitstar_reco = (wMinusDecay1+wMinusDecay2+bAntiTStar).M();

    M_tAntiTStar_reco->Fill( mtantitstar_reco,e.weight);

}
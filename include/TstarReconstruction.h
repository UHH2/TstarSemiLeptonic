#pragma once

#include "UHH2/TstarSemiLeptonic/include/TstarReconstructionHypothesisDiscriminators.h"
#include "UHH2/TstarSemiLeptonic/include/TStarGen.h"
#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/TstarSemiLeptonic/include/TstarReconstructionHypothesis.h"
#include "UHH2/TstarSemiLeptonic/include/TopReconstructionHypothesis.h"
#include "TMinuit.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/ObjectIdUtils.h"
#include "UHH2/common/include/TopJetIds.h"
#include "UHH2/common/include/PrimaryLepton.h"
#include "UHH2/TstarSemiLeptonic/include/NeutrinoFit.h"

typedef std::function< std::vector<LorentzVector>  (const LorentzVector & lepton, const LorentzVector & met)> NeutrinoReconstructionMethod;


/** \brief Make a list of ttbar reconstruction hypotheses as used in high-mass semileptonic ttbar 13 TeV analysis
 *
 * Make a list of ttbar reconstruction hypothesis using all (~ 3^Njet) combinations
 * of assigning jets to either the leptonic top, the hadronic top, or none of them;
 * hypotheses not assigning any jet to either the hadronic or leptonic top
 * are discarded.
 * For the leptonic side, the primary lepton and the neutrino reconstruction
 * according to the neutrinofunction parameter is done, which typically doubles
 * the number of hypotheses.
 *
 * Make sure to run an appropriate cleaner to keep only jets which should be used
 * in the hypothesis. Only works for events with Njets >= 2.
 *
 * neutrinofunction can be e.g. NeutrinoReconstruction or NeutrinoFitPolar
 *
 * label = name of the hypotheses list in the event / output tree
 */

class HighMassTopReconstruction: public uhh2::AnalysisModule {
public:
    explicit HighMassTopReconstruction(uhh2::Context & ctx, const NeutrinoReconstructionMethod & neutrinofunction, const std::string & label="HighMassTopReconstruction");

    virtual bool process(uhh2::Event & event) override;
    virtual ~HighMassTopReconstruction();

private:
    NeutrinoReconstructionMethod m_neutrinofunction;
    uhh2::Event::Handle<std::vector<TstarReconstructionHypothesis>> h_recohyps;
    uhh2::Event::Handle<FlavorParticle> h_primlep;
    uhh2::Event::Handle<std::vector<TstarReconstructionHypothesis>> hypothesis;
    std::vector<TstarReconstructionHypothesis> reconstruct_TopHyps(const std::vector<Jet> & jets, const std::vector<LorentzVector> & Wleps, double cutoff_Top_min=50, double cutoff_Top_max=400);
    uhh2::Event::Handle<std::vector<TstarReconstructionHypothesis>> h_hyps;
    uhh2::Event::Handle<TStarGen> h_tstargen;
    NeutrinoFit FitNeutrino;
};

class HighMassGluonReconstruction: public uhh2::AnalysisModule {
public:
  explicit HighMassGluonReconstruction(uhh2::Context & ctx, const std::string & label="HighMassGluonReconstruction");

    virtual bool process(uhh2::Event & event) override;

private:
    uhh2::Event::Handle<std::vector<TstarReconstructionHypothesis>> h_recohyps;
    uhh2::Event::Handle<std::vector<TstarReconstructionHypothesis>> hypothesis;
    uhh2::Event::Handle<std::vector<TstarReconstructionHypothesis>> h_hyps;
    uhh2::Event::Handle<std::vector<TstarReconstructionHypothesis>> h_topHyps;
    uhh2::Event::Handle<TStarGen> h_tstargen;
};


/** \brief Make a list of ttbar reconstruction hypotheses using top tagging
 *
 * Take the C/A-Jet with the TopTag and use it as hadronic Top.
 * For the leptonic side the primary lepton is taken. A list is generated
 * with all possible combinations of single ak5 jets.
 * Make sure a reasonable selection is run before to get hypotheses.
 * There is hardcoded a minimum
 * distance of lepton and TopTag of 0.8 and between ak5 jet and TopTag of 1.3 (measure in delta R)
 *
 * neutrinofunction can be either NeutrinoReconstruction or NeutrinoFitPolar
 *
 * label = name of the hypotheses list in the event / output tree
 *
 * TODO: implement, document better.
 */

class HighMassTopReconstructionTTag: public uhh2::AnalysisModule {
 public:
  explicit HighMassTopReconstructionTTag(uhh2::Context&, const NeutrinoReconstructionMethod&, const std::string& label="HighMassTopReconstructionTTag", TopJetId id=CMSTopTag(), float dr=1.2);
  virtual bool process(uhh2::Event&) override;

 private:
  NeutrinoReconstructionMethod m_neutrinofunction;
  uhh2::Event::Handle<std::vector<TstarReconstructionHypothesis>> h_recohyps;
  uhh2::Event::Handle<FlavorParticle> h_primlep;
  
  NeutrinoFit FitNeutrino;
  TopJetId topjetID_;
  float minDR_topjet_jet_;
};



/** \brief Make a list of ttbar reconstruction hypotheses as used in high-mass semileptonic ttbar 13 TeV analysis
 *
 * Make a list of ttbar reconstruction hypothesis using all (~ 3^Njet) combinations
 * of assigning jets to either the leptonic top, the hadronic top, or none of them;
 * hypotheses not assigning any jet to either the hadronic or leptonic top
 * are discarded.
 * For the leptonic side, the primary lepton and the neutrino reconstruction
 * according to the neutrinofunction parameter is done, which typically doubles
 * the number of hypotheses.
 *
 * Make sure to run an appropriate cleaner to keep only jets which should be used
 * in the hypothesis. Only works for events with Njets >= 2.
 *
 * neutrinofunction can be e.g. NeutrinoReconstruction or NeutrinoFitPolar
 *
 * label = name of the hypotheses list in the event / output tree
 */

class HighMassTstarKinReconstruction: public uhh2::AnalysisModule {
public:
    explicit HighMassTstarKinReconstruction(uhh2::Context & ctx, const NeutrinoReconstructionMethod & neutrinofunction, const std::string & label="HighMassTstarReconstruction");

    virtual bool process(uhh2::Event & event) override;
    virtual ~HighMassTstarKinReconstruction();

private:
    NeutrinoReconstructionMethod m_neutrinofunction;
    uhh2::Event::Handle<std::vector<TstarReconstructionHypothesis>> h_recohyps;
    uhh2::Event::Handle<FlavorParticle> h_primlep;
    uhh2::Event::Handle<std::vector<TstarReconstructionHypothesis>> hypothesis;
    std::vector<TstarReconstructionHypothesis> reconstruct_TopHyps(const std::vector<Jet> & jets, const std::vector<LorentzVector> & Wleps, double cutoff_Top_min=50, double cutoff_Top_max=400);
    uhh2::Event::Handle<std::vector<TstarReconstructionHypothesis>> h_hyps;

    NeutrinoFit FitNeutrino;
};


class TstarTopTagKinReconstruction: public uhh2::AnalysisModule {
 public:
  explicit TstarTopTagKinReconstruction(uhh2::Context&, const NeutrinoReconstructionMethod&, const std::string& label="TstarTopTagReconstruction", TopJetId id=CMSTopTag(), float dr=1.2);
  virtual bool process(uhh2::Event&) override;

 private:
  NeutrinoReconstructionMethod m_neutrinofunction;
  uhh2::Event::Handle<std::vector<TstarReconstructionHypothesis>> h_recohyps;
  uhh2::Event::Handle<FlavorParticle> h_primlep;
  
  NeutrinoFit FitNeutrino;
  TopJetId topjetID_;
  float minDR_topjet_jet_;
};


/** \brief Calculate the neutrino four-momentum from MET and charged lepton momenta
 *
 * Given the Decay:
 *
 * W -> lepton + Neutrino
 *
 * reconstruct the complete possible Neutrino four-vector(s), using the fact that the lepton
 * is well-measured and assuming that the MET corresponds to the x and y components
 * of the Neutrino momentum. With these assumptions, the neutrino pZ is given by a quadratic equation
 * for pZ which follows from the W-mass constraint
 *
 * W_p4^2 = (lepton_p4 + Neutrino_p4)^2  = m_W^2
 *
 * The code assumes that the lepton mass and the neutrino mass are 0 and uses
 * m_W = 80.399 GeV.
 *
 * The returned solutions are the neutrino four-vectors where pZ was calculated from the W-mass contraint.
 * As pZ is a solution of a quadratic equation, there can be either 0, 1 or 2 real solutions.
 *  - In case there is no real solution, the real part of the complex solution is used instead; the returned
 *    vector has size=1 in this case. If this happens, the W mass constrained is not fulfilled exactly, but only approximately.
 *    This happens if the transverse W mass exceeds the W mass.
 *  - In all other cases, 2 solutions are returned and the W-mass constraint is fulfilled exactly (note that
 *    2 solutions are returned even if there is only 1, as the distiction between 1 or 2 real solutions is not
 *    possible in a numerically stable way).
 */
std::vector<LorentzVector> TstarNeutrinoReconstruction(const LorentzVector & lepton, const LorentzVector & met);


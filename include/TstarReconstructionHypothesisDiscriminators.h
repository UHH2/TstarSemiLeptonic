#pragma once

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"

#include "UHH2/TstarSemiLeptonic/include/TstarReconstructionHypothesis.h"
#include "UHH2/TstarSemiLeptonic/include/TopReconstructionHypothesis.h"
#include "UHH2/TstarSemiLeptonic/include/TStarGen.h"
#include "UHH2/common/include/TTbarGen.h"
/**  @short This file defines analysis modules which add discriminator values to reconstruction hypotheses
 *
 * As input, all modules need a vector of TstarReconstructionHypothesis which must be in the event
 * when the module is called. The name of this vector can be specified at time of construction.
 * Further parameter can be defined by passing a 'cfg' object which typically controls the name of the
 * discriminator in th ReconstructionHypothesis or the name of additional event input.
 * 
 * The convention is that smaller values of the discriminator flag are better (as e.g. naturally the case
 * for chi-square). The exact meaning of the discriminators depends on the method, though.
 */


/** \brief Get the best hypothesis, i.e. the one with the smallest discriminator value
 * 
 * If no hypothesis exists with that name or if the smallest discriminator is infinite, returns
 * nullptr.
 * 
 * label is the discriminator label, e.g. "Chi2".
 */
const TstarReconstructionHypothesis * get_best_hypothesis(const std::vector<TstarReconstructionHypothesis> & hyps, const std::string & label);

const TstarReconstructionHypothesis * get_best_top_hypothesis(const std::vector<TstarReconstructionHypothesis> & hyps, const std::string & label);

/** \brief Calculate the chi-square reconstruction discriminator
 * 
 * The Chi-square value is calculated from leptonic and hadronic reconstructed top-quark masses. This
 * is the default reconstruction-level method used in the 8TeV semi-leptonic high-mass CMS analyses.
 * 
 * Per default, fills discriminators "Chi2", "Chi2_tlep" and "Chi2_thad" which are the over chi-square,
 * the chi-square only for the leptonic lep and the chi-square only for the hadronic leg, resp.
 * The name / prefix "Chi2" can be overridden via cfg::discriminator_label.
 * 
 * For numeric values of the means and widths for the masses used see the implementation in the .cxx file;
 * they are the 8TeV values.
 */
class TopChi2Discriminator: public uhh2::AnalysisModule {
public:
    struct cfg {
        std::string discriminator_label;
        cfg(): discriminator_label("Chi2"){}
    };
    
    TopChi2Discriminator(uhh2::Context & ctx, const std::string & rechyps_name, const cfg & config = cfg());
    virtual bool process(uhh2::Event & event) override;
    
private:
    uhh2::Event::Handle<std::vector<TstarReconstructionHypothesis>> h_hyps;
    cfg config;
};

/** \brief Calculate the chi-square reconstruction discriminator
 *         (only for events with a top-tagged jet; see TopTagReconstruction class)
 * 
 * Chi-square discriminator specific to events where the hadronic-top corresponds to one jet passing top-tagging.
 * Class implementation follows the same structure of the Chi2Discriminator class.
 * The chi2 term for the hadronic-top is calculated using the groomed mass of the top-tagged jet.
 */

class TopChi2DiscriminatorTTAG: public uhh2::AnalysisModule {
public:
    struct cfg {
        std::string discriminator_label;
        cfg(): discriminator_label("Chi2"){}
    };
    
    TopChi2DiscriminatorTTAG(uhh2::Context & ctx, const std::string & rechyps_name, const cfg & config = cfg());
    virtual bool process(uhh2::Event & event) override;
    
private:
    uhh2::Event::Handle<std::vector<TstarReconstructionHypothesis>> h_hyps;
    cfg config;
};
//******************************************************************************************************************




class TstarChi2Discriminator: public uhh2::AnalysisModule {
public:
    struct cfg {
        std::string discriminator_label;
        cfg(): discriminator_label("Chi2"){}
    };
    
    TstarChi2Discriminator(uhh2::Context & ctx, const std::string & rechyps_name, const cfg & config = cfg());
    virtual bool process(uhh2::Event & event) override;
    
private:
    uhh2::Event::Handle<std::vector<TstarReconstructionHypothesis>> h_hyps;
    cfg config;
};

/** \brief Calculate the chi-square reconstruction discriminator
 *         (only for events with a top-tagged jet; see TopTagReconstruction class)
 * 
 * Chi-square discriminator specific to events where the hadronic-top corresponds to one jet passing top-tagging.
 * Class implementation follows the same structure of the Chi2Discriminator class.
 * The chi2 term for the hadronic-top is calculated using the groomed mass of the top-tagged jet.
 */

class TstarChi2DiscriminatorTTAG: public uhh2::AnalysisModule {
public:
    struct cfg {
        std::string discriminator_label;
        cfg(): discriminator_label("Chi2"){}
    };
    
    TstarChi2DiscriminatorTTAG(uhh2::Context & ctx, const std::string & rechyps_name, const cfg & config = cfg());
    virtual bool process(uhh2::Event & event) override;
    
private:
    uhh2::Event::Handle<std::vector<TstarReconstructionHypothesis>> h_hyps;
    cfg config;
};

/** \brief Top-DeltaR quality flag for Monte-Carlo
 * 
 * Requires a TTbarGen object in the event (see TTbarGen.h).
 * 
 * Writes "TopDRMC" quality flags to the reconstruction hypotheses, which is the sum of Delta R values
 * between the generated and reconstructed top quarks for the top and anti-top.
 * 
 * NOTE: This used to be called "BestPossible" in UHHAnalysis.
 */
class TstarTopDRMCDiscriminator: public uhh2::AnalysisModule {
public:
    struct cfg {
        std::string ttbargen_name;
        std::string discriminator_label;
        cfg(): ttbargen_name("ttbargen"), discriminator_label("TopDRMC"){}
    };
    
    TstarTopDRMCDiscriminator(uhh2::Context & ctx, const std::string & rechyps_name, const cfg & config = cfg());
    virtual bool process(uhh2::Event & event) override;
    
private:
    
    uhh2::Event::Handle<std::vector<TstarReconstructionHypothesis>> h_hyps;
    uhh2::Event::Handle<TTbarGen> h_ttbargen;
    cfg config;
};


/** \brief Try to match the reconstruction hypotheses to Monte-Carlo truth, jet-by-jet
 * 
 * Requires a TTbarGen object in the event (see TTbarGen.h).
 * 
 * Writes a "CorrectMatch" quality flags to the reconstruction hypotheses, which is the sum of Delta R values
 * between the four generated and reconstructed matrix-element final-state partons and the DeltaR between the
 * true neutrino and the reconstructed neutrino. The discriminator is set to infinity
 * if one of the final-state partons could not be matched to a jet within Delta R < 0.3 (note that no
 * such match is done for the neutrino).
 * 
 * NOTE: This class only works for events which are (on gen-level) electron+jets or muon+jets. Otherwise, all discriminator
 * values are set to +infinity. The reconstructed lepton is ignored in this discriminator criterion.
 */
class TstarCorrectMatchDiscriminator: public uhh2::AnalysisModule {
public:
    struct cfg {
        std::string tstargen_name;
	std::string ttbargen_name;
        std::string discriminator_label;
        cfg(): tstargen_name("tstargen"), ttbargen_name("ttbargen"), discriminator_label("CorrectMatch"){}
    };
    
    TstarCorrectMatchDiscriminator(uhh2::Context & ctx, const std::string & rechyps_name, const cfg & config = cfg());
    virtual bool process(uhh2::Event & event) override;
    
private:
    
    uhh2::Event::Handle<std::vector<TstarReconstructionHypothesis>> h_hyps;
    uhh2::Event::Handle<TTbarGen> h_ttbargen;
    uhh2::Event::Handle<TStarGen> h_tstargen;
    cfg config;
};


class TstarCorrectMatchDiscriminatorTTAG: public uhh2::AnalysisModule {
public:
    struct cfg {
        std::string tstargen_name;
	std::string ttbargen_name;
        std::string discriminator_label;
        cfg(): tstargen_name("tstargen"), ttbargen_name("ttbargen"), discriminator_label("CorrectMatch"){}
    };
    
    TstarCorrectMatchDiscriminatorTTAG(uhh2::Context & ctx, const std::string & rechyps_name, const cfg & config = cfg());
    virtual bool process(uhh2::Event & event) override;
    
private:
    
    uhh2::Event::Handle<std::vector<TstarReconstructionHypothesis>> h_hyps;
    uhh2::Event::Handle<TTbarGen> h_ttbargen;
    uhh2::Event::Handle<TStarGen> h_tstargen;
    cfg config;
};



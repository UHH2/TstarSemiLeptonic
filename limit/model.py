# -*- coding: utf-8 -*-
def model(fname="/nfs/dust/cms/user/multh/RunII_76X_v1/Limit/Tstar_comb.root", write_report = True):
    model = build_model_from_rootfile(fname)
#    model.scale_predictions(5.0 / 1.1)
    model.fill_histogram_zerobins()
    model.set_signal_processes('Tstar*')
    for p in model.processes:
        model.add_lognormal_uncertainty('lumi', math.log(1.027), p)
     
    model.add_lognormal_uncertainty('zj_rate', math.log(1.5), 'ZJets')
    model.add_lognormal_uncertainty('wj_rate', math.log(1.5), 'WJets')
    model.add_lognormal_uncertainty('qcd_rate', math.log(2.0), 'QCD')
    model.add_lognormal_uncertainty('ttbar_rate', math.log(1.5), 'TTbar')
    model.add_lognormal_uncertainty('singletop_rate', math.log(1.5), 'SingleTop')
    model.add_lognormal_uncertainty('diboson_rate', math.log(1.5), 'Diboson')
    model_summary(model)
    
    #dist = model.distribution
    #model.distribution = get_fixed_dist(dist)
    #res = pl_intervals(model, input = 'toys-asimov:0.0', n = 1, write_report = False, nuisance_constraint = dist)
    
    exp, obs = asymptotic_cls_limits(model, use_data=False, signal_process_groups=None, beta_signal_expected=0.0, bootstrap_model=True, input=None, n=1, options=None)
    print "expected limit"
    """    
    for spid in res:
        print spid, res[spid][0.90][0][1]
        
    """
    for x, y in zip(exp.x, exp.y):
        print x, y
       
   # print "end"
    if write_report: report.write_html('./htmlout')

    plt.clf()

    x = np.linspace(0, 2, 100)
    theroy13TeV_x =[700,800,900,1000,1100,1200,1300,1400,1500,1600]
    theory13TeV_y =[4.92281, 1.6752, 0.6357, 0.2623, 0.1156, 0.05372, 0.026058, 0.01309, 0.00677, 0.003588]

    plt.semilogy()
    plt.plot(exp.x, exp.y, label='T*->tg MC',color = 'black')
    plt.fill_between(exp.x, exp.bands[0][0] ,  exp.bands[0][1],
                     alpha=0.2, edgecolor=exp.bands[0][2], facecolor=exp.bands[0][2],
                     linewidth=2)

    plt.plot(theroy13TeV_x, theory13TeV_y, label='Theory cross section 13TeV')
    
    #plt.ylim([0.4,10])
    plt.xlabel('M_{T*} [GeV]')
    plt.ylabel('cross section times branching ratio [pb]')
    plt.xlim([700,1600])
   

    plt.title("T* -> tg 100%")
    
    plt.legend(loc=1,prop={'size':10})
    plt.savefig("Tstar_limit_theta_comb.pdf")

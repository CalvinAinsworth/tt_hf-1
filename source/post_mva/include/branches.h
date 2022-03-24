
  // ///
  // Nominal tree variables
  
  // Jet variables
  std::vector<Float_t> *jet_pt;
  std::vector<Float_t> *jet_DL1r;
  std::vector<Float_t> *jet_eta;
  std::vector<Float_t> *jet_phi;
  std::vector<Float_t> *jet_e;
  std::vector<Float_t> *jet_jvt;
  std::vector<Float_t> *jet_tagWeightBin_DL1r_Continuous;
  std::vector<Int_t> *jet_truthflav;
  std::vector<Int_t> *jet_truthPartonLabel;
  std::vector<Int_t> *jet_GBHInit_topHadronOriginFlag;
  std::vector<Int_t> *jet_nGhosts_bHadron;
  std::vector<Int_t> *jet_nGhosts_cHadron;
  std::vector<char> *jet_DL1r_77;
  std::vector<char> *jet_isTrueHS;

  jet_pt = jet_DL1r = jet_eta = jet_phi = jet_e = jet_jvt = jet_tagWeightBin_DL1r_Continuous = 0;
  jet_truthflav = jet_truthPartonLabel = jet_GBHInit_topHadronOriginFlag = jet_nGhosts_bHadron = jet_nGhosts_cHadron = 0;
  jet_DL1r_77 = jet_isTrueHS = 0;

  
  // Electrons
  std::vector<Float_t> *el_pt; 
  std::vector<Float_t> *el_eta;
  std::vector<Float_t> *el_cl_eta;
  std::vector<Float_t> *el_phi;
  std::vector<Float_t> *el_charge;
  std::vector<Float_t> *el_e;
  std::vector<Float_t> *el_topoetcone20;
  std::vector<Float_t> *el_ptvarcone20;
  std::vector<Float_t> *el_CF;
  std::vector<Float_t> *el_d0sig;
  std::vector<Float_t> *el_delta_z0_sintheta;
  std::vector<Int_t> *el_true_type;
  std::vector<Int_t> *el_true_origin;
  std::vector<Int_t> *el_true_firstEgMotherTruthType;
  std::vector<Int_t> *el_true_firstEgMotherTruthOrigin;
  std::vector<Int_t> *el_true_firstEgMotherPdgId;
  std::vector<char> *el_true_isPrompt;
  std::vector<char> *el_true_isChargeFl;
  std::vector<Float_t> *el_pt_bare;
  std::vector<Float_t> *el_eta_bare;
  std::vector<Float_t> *el_phi_bare;
  std::vector<Float_t> *el_e_bare;
  
  el_pt = el_eta = el_cl_eta = el_phi = el_charge = el_e = el_topoetcone20 = el_ptvarcone20 = el_CF = el_d0sig = el_delta_z0_sintheta = el_pt_bare = el_eta_bare = el_phi_bare = el_e_bare = 0;
  el_true_type = el_true_origin = el_true_firstEgMotherTruthType = el_true_firstEgMotherTruthOrigin = el_true_firstEgMotherPdgId = 0;
  el_true_isPrompt = el_true_isChargeFl = 0;
  
  
  // Muons
  std::vector<Float_t> *mu_pt;
  std::vector<Float_t> *mu_eta;
  std::vector<Float_t> *mu_phi;
  std::vector<Float_t> *mu_charge;
  std::vector<Float_t> *mu_e; 
  std::vector<Float_t> *mu_topoetcone20;
  std::vector<Float_t> *mu_ptvarcone30;
  std::vector<Float_t> *mu_d0sig;
  std::vector<Float_t> *mu_delta_z0_sintheta;
  std::vector<Int_t> *mu_true_type; 
  std::vector<Int_t> *mu_true_origin;
  std::vector<char> *mu_true_isPrompt;
  std::vector<Float_t> *mu_pt_bare;
  std::vector<Float_t> *mu_eta_bare;
  std::vector<Float_t> *mu_phi_bare;
  std::vector<Float_t> *mu_e_bare;
  
  mu_pt = mu_eta = mu_phi = mu_charge = mu_e = mu_topoetcone20 = mu_ptvarcone30 = mu_d0sig = mu_delta_z0_sintheta = mu_pt_bare = mu_eta_bare=  mu_phi_bare = mu_e_bare = 0;
  mu_true_type = mu_true_origin = 0;
  mu_true_isPrompt = 0;
  

  // Other
  Float_t met;
  Float_t met_phi;
  UInt_t runNumber;
  ULong64_t eventNumber;
  Int_t topHeavyFlavorFilterFlag = -999;


  // Weights
  Float_t w_mc;
  Float_t w_pu;
  Float_t w_leptonSF;
  Float_t w_DL1r_77;
  Float_t w_jvt;


  // Vertices
  std::vector<Int_t> *vertex_type;
  std::vector<Double_t> *vertex_z;

  vertex_type = 0;
  vertex_z = 0;


  // Systematics
  Float_t weight_pileup_UP;
  Float_t weight_pileup_DOWN;
  Float_t weight_jvt_UP;
  Float_t weight_jvt_DOWN;
  Float_t weight_leptonSF_EL_SF_Trigger_UP;
  Float_t weight_leptonSF_EL_SF_Trigger_DOWN;
  Float_t weight_leptonSF_EL_SF_Reco_UP;
  Float_t weight_leptonSF_EL_SF_Reco_DOWN;
  Float_t weight_leptonSF_EL_SF_ID_UP;
  Float_t weight_leptonSF_EL_SF_ID_DOWN;
  Float_t weight_leptonSF_EL_SF_Isol_UP;
  Float_t weight_leptonSF_EL_SF_Isol_DOWN;
  Float_t weight_leptonSF_MU_SF_Trigger_STAT_UP;
  Float_t weight_leptonSF_MU_SF_Trigger_STAT_DOWN;
  Float_t weight_leptonSF_MU_SF_Trigger_SYST_UP;
  Float_t weight_leptonSF_MU_SF_Trigger_SYST_DOWN;
  Float_t weight_leptonSF_MU_SF_ID_STAT_UP;
  Float_t weight_leptonSF_MU_SF_ID_STAT_DOWN;
  Float_t weight_leptonSF_MU_SF_ID_SYST_UP;
  Float_t weight_leptonSF_MU_SF_ID_SYST_DOWN;
  Float_t weight_leptonSF_MU_SF_ID_STAT_LOWPT_UP;
  Float_t weight_leptonSF_MU_SF_ID_STAT_LOWPT_DOWN;
  Float_t weight_leptonSF_MU_SF_ID_SYST_LOWPT_UP;
  Float_t weight_leptonSF_MU_SF_ID_SYST_LOWPT_DOWN;
  Float_t weight_leptonSF_MU_SF_Isol_STAT_UP;
  Float_t weight_leptonSF_MU_SF_Isol_STAT_DOWN;
  Float_t weight_leptonSF_MU_SF_Isol_SYST_UP;
  Float_t weight_leptonSF_MU_SF_Isol_SYST_DOWN;
  Float_t weight_leptonSF_MU_SF_TTVA_STAT_UP;
  Float_t weight_leptonSF_MU_SF_TTVA_STAT_DOWN;
  Float_t weight_leptonSF_MU_SF_TTVA_SYST_UP;
  Float_t weight_leptonSF_MU_SF_TTVA_SYST_DOWN;
  std::vector<Float_t> weight_bTagSF_DL1r_77_eigenvars_B_up; 
  std::vector<Float_t> weight_bTagSF_DL1r_77_eigenvars_C_up;
  std::vector<Float_t> weight_bTagSF_DL1r_77_eigenvars_Light_up;
  std::vector<Float_t> weight_bTagSF_DL1r_77_eigenvars_B_down;
  std::vector<Float_t> weight_bTagSF_DL1r_77_eigenvars_C_down;
  std::vector<Float_t> weight_bTagSF_DL1r_77_eigenvars_Light_down;
  Float_t weight_bTagSF_DL1r_77_extrapolation_up;
  Float_t weight_bTagSF_DL1r_77_extrapolation_down;
  Float_t weight_bTagSF_DL1r_77_extrapolation_from_charm_up;
  Float_t weight_bTagSF_DL1r_77_extrapolation_from_charm_down;
  std::vector<Float_t> weight_bTagSF_DL1r_60_eigenvars_B_up;
  std::vector<Float_t> weight_bTagSF_DL1r_60_eigenvars_C_up;
  std::vector<Float_t> weight_bTagSF_DL1r_60_eigenvars_Light_up;
  std::vector<Float_t> weight_bTagSF_DL1r_60_eigenvars_B_down;
  std::vector<Float_t> weight_bTagSF_DL1r_60_eigenvars_C_down;
  std::vector<Float_t> weight_bTagSF_DL1r_60_eigenvars_Light_down;
  Float_t weight_bTagSF_DL1r_60_extrapolation_up;
  Float_t weight_bTagSF_DL1r_60_extrapolation_down;
  Float_t weight_bTagSF_DL1r_60_extrapolation_from_charm_up;
  Float_t weight_bTagSF_DL1r_60_extrapolation_from_charm_down;
  std::vector<Float_t> weight_bTagSF_DL1r_85_eigenvars_B_up;
  std::vector<Float_t> weight_bTagSF_DL1r_85_eigenvars_C_up;
  std::vector<Float_t> weight_bTagSF_DL1r_85_eigenvars_Light_up;
  std::vector<Float_t> weight_bTagSF_DL1r_85_eigenvars_B_down;
  std::vector<Float_t> weight_bTagSF_DL1r_85_eigenvars_C_down;
  std::vector<Float_t> weight_bTagSF_DL1r_85_eigenvars_Light_down;
  Float_t weight_bTagSF_DL1r_85_extrapolation_up;
  Float_t weight_bTagSF_DL1r_85_extrapolation_down;
  Float_t weight_bTagSF_DL1r_85_extrapolation_from_charm_up;
  Float_t weight_bTagSF_DL1r_85_extrapolation_from_charm_down;
  std::vector<Float_t> weight_bTagSF_DL1r_Continuous_eigenvars_B_up;
  std::vector<Float_t> weight_bTagSF_DL1r_Continuous_eigenvars_C_up;
  std::vector<Float_t> weight_bTagSF_DL1r_Continuous_eigenvars_Light_up;
  std::vector<Float_t> weight_bTagSF_DL1r_Continuous_eigenvars_B_down;
  std::vector<Float_t> weight_bTagSF_DL1r_Continuous_eigenvars_C_down;
  std::vector<Float_t> weight_bTagSF_DL1r_Continuous_eigenvars_Light_down;


  // ///
  // Set the branches
 
  tree->SetBranchAddress("jet_pt", &jet_pt);
  tree->SetBranchAddress("jet_eta", &jet_eta);
  tree->SetBranchAddress("jet_phi", &jet_phi);
  tree->SetBranchAddress("jet_e", &jet_e);
  tree->SetBranchAddress("jet_GBHInit_topHadronOriginFlag", &jet_GBHInit_topHadronOriginFlag);
  tree->SetBranchAddress("jet_nGhosts_bHadron", &jet_nGhosts_bHadron);
  tree->SetBranchAddress("jet_nGhosts_cHadron", &jet_nGhosts_cHadron);
  tree->SetBranchAddress("jet_truthPartonLabel", &jet_truthPartonLabel);
  if (obj_name!="particleLevel") {
    tree->SetBranchAddress("jet_jvt", &jet_jvt);
    tree->SetBranchAddress("jet_DL1r", &jet_DL1r);
    tree->SetBranchAddress("jet_isbtagged_DL1r_77", &jet_DL1r_77);
    tree->SetBranchAddress("jet_tagWeightBin_DL1r_Continuous", &jet_tagWeightBin_DL1r_Continuous);
    tree->SetBranchAddress("jet_truthflav", &jet_truthflav);
    tree->SetBranchAddress("jet_isTrueHS", &jet_isTrueHS);
  }

  tree->SetBranchAddress("el_pt", &el_pt);
  tree->SetBranchAddress("el_eta", &el_eta);
  tree->SetBranchAddress("el_phi", &el_phi);
  tree->SetBranchAddress("el_charge", &el_charge);
  tree->SetBranchAddress("el_e", &el_e);
  if (obj_name!="particleLevel") {
    tree->SetBranchAddress("el_cl_eta", &el_cl_eta);
    tree->SetBranchAddress("el_topoetcone20", &el_topoetcone20);
    tree->SetBranchAddress("el_ptvarcone20", &el_ptvarcone20);
    tree->SetBranchAddress("el_CF", &el_CF);
    tree->SetBranchAddress("el_d0sig", &el_d0sig);
    tree->SetBranchAddress("el_delta_z0_sintheta", &el_delta_z0_sintheta);
    tree->SetBranchAddress("el_true_type", &el_true_type);
    tree->SetBranchAddress("el_true_origin", &el_true_origin);
    tree->SetBranchAddress("el_true_firstEgMotherTruthType", &el_true_firstEgMotherTruthType);
    tree->SetBranchAddress("el_true_firstEgMotherTruthOrigin", &el_true_firstEgMotherTruthOrigin);
    tree->SetBranchAddress("el_true_firstEgMotherPdgId", &el_true_firstEgMotherPdgId);
    tree->SetBranchAddress("el_true_isPrompt", &el_true_isPrompt);
    tree->SetBranchAddress("el_true_isChargeFl", &el_true_isChargeFl);
  } else {
    tree->SetBranchAddress("el_pt_bare", &el_pt_bare);
    tree->SetBranchAddress("el_eta_bare", &el_eta_bare);
    tree->SetBranchAddress("el_phi_bare", &el_phi_bare);
    tree->SetBranchAddress("el_e_bare", &el_e_bare);    
  }

  tree->SetBranchAddress("mu_pt", &mu_pt);
  tree->SetBranchAddress("mu_eta", &mu_eta);
  tree->SetBranchAddress("mu_phi", &mu_phi);
  tree->SetBranchAddress("mu_charge", &mu_charge);
  tree->SetBranchAddress("mu_e", &mu_e);
  if (obj_name!="particleLevel") {
    tree->SetBranchAddress("mu_topoetcone20", &mu_topoetcone20);
    tree->SetBranchAddress("mu_ptvarcone30", &mu_ptvarcone30);
    tree->SetBranchAddress("mu_d0sig", &mu_d0sig);
    tree->SetBranchAddress("mu_delta_z0_sintheta", &mu_delta_z0_sintheta);
    tree->SetBranchAddress("mu_true_type", &mu_true_type);
    tree->SetBranchAddress("mu_true_origin", &mu_true_origin);
    tree->SetBranchAddress("mu_true_isPrompt", &mu_true_isPrompt);
  } else {
    tree->SetBranchAddress("mu_pt_bare", &mu_pt_bare);
    tree->SetBranchAddress("mu_eta_bare", &mu_eta_bare);
    tree->SetBranchAddress("mu_phi_bare", &mu_phi_bare);
    tree->SetBranchAddress("mu_e_bare", &mu_e_bare);
  }

  tree->SetBranchAddress("met_met", &met);
  tree->SetBranchAddress("met_phi", &met_phi);
  tree->SetBranchAddress("runNumber", &runNumber);
  tree->SetBranchAddress("eventNumber", &eventNumber);
  tree->SetBranchAddress("topHeavyFlavorFilterFlag", &topHeavyFlavorFilterFlag);

  tree->SetBranchAddress("weight_mc", &w_mc);
  tree->SetBranchAddress("weight_pileup", &w_pu);
  if (obj_name!="particleLevel") {
    tree->SetBranchAddress("weight_leptonSF", &w_leptonSF);
    tree->SetBranchAddress("weight_bTagSF_DL1r_77", &w_DL1r_77);
    tree->SetBranchAddress("weight_jvt", &w_jvt);
    
    tree->SetBranchAddress("vertex_type", &vertex_type);
    tree->SetBranchAddress("vertex_z", &vertex_z);
  }


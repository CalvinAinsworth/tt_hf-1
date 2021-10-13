
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
  
  el_pt = el_eta = el_cl_eta = el_phi = el_charge = el_e = el_topoetcone20 = el_ptvarcone20 = el_CF = el_d0sig = el_delta_z0_sintheta = 0;
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
  
  mu_pt = mu_eta = mu_phi = mu_charge = mu_e = mu_topoetcone20 = mu_ptvarcone30 = mu_d0sig = mu_delta_z0_sintheta = 0;
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


  // ///
  // Particle level tree variables

  // Jet variables
  std::vector<Float_t> *jet_pt_pl;
  std::vector<Float_t> *jet_eta_pl;
  std::vector<Float_t> *jet_phi_pl;
  std::vector<Float_t> *jet_e_pl;
  std::vector<Int_t> *jet_nGhosts_bHadron_pl;
  std::vector<Int_t> *jet_nGhosts_cHadron_pl;
  std::vector<Int_t> *jet_truthPartonLabel_pl;
  std::vector<Int_t> *jet_GBHInit_topHadronOriginFlag_pl;

  jet_pt_pl = jet_eta_pl = jet_phi_pl = jet_e_pl = 0;
  jet_nGhosts_bHadron_pl = jet_nGhosts_cHadron_pl = jet_truthPartonLabel_pl = jet_GBHInit_topHadronOriginFlag_pl = 0;


  // Electrons
  std::vector<Float_t> *el_pt_pl;
  std::vector<Float_t> *el_eta_pl;
  std::vector<Float_t> *el_phi_pl;
  std::vector<Float_t> *el_e_pl;
  std::vector<Float_t> *el_charge_pl;
  std::vector<Float_t> *el_pt_bare;
  std::vector<Float_t> *el_eta_bare;
  std::vector<Float_t> *el_phi_bare;
  std::vector<Float_t> *el_e_bare;

  el_pt_pl = el_eta_pl = el_phi_pl = el_e_pl = el_charge_pl = el_pt_bare = el_eta_bare = el_phi_bare = el_e_bare = 0;
  

  // Muons
  std::vector<Float_t> *mu_pt_pl;
  std::vector<Float_t> *mu_eta_pl;
  std::vector<Float_t> *mu_phi_pl;
  std::vector<Float_t> *mu_e_pl;
  std::vector<Float_t> *mu_charge_pl;
  std::vector<Float_t> *mu_pt_bare;
  std::vector<Float_t> *mu_eta_bare;
  std::vector<Float_t> *mu_phi_bare;
  std::vector<Float_t> *mu_e_bare;
  
  mu_pt_pl = mu_eta_pl = mu_phi_pl = mu_e_pl = mu_charge_pl = mu_pt_bare = mu_eta_bare = mu_phi_bare = mu_e_bare = 0;
  

  // Other
  Float_t met_pl;
  Float_t met_phi_pl;
  UInt_t runNumber_pl;
  ULong64_t eventNumber_pl;
  Int_t topHeavyFlavorFilterFlag_pl = -999;


  // Weights
  Float_t w_mc_pl;
  Float_t w_pu_pl;  



  // ///
  // Set the branches
    
  tree_nominal->SetBranchAddress("jet_pt", &jet_pt);
  tree_nominal->SetBranchAddress("jet_eta", &jet_eta);
  tree_nominal->SetBranchAddress("jet_phi", &jet_phi);
  tree_nominal->SetBranchAddress("jet_e", &jet_e);
  tree_nominal->SetBranchAddress("jet_jvt", &jet_jvt);
  tree_nominal->SetBranchAddress("jet_DL1r", &jet_DL1r);
  tree_nominal->SetBranchAddress("jet_isbtagged_DL1r_77", &jet_DL1r_77);
  tree_nominal->SetBranchAddress("jet_tagWeightBin_DL1r_Continuous", &jet_tagWeightBin_DL1r_Continuous);
  tree_nominal->SetBranchAddress("jet_truthflav", &jet_truthflav);
  tree_nominal->SetBranchAddress("jet_GBHInit_topHadronOriginFlag", &jet_GBHInit_topHadronOriginFlag);
  tree_nominal->SetBranchAddress("jet_nGhosts_bHadron", &jet_nGhosts_bHadron);
  tree_nominal->SetBranchAddress("jet_nGhosts_cHadron", &jet_nGhosts_cHadron);
  tree_nominal->SetBranchAddress("jet_truthPartonLabel", &jet_truthPartonLabel);
  tree_nominal->SetBranchAddress("jet_isTrueHS", &jet_isTrueHS);  
  
  tree_nominal->SetBranchAddress("el_pt", &el_pt);
  tree_nominal->SetBranchAddress("el_eta", &el_eta);
  tree_nominal->SetBranchAddress("el_cl_eta", &el_cl_eta);
  tree_nominal->SetBranchAddress("el_phi", &el_phi);
  tree_nominal->SetBranchAddress("el_charge", &el_charge);
  tree_nominal->SetBranchAddress("el_e", &el_e);
  tree_nominal->SetBranchAddress("el_topoetcone20", &el_topoetcone20);
  tree_nominal->SetBranchAddress("el_ptvarcone20", &el_ptvarcone20);
  tree_nominal->SetBranchAddress("el_CF", &el_CF);
  tree_nominal->SetBranchAddress("el_d0sig", &el_d0sig);
  tree_nominal->SetBranchAddress("el_delta_z0_sintheta", &el_delta_z0_sintheta);
  tree_nominal->SetBranchAddress("el_true_type", &el_true_type);
  tree_nominal->SetBranchAddress("el_true_origin", &el_true_origin);
  tree_nominal->SetBranchAddress("el_true_firstEgMotherTruthType", &el_true_firstEgMotherTruthType);
  tree_nominal->SetBranchAddress("el_true_firstEgMotherTruthOrigin", &el_true_firstEgMotherTruthOrigin);
  tree_nominal->SetBranchAddress("el_true_firstEgMotherPdgId", &el_true_firstEgMotherPdgId);
  tree_nominal->SetBranchAddress("el_true_isPrompt", &el_true_isPrompt);
  tree_nominal->SetBranchAddress("el_true_isChargeFl", &el_true_isChargeFl);
  
  tree_nominal->SetBranchAddress("mu_pt", &mu_pt);
  tree_nominal->SetBranchAddress("mu_eta", &mu_eta);
  tree_nominal->SetBranchAddress("mu_phi", &mu_phi);
  tree_nominal->SetBranchAddress("mu_charge", &mu_charge);
  tree_nominal->SetBranchAddress("mu_e", &mu_e);
  tree_nominal->SetBranchAddress("mu_topoetcone20", &mu_topoetcone20);
  tree_nominal->SetBranchAddress("mu_ptvarcone30", &mu_ptvarcone30);
  tree_nominal->SetBranchAddress("mu_d0sig", &mu_d0sig);
  tree_nominal->SetBranchAddress("mu_delta_z0_sintheta", &mu_delta_z0_sintheta);
  tree_nominal->SetBranchAddress("mu_true_type", &mu_true_type);
  tree_nominal->SetBranchAddress("mu_true_origin", &mu_true_origin);
  tree_nominal->SetBranchAddress("mu_true_isPrompt", &mu_true_isPrompt);
  
  tree_nominal->SetBranchAddress("met_met", &met);
  tree_nominal->SetBranchAddress("met_phi", &met_phi);
  tree_nominal->SetBranchAddress("runNumber", &runNumber);
  tree_nominal->SetBranchAddress("eventNumber", &eventNumber);
  tree_nominal->SetBranchAddress("topHeavyFlavorFilterFlag", &topHeavyFlavorFilterFlag);
  
  tree_nominal->SetBranchAddress("weight_mc", &w_mc);
  tree_nominal->SetBranchAddress("weight_pileup", &w_pu);
  tree_nominal->SetBranchAddress("weight_leptonSF", &w_leptonSF);
  tree_nominal->SetBranchAddress("weight_bTagSF_DL1r_77", &w_DL1r_77);
  tree_nominal->SetBranchAddress("weight_jvt", &w_jvt);
  
  tree_nominal->SetBranchAddress("vertex_type", &vertex_type);
  tree_nominal->SetBranchAddress("vertex_z", &vertex_z);
  
  
  if (tree_pl_exists==true) {
    
    tree_pl->SetBranchAddress("jet_pt", &jet_pt_pl);
    tree_pl->SetBranchAddress("jet_eta", &jet_eta_pl);
    tree_pl->SetBranchAddress("jet_phi", &jet_phi_pl);
    tree_pl->SetBranchAddress("jet_e", &jet_e_pl);
    tree_pl->SetBranchAddress("jet_nGhosts_bHadron", &jet_nGhosts_bHadron_pl);
    tree_pl->SetBranchAddress("jet_nGhosts_cHadron", &jet_nGhosts_cHadron_pl);
    tree_pl->SetBranchAddress("jet_truthPartonLabel", &jet_truthPartonLabel_pl);
    tree_pl->SetBranchAddress("jet_GBHInit_topHadronOriginFlag", &jet_GBHInit_topHadronOriginFlag_pl);
    
    tree_pl->SetBranchAddress("el_pt", &el_pt_pl);
    tree_pl->SetBranchAddress("el_eta", &el_eta_pl);
    tree_pl->SetBranchAddress("el_phi", &el_phi_pl);
    tree_pl->SetBranchAddress("el_e", &el_e_pl);
    tree_pl->SetBranchAddress("el_charge", &el_charge_pl);
    tree_pl->SetBranchAddress("el_pt_bare", &el_pt_bare);
    tree_pl->SetBranchAddress("el_eta_bare", &el_eta_bare);
    tree_pl->SetBranchAddress("el_phi_bare", &el_phi_bare);
    tree_pl->SetBranchAddress("el_e_bare", &el_e_bare);
    
    tree_pl->SetBranchAddress("mu_pt", &mu_pt_pl);
    tree_pl->SetBranchAddress("mu_eta", &mu_eta_pl);
    tree_pl->SetBranchAddress("mu_phi", &mu_phi_pl);
    tree_pl->SetBranchAddress("mu_e", &mu_e_pl);
    tree_pl->SetBranchAddress("mu_charge", &mu_charge_pl);
    tree_pl->SetBranchAddress("mu_pt_bare", &mu_pt_bare);
    tree_pl->SetBranchAddress("mu_eta_bare", &mu_eta_bare);
    tree_pl->SetBranchAddress("mu_phi_bare", &mu_phi_bare);
    tree_pl->SetBranchAddress("mu_e_bare", &mu_e_bare);
    
    tree_pl->SetBranchAddress("met_met", &met_pl);
    tree_pl->SetBranchAddress("met_phi", &met_phi_pl);
    tree_pl->SetBranchAddress("runNumber", &runNumber_pl);
    tree_pl->SetBranchAddress("eventNumber", &eventNumber_pl);
    tree_pl->SetBranchAddress("topHeavyFlavorFilterFlag", &topHeavyFlavorFilterFlag_pl);
    
    tree_pl->SetBranchAddress("weight_mc", &w_mc_pl);
    tree_pl->SetBranchAddress("weight_pileup", &w_pu_pl);
    
  } // if tree_pl_exists == true

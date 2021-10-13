
  // Jet variables
  std::vector<Float_t> *jet_pt;
  std::vector<Float_t> *jet_DL1r;
  std::vector<Float_t> *jet_eta;
  std::vector<Float_t> *jet_phi;
  std::vector<Float_t> *jet_e;
  std::vector<Float_t> *jet_jvt;
  std::vector<Float_t> *jet_tagWeightBin_DL1r_Continuous;
  std::vector<char> *jet_DL1r_77;
  
  jet_pt = jet_DL1r = jet_eta = jet_phi = jet_e = jet_jvt = jet_tagWeightBin_DL1r_Continuous = 0;
  jet_DL1r_77 = 0;


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
  
  el_pt = el_eta = el_cl_eta = el_phi = el_charge = el_e = el_topoetcone20 = el_ptvarcone20 = el_CF = el_d0sig = el_delta_z0_sintheta = 0;


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

  mu_pt = mu_eta = mu_phi = mu_charge = mu_e = mu_topoetcone20 = mu_ptvarcone30 = mu_d0sig = mu_delta_z0_sintheta = 0;

  
  // Other
  Float_t met;
  Float_t met_phi;
  UInt_t runNumber;
  ULong64_t eventNumber;


  // Vertices
  std::vector<Int_t> *vertex_type;
  std::vector<Double_t> *vertex_z;

  vertex_type = 0;
  vertex_z = 0;


  
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

  tree_nominal->SetBranchAddress("mu_pt", &mu_pt);
  tree_nominal->SetBranchAddress("mu_eta", &mu_eta);
  tree_nominal->SetBranchAddress("mu_phi", &mu_phi);
  tree_nominal->SetBranchAddress("mu_charge", &mu_charge);
  tree_nominal->SetBranchAddress("mu_e", &mu_e);
  tree_nominal->SetBranchAddress("mu_topoetcone20", &mu_topoetcone20);
  tree_nominal->SetBranchAddress("mu_ptvarcone30", &mu_ptvarcone30);
  tree_nominal->SetBranchAddress("mu_d0sig", &mu_d0sig);
  tree_nominal->SetBranchAddress("mu_delta_z0_sintheta", &mu_delta_z0_sintheta);
  
  tree_nominal->SetBranchAddress("met_met", &met);
  tree_nominal->SetBranchAddress("met_phi", &met_phi);
  tree_nominal->SetBranchAddress("runNumber", &runNumber);
  tree_nominal->SetBranchAddress("eventNumber", &eventNumber);

  tree_nominal->SetBranchAddress("vertex_type", &vertex_type);
  tree_nominal->SetBranchAddress("vertex_z", &vertex_z);


  // Declare variables first

  // Jets
  std::vector<Float_t> *jet_pt;
  std::vector<Float_t> *jet_eta;
  std::vector<Float_t> *jet_phi;
  std::vector<Float_t> *jet_e;
  std::vector<Int_t> *topHadronOriginFlag;
  std::vector<Int_t> *jet_truthPartonLabel;
  std::vector<Int_t> *jet_nGhosts_bHadron;

  jet_pt = jet_eta = jet_phi = jet_e = 0;
  jet_nGhosts_bHadron = topHadronOriginFlag = jet_truthPartonLabel = 0;


  // Electrons
  std::vector<Float_t> *el_pt;
  std::vector<Float_t> *el_eta;
  std::vector<Float_t> *el_phi;
  std::vector<Float_t> *el_charge;
  std::vector<Float_t> *el_e;
  
  el_pt = el_eta = el_phi = el_charge = el_e = 0;


  // Muons
  std::vector<Float_t> *mu_pt;
  std::vector<Float_t> *mu_eta;
  std::vector<Float_t> *mu_phi;
  std::vector<Float_t> *mu_charge;
  std::vector<Float_t> *mu_e;

  mu_pt = mu_eta = mu_phi = mu_charge = mu_e = 0;


  // Other
  Float_t met;
  Float_t met_phi;
  Int_t topHFFF;
  UInt_t runNumber;



  // Set the branches
  tree_pl->SetBranchAddress("jet_pt", &jet_pt);
  tree_pl->SetBranchAddress("jet_eta", &jet_eta);
  tree_pl->SetBranchAddress("jet_phi", &jet_phi);
  tree_pl->SetBranchAddress("jet_e", &jet_e);
  tree_pl->SetBranchAddress("jet_truthPartonLabel", &jet_truthPartonLabel);
  tree_pl->SetBranchAddress("el_pt", &el_pt);
  tree_pl->SetBranchAddress("el_eta", &el_eta);
  tree_pl->SetBranchAddress("el_phi", &el_phi);
  tree_pl->SetBranchAddress("el_charge", &el_charge);
  tree_pl->SetBranchAddress("el_e", &el_e);
  tree_pl->SetBranchAddress("mu_pt", &mu_pt);
  tree_pl->SetBranchAddress("mu_eta", &mu_eta);
  tree_pl->SetBranchAddress("mu_phi", &mu_phi);
  tree_pl->SetBranchAddress("mu_charge", &mu_charge);
  tree_pl->SetBranchAddress("mu_e", &mu_e);
  tree_pl->SetBranchAddress("jet_GBHInit_topHadronOriginFlag", &topHadronOriginFlag); // https://gitlab.cern.ch/TTJ/Ntuple/-/blob/master/TTJNtuple/TTJNtuple/EventSaver.h#L55
  tree_pl->SetBranchAddress("jet_nGhosts_bHadron", &jet_nGhosts_bHadron);
  tree_pl->SetBranchAddress("met_met", &met);
  tree_pl->SetBranchAddress("met_phi", &met_phi);
  tree_pl->SetBranchAddress("topHeavyFlavorFilterFlag", &topHFFF);
  tree_pl->SetBranchAddress("runNumber", &runNumber);
  


  // Weights
  Float_t w_mc, w_pu;
  tree_pl->SetBranchAddress("weight_mc", &w_mc);
  tree_pl->SetBranchAddress("weight_pileup", &w_pu);


  // Declare variables first

  // Jets
  std::vector<Float_t> *jet_pt;
  std::vector<Float_t> *jet_DL1r;
  std::vector<Float_t> *jet_eta;
  std::vector<Float_t> *jet_phi;
  std::vector<Float_t> *jet_e;
  std::vector<Int_t> *topHadronOriginFlag;
  std::vector<Int_t> *jet_truthflav;
  std::vector<char> *jet_DL1r_77;

  jet_pt = jet_DL1r = jet_eta = jet_phi = jet_e = 0;
  topHadronOriginFlag = jet_truthflav = 0;
  jet_DL1r_77 = 0;


  // Electrons
  std::vector<Float_t> *el_pt;
  std::vector<Float_t> *el_eta;
  std::vector<Float_t> *el_cl_eta;
  std::vector<Float_t> *el_phi;
  std::vector<Float_t> *el_charge;
  std::vector<Float_t> *el_e;
  
  el_pt = el_eta = el_cl_eta = el_phi = el_charge = el_e = 0;
  

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
  tree_nominal->SetBranchAddress("jet_pt", &jet_pt);
  tree_nominal->SetBranchAddress("jet_eta", &jet_eta);
  tree_nominal->SetBranchAddress("jet_phi", &jet_phi);
  tree_nominal->SetBranchAddress("jet_e", &jet_e);
  tree_nominal->SetBranchAddress("jet_DL1r", &jet_DL1r);
  tree_nominal->SetBranchAddress("jet_isbtagged_DL1r_77", &jet_DL1r_77);
  tree_nominal->SetBranchAddress("jet_truthflav", &jet_truthflav);
  tree_nominal->SetBranchAddress("el_pt", &el_pt);
  tree_nominal->SetBranchAddress("el_eta", &el_eta);
  tree_nominal->SetBranchAddress("el_cl_eta", &el_cl_eta);
  tree_nominal->SetBranchAddress("el_phi", &el_phi);
  tree_nominal->SetBranchAddress("el_charge", &el_charge);
  tree_nominal->SetBranchAddress("el_e", &el_e);
  tree_nominal->SetBranchAddress("mu_pt", &mu_pt);
  tree_nominal->SetBranchAddress("mu_eta", &mu_eta);
  tree_nominal->SetBranchAddress("mu_phi", &mu_phi);
  tree_nominal->SetBranchAddress("mu_charge", &mu_charge);
  tree_nominal->SetBranchAddress("mu_e", &mu_e);
  tree_nominal->SetBranchAddress("truth_hadron_TopHadronOriginFlag", &topHadronOriginFlag); // https://gitlab.cern.ch/TTJ/Ntuple/-/blob/master/TTJNtuple/TTJNtuple/EventSaver.h#L55
  tree_nominal->SetBranchAddress("met_met", &met);
  tree_nominal->SetBranchAddress("met_phi", &met_phi);
  tree_nominal->SetBranchAddress("TopHeavyFlavorFilterFlag", &topHFFF);
  tree_nominal->SetBranchAddress("runNumber", &runNumber);
  


  // Weights
  Float_t w_mc, w_pu, w_leptonSF, w_DL1r_77, w_jvt;
  tree_nominal->SetBranchAddress("weight_mc", &w_mc);
  tree_nominal->SetBranchAddress("weight_pileup", &w_pu);
  tree_nominal->SetBranchAddress("weight_leptonSF", &w_leptonSF);
  tree_nominal->SetBranchAddress("weight_bTagSF_DL1r_77", &w_DL1r_77);
  tree_nominal->SetBranchAddress("weight_jvt", &w_jvt);

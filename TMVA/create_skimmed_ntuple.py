import ROOT
from array import array


##################
##     MAIN     ##
##################

# Get name of the TMVA method
method_name = ""
config_file = open("tmva_config.txt", "r")
for line in config_file:
  par_name_and_var = line.split(": ")    
  if ("TMVA Method" in line):
    method_name += par_name_and_var[1].strip() # remove the \n
config_file.close()
print(method_name)


# Set some variables for the analysis
classifier_cut = 0.1
n_jets_from_top = 0
event_jet_jetinfo = {}
event_lepinfo = {}


# Open the tfile - tmva ouptut
tmva_file = ROOT.TFile.Open("TMVA.root", "READ")


# Open trees
tree_names = ["TestTree", "TrainTree"]
for tree_name in tree_names:
  tree = tmva_file.Get("dataset/" + tree_name)
  print("Working with ", tree_name, " tree!")

  # Gen number of entries
  nEntries = tree.GetEntries()
  print("nEntries: ", nEntries)

  # Run a loop over entries=jets
  for entry in range(0, nEntries):
    entry = int(entry)
  
    if (entry%1000==0): 
      print("\t", entry, end="\r")

    tree.GetEntry(entry)
      
    # Declare single values
    event_number          = getattr(tree, "event_number")
    
    weight                = getattr(tree, "tot_event_weight")
    met                   = getattr(tree, "met")
    el_pt                 = getattr(tree, "el_pt")
    el_eta                = getattr(tree, "el_eta")
    el_phi                = getattr(tree, "el_phi")
    el_e                  = getattr(tree, "el_e")
    el_charge             = getattr(tree, "el_charge")
    mu_pt                 = getattr(tree, "mu_pt")
    mu_eta                = getattr(tree, "mu_eta")
    mu_phi                = getattr(tree, "mu_phi")
    mu_e                  = getattr(tree, "mu_e")
    mu_charge             = getattr(tree, "mu_charge")

    # Declate "vectors-to-be" values
    topHOF                = getattr(tree, "topHadronOriginFlag")
    jet_isbtagged_DL1r_77 = getattr(tree, "jet_isbtagged_DL1r_77")
    jet_truthflav         = getattr(tree, "jet_truthflav")
    jet_DL1r              = getattr(tree, "jet_DL1r")
    classifier            = getattr(tree, method_name)
    jet_pt                = getattr(tree, "jet_pt")
    jet_eta               = getattr(tree, "jet_eta")
    jet_phi               = getattr(tree, "jet_phi")
    jet_e                 = getattr(tree, "jet_e")
    
    jetinfo = [topHOF, jet_isbtagged_DL1r_77, jet_truthflav, jet_DL1r, classifier, jet_pt, jet_eta, jet_phi, jet_e]

    if event_number in event_jet_jetinfo:
      jetn = len((event_jet_jetinfo[event_number]))
      jet_jetinfo = event_jet_jetinfo[event_number]
      jet_jetinfo[jetn] = jetinfo
      event_jet_jetinfo[event_number] = jet_jetinfo
    else:
      jet_jetinfo= {}
      jet_jetinfo[0] = jetinfo
      event_jet_jetinfo[event_number] = jet_jetinfo

    lepinfo = [weight, met, el_pt, el_eta, el_phi, el_e, el_charge, mu_pt, mu_eta, mu_phi, mu_e, mu_charge]

    if event_number not in event_lepinfo:
      event_lepinfo[event_number] = lepinfo

# Close the tmva file
tmva_file.Close()


# ###
# Create the new ntuple
# ###

# Create the file and a tree
new_file = ROOT.TFile.Open("skimmed_test_ntuple.root", "RECREATE")
new_tree = ROOT.TTree("test", "Tree with entries from test tmva tree")

# Create branches for "single" values per event
weight_save    = array("f", [0.0])
met_save       = array("f", [0.0])
el_pt_save     = array("f", [0.0])
el_eta_save    = array("f", [0.0])
el_phi_save    = array("f", [0.0])
el_e_save      = array("f", [0.0])
el_charge_save = array("f", [0.0])
mu_pt_save     = array("f", [0.0])
mu_eta_save    = array("f", [0.0])
mu_phi_save    = array("f", [0.0])
mu_e_save      = array("f", [0.0])
mu_charge_save = array("f", [0.0])

new_tree.Branch("tot_event_weight", weight_save,    "tot_event_weight/F")
new_tree.Branch("met",              met_save,       "met/F")
new_tree.Branch("el_pt",            el_pt_save,     "el_pt/F")
new_tree.Branch("el_eta",           el_eta_save,    "el_eta/F")
new_tree.Branch("el_phi",           el_phi_save,    "el_phi/F")
new_tree.Branch("el_e",             el_e_save,      "el_e/F")
new_tree.Branch("el_charge",        el_charge_save, "el_charge/F")
new_tree.Branch("mu_pt",            mu_pt_save,     "mu_pt/F")
new_tree.Branch("mu_eta",           mu_eta_save,    "mu_eta/F")
new_tree.Branch("mu_phi",           mu_phi_save,    "mu_phi/F")
new_tree.Branch("mu_e",             mu_e_save,      "mu_e/F")
new_tree.Branch("mu_charge",        mu_charge_save, "mu_charge/F")

# Create branches for "vector" values per event
topHOF_save                = ROOT.vector("int")()
jet_isbtagged_DL1r_77_save = ROOT.vector("int")()
jet_truthflav_save         = ROOT.vector("int")()
jet_DL1r_save              = ROOT.vector("float")()
classifier_save            = ROOT.vector("float")()
jet_pt_save                = ROOT.vector("float")()
jet_eta_save               = ROOT.vector("float")()
jet_phi_save               = ROOT.vector("float")()
jet_e_save                 = ROOT.vector("float")()

new_tree.Branch("topHadronOriginFlag",   topHOF_save)
new_tree.Branch("jet_isbtagged_DL1r_77", jet_isbtagged_DL1r_77_save)
new_tree.Branch("jet_truthflav",         jet_truthflav_save)
new_tree.Branch("jet_DL1r",              jet_DL1r_save)
new_tree.Branch(method_name,             classifier_save)
new_tree.Branch("jet_pt",                jet_pt_save)
new_tree.Branch("jet_eta",               jet_eta_save)
new_tree.Branch("jet_phi",               jet_phi_save)
new_tree.Branch("jet_e",                 jet_e_save)


# Loop over key of array - event numbers
for event_number in event_jet_jetinfo.keys():

  event_number = int(event_number)

  topHOF_save.clear()
  jet_isbtagged_DL1r_77_save.clear()
  jet_truthflav_save.clear()
  jet_DL1r_save.clear()
  classifier_save.clear()
  jet_pt_save.clear()
  jet_eta_save.clear()
  jet_phi_save.clear()
  jet_e_save.clear()

  # Loop over jets in the event - fill the "vector" values
  for jet_n in event_jet_jetinfo[event_number].keys():
    topHOF_save.push_back(int(event_jet_jetinfo[event_number][jet_n][0]))
    jet_isbtagged_DL1r_77_save.push_back(int(event_jet_jetinfo[event_number][jet_n][1]))
    jet_truthflav_save.push_back(int(event_jet_jetinfo[event_number][jet_n][2]))
    jet_DL1r_save.push_back(event_jet_jetinfo[event_number][jet_n][3])
    classifier_save.push_back(event_jet_jetinfo[event_number][jet_n][4])
    jet_pt_save.push_back(event_jet_jetinfo[event_number][jet_n][5])
    jet_eta_save.push_back(event_jet_jetinfo[event_number][jet_n][6])
    jet_phi_save.push_back(event_jet_jetinfo[event_number][jet_n][7])
    jet_e_save.push_back(event_jet_jetinfo[event_number][jet_n][8])


  # Fill the "single" values
  weight_save[0]    = event_lepinfo[event_number][0]
  met_save[0]       = event_lepinfo[event_number][1]
  el_pt_save[0]     = event_lepinfo[event_number][2]
  el_eta_save[0]    = event_lepinfo[event_number][3]
  el_phi_save[0]    = event_lepinfo[event_number][4]
  el_e_save[0]      = event_lepinfo[event_number][5]
  el_charge_save[0] = event_lepinfo[event_number][6]
  mu_pt_save[0]     = event_lepinfo[event_number][7]
  mu_eta_save[0]    = event_lepinfo[event_number][8]
  mu_phi_save[0]    = event_lepinfo[event_number][9]
  mu_e_save[0]      = event_lepinfo[event_number][10]
  mu_charge_save[0] = event_lepinfo[event_number][11]

  # Fill the new tree
  new_tree.Fill()

# Save the new file and close it
new_file.Write()
new_file.Close()

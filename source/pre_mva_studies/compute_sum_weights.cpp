#include "../common_functions.h"

// ///
// MAIN
// ///
int main(int argc, char *argv[]) {

  // Get config info
  std::map<TString, TString> mc_config_info = get_mc_config_info(std::string(argv[1]));
  if (mc_config_info.size()==0) return 0;
  std::map<TString, TString>::iterator it;
  for (it=mc_config_info.begin(); it!=mc_config_info.end(); it++) {
    std::cout << it->first << " :\t" << it->second << std::endl;
  }
  TString process = mc_config_info["process"];
  TString generator = mc_config_info["generator"];
  TString lep_pt_cut_suffix = mc_config_info["lep_pt_cut_suffix"];
  TString campaign = mc_config_info["campaign"];

  
  // Declare events counter
  std::map<TString, double> sumWeights_map; // keys will be as "did mc16X"


  // Run a loop over ntuples names from a txt file
  std::string in_line = "";
  std::ifstream ntuples_list_file("ntuples_list.txt");
  if (ntuples_list_file.is_open()) {
    while (getline(ntuples_list_file, in_line)) {
      TString ntuple_name = in_line;

      // Split the ntuple name (that also contains ful path) into components to analyze them
      std::vector<TString> ntuple_name_components = split(ntuple_name, '/');

      // Analyze the ntuple name: data or mc? which campaign?
      bool is_data = false;
      bool is_mc16a = false;
      bool is_mc16d = false;
      bool is_mc16e = false;
      TString mc16X_str = "";
      bool is_fullsim = false;
      std::string job_DID = "";
      for (int i=0; i<ntuple_name_components.size(); i++) {
	std::vector<TString> pieces = split(ntuple_name_components[i], '.');
        for (int j=0; j<pieces.size(); j++) {
	  std::string piece = std::string(pieces[j]);
          if (piece == "data") is_data = true;
          if (piece == "mc16a") { is_mc16a = true; mc16X_str = "mc16a"; }
          if (piece == "mc16d") { is_mc16d = true; mc16X_str = "mc16d"; }
          if (piece == "mc16e") { is_mc16e = true; mc16X_str = "mc16e"; }
          if (piece == "s3126") is_fullsim = true;
          if (i==ntuple_name_components.size()-2 && j==2) job_DID = piece;
        }
      }

      // We work with MC only
      if (is_data == true) continue;

      // Select only dids of our interest
      bool correct_did = false;
      #include "include/did_selection.h"
      if (correct_did==false) continue;
      std::cout << "\nWorking with:\n" << ntuple_name << std::endl;

      
      // Open the ntuple
      TFile *ntuple = new TFile (ntuple_name);
      TTree *tree_sw = (TTree*)ntuple->Get("sumWeights");


      // Set Branches
      float sw;
      tree_sw->SetBranchAddress("totalEventsWeighted", &sw);

      
      // Get the number
      for (int entry=0; entry<tree_sw->GetEntries(); entry++) {
	tree_sw->GetEntry(entry);
	TString map_key = job_DID + " " + mc16X_str;
	if (!sumWeights_map[map_key]) {
	  sumWeights_map[map_key] = sw;
	} else {
	  sumWeights_map[map_key] += sw;
	}
      }

      ntuple->Close();

    } // loop over files names
  } // if (ntuples_list_file.is_open())
	
  
  std::map<TString, double>::iterator iter;
  for (iter=sumWeights_map.begin(); iter!=sumWeights_map.end(); iter++) {
    std::cout << iter->first << "  --  " << iter->second << std::endl;
  }

  return 0;
}

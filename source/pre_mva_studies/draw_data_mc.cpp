#include "../common_functions.h"
#include "../draw_functions.h"


// ///
// MAIN
// ///
int main(int argc, char *argv[])
{
  // Create directories for plots
  gSystem->Exec("mkdir results/plots");
  gSystem->Exec("mkdir results/plots/data_mc");


  // Leptons pT cut choise
  TString lep_pt_cut_suffix = leptons_pt_cut_suffix();


  // Get data hists
  TString data_hists_fname = "results/hists_data_" + lep_pt_cut_suffix + ".root";
  TFile *data_hists_file = new TFile(data_hists_fname);
  TIter next(data_hists_file->GetListOfKeys());
  TKey *key;
  std::vector<TString> hists_names = {};
  std::vector<TH1*> data_hists = {};
  
  while ( (key = (TKey*)next()) ) {
    
    TString hist_name;
    TString class_name = key->GetClassName();
    
    if (class_name=="TH1F") {
      TString hist_name = key->GetName();
      hists_names.push_back(hist_name);
      TH1 *h_tmp = (TH1*)data_hists_file->Get(hist_name);
      data_hists.push_back(h_tmp);
    }
    
  } // [while] - loop over hists in tfile
  
  
  
  // Get mc hists
  std::vector<TString> processes = {"z_jets", "other", "diboson", "ttH", "ttV", "singletop", "tt"};
  std::vector<std::vector<TH1*>> mc_hists;

  for (int i=0; i<processes.size(); i++) {
    TString mc_hists_file_name = "results/hists_" + processes[i] + "_reco_nominal" + lep_pt_cut_suffix + ".root";
    TFile *mc_hists_file = new TFile(mc_hists_file_name);
    
    std::vector<TH1*> mc_hists_tmp;

    for (int j=0; j<hists_names.size(); j++) {
      TH1 *h_tmp = (TH1*)mc_hists_file->Get(hists_names[j]);
      mc_hists_tmp.push_back(h_tmp);
    } // [j] - different hists (hists_names)
    mc_hists.push_back(mc_hists_tmp);

  } // [i] - processes



  // Prepare vectors of hists
  std::vector<std::vector<TH1*>> mc_hists_to_stack;
  
  for (int j=0; j<hists_names.size(); j++) {
    std::vector<TH1*> h_tmp;
    for (int i=0; i<processes.size(); i++) {
      h_tmp.push_back(mc_hists[i][j]);
    } // [i] - loop over processes
    mc_hists_to_stack.push_back(h_tmp);
  } // [j] - loop over names of hists

  
  
  // Draw hists
  for (int i=0; i<data_hists.size(); i++) {

    // Split the hits name: its root is the savename and its suffix is the x-axis title
    std::vector<TString> hist_strings = split(hists_names[i], '.');
    int draw_the_hists = draw_data_mc_plot(data_hists[i], mc_hists_to_stack[i], "#bf{" + hist_strings[1] + "}", hist_strings[0], processes, false);
   
    //std::cout << hist_strings[0] << " ; " << hist_strings[1] << std::endl;
  }

  
  // Close the hists files
  data_hists_file->Close();

  return 0;
}

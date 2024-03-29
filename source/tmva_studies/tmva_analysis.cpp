#include "../common_functions.h"

// ///
// MAIN
// ///
int main(int argc, char *argv[])
{
  // Get MC config info
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

  
  // Get config info about MVA setup
  std::vector<TString> tmva_config_info = get_tmva_config_info("source/tmva_config.txt");

  
  // Create an output file
  TString out_fname = "/afs/cern.ch/work/e/eantipov/public/tt_jets_analyses/tt_hf/results/TMVA_pl_" + generator + lep_pt_cut_suffix + ".root";
  TFile *outputFile = new TFile(out_fname, "RECREATE");


  // Create factory and dataloader
  TMVA::Factory *factory = new TMVA::Factory("MVAnalysis", outputFile, "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");
  TMVA::DataLoader *dataloader = new TMVA::DataLoader("dataset");
  std::cout << "==> Created a Factory and a DataLoader\n" << std::endl;

  
  // Open an input file
  TString in_fname = "/afs/cern.ch/work/e/eantipov/public/tt_jets_analyses/tt_hf/results/tt_hf_MVA_input_pl_" + generator + lep_pt_cut_suffix + ".root";
  TFile *input = TFile::Open(in_fname);
  std::cout << "==> Opened an input file\n" << std::endl;


  // Add varialbes to the dataloader
  dataloader->AddVariable("dR_jet_lep0", 'F');
  dataloader->AddVariable("dR_jet_lep1", 'F');
  dataloader->AddVariable("min_dR_jet_lep", 'F');
  dataloader->AddVariable("m_jet_el", 'F');
  dataloader->AddVariable("m_jet_mu", 'F');
  std::cout << "==> Added variables\n" << std::endl;


  // Add one spectator
  dataloader->AddSpectator("jet_GBHInit_topHadronOriginFlag", 'I');
  std::cout << "==> Added a spectator\n" << std::endl;

  
  // Add weights
  dataloader->SetSignalWeightExpression("tot_event_weight");
  dataloader->SetBackgroundWeightExpression("tot_event_weight");
  std::cout << "==> Added weights\n" << std::endl;


  // Initialize trees
  dataloader->AddSignalTree( (TTree*)input->Get("Signal"), 1.0 );
  dataloader->AddBackgroundTree( (TTree*)input->Get("Background"), 1.0);
  std::cout << "==> Loaded Signal and Bkgd trees\n" << std::endl;
  dataloader->PrepareTrainingAndTestTree("", "SplitMode=random:!V");
  std::cout << "==> Prepared the trees\n" << std::endl;


  // Book MVA method (I'm using BDT; MLP is an alternative; other options can be added in a similar way)
  if (tmva_config_info[0] == "BDT") factory->BookMethod(dataloader, TMVA::Types::kBDT, tmva_config_info[0], tmva_config_info[1]);
  if (tmva_config_info[1] == "MLP") factory->BookMethod(dataloader, TMVA::Types::kMLP, tmva_config_info[0], tmva_config_info[1]);
  std::cout << "==> Booked a specific BDT method\n" << std::endl;


  // Train, test and evaluate
  factory->TrainAllMethods();
  std::cout << "==> Trained\n" << std::endl;
  factory->TestAllMethods();
  std::cout << "==> Tested\n" << std::endl;
  factory->EvaluateAllMethods();
  std::cout << "==> Evaluated\n" << std::endl;

  std::cout << "==> Wrote root file: " << outputFile->GetName() << "\n" << std::endl;
  std::cout << "==> TMVAClassification is done!" << std::endl;

  outputFile->Close();
  delete factory;
  delete dataloader;


  //Save a copy of the dataset
  TString exec_str = "cp -r dataset results/dataset_" + generator + lep_pt_cut_suffix;
  gSystem->Exec(exec_str);


  return 0;
}

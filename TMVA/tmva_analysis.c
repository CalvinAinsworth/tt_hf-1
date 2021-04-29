// reference: https://github.com/root-project/root/blob/bec9f2d1f778771703f0006a32436e5e23180c00/tutorials/tmva/TMVAClassification.C

#include <TFile.h>
#include <TTree.h>
#include <map>
#include <iostream>
#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

void tmva_analysis()
{
  // Create TStrings for TMVA method options and name
  TString method_options = "!H:!V";
  TString method_name = "";
  ifstream config_file("tmva_config.txt", ios::binary);
  if (config_file.is_open()) {
    string str1;
    string delim = ": ";
    while (getline(config_file, str1)) {
      string par_name = str1.substr(0, str1.find(delim));
      string par_val  = str1.substr(str1.find(delim)+2, str1.size()); // +2 due to delim length
      string addition = ":" + par_name + "=" + par_val;
      if (par_name != "TMVA Method") { method_options += addition; }
      else { method_name += par_val; } }
  }
  config_file.close();


  // Create an output file
  TFile *outputFile = new TFile("TMVA.root", "RECREATE");
  cout << "==> Created an output file" << endl;
  

  // Create Factory
  TMVA::Factory *factory = new TMVA::Factory("MVAnalysis", outputFile, "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");
  TMVA::DataLoader *dataloader = new TMVA::DataLoader("dataset");
  cout << "==> Created a Factory and a DataLoader" << endl;
  
  
  // Open the input file
  TFile *input = TFile::Open("tt_jets_NN_input.root");
  cout << "==> Opened an input file" << endl;
  

  // Add variables
  dataloader->AddVariable("dR_jet_lep0", 'F');
  dataloader->AddVariable("dR_jet_lep1", 'F');
  dataloader->AddVariable("min_dR_jet_lep", 'F');
  //dataloader->AddVariable("m_jet_lep_min_dR", 'F');
  dataloader->AddVariable("m_jet_el", 'F');
  dataloader->AddVariable("m_jet_mu", 'F');
  dataloader->AddVariable("m_jet_lep_max", 'F');
  //dataloader->AddVariable("m_jet_lep_min", 'F');
  dataloader->AddVariable("min_dR_jet_bjet", 'F');
  cout << "==> Added variables" << endl;


  // Add spectators
  dataloader->AddSpectator("topHadronOriginFlag", 'I');
  dataloader->AddSpectator("jet_isbtagged_DL1r_77", 'I');
  dataloader->AddSpectator("jet_truthflav", 'I');
  dataloader->AddSpectator("jet_DL1r", "F");
  dataloader->AddSpectator("event_number", "I");
  dataloader->AddSpectator("tot_event_weight", "F");
  dataloader->AddSpectator("met", "F");
  dataloader->AddSpectator("jet_pt", "F");
  dataloader->AddSpectator("jet_eta", "F");
  dataloader->AddSpectator("jet_phi", "F");
  dataloader->AddSpectator("jet_e", "F");
  dataloader->AddSpectator("mu_pt", "F");
  dataloader->AddSpectator("mu_eta", "F");
  dataloader->AddSpectator("mu_phi", "F");
  dataloader->AddSpectator("mu_e", "F");
  dataloader->AddSpectator("mu_charge", "F");
  dataloader->AddSpectator("el_pt", "F");
  dataloader->AddSpectator("el_eta", "F");
  dataloader->AddSpectator("el_phi", "F");
  dataloader->AddSpectator("el_e", "F");
  dataloader->AddSpectator("el_charge", "F");  
  cout << "==> Added spectators" << endl;


  // Add weights
  dataloader->SetSignalWeightExpression("tot_event_weight");
  dataloader->SetBackgroundWeightExpression("tot_event_weight");
  cout << "==> Added weights" << endl;


  // Initialize trees
  dataloader->AddSignalTree( (TTree*)input->Get("signal"), 1.0 );
  dataloader->AddBackgroundTree( (TTree*)input->Get("background"), 1.0);
  cout << "==> Loaded Signal and Bkgd trees" << endl;
  dataloader->PrepareTrainingAndTestTree("", "SplitMode=random:!V");
  cout << "==> Prepared the trees" << endl;
  

  // Book MVA method
  if (method_name == "BDT") factory->BookMethod(dataloader, TMVA::Types::kBDT, method_name, method_options);
  if (method_name == "MLP") factory->BookMethod(dataloader, TMVA::Types::kMLP, method_name, method_options);
  cout << "==> Booked a specific BDT method" << endl;
  

  // Train, test and evaluate
  factory->TrainAllMethods();
  cout << "==> Trained" << endl;
  factory->TestAllMethods();
  cout << "==> Tested" << endl;
  factory->EvaluateAllMethods();
  cout << "==> Evaluated" << endl;

  cout << "==> Wrote root file: " << outputFile->GetName() << endl;
  cout << "==> TMVAClassification is done!" << endl;
  
  outputFile->Close();
  delete factory;
  delete dataloader;

}

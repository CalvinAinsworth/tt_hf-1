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
  // Create TString for TMVA method options
  TString method_options = "!H:!V";
  TString method_name = "";
  ifstream config_file("tmva_config.txt", ifstream::binary);
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
  
  
  // Default MVA methods to be trained + tested
  map<string, bool> Use;
  Use["BDT"] = false;
  Use["BDTG"] = false;
  Use["BDTB"] = false;
  Use["BDTD"] = false;
  Use["BDTF"] = false;


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
  dataloader->AddVariable("NN_dR_jet_lep0", 'F');
  dataloader->AddVariable("NN_dR_jet_lep1", 'F');
  dataloader->AddVariable("NN_min_dR_jet_lep", 'F');
  //dataloader->AddVariable("NN_m_jet_lep_min_dR", 'F');
  dataloader->AddVariable("NN_m_jet_el", 'F');
  dataloader->AddVariable("NN_m_jet_mu", 'F');
  dataloader->AddVariable("NN_m_jet_lep_max", 'F');
  //dataloader->AddVariable("NN_m_jet_lep_min", 'F');
  dataloader->AddVariable("NN_min_dR_jet_bjet", 'F');
  cout << "==> Added variables" << endl;


  // Add spectators
  dataloader->AddSpectator("topHadronOriginFlag", 'I');
  dataloader->AddSpectator("jet_isbtagged_DL1r_77", 'I');
  dataloader->AddSpectator("NN_jet_DL1r", "F");
  dataloader->AddSpectator("NN_event_number", "I");
  dataloader->AddSpectator("NN_tot_event_weight", "F");
  dataloader->AddSpectator("NN_met", "F");
  dataloader->AddSpectator("NN_jet_pt", "F");
  dataloader->AddSpectator("NN_jet_eta", "F");
  dataloader->AddSpectator("NN_jet_phi", "F");
  dataloader->AddSpectator("NN_jet_e", "F");
  dataloader->AddSpectator("NN_mu_pt", "F");
  dataloader->AddSpectator("NN_mu_eta", "F");
  dataloader->AddSpectator("NN_mu_phi", "F");
  dataloader->AddSpectator("NN_mu_e", "F");
  dataloader->AddSpectator("NN_mu_charge", "F");
  dataloader->AddSpectator("NN_el_pt", "F");
  dataloader->AddSpectator("NN_el_eta", "F");
  dataloader->AddSpectator("NN_el_phi", "F");
  dataloader->AddSpectator("NN_el_e", "F");
  dataloader->AddSpectator("NN_el_charge", "F");
  
  cout << "==> Added spectators" << endl;


  // Add weights
  dataloader->SetSignalWeightExpression("NN_tot_event_weight");
  dataloader->SetBackgroundWeightExpression("NN_tot_event_weight");
  cout << "==> Added weights" << endl;


  // Initialize trees
  dataloader->AddSignalTree( (TTree*)input->Get("NN_signal"), 1.0 );
  dataloader->AddBackgroundTree( (TTree*)input->Get("NN_background"), 1.0);
  cout << "==> Loaded Signal and Bkgd trees" << endl;
  dataloader->PrepareTrainingAndTestTree("", "SplitMode=random:!V");
  cout << "==> Prepared the trees" << endl;
  

  // Book MVA methods
  // There are different BDTs (Boosted Decision Trees)
  /*  
  if (Use["BDTG"]==true) // Gradient Boost
    factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDTG", 
			"!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=2");

  if (Use["BDT"]==true) // Adaptive Boost
    factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDT",
			"!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20");
  
  if (Use["BDTB"]==true) // Bagging
    factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDTB",
			"!H:!V:NTrees=400:BoostType=Bagging:SeparationType=GiniIndex:nCuts=20");

  if (Use["BDTD"]==true) // Decorrelation + Adaptive Boost
    factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDTD",
			"!H:!V:NTrees=400:MinNodeSize=5%:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:VarTransform=Decorrelate");

  if (Use["BDTF"]==true) // Allow using Fisher discriminant in mode splitting for (strong) linearly correlated variables
    factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDTF",
			"!H:!V:NTrees=50:MinNodeSize=2.5%:UseFisherCuts:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=20");
  */
  // Testing BDT setup
  factory->BookMethod(dataloader, TMVA::Types::kBDT, "KBDT", method_options);

  //factory->BookMethod( dataloader, TMVA::Types::kMLP, "MLP", 
  //		       "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:!UseRegulator" );

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

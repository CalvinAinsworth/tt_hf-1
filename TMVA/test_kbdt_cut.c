#include <TTree.h>
#include <TFile.h>


///////////////////
//     MAIN      //
//////////////////
void test_kbdt_cut()
{
  // Open the tfile - mtva ouput
  TFile *tmva_file = new TFile("TMVA.root");

  
  // Open the test tree
  TTree *test_tree = (TTree*)tmva_file->Get("dataset/TestTree");
  
  
  // Set branches
  float kbdt_val, weight, topHOF;
  kbdt_val = weight = topHOF = 0;  

  test_tree->SetBranchAddress("topHadronOriginFlag", &topHOF);
  test_tree->SetBranchAddress("weight", &weight);
  test_tree->SetBranchAddress("KBDT", &kbdt_val);


  // Get number of entries
  Int_t nEntries = test_tree->GetEntries();


  // Set an array of KBDT cuts to test and an array to store efficiency
  vector<double> kbdt_cut =     {};
  vector<double> n_match =      {};
  vector<double> n_miss_match = {};
  vector<double> eff =          {};

  vector<double> purity   = {};
  vector<double> purity_a = {};
  vector<double> purity_b = {};


  // Set some kbdt cuts
  for (int i=0; i<8; i++) {
    kbdt_cut.push_back(i*0.1); }

  // Loop over the entries
  for (int entry=0; entry<nEntries; entry++) {
    
    if (entry%1000==0) { cout << "\t" << entry << "\r"; cout.flush(); }
    test_tree->GetEntry(entry);
    
    for (int cut_i=0; cut_i<kbdt_cut.size(); cut_i++) {
      
      if (entry==0) {
	n_match.push_back(0);
	n_miss_match.push_back(0); 
	purity_a.push_back(0);
	purity_b.push_back(0); }
      
      if (topHOF!=4) {
	if ( kbdt_val>kbdt_cut[cut_i] ) { n_match[cut_i] += weight; }
	else { n_miss_match[cut_i] += weight; } }

      if (kbdt_val>kbdt_cut[cut_i]) {
	if (topHOF!=4) { purity_a[cut_i] += weight; }
	else { purity_b[cut_i] += weight; } }
    } // [cut] - loop over given kbdt cut values under the study
    
  } // [entry] - loop over entries


  // Compute the discriminative power / efficiency
  for (int cut_i=0; cut_i<kbdt_cut.size(); cut_i++) {
    double eff_tmp = n_match[cut_i]/(n_match[cut_i] + n_miss_match[cut_i]);
    eff.push_back(eff_tmp);
    cout << "KBDT cut = " << kbdt_cut[cut_i] << "  \tEff. = " << eff_tmp << "  \tN match = " << n_match[cut_i] << " \tN missmatch = " << n_miss_match[cut_i] << endl; }

  for (int cut_i=0; cut_i<kbdt_cut.size(); cut_i++) {
    double tmp_purity = purity_a[cut_i] / (purity_a[cut_i] + purity_b[cut_i]);
    purity.push_back(tmp_purity);
    cout << "KBDT cut = " << kbdt_cut[cut_i] << "  \tPurity = " << tmp_purity << endl; }

}

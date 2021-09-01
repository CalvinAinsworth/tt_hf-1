#include "common_functions.h"



// ##################################
// ## Split string into components ##
// ##################################
std::vector<TString> split(TString split_string, char delimiter)
{ 
  std::stringstream ss;
  ss << split_string;
  std::string component;
  
  std::vector<TString> container;
  while(getline(ss, component, delimiter))
    { 
      container.push_back(component);
    }
  
  return container;
}



// #################################################
// ## Make a list of files in the given directory ##
// #################################################
std::vector<TString> get_list_of_files(TString dirname, std::vector<TString> container)
{ 
  TSystemDirectory dir(dirname, dirname);
  TList *files = dir.GetListOfFiles();
  if (files) {
    TSystemFile *file;
    TString fname;
    TIter next(files);
    while ((file=(TSystemFile*)next())) {
      fname = file->GetName();
      if (fname != "." && fname != "..") {
        if (!(file->IsDirectory())) { container.push_back(dirname + fname); }
        //if (fname.EndsWith(".root")) { container.push_back(dirname + fname); }
        else { container.push_back(dirname + fname + "/"); }
      }
    }
  }
  return container;
}



// #################################################################
// ## Analyze the tmva config file to get method name and options ##
// #################################################################
std::vector<TString> get_tmva_config_info(TString config_file_name)
{
  std::vector<TString> container = {};

  TString method_options = "!H:!V";
  TString method_name = "";
  std::ifstream config_file(config_file_name, std::ios::binary);
  if (config_file.is_open()) {
    std::string str1;
    std::string delim = ": ";
    while (getline(config_file, str1)) {
      std::string par_name = str1.substr(0, str1.find(delim));
      std::string par_val  = str1.substr(str1.find(delim)+2, str1.size()); // +2 due to delim length
      std::string addition = ":" + par_name + "=" + par_val;
      if (par_name != "TMVA Method") { method_options += addition; }
      else { method_name += par_val; } }
  }
  config_file.close();

  container.push_back(method_name);
  container.push_back(method_options);

  return container;
}

#include "common_headers.h"

std::string ttbar_generator();

bool mc16a_only_choise();

std::vector<TString> split(TString split_string, char delimiter);

std::vector<TString> get_list_of_files(TString dirname, std::vector<TString> container = {});

std::vector<TString> get_tmva_config_info(TString config_file_name);

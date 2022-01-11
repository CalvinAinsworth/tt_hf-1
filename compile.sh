#mkdir run

echo "Compiling prepare_hists_mc.cpp"
gcc source/pre_mva_studies/prepare_hists_mc.cpp   source/common_functions.cpp -o run/prepare_hists_mc   -lstdc++ `root-config --cflags --glibs`

echo "Compiling prepare_hists_mc_pl.cpp"
gcc source/pre_mva_studies/prepare_hists_mc_pl.cpp source/common_functions.cpp -o run/prepare_hists_mc_pl -lstdc++ `root-config --cflags --glibs`

echo "Compiling prepare_hists_data.cpp"
gcc source/pre_mva_studies/prepare_hists_data.cpp source/common_functions.cpp -o run/prepare_hists_data -lstdc++ `root-config --cflags --glibs`

echo "Compiling draw_data_mc.cpp"
gcc source/pre_mva_studies/draw_data_mc.cpp source/common_functions.cpp source/draw_functions.cpp -o run/draw_data_mc -lstdc++ `root-config --cflags --glibs`

echo "Compiling draw_mc.cpp"
gcc source/pre_mva_studies/draw_mc.cpp source/common_functions.cpp source/draw_functions.cpp -o run/draw_mc -lstdc++ `root-config --cflags --glibs`

echo "Compiling tmva_analysis.cpp"
gcc source/tmva_studies/tmva_analysis.cpp source/common_functions.cpp -o run/tmva_analysis -lstdc++ `root-config --cflags --glibs` -lTMVA

echo "Compiling draw_tmva_plots.cpp"
gcc source/tmva_studies/draw_tmva_plots.cpp source/common_functions.cpp source/draw_functions.cpp -o run/draw_tmva_plots -lstdc++ `root-config --cflags --glibs`

echo "Compiling study_tmva_perpormnce_results.cpp"
gcc source/tmva_studies/study_tmva_perpormnce_results.cpp source/common_functions.cpp source/draw_functions.cpp -o run/study_tmva_perpormnce_results -lstdc++ `root-config --cflags --glibs` -lTMVA

echo "Compiling optimize_classifier_cut.cpp"
gcc source/tmva_studies/optimize_classifier_cut.cpp source/common_functions.cpp source/draw_functions.cpp -o run/optimize_classifier_cut -lstdc++ `root-config --cflags --glibs` -lTMVA

echo "Compiling bjets_from_tops_stat.cpp"
gcc source/tmva_studies/bjets_from_tops_stat.cpp source/common_functions.cpp source/draw_functions.cpp -o run/bjets_from_tops_stat -lstdc++ `root-config --cflags --glibs` -lTMVA

echo "Compiling btags_from_tops_data.cpp"
gcc source/tmva_studies/btags_from_tops_data.cpp source/common_functions.cpp source/draw_functions.cpp -o run/btags_from_tops_data -lstdc++ `root-config --cflags --glibs` -lTMVA

echo "Compiling draw_from_top_stats.cpp"
gcc source/tmva_studies/draw_from_top_stats.cpp source/common_functions.cpp source/draw_functions.cpp -o run/draw_from_top_stats -lstdc++ `root-config --cflags --glibs` -lTMVA

echo "Compiling prepare_mc_samples.cpp"
gcc source/post_mva/prepare_mc_samples.cpp source/common_functions.cpp -o run/prepare_mc_samples -lstdc++ `root-config --cflags --glibs` -lTMVA

echo "Compiling prepare_data_samples.cpp"
gcc source/post_mva/prepare_data_samples.cpp source/common_functions.cpp -o run/prepare_data_samples -lstdc++ `root-config --cflags --glibs` -lTMVA

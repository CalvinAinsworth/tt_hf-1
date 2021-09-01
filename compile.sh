mkdir run

echo "Compiling prepare_hists_mc.cpp"
gcc source/pre_mva_studies/prepare_hists_mc.cpp   source/common_functions.cpp -o run/prepare_hists_mc   -lstdc++ `root-config --cflags --glibs`

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

echo "Compiling prepare_bkg_samples.cpp"
gcc source/post_mva/prepare_bkg_samples.cpp source/common_functions.cpp -o run/prepare_bkg_samples -lstdc++ `root-config --cflags --glibs` -lTMVA

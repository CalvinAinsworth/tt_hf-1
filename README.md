# tt+HF analysis macros
This repository contains some useful macros for the tt+HF OSU analysis. To be run at lxplus.

## Get the repo
```bash
git clone --recursive https://github.com/egorantipov/tt_hf.git
```

## Build KLFitter
Although KLFitter hasn't been implemented yet, it is requred to build it since `prepare_hists_mc.c` macro calls some KLFitter methods. This step is needed to be done just once.
```bash
cd KLFitter
mkdir build
cd build
cmake3 -DBUILTIN_BAT=TRUE ..
make -j
```

## Setup the environment
The environment uses root version suitable for KLFitter. To run TMVA analysis macros the default lxplus `ROOT Version: 6.22/08` is needed (or 6.20.06 that is available via lsetup root).
```bash
source setup.sh
```

## Prepare a histfile
KLFitter implementation is a work in progress. Run the two commands to prepare hists files for mc and data:
```bash
root -l -b load_klf.C+
root -l -b prepare_hists_data.c+
```
The first command is set to load KLFitter libraries and run `prepare_hists_mc.c` macro. The macro prepares `hists_mc.root` and `TMVA/tt_jets_NN_input.root` ntuples. The first ntuple is further used for whatever hists studies are needed: data/mc comparison, DL1r templates studes etc. 
The second command runs a macro that prepares `hists_data.root` ntuple that is used for data/mc studies. 
Both mc and data `prepare_hists*` must create same set of hists and save them with exactly the same names.

## Draw histograms
To draw mc studies histograms prepared by the `prepare_histograms.c` run:
```bash
root -l -b draw_hists.c
```
To draw data/mc comparison run:
```bash
root -l -b draw_data_mc.c
```
The optut of the macros will be stored in the `Plots/` folder.


# TMVA analysis

## Run TMVA analysis
An input file named `tt_jets_NN_input.root` is needed. The ntupe is prepared by `load_klf.c` macro. The ntuple consists of jets as separate entries rather then events (collections of jets). One can set MVA method's parameters with the `tmva_config.txt` file (example for BDT is used here). To run the MVA analysis:
```bash
root -l -b tmva_analysis.c
```
AN output file named `TMVA.root` will be created. The file is further used by other macros. To change from BDT to other MVA method change the `tmva_analysis.c` macro and `tmca_config.txt` config accordingly.

## Draw plots from the MVA output
Run the following macro:
```bash
root -l -b draw_tmva_plots.c
```
If other than BDT method used, the macro must be changed accordingly.

## Optimize the MVA classiffier cut:
```bach
root -l -b optimize_kbdt_cut.c
root -l -b study_tmva_perpormnce_results.c
```
Analyze the resulting plots and select the best MVA method cut with respect to the classifier. If other than BDT method is used, the macros must be changed accorgingly.

## Preape a sample with the classifier value for each jet
Since the MVa analysis takes separate jets as inputs, one needs to recollct back the jets into events. To prepare such an ntuple, run:
```bash
create_skimmed_ntuple.c
```
The macro gathers jets takes the `TMVA.root` ntuple as an input, gathers jets from event inco collections=events and also collects other information, such as MET, leptons info, total event weight. If more branches are needed, they must be added to `tt_jets_NN_input.root` ntuple at the `root -l -b load_klf.c+` porduction (in the `prepare_hists_mc.c` macro), and then added as spectators in the `tmva_analysis.c` macro.

## Study DL1r tag weight with the new cut in MVA classifier
Simply run:
```bash
prepare_hists_kbdt.c
```
If method other than BDT is used, the macro must be changed accordingly.


# General Status

All the macros are "work in progress", thus will be optimized. Eventually there framework will be as a collection of macros with small/no changes needed and config file that sets all needed (1) data/mc hists, (2) mc studies hists, (3) MVA analysis method and parameters.
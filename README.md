# tt+HF analysis macros
This repository contains some useful macros for the tt+HF OSU analysis. To be run at lxplus.

## Get the repo
```bash
git clone --recursive https://github.com/egorantipov/tt_hf.git
```

## Build KLFitter
Although KLFitter hasn't been implemented yet: all methods in macros are commented out. However it's a good practice to build external package so they'll work once KLFitter will be implemented properly. The following procedure should be done just once:
```bash
cd KLFitter
mkdir build
cd build
cmake3 -DBUILTIN_BAT=TRUE ..
make -j
```

## Setup the environment
Since KLFitter is not implemented yet, skip the environment setup step. 
```bash
source setup.sh
```

## Prepare a histfile
Run the two commands to prepare hists files for mc and data:
```bash
root -l -b -q prepare_hists_mc.c+
root -l -b -q prepare_hists_data.c+
```
Both mc and data `prepare_hists*` must create the same set of hists and save them with exactly the same names.

## Draw histograms
To draw mc studies histograms prepared by the `prepare_histograms.c` run:
```bash
root -l -b -q draw_hists.c
```
To draw data/mc comparison run:
```bash
root -l -b -q draw_data_mc.c
```
The optut of the macros will be stored in the `Plots/mc_studies/` `Plots/data_mc_comparison/` folders that must be created in advance.


# TMVA analysis

## Run TMVA analysis
An input file named `tt_jets_NN_input.root` is needed. The ntupe is prepared by `prepare_hists_mc.c` macro. The ntuple consists of jets as separate entries rather than regular events. One must set MVA method's parameters in the `tmva_config.txt` file (example for BDT is used here). Note there mustn't be any blanck lines, and the structude of each line must follow the regular pattern: `Parameter: value`. All further macros use TMVA method name and parameters values from the config file. So far the `tmva_analysis.c` macro can distinguish BDT amd MLP methods; updates are coming. To run the MVA analysis run:
```bash
root -l -b -q tmva_analysis.c
```
An output file `TMVA.root` will be created. The file is further used by other macros.

## Draw plots from the MVA output
Run the following macro:
```bash
root -l -b -q draw_tmva_plots.c
```
Plots will be stored in `Plots/` folder that must be created in advance.

## Optimize the MVA classiffier cut:
```bach
root -l -b -q optimize_claasifier_cut.c
root -l -b -q study_tmva_perpormnce_results.c
```
Analyze the resulting plots and select the best MVA method cut with respect to the classifier.

## Preape a sample with the classifier value for each jet
Since the MVA analysis takes separate jets as inputs, one needs to recollect the jets into events. So far there are two macros, one written in c++ ROOT and other in pyROOT. The first is not optimized yet, use the latter:
```bash
create_skimmed_ntuple.py
```
The macro takes the `TMVA.root` ntuple as an input, gathers jets from each event into collections=events and also adds other information, such as MET, leptons info, total event weight. If more branches are needed, they must be added to `tt_jets_NN_input.root` ntuple using `prepare_hists_mc.c` macro, and then added as spectators in the `tmva_analysis.c` macro.

## Study DL1r tag weight with the new cut in MVA classifier
The study is done for a few cuts in the classifier value. The macro takes the full range of classifier value, and sets N cuts at the equal step whithin the range. To set the number of cuts change `n_steps` in `prepare_hists_classifier.c` macto (L18). 
```bash
prepare_hists_classifier.c
```


## Analyze Classifier Performance on the event level
The idea is to deffirintiate events with both jets originating from top identified by the classifier correctly from the events where at least of the jets was miss-identified. The algorythm is as following: 
(1) Sort jets with respect to the classifier value from the lowest to the highest. For BDT and MLP is was shown that jets originating fro top quarks decay has lower classifier value than jets not from top. 
(2) Also we expect both jets originating from top to be b-jets. 
(3) Thus we analyze only two candidates with the following parameters: `classifier < cut vaue` AND `jet_isbtagged_DL1r_77 = 1`. There are a few possible scenarios:
   (a) The classifier cut is too low, and there is less than 2 candidates: such an event is considered as bad.
   (b) There is exactly two candidates, and one of them is actually not frop top - such an event is considered as bad.
   (c) There is exactly two candidates, and both of them are from top - good event.
   (d) Events with more than two candidates are not considered (reffer to (1) ).
(4) For each point in the classifier cuts range (by defauls there are 20 points along the entire classifier range) event level efficiency is computed as the ratio of good event to the total number of events. 
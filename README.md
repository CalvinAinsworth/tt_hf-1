Setup
```bash
source setup.src
```

Compile:
```bash
./compile
```

Prepare data hists (y15-16 enabled for testing, uncomment if need all Run2):
``bash
./run/prepare_hists_data
```

Prepare mc hists (mc16a only enabled for testing, uncomment if need all Run2):
```bash
./run/prepare_hists_mc
```
(see the option the code returns and add needed)

Run TMVA:
```bash
./run/tmva_analysis
```

Other codes work, but need to readjust input files names
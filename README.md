# DFT-1-2

This python script generates the corrected POTCAR for Quasiparticle Correction by applying the DFT-1/2 approach.

## 1. Python Setup

To get this script up and running on your available computational resources, make sure to have the below libraries installed on Python 3.6 or newer.

```
1. yaml, os, shutil
2. ase 
```

## 2. Input files
All the input files for the system under study must be in the folder where you run the script. The mandatory folders and files are:

```
1. atm_cGuima: Program to pseudopotential generation.
2. program_m05: Program to POTCAR generation corrected by the DFT-1/2 approach.
3. potpaw_PBE.54: Folder with all original POTCAR provided by VASP code (for legal reason we will keep only a few of them). 
4. rendered_wano.yml: File with all the inputs we need to run the script. At this point, only the chemical species are required.

## 3. Running the scripts and output files

By running `python dft_half.py,` you will generate all the output files for the system under study, which are in the `x_potcar` directory, where `x` is the chemical species. In this folder,  you will find the `POTCARcut_y,` where `_y` is the value of the cut parameter.

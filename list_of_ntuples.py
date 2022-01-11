import sys
from os import walk

mypath = "/eos/atlas/atlascerngroupdisk/phys-top/ttjets/v4/"
f_out = open("ntuples_list.txt", "a")

for (dirpath, dirnames, filenames) in walk(mypath):
  if len(filenames)!=0:
    for filename in filenames:
      good_file = True
      for arg in (sys.argv)[1:]:
        if arg[0]!="X" and arg not in (dirpath+filename):
          good_file = False
        if arg[0]=="X" and arg[1:] in (dirpath+filename):
          good_file = False
      if good_file==True:
        print("{}/{}".format(dirpath, filename))
        f_out.write("{}/{}\n".format(dirpath, filename))

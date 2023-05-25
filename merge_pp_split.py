import os
import yaml

file_counter = 1
for i in range(30):
    tot = 12
    if i > 23:
        tot = 11
    #print(f"{i} -> {tot}")
    f = 0
    files = []
    while f < tot:
        if os.path.isfile(f"AnalysisResults-{file_counter}.root"):
            files.append(file_counter)
            f = f + 1
        file_counter = file_counter + 1
    #print(files)

    hadd_string = f"hadd AnalysisResults_merge_{i}.root"
    for f in files:
        hadd_string = hadd_string + f" AnalysisResults-{f}.root"
    #print(hadd_string)

    os.system(hadd_string)

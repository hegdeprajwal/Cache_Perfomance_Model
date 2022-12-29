import subprocess
import re
import json
import sys
import matplotlib.pyplot as plt

cache_block_list = [ 8 , 16, 32, 64, 128 ]
cache_sizes= [1024, 1024>>1, 1024>>2, 1024>>3, 1024>>4, 1024>>5]
assoc = [1, 2, 4, 8, 16]

l2_cache_size = 1024
l2_assoc = 8

json_data = []

plot = 0
verbose = 1

for cache_blk in cache_block_list : 
    for cache_size in cache_sizes :
        for ass in assoc : 
            # Run the C++ program with the given arguments and capture its output
            result = subprocess.run(["./run", str(cache_blk), str(cache_size), str(ass), "gcc_trace.txt", str(l2_cache_size), str(l2_assoc)],
                                    stdout=subprocess.PIPE, stderr=subprocess.PIPE)

            if result.returncode == 0 :
                config = {
                    "blk_size" : cache_blk, 
                    "l1_size": cache_size, 
                    "l1_assoc": ass , 
                    "l2_size" : l2_cache_size,
                    "l2_assoc": l2_assoc
                }
                
                # Convert the output to a string 
                output = result.stdout.decode()
                #print("For config blk_size = {} cache_size = {} cache assoc = {} ".format(cache_blk, cache_size, ass))
                
                out = re.findall(r"(\w+)\s*:\s*(\d+(?:\.\d+)?).*", output)
                items = {k: v for k, v in out}
                config.update(items)
                json_data.append(config)

                # Split the output of the program into a list of lines
                if verbose : 
                    lines = output.split('\n')
                    for line in lines:
                        print(line)

if json_data : 
    with open("output.json", "w") as f:
    # Use the json.dump() function to write the dictionary to the file
        data = json.dumps(json_data, indent=2, sort_keys=True)
        f.write(data)

if plot  : 
    x = []
    y = []
    for data in json_data : 
        if data.get("l1_size") == 1024 : 
            y.append(data["l1_Miss_Rate"])
            x.append(data["l1_assoc"])

    plt.scatter(x, y) 
    plt.show()

                    





        
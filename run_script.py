import subprocess

cache_block_list = [ 8 , 16, 32, 64, 128 ]
cache_sizes= [1024, 1024>>1, 1024>>2, 1024>>3, 1024>>4, 1024>>5]
assoc = [1, 2, 4, 8, 16]

l2_cache_size = 1024
l2_assoc = 8

for cache_blk in cache_block_list : 
    for cache_size in cache_sizes :
        for ass in assoc : 
            # Run the C++ program with the given arguments and capture its output
            result = subprocess.run(["./run", str(cache_blk), str(cache_size), str(ass), "gcc_trace.txt", str(l2_cache_size), str(l2_assoc)],
                                    stdout=subprocess.PIPE, stderr=subprocess.PIPE)



            if result.returncode == 0 :
                
                # Convert the output to a string 
                output = result.stdout.decode()
                print("For config blk_size = {} cache_size = {} cache assoc = {} ".format(cache_blk, cache_size, ass))
                
                # Split the output of the program into a list of lines
                lines = output.split('\n')
                for line in lines:
                    print(line)


        
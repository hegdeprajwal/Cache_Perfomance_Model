#include "cache.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
 
using namespace std;

int main ( int argc, char *argv[]) {

    //cache 64bytes block, 4k 
    fstream newfile;
    int L2_enable = 0;
    int	L2_cache_size;
    int	L2_cache_sssoc;
    int blockSize = 0;
    int	L1_cache_size = 0;	
    int	L1_cache_sssoc = 0;
    char* file_name;
    if ( argc >= 5 ) {
        blockSize = atoi(argv[1]);	
        L1_cache_size = atoi(argv[2]);		
        L1_cache_sssoc = atoi(argv[3]);
        file_name = argv[4];
        if (argc >= 6) {
            L2_enable = 1;
            L2_cache_size = atoi(argv[5]);	
            L2_cache_sssoc = atoi(argv[6]);
        }
    }
    else {
        perror ("Less arguments check the arguments");
    }

    if ( (L1_cache_sssoc * blockSize) > L1_cache_size) 
        return -1;

    Cache L1(blockSize, L1_cache_size, L1_cache_sssoc, L2_enable);
    L2Cache L2(blockSize, L2_cache_size, L2_cache_sssoc, 0);

    newfile.open(file_name, ios::in);
    if (!newfile.good()) {
        // file was opened successfully
        perror("File could not be opened \n");
    }
    if (newfile.is_open()) {
        string tp;
        while (getline(newfile, tp)) {
            vector <string> tokens;
            istringstream iss(tp);
            string token;
            const char* command;
            unsigned long long address;

            while ( getline(iss,token , ' ')) {
                tokens.push_back(token);
            } 

            if (tokens.size() == 2 ) {
                command = tokens[0].c_str();
                address = std::stoul(tokens[1], nullptr, 16);    
                L1.cache_access(command, address, &L2);
            }
            else {
                perror ("Invalid input\n");
            }

        }

    }

    L1.print_stats();
    cout << "\n\n L2 Stats " << endl ;
    if(L2_enable) {
        L2.print_stats();        
    }

    return 0;
}

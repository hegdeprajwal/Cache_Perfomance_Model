
#include "cache.h"
#include "cmath"
#include <iostream>
using namespace std;


Cache::Cache(int blk_size, int cache_size, int assoc, int next_enable) {
    this->block_size = blk_size;
    this->cache_size = cache_size;
    this->cache_assoc = assoc;
    this->next_cache_enable = next_enable;

    //find number of ways and sets
    this->num_sets = (cache_size/(block_size*cache_assoc));
    this->num_ways = cache_assoc;

    //find the number of bits for index, offset and tag
    this->offset_bits = log2 (block_size);
    this->index_bits = log2(num_sets);
    this->tag_bits = ADDRESS_SPACE - index_bits - offset_bits;

    //initialize the cache contents
    this->cache = (cache_line_t**) malloc(this->num_sets * sizeof(cache_line_t*));
    this->cache_lru = new int*[num_sets];
    for (int i = 0 ; i < num_sets; i++) {
        this->cache[i] = (cache_line_t*) malloc ( this->num_ways * sizeof(cache_line_t));
        this->cache_lru[i] = new int[this->num_ways];
    }

    //init()
    //set all the content of the cache to invalid
    //intialize the lru in increasing order
    for ( int i = 0 ; i < this->num_sets ; i++) {
        int k = 0 ;
        for (int j = 0 ; j < this->num_ways ; j++) {
            this->cache[i][j].valid = false;
            this->cache_lru[i][j] = k++;
        }
    }   

    //intialize cache stats
    this->cache_stats.reads = 0; 
    this->cache_stats.writes = 0;
    this->cache_stats.miss_rate=0;
    this->cache_stats.read_hits=0;
    this->cache_stats.read_miss=0;
    this->cache_stats.swaps=0;
    this->cache_stats.write_hits=0;
    this->cache_stats.write_miss=0;
    this->cache_stats.write_back = 0;
}

void Cache::cache_access(const char* access, unsigned long long address, L2Cache* L2) {
    offset_mask = (1<< this->offset_bits) -1; 
    index_mask = ((1<< this->index_bits) -1) << this->offset_bits;

    offset = address & offset_mask;
    index = (address & index_mask) >> this->offset_bits;
    tag = address >> (this->offset_bits+this->index_bits);

    // search for the cache set
    if ( *access == 'r') {
        this->cache_stats.reads++;
        int found = 0;
        int way_found = 0;
        for ( int i =0 ; i < this->num_ways ; i++) {

            if ( this->cache[index][i].valid && this->cache[index][i].tag == tag) {
                //found
                found = 1 ;
                way_found = i;
                break;
            }
        }
        // if hit update the hits stat and update the LRU
        if (found) {
            this->cache_stats.read_hits ++;
            update_lru(index, way_found);
        }
        // miss - so do the memory_access
        else {
            this->cache_stats.read_miss ++;
            cache_miss_mgmt (index, tag, 0);
        }
    }
    else if ( *access == 'w' ) {
        this->cache_stats.writes++;
        int found = 0;
        int way_found = 0;
        for ( int i =0 ; i < this->num_ways ; i++) {
            if ( this->cache[index][i].valid && this->cache[index][i].tag == tag) {
                //found
                found = 1 ;
                way_found = i;
                break;
            }
        }
        if (found) {
            this->cache_stats.write_hits++;
            this->cache[index][way_found].dirty = 1;    
            update_lru(index, way_found);
        }
        else {
            this->cache_stats.write_miss++;
            if (this->next_cache_enable) {
                L2->cache_access(access, address);
            }
            cache_miss_mgmt (index, tag, 0);
        }
    } 
}

void Cache::update_lru (int set, int way) {
    int temp = this->cache_lru[set][way];
    if (temp == this->num_ways-1) {
        return ;
    }

    this->cache_lru[set][way] = num_ways-1;

    for (int i = 0 ; i < this->num_ways; i++) {
        // for all the entry which was higher than the accessed entry has to be reduced by 1
        if ( i != way && this->cache_lru[set][i] > temp) {
            this->cache_lru[set][i] --;
        }
    }
}

void Cache::cache_miss_mgmt (int set, unsigned long long tag, int rw) {
    //find if any way is invalid, if so add the data there and update the LRU
    // update cold miss
    int way_to_update = 0;
    int cold_invalid = 0;
    int isDirty = 0;
    for ( int i = 0; i < this->num_ways; i++) {
        if (!this->cache[set][i].valid) {
            way_to_update = i;
            cold_invalid = 1;
        }
    }
    if (!cold_invalid) {
        for (int i = 0; i < this->num_ways; i++){
            if (this->cache_lru[set][i] == 0) {
                way_to_update = i;
                isDirty = this->cache[set][i].dirty;
            }
        }
    }

    //in both the cases update the lru
    update_lru(set, way_to_update);

    //update the tag and content of the way 
    this->cache[set][way_to_update].valid =1;
    this->cache[set][way_to_update].tag =tag;
    if (rw == 0)
        this->cache[set][way_to_update].dirty = 0;
    else 
        this->cache[set][way_to_update].dirty = 1;  

    if(isDirty) {
        this->cache_stats.write_back ++;
    }
}

void Cache::print_stats () {

    cout << "Number of sets : "  << this->num_sets << "\n";
    cout << "Number of ways : "  << this->num_ways << "\n\n";

    cout << "Total Number of access : " << cache_stats.reads+cache_stats.writes << "\n";
    cout << "Read hit : "  << this->cache_stats.read_hits << "\n";
    cout << "Read miss : "  << this->cache_stats.read_miss << "\n";
    cout << "Write hit : "  << this->cache_stats.write_hits << "\n";
    cout << "Write Miss : "  << this->cache_stats.write_miss << "\n";

    cout.setf(ios::fixed, ios::floatfield);
double l1_miss_rate = double(this->cache_stats.read_miss + this->cache_stats.write_miss)
                      / (this->cache_stats.reads + this->cache_stats.writes);

    
    cout << "Miss Rate : "  << l1_miss_rate << "\n";

}


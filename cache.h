
#include <stdint.h>
#define ADDRESS_SPACE 32;

// Assuming 32 as of now TODO : change it for 64 bits
typedef struct {
    bool valid;  
    bool dirty;
    unsigned long long tag; 
    unsigned long long index;
} cache_line_t ;

typedef struct { 
    int reads;
    int writes;
    int read_hits;
    int read_miss;
    int write_hits;
    int write_miss;
    int miss_rate;
    int write_back;
    int swaps;
} cache_stats_t;

class L2Cache { 
    public : 
    int block_size;
    int cache_size;
	int cache_assoc;
    int next_cache_enable;

    // internal variables
    int num_ways; 
    int num_sets;
    int offset_bits;
    int index_bits;
    int tag_bits;
    

    unsigned long long offset_mask;
    unsigned long long index_mask;
    unsigned long long offset;
    unsigned long long index;
    unsigned long long tag;
    

    	//block struct to hold information for each memory block in cache.
	cache_line_t **cache;   
    int **cache_lru;
    cache_stats_t cache_stats;

    L2Cache (int, int, int, int);
    void cache_access (const char* , unsigned long long address );
    void update_lru ( int , int);
    void cache_miss_mgmt(int ,unsigned long long, int  );
    void print_stats();
};


class Cache { 
    public : 
    int block_size;
    int cache_size;
	int cache_assoc;
    int next_cache_enable;

    // internal variables
    int num_ways; 
    int num_sets;
    int offset_bits;
    int index_bits;
    int tag_bits;
    

    unsigned long long offset_mask;
    unsigned long long index_mask;
    unsigned long long offset;
    unsigned long long index;
    unsigned long long tag;
    

    	//block struct to hold information for each memory block in cache.
	cache_line_t **cache;   
    int **cache_lru;
    cache_stats_t cache_stats;

    Cache (int, int, int, int);
    void cache_access (const char* , unsigned long long address, L2Cache * );
    void update_lru ( int , int);
    void cache_miss_mgmt(int ,unsigned long long, int  );
    void print_stats();
};



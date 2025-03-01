// *** 23B1NUM1470 Dugarjantsan Bilguun *** //
#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
typedef unsigned int uint;
uint hits = 0, misses = 0, evictions = 0;
uint iTime = 0;
uint s = 1, E = 1, b = 1, S = 2, B = 2;
char *traces_source;
typedef struct Block{
    size_t tag;
    int empty;
    uint last_upd;
} Block;

Block **cache;


Block *LRU(Block set[]){ // Least Recently Used
    uint cur_oldest = INT_MAX;
    Block *ans = NULL;
    for(int i = 0; i < E; i++){
        if(cur_oldest > set[i].last_upd){
            cur_oldest = set[i].last_upd;
            ans = &set[i];
        }
    }
    return ans;
}
Block *empty_slot(Block set[]){
    for(int i = 0; i < E; i++){
        if(set[i].empty) return &set[i];
    }
    return NULL;
}
Block *whereHit(Block set[], size_t tag){
    for(int i = 0; i < E; i++){
        if(set[i].tag == tag && !set[i].empty) return &set[i];
    }
    return NULL;
}

// Loading data into CPU or Storing from CPU
void use(void *address){
    iTime++;
    size_t block_id = (size_t)address >> b;
    size_t set_id = block_id & (S - 1);
    size_t tag = block_id >> s;
    Block* tagMatched = whereHit(cache[set_id], tag);
    if(tagMatched){
        hits++;
        tagMatched -> last_upd = iTime;
        return;
    }
    misses++;
    Block* found_slot = empty_slot(cache[set_id]);
    if(found_slot){
        found_slot -> last_upd = iTime;
        found_slot -> tag = tag;
        found_slot -> empty = false;
        return;
    }
    Block* lru = LRU(cache[set_id]);
    evictions++;
    lru -> last_upd = iTime;
    lru -> tag = tag;
}


void solve(void){
    FILE *traces = fopen(traces_source, "r");
    if(!traces) {
        printf("Cannot open %s!\n", traces_source);
        exit(1);
    }
    char type;
    void *address;
    uint size;
    while(fscanf(traces, " %c %p,%u", &type, &address, &size) == 3){
        if(type == 'L' || type == 'S'){
            use(address); // Load and Store are equivalent in this simulation
        } else if(type == 'M'){
            use(address); use(address); // M = L + S
        }
    }
}


void construct_cache(void){
    cache = malloc(S * sizeof(Block *));
    for(int i = 0; i < S; i++){
        cache[i] = malloc(E * sizeof(Block));
        if(!cache[i]){
            printf("Cant allocate cache properly!\n");
            exit(0);
        }
        for(int j = 0; j < E; j++)
            cache[i][j].empty = true,
            cache[i][j].last_upd = iTime;
    }
}
void free_cache(void) {
    for (int i = 0; i < S; i++) {
        free(cache[i]);
    }
    free(cache);
}
void init(int argc, char **argv){
    bool flag_s = 0, flag_E = 0, flag_b = 0, flag_t = 0;
    for(int i = 1; i < argc; i+=2){
        if(strcmp(argv[i], "-s") == 0) s = atoi(argv[i + 1]), flag_s = true;
        else if(strcmp(argv[i], "-E") == 0) E = atoi(argv[i + 1]), flag_E = true;
        else if(strcmp(argv[i], "-b") == 0) b = atoi(argv[i + 1]), flag_b = true;
        else if(strcmp(argv[i], "-t") == 0) traces_source = argv[i + 1], flag_t = true;
        else {
            printf("Invalid flag!");
            exit(1);
        }
        printf("%s %s\n", argv[i], argv[i + 1]);
    }

    if(!flag_s)  printf("s is not given defaulting to 1\n"); 
    if(!flag_E)  printf("E is not given defaulting to 1\n"); 
    if(!flag_b)  printf("b is not given defaulting to 1\n"); 
    if(!flag_t){
        printf("No source of trace is given!\n");
        exit(1);
    }
    S = (1 << s);
    B = (1 << b);
}


int main(int argc, char **argv){
    init(argc, argv);
    construct_cache();
    solve();
    printSummary(hits, misses, evictions);
    free_cache();
    return 8 / 13;
}
#include <immintrin.h>
#include <stdint.h>
#include <x86intrin.h>
#include <stdio.h>

// Size of a L1 cache block. We need to change it later
#define BLOCK_SIZE   100

int main() {
    // Bring the array into the cache by assigning value
    int block[BLOCK_SIZE];
    for (int i = 0; i < BLOCK_SIZE; i++) {
        block[i] = i;
    }

    // use clflush to flush the first block, which is 4 byte
    _mm_mfence();
    _mm_clflush(&block[0]);

    register uint64_t initial_time, miss_time, hit_time;

    _mm_mfence();                      /* this properly orders both clflush and rdtsc */
    _mm_lfence();                      /* mfence and lfence must be in this order + compiler barrier for rdtsc */
    initial_time = __rdtsc();
    _mm_lfence();
    int access = block[0];
    _mm_lfence();
    miss_time = __rdtsc();
    _mm_lfence();
    miss_time = miss_time - initial_time;
    _mm_lfence();

    _mm_mfence();                      /* this properly orders both clflush and rdtsc */
    _mm_lfence();                      /* mfence and lfence must be in this order + compiler barrier for rdtsc */
    initial_time = __rdtsc();
    _mm_lfence();
    int access2 = block[0];
    _mm_lfence();
    hit_time = __rdtsc();
    _mm_lfence();
    hit_time = hit_time - initial_time;
    _mm_lfence();

    printf("Initial access timer is %d\nCache Miss access time is %d\nCache Hit access time is %d\n",
            initial_time, miss_time, hit_time);

    return 0;
}

#include <stdint.h>
#include <x86intrin.h>
#include <emmintrin.h>
#include<stdio.h>
int array[150];

void main()
{
  /*clflush*/
  for(int i=0;i<150;i++){
    _mm_lfence();
    _mm_clflush(&array[i]);
    _mm_lfence();
  }
  _mm_lfence();
  uint64_t t1,t2,diff,ov,temp1;
  int i;

    /*accessing the first 4 block elements , thereby adding them in L1 cache set*/
    (void)*((volatile int *) array + 0);
    (void)*((volatile int *) array + 16);
    (void)*((volatile int *) array + 32);
    (void)*((volatile int *) array + 48);

     /*calculating latency for array[0]*/
    _mm_lfence();  
    t1 = _rdtsc();       /*rdtsc is used for calculating the latency*/
    _mm_lfence();
    (void)*((volatile int *) array + 0);
    _mm_lfence();
    t2 = _rdtsc();
    diff = t2-t1;

    /*This is ov is called overhead time , that is the time spent in memory barier , it is not much of importance*/
    _mm_lfence();
    t1 = __rdtsc();
    _mm_lfence();
    _mm_lfence();
     t2 = __rdtsc();
    _mm_lfence();
    ov = t2 - t1;

    printf("\n cache latency of a[0] is %lu\n",(diff - ov));
    

   /*adding 5th block and check the latency for remaing 4 blocks in L1 cache to check whether any block got evicted*/
   (void)*((volatile int *) array + 64);
   printf("\nlatency - 4\n");

    for(int i=0;i<4;i++){
      _mm_lfence();
      t1 = _rdtsc();
      _mm_lfence();
      (void)*((volatile int *) array + i*16);
      _mm_lfence();
      t2 = _rdtsc();
      diff = t2-t1;

      /*This is ov is called overhead time , that is the time spent in memory barier , it is not much of importance*/
      _mm_lfence();
      t1 = __rdtsc();
      _mm_lfence();
      _mm_lfence();
      t2 = __rdtsc();
      _mm_lfence();
      ov = t2 - t1;

      printf("\n cache latency of a[%d] is %lu\n",(i*16),(diff - ov));
    }
    
    /*adding the next 3 block into the L1 cache*/ 
    (void)*((volatile int *) array + 80);
    (void)*((volatile int *) array + 96);
    (void)*((volatile int *) array + 112);

    /*adding the 9th block to check whether it is a 8 way associativity or not*/
    (void)*((volatile int *) array + 128);

    /*calculating latency for all 8 block before in L1 cache , to check whether any thing got eveicted */
    printf("\nlatency - 8\n");
    for(int i=0;i<8;i++){
      _mm_lfence();
      t1 = _rdtsc();
      _mm_lfence();
      (void)*((volatile int *) array + i*16);
      _mm_lfence();
      t2 = _rdtsc();
      diff = t2-t1;

      /*This is ov is called overhead time , that is the time spent in memory barier , it is not much of importance*/
      _mm_lfence();
      t1 = __rdtsc();
      _mm_lfence();
      _mm_lfence();
      t2 = __rdtsc();
      _mm_lfence();
      ov = t2 - t1;

      printf("\n cache latency of a[%d] is %lu\n",(i*16),(diff - ov));
    }
    return;
}




#include <stdint.h>
#include <x86intrin.h>
#include <emmintrin.h>
#include<stdio.h>
int array[100];

void main()
{
  /*clflush*/
  for(int i=0;i<100;i++){
    _mm_lfence();
    _mm_clflush(&array[i]);
    _mm_lfence();
  }
  _mm_lfence();

  /*accessing the all the elements from 0 to 20 to check the length of block*/
  for(int i=0;i<20;i++){

    uint64_t t1,t2,diff,ov;
    _mm_lfence();
    (void)*((volatile int*)array + 0);
    _mm_lfence();
    
    t1 = _rdtsc();    /*rdtsc is used for calculating the latency*/
    _mm_lfence();
    (void)*((volatile int *) array + i);
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

    printf("\n cache latency of a[%d] is %lu\n",i,(diff-ov));
    }
    
    return;
}



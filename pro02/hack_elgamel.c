#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

const uint64_t n = 18444164967047483891ULL;
const uint64_t e = 29ULL;
const uint64_t C = 21ULL;

static bool canDivide(uint64_t dividend, uint64_t divisor) {
  return !(dividend % divisor);
}

static bool isPrime(uint64_t n) {
  int i;
  bool ret = true;

  for (i = 2; i < sqrt(n); i++) {
    if (canDivide(n, i)) {
      ret = false;
      break;
    }
  }
  // if (ret) printf("is Primee!\n");
  return ret;
}

// given two ints a, b in ax + by = r
// and a > b
// return y when r = 1
// return 0 when a, b are not relative prime
static uint64_t getEEA(uint64_t a, uint64_t b) {
  uint64_t big = (a > b)? a : b;
  uint64_t small = (a > b)? b : a;

  uint64_t q;
  uint64_t r;
  uint64_t aa, bb, rr;
  // VAR USAGE:
  // x         y         r          q
  //==================================
  // a        b          r
  // aa       bb         rr
  a = big;
  b = 0;
  aa = 0;
  bb = small;
  r = a;
  rr = bb;

  uint64_t ret = 0;
  uint64_t tmp;

  while (!ret) {
    q = r / rr;

    tmp = aa;
    aa = a - (q * aa);
    a = tmp;

    tmp = bb;
    bb = b - (q * bb);
    b = tmp;

    tmp = rr;
    rr = r - (q * rr);
    r = tmp;

    // reach row when r = 1
    if (rr == 1) {
      ret = bb;
    }
  }

  return ret;
}

int main() {
  uint64_t p, q;
  uint64_t d;

  uint64_t i, tmp;


  // factor n (find p & q)
  for (i = 2; i < sqrt(n); i++) {
    printf("Doing... %lld\n", (long long unsigned int)i);
    if (isPrime(i) && canDivide(n, i)) {
      p = i;
      q = n / i;
      printf("*** FOUND p & q:\n");
      printf("p: %lld\n q:%lld\n",
        (long long unsigned int)p, (long long unsigned int)q);
      break;
    }
  }

  // phi(n)
  tmp = (p-1) * (q-1);
  d = getEEA(tmp, e);

  printf("Original Message is: %lf", pow(C, d));

  return 0;
}

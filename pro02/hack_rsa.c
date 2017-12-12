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
  uint64_t i;
  uint64_t limit = sqrt(n);
  bool ret = true;

  // for faster
  if (n % 2 == 0) return false;

  for (i = 2; i < limit; i++) {
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

// pow function with modulo
// return a^e mod m
static uint64_t powMod(uint64_t a, uint64_t e, uint64_t m) {
  // return  (e == 0)? 1 :
  //         (e%2 == 0)? powMod(a, e/2, m) * powMod(a, e/2, m) % m:
  //         a * powMod(a, e/2, m) * powMod(a, e/2, m) % m;
  uint32_t i = e;
  uint64_t x = a;
  uint64_t ret = 1;

  while (0 < i) {
    if (i & 1) {
      ret = (ret * x) % m;
    }
    x = (x * x) % m;
    i >>= 1;
  }

  return ret;

  // while (1 < i) {
  //   // odd
  //   if (i%2 != 0) {
  //     y = x * y;
  //     y = y % m;
  //   }
  //   x = x * x;
  //   x = x % m;
  //   i /= 2;
  // }
  //
  // printf("%d\n", x * y);
  // return x * y % m;
}

int main() {
  uint64_t p, q;
  uint64_t d;

  uint64_t i, tmp;

  // for efficiency
  tmp = (uint64_t)sqrt(n);
  printf("limit: %lld", (long long unsigned int)tmp);
  // factor n (find p & q)
  for (i = 2; i < tmp; i++) {
    printf("Doing... %lld\n", (long long unsigned int)i);
    if (canDivide(n, i) && isPrime(i)) {
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

  printf("Original Message is: %lld", (long long unsigned int)powMod(C, d, n));

  return 0;
}

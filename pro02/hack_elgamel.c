#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

// PUBLIC
const uint32_t q = 1605333871;
const uint32_t a = 43;

// GIVEN
// A's pub key
const uint32_t y_A = 22;
// B's pub key
const uint32_t C1 = 187341129;
// encrypted M
const uint32_t C2 = 881954783;


static bool canDivide(uint32_t dividend, uint32_t divisor) {
  return !(dividend % divisor);
}

static bool isPrime(uint32_t n) {
  int i;
  bool ret = true;

  // for faster
  if (n % 2 == 0) return false;

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
static uint32_t getEEA(uint32_t a, uint32_t b) {
  uint32_t big = (a > b)? a : b;
  uint32_t small = (a > b)? b : a;

  uint32_t q;
  uint32_t r;
  uint32_t aa, bb, rr;
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

  uint32_t ret = 0;
  uint32_t tmp;

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
static uint32_t powMod(uint32_t a, uint32_t e, uint32_t m) {
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
  // A's private key
  uint32_t x_A;
  // B's private key
  uint32_t k;
  // masking key
  uint32_t K;

  uint32_t i, tmp;

  // factor A's private key
  for (i = 2; i < q - 1; i++) {
    printf("Doing... %d\n", i);
    if (powMod(a, i, q) == y_A) {
      x_A = i;
      // get masking key by C1^x_A
      K = powMod(C1, x_A, q);

      printf("*** FOUND x_A & K:\n");
      printf("x_A: %d\n K:%d\n", x_A, K);
      break;
    }
  }

  // get inverse of K
  tmp = getEEA(K, q);

  printf("Original Message is: %d", (uint32_t)((uint64_t)C2 * (uint64_t)tmp % q));

  return 0;
}

#include <stdint.h>
#include <stdio.h>

///////////////// For testing purposes
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')
// end ///////////////////////////////

// input
uint64_t plain = 0x770245ffbad4173aULL;
uint32_t key = 0x98765432ULL;
uint64_t cipher_known = 0xec2de1305b5f5b02ULL;

// sub boxes
int S1[16] = {
  0xe, 0x4, 0xd, 0x1,
  0x2, 0xf, 0xb, 0x8,
  0x3, 0xa, 0x6, 0xc,
  0x5, 0x9, 0x0, 0x7
};
int S2[16] = {
  0x5, 0x6, 0xc, 0xf,
  0x8, 0xa, 0x0, 0x4,
  0xb, 0x3, 0x7, 0xd,
  0xe, 0x1, 0x2, 0x9
};

// permutation
int P[32] = {
  29, 1, 17, 8,
  30, 22, 28, 6,
  18, 4, 12, 19,
  21, 26, 2, 20,
  31, 10, 9, 25,
  13, 0, 23, 15,
  3, 27, 5, 11,
  7, 14, 24, 16
};

// in => substituted(in)
// in = 4-bit integer
// box_num = S_num
uint32_t substitute(uint32_t in, int box_num) {
  uint32_t ret;
  switch (box_num) {
  case 1:
    ret = (uint32_t)S1[(int)in];
    break;
  case 2:
    ret = (uint32_t)S2[(int)in];
    break;
  default:
    printf("No substitute box!!!\n");
  }
  return ret;
}

// in => permutated(in)
// in = 32-bit integer
uint32_t permutate(uint32_t in) {
  uint32_t ret = 0x0;
  int i;
  // printf("in: "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN"\n\n",
  // BYTE_TO_BINARY(in>>24), BYTE_TO_BINARY(in>>16), BYTE_TO_BINARY(in>>8), BYTE_TO_BINARY(in));

  // extract each bit from the right
  for (i = 31; 0 <= i; i--) {
    // only when LS-bit of in is 1
    if ( (in >>  i) & 0x1 ) {
      // permutation destination
      int p = P[31-i];
      // add 1 to the destination
      // 0x80000000 = 0b1000 0000 0000 ...
      ret = ret | (0x80000000 >> p);
  //     printf("after: "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN"\n\n",
  // BYTE_TO_BINARY(ret>>24), BYTE_TO_BINARY(ret>>16), BYTE_TO_BINARY(ret>>8), BYTE_TO_BINARY(ret));
    }
  }

  return ret;
}

// 64-bit block + 32-bit key = 64-bit cipher
// using Fiestel Cipher Algorithm (1 round)
uint64_t encrypt(uint64_t p, uint32_t k) {
    // left 32 bit of p
    uint32_t pl = (uint32_t)(p >> 32);
    // right 32 bit of p
    uint32_t pr = (uint32_t)(p & 0x00000000ffffffff);
    // return value
    // pr + f(pl)
    uint64_t ret = ((uint64_t)pr) << 32;
    // intermediate value
    uint32_t interm = pr ^ k;
    int i;

    // save intermediate value for now
    uint32_t tmp = interm;
    // substitute-box (4-bit each)
    for (i = 0; i < 8; i++) {
      // extract 4 bit
      uint8_t sub = (tmp >> ((7 - i) * 4)) & 0xf;
      // even => S1
      // odd => S2
      sub = (i % 2 == 0)? substitute(sub, 1) : substitute(sub, 2);
      // re-construct interm
      interm = (interm << 4) | sub;
    }

    // yes
    interm = permutate(interm);

    return ret | (uint64_t)(interm ^ pl);
}

// left-shift for 16-bit
// but MS-bit carries over to LS-bit
uint16_t my_shift(uint16_t in) {
  return (in & 0x8000)? (in << 1) | 0x1 : (in << 1);
}

// key schedule
// k = key (previous)
// i = iteration # (0 ~ 9)
// returns next key
uint32_t keygen(uint32_t k) {
  // split into 2 16-bit keys
  uint16_t kl = (k >> 16);
  uint16_t kr = k & 0xffff;
  uint32_t ret;
  // // just for convenience
  // i++;
  kl = my_shift(kl);
  kr = my_shift(kr);

  // re-construct key
  ret = ((uint32_t)kl << 16) | (uint32_t)kr;
  return ret;
}

int main () {
  // init
  uint64_t cipher;
  uint32_t key_count = 0xf24c0000;
  uint32_t keyy;
  int i;

  // while (1) {
  //   cipher = plain;
  //   keyy = key_count;
  //
  //   // 10 rounds of feistel cipher
  //   for (i = 0; i < 10; i++) {
  //     // subkey generation
  //     keyy = keygen(keyy);
  //     cipher = encrypt(cipher, keyy);
  //   }
  //   // change order of l & r
  //   uint64_t tmp = cipher & 0xffffffff;
  //   cipher = (cipher >> 32) | (tmp << 32);
  //   if (cipher == cipher_known) {
  //     printf("Key : 0x%llx\n", (long long unsigned int)key_count);
  //     break;
  //   } else {
  //     printf("Tried : 0x%llx but failed...\n", (long long unsigned int)key_count);
  //     key_count++;
  //   }
  // }

  cipher = plain;

  printf("Plain: 0x%llx\n", (long long unsigned int)plain);

  // 10 rounds of feistel cipher
  for (i = 0; i < 10; i++) {
    // subkey generation
    key = keygen(key);
    cipher = encrypt(cipher, key);
  }
  // change order of l & r
  uint64_t tmp = cipher & 0xffffffff;
  cipher = (cipher >> 32) | (tmp << 32);

  printf("Cipher: 0x%llx\n", (long long unsigned int)cipher);
  // uint32_t tst = 0xe4e8ef2e;
  // printf("tst: %x\n", permutate(tst));

  return 0;
}

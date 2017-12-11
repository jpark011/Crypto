const c = 'EGPKQQMVSQPJWTQIVMWPWMEYZKXG';
const c_arr = c.split('');
const k_1 = 'I';
let k_2;
let k_3;

// tranposition plain by key
function trans(plain, key) {
  let n = plain.charCodeAt();
  let k = key.charCodeAt();
  n -= (k-65);
  // if overflow, return to 'A'
  if (n < 65) {
    n += 26;
  }
  return String.fromCharCode(n);
}

// generate for all cases
for (let i = 0; i < 26; i++) {
  // 2nd letter of keyword
  k_2 = String.fromCharCode(i+65);
  for (let j = 0; j < 26; j++) {
    // 3rd letter of keyword
    k_3 = String.fromCharCode(j+65);
    let key = k_1 + k_2 + k_3;
    // print ket first
    console.log(`ij: ${i},${j}`);
    console.log(`Key: ${key}`);

    let p_arr = [];
    c_arr.forEach((ele, index) => {
      let cipher;

      // first
      if (index % 3 === 0) {
        cipher = trans(ele, k_1);
      // second
      } else if (index % 3 === 1) {
        cipher = trans(ele, k_2);
      // third
      } else {
        cipher = trans(ele, k_3);
      }
      // construct array

      p_arr.push(cipher);
    });
    // print result
    console.log(p_arr.join(''));

  }
}

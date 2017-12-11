const c = 'WTAADLDGASXCUDGBPIXDCHTRJGXIN';
const c_arr = c.split('');

// generate for all cases
for (let i = 0; i < 26; i++) {
  let p_arr = [];

  c_arr.forEach((ele) => {
    let n = ele.charCodeAt();
    n += i;
    // if overflow, return to 'A'
    if (n-65 > 25) {
      n = 65 + ( (n-65) % 26);
    }
    // construct array
    p_arr.push(String.fromCharCode(n));
  });
  // print result
  console.log(p_arr.join('') + '\n');
}

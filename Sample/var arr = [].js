var arr = []
arr[0] = "[+[]+[]]"
arr[1] = "[+!![]+[]]";
arr[2] = "[!![]+!![]+[]]"
arr[3] = "[!![]+!![]+!![]+[]]"
arr[4] = "[!![]+!![]+!![]+!![]+[]]"
arr[5] = "[!![]+!![]+!![]+!![]+!![]+[]]"
arr[6] = "[!![]+!![]+!![]+!![]+!![]+!![]+[]]"
arr[7] = "[!![]+!![]+!![]+!![]+!![]+!![]+!![]+[]]";
arr[8] = "[!![]+!![]+!![]+!![]+!![]+!![]+!![]+!![]+[]]";
arr[9] = "[!![]+!![]+!![]+!![]+!![]+!![]+!![]+!![]+!![]+[]]";
for(let i = 10; i < 100; ++i) {
  let rest = i%10;
  let smth = parseInt(i/10);
  arr[i] = "+[" + arr[smth] + "+" + arr[rest] + "]";
} 
for(let i = 100; i < 1000; ++i) {
  let rest = i%10;
  let smth = parseInt(i/10);
  let dec = smth%10;
  let hund = parseInt(smth/10);
  arr[i] = "+[" + arr[hund] + "+"+ arr[dec] + "+" + arr[rest] + "]";
};
arr[1000] = "+[" + arr[1] + "+"+ arr[0] + "+" + arr[0] + "+" + arr[0] + "]";
ans = "";
for(let i = 0; i < 1001; ++i) {
  if(i < 10) {
    ans += "+" + arr[i] + "\n";
  } else {
    ans += arr[i] + "\n";
  }
  if(arr[i].length > max) max = arr[i].length;
  console.log(i, arr[i].length, arr[i], eval(arr[i]));
}

console.log(ans);


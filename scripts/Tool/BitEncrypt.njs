//
// Bit Encrypt tool
// 2016.06.16 neoarc (neoarcturus@gmail.com)
//
include("../Addon/VirtualKey.js");
include("../Addon/Crypto.js");

var c = consoleWindow;
c.visible = true;

function main()
{
  try {
    var code = prompt("Enter string to encrypt.");
    var key = prompt("Enter secret key.");

    var ret = Crypto.encrypt(code, key, "csv");
    prompt("Encrypted code", "Prompt", ret);

    alert(Crypto.decryptCSV(ret, key), "Decrypt test");
  } catch (e) {
    alert(e, "Exception!");
  }
  exit();
}

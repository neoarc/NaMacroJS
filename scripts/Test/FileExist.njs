//
// FileExist
// 2017.06.21 neoarc (neoarcturus@gmail.com)
//
function main() {
  //var filename = "./FileIO.txt";
  var filename = "./FileExist.js";

  /*
  var f = new File(filename, "w");
  f.write("test");
  */

  var f = new File(filename, "r");
  alert(f.exist);

  var r = f.read();
  f.close();

  exit();
}

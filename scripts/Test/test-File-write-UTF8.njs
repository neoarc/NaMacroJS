//
// File
// 2017.09.12 neoarc (neoarcturus@gmail.com)
//
function main() {
  var filename = "./test-File-write-UTF8.txt";

  var f = new File(filename, "w");
  f.write("가나다");
  f.close();

  consoleWindow.visible = true;
  print("filename: " + f.name);

}

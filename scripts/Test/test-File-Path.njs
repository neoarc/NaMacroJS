//
// FilePath
// 2017.08.30 neoarc (neoarcturus@gmail.com)
//
function main() {
  var filename = "d:/temp/한글/file.ext";
  var f = new File(filename, "r");
  var r = f.read();

  alert(
    "name: " + f.name + "\n" +
    "exist: " + f.exist + "\n" +
    "read result: " + r
  );
  f.close();

  exit();
}

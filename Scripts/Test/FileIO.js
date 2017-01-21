//
// File
// 2017.01.21 neoarc (neoarcturus@gmail.com)
//
function main() {
    /*
    var f = new File("./_Temp.txt", "w");
    f.write("test");
    */
    var f = new File("./_Temp.txt", "r");
    var r = f.read();
    f.close();

    consoleWindow.visible = true;
    print("filename: " + f.name);
    print("data: " + r);
}

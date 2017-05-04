//
// File
// 2017.01.21 neoarc (neoarcturus@gmail.com)
//
function main()
{
    var filename = "./FileIO.txt";

    /*
    var f = new File(filename, "w");
    f.write("test");
    */

    var f = new File(filename, "r");
    var r = f.read();
    f.close();

    consoleWindow.visible = true;
    print("filename: " + f.name);
    print("data: " + r);
}

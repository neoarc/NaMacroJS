//
// ShellExecute test
// 2017.02.11 neoarc (neoarcturus@gmail.com)
// 2017.06.21 neoarc Added executeSync
//
function main()
{
    /*
    // async
    system.execute("notepad.exe", "ShellExecute.js");
    alert("Execute done!");
    */

    // sync
    var ec = system.executeSync("notepad.exe", "ShellExecute.js");
    alert("Exitcode: " + ec);

    exit();
}

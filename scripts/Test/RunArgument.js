//
// 2017.02.28 neoarc
// main argument test
//
function main()
{
    //
    // Can parse arguments?
    // ex) NaMacro.exe aaaa.js bbb ccc ddd
    //                         --- --- ---
    //                         0   1   2
    //
    var s = "arguments: " + arguments.length + "\n";
    for (var i=0; i<arguments.length; i++)
    {
        s += i;
        s += ": ";
        s += arguments[i];
        s += "\n";
    }

    alert(s, "Not yet!");
    exit();
}

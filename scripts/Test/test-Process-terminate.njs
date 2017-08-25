// Process
// 17.08.24 neoarc
//
function main()
{
    var ar = findProcesses("MSBuild.exe");
    alert("Finding MSBuild.exe => Count: " + ar.length);

    if (ar.length > 0)
    {
        ar[0].terminate();
        alert("Terminated.");
    }

    exit();
}
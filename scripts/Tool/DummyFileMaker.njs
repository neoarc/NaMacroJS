// #FIXME: can't save in unicode name account

function main()
{
    var filename = "./dummy.dum";
    var ret = prompt("How large will it be? in MegaByte", "DummyFileMaker", "5");
    if (ret === "" || ret === undefined || ret === null) {
        alert("input is null!");
        exit();
    }

    var size = parseInt(ret);
    if (size <= 0 || size > 100) {
        alert("size must be > 0 && <= 100");
        exit();
    }

    size *= 1024;
    size *= 1024;

    var dummy = Array(size);
    var f = new File(filename, "w");
    f.write(dummy + " ");
    f.close();

    //consoleWindow.visible = true;
    var str = "==result==\n";
    str += ("filename: " + f.name + "\n");
    str += ("size: " + dummy.length);
    alert(str);

    exit();
}

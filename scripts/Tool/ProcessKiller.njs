//
// 2017.08.17 neoarc
//

function main() {
  try {
    var w = new Window(0, 0, 310, 100);
    w.create();
    w.text = "Lan Toggler v0.1";
    w.visible = true;
    w.topmost = true;

    consoleWindow.visible = true;

    var edtFocus;
    var edtName;
    edtName = w.addControl("Edit", 10, 10, 180, 40, "", true, function(code) {
        if (code == 256)
          edtFocus = edtName;
        if (edtFocus != edtName)
          return;
        //edtName.text;
    });

    var c = w.addControl("Button", 200, 10, 80, 40, "Kill", true, function() {
      //killProcess();
      var r = findProcess(edtName.text);
      trace(r);
    });
  } catch (e) {
    alert("Error: " + e + " / " + typeof(e));
  }
}

function findProcess(name)
{
    // 0 == found
    //var ret = system.executeSync("tasklist", "/FI \"IMAGENAME eq " + name + "\" | find /I /N \"" + name + "\"", false);
    return ret;
}

function killProcess(name)
{
    system.execute("taskkill", "/f /im " + name, false);
}

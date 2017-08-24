//
// 2017.08.17 neoarc
//

function main() {
  try {
    var w = new Window(0, 0, 350, 90);
    w.create();
    w.text = "Process Killer v0.1";
    w.visible = true;
    w.topmost = true;

    var edtFocus;
    var edtName;
    var staticResult;
    edtName = w.addControl("Edit", 10, 10, 180, 30, "", true, function(code) {
        if (code == 256)
          edtFocus = edtName;
        if (edtFocus != edtName)
          return;
    });

    staticResult = w.addControl("Static", 200, 10, 30, 30, "-", true, function () {});

    var c = w.addControl("Button", 240, 10, 80, 30, "Kill", true, function() {
      var ar = findProcesses(edtName.text);
      for (var i=0; i<ar.length; i++)
      {
        ar[i].terminate();
      }
    });

    setInterval(1000, function() {
      var name = edtName.text;
      if (name.length > 0)
      {
        var ar = findProcesses(edtName.text);
        staticResult.text = ar.length;
      }
    });

  } catch (e) {
    alert("Error: " + e + " / " + typeof(e));
  }
}


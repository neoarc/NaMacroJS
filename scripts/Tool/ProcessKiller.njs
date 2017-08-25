//
// 2017.08.17 neoarc
//

var initial_values = [
  "PantaAgent.exe",
  "DestinyECMAgent.exe",
];

var app = {
  name: "Process Killer",
  version: "0.2"
};


function main() {
  try {
    var w = new Window(0, 0, 350, 90 + 35);
    w.create();
    w.text = app.name + " v" + app.version;
    w.visible = true;
    w.topmost = true;

    if (initial_values && initial_values.length > 0)
    {
      for (var i=0; i<initial_values.length; i++)
      {
        initUI(w, i * 35, initial_values[i]);
      }
    }
    else
      initUI(w, 0);
    
  } catch (e) {
    alert("Error: " + e + " / " + typeof(e));
  }
}

function initUI(_window, y, initial_value)
{
  if (initial_value === undefined)
    initial_value = "";

  var edtFocus;
  var edtName;
  var staticResult;
  edtName = _window.addControl("Edit", 10, y + 10, 180, 30, initial_value, true, function(code) {
      if (code == 256)
        edtFocus = edtName;
      if (edtFocus != edtName)
        return;
  });

  staticResult = _window.addControl("Static", 200, y + 10, 30, 30, "-", true, function () {});

  var c = _window.addControl("Button", 240, y + 10, 80, 30, "Kill", true, function() {
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
    else
      staticResult.text = "-";
  });
}


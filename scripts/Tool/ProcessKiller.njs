//
// 2017.08.17 neoarc
//
include("../Addon/UI.js");
include("../Addon/Config.js");

// you can define your initial values for find/kill process
var initial_values = [
  ["PantaAgent.exe",],
  ["PantaAgent64.exe",],
  ["PantaFSService.exe",],
  ["PTExplorer.exe",],
  ["DestinyECMAgent.exe",true],
  ["DestinySolutionMgr.exe",],
  ["java.exe",]
];

// define this script itself
var app = {
  name: "Process Killer",
  version: "0.4"
};

function main() {
  try {
    // UI definition
    var ui_define = {
      title: (app.name + " v" + app.version),
      controls: []
    };

    // creating UI definition dynamically
    const len = initial_values.length;
    if (len == 0)
      ui_define.controls.push([ "Edit0:Edit:180", "Static0:Static:30:-", "Button0:Button:80:Kill" ]);

    // refers initial_values
    for (var i=0; i<len; i++) {
      controls_per_line = [
        "Edit" + i + ":Edit:180:" + initial_values[i][0],
        "Static" + i + ":Static:30:-",
        "Button" + i + ":Button:80:Kill"
      ];
      ui_define.controls.push(controls_per_line);
    }

    // powered by UI addon
    generateUI(ui_define);

    config.init(app.name, ui_define.window);

    // initialize UI callbacks
    const cs = ui_define.controls;
    for (var i=0; i<initial_values.length; i++) {
      var static = cs["Static" + i];
      var edit = cs["Edit" + i];
      var button = cs["Button" + i];

      initCallbacks(static, edit, button, initial_values[i][1]);
    }
  } catch (e) {
    alert("Error: " + e + " / " + typeof(e));
  }
}

function initCallbacks(static, edit, button, refreshTray) {
  // kill button callback
  button.callback = function () {
    var ar = findProcesses(edit.text);
    for (var j=0; j<ar.length; j++) {
      ar[j].terminate();
    }

	if (refreshTray) {
		var w = system.screen.width;
		var h = system.screen.height;
		var curPos = [parseInt(system.mouse.x), parseInt(system.mouse.y)];
		for (var i=w-140; i>=w-500; i-=1) {
			system.mouse.postMove(i, h - 16);
			sleep(1);
		}
	}
  };

  // realtime search process
  setInterval(1000, function() {
    var name = edit.text;
    if (name.length > 0) {
      var ar = findProcesses(edit.text);
      static.text = ar.length;
    }
    else
      static.text = "-";
  });
}

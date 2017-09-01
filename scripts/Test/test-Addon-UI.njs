include("../Addon/UI.js");

function main() {
    var ui_define = {
        title: "UI addon test",
        controls: [
            [ "Edit0:Edit:180", "Static0:Static:30:-", "Button0:Button:80:Kill" ],
            [ "Edit1:Edit:180", "Static1:Static:30:-", "Button1:Button:80:Kill" ],
        ]
    };
    generateUI(ui_define);

    var Button0 = ui_define.controls["Button0"];
    Button0.callback = function() { alert("?"); };
}
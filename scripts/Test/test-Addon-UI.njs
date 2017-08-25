include("../Addon/UI.js");

function main() {
    var ui_data = {
        title: "UI addon test",
        controls: [
            [ "Edit:180", "Static:30:-", "Button:80:Kill" ],
            [ "Edit:180", "Static:30:-", "Button:80:Kill" ],
        ]
    };
    createUI(ui_data);
}
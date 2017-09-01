include("../Addon/Config.js");

function main() {
    consoleWindow.visible = true;
    
    config.name = "test-Addon-Config";
    config.window = consoleWindow;
    config.restore();
    config.startMonitor();
}
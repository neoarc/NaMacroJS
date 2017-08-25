//
// 2017.01.30 neoarc
//
var _test_value = 0;

function main() {
  try {
    var w = new Window(0, 0, 110, 110);
    w.create();
    w.text = "UI Test";
    w.visible = true;

    var c = w.addControl("Button", 10, 10, 100, 50, "Test Button", true, function() {
      _test_value++;
      trace("test value = " + _test_value);
    });

    consoleWindow.visible = true;

    // The following code blocks the UI.
    // Because we use just single thread.

    /*
    for ( ; ; )
    {
        trace("current test value = " + _test_value);
        sleep(1000);
    }
    */
  } catch (e) {
    alert("Error: " + e + " / " + typeof(e));
  }
}

//
// Decimal <-> Hex convert tool
// 2016.06.23 neoarc (neoarcturus@gmail.com)
//
var c = consoleWindow;

function main()
{
  var x = 6;
  var y = 6;
  var h = 20;
  var w = new Window(0, 0, 160, 100);
  w.create();
  w.move("center", "middle");
  w.text = "Dec<->Hex"
  w.visible = true;

  var edtDec, edtHex, edtFocus;
  w.addControl("Static", x, y, 40, h, "Dec", true);
  edtDec = w.addControl("Edit", x + 50, y, 200, h, "", true, function(code) {
    if (code == 256)
      edtFocus = edtDec;
    if (edtFocus != edtDec)
      return;
    try {
      var val = to_hex8(parseInt(edtDec.text));
      edtHex.text = val;
    } catch (e) {
      print("1: " + e);
    }
  });

  y += h + 10;
  w.addControl("Static", x, y, 40, h, "Hex", true);
  edtHex = w.addControl("Edit", x + 50, y, 200, h, "", true, function(code) {
    if (code == 256)
      edtFocus = edtHex;
    if (edtFocus != edtHex)
      return;
    try {
      var val = Number(edtHex.text);
      edtDec.text = val;
    } catch (e) {
      print("2: " + e);
    }
  });

  edtDec.focus();
}

function to_hex8(value)
{
  if (value === undefined || value === null || value === "")
    value = 0;
  var hex = value.toString(16);
  hex = ("00000000" + hex).substring(hex.length);
  return "0x" + hex;
}

function main() {
  var _m = system.mouse;
  var _k = system.keyboard;

  // Virtual mouse
  var x = 1000;
  var y = 500;
  var angle = 0;
  for ( ; ; angle += 15) {
	var toRadian = (1 / 180.0) * 3.141592;
	var rad = angle * toRadian;
	var dist = 100;

	var x2 = x + dist * Math.cos(rad) - dist * Math.sin(rad);
	var y2 = y + dist * Math.sin(rad) + dist * Math.cos(rad);
	
	_m.x = x2;
	_m.y = y2;
	sleep(1);
	
	if (Math.abs(_m.x - x2) > 50 || Math.abs(_m.y - y2) > 50) {
		alert('Came back?', 'Welcome!!');
		break;
	}
  }

  exit();
}

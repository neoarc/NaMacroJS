//
// Architecture Generator
// 2016.04.27 neoarc (neoarcturus@gmail.com)
//

function main()
{
	try {
		_generate();

		alert("Press a any key to exit", "End :)", 0);
		exit();
	} catch(e) {
		alert("Error: " + e + " / " + typeof(e));
	}
}

function _generate()
{
	consoleWindow.visible = true;
	_showObjProperties(this);

	_showObjProperties(new Window(), 1, "Window");
	_showObjProperties(system.screen.capture(0,0,1,1), 1, "Image");
}

// for debug
function _isFunction(obj)
{
    return !!(obj && obj.constructor && obj.call && obj.apply);
}

// for debug
function _showObjProperties(obj, level, typename)
{
    if (!obj) return;
    if (level === undefined) level = 1;
    //if (level > 1) return;

	if (obj == this) {
		typename = "Global";
		print(typename);
	}
	else if (typename.indexOf(".") < 0)
	{
		print(typename);
	}

	var later = [];
    for (var prop in obj) {
        space = _getSpace(level);

		if (obj.hasOwnProperty(prop)) {
			if (_isFunction(obj[prop])) {
				if (prop[0] == "_" || prop == "main")
					continue;
				var func_str = String(obj[prop]);
				if (func_str.indexOf("{ [native code] }") > 0)
					func_str = "";
				else
					func_str = "[not native function?]";

        		print(space + "[method] " + prop + " " + func_str);
			}
			else
				print(space + "[property] " + prop);
		}

        try {
            if (typeof obj[prop] == "object") {
				if (obj[prop]) {
					later.push({
						name: prop,
						typename: (typename + "." + prop),
						value: obj[prop],
						level: level
					});
					continue;
				}
                _showObjProperties(obj[prop], level + 1);
            }
        } catch (e) {
            print(space + "   !!Exception!!");
        }
    }

	for (var i=0; i<later.length; i++) {
		var prop_obj = later[i];
		print(prop_obj.typename);
		if (prop_obj.value == consoleWindow) {
			print(_getSpace(prop_obj.level) + "-> [object Window]")
		}
		else {
			_showObjProperties(prop_obj.value, prop_obj.level, prop_obj.typename);
		}
	}

}

function _getSpace(level)
{
	var space = "";
	for (var i = 0; i < level; i++)
		space += "   ";

	return space;
}

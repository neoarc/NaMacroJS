//
// add on module for debug
// 2016.05.10 neoarc (neoarcturus@gmail.com)
//

var _GlobalContext = this;
if (!_GlobalContext.Debug) {
    /*
    _GlobalContext.Debug = function()
    {
        this.typename = "Debug";
    }

    _GlobalContext.Debug.prototype = new Object();
    var _pDebug = _GlobalContext.Debug.prototype;
    */
    var _pDebug = _GlobalContext.Debug = {};
    _pDebug.isFunction = function (obj)
    {
        return !!(obj && obj.constructor && obj.call && obj.apply);
    };

    _pDebug.showObjProperties = function(obj, level)
    {
        /*
        if (!obj) return;
        if (level === undefined) level = 0;
        if (level > 1) return;

        for (var prop in obj) {
            for (var i = 0, space = ""; i < level; i++) space += "   ";

            //if (isFunction(obj[prop])) continue;
            print(space + prop + " : " + obj[prop]);

            try {
                if (typeof obj[prop] == "object") {
                    this.showObjProperties(obj[prop], level + 1);
                }
            } catch (e) {
                print(space + "   !!Exception!!");
            }
        }
        */

        if (!obj) return;
        if (level === undefined) level = 1;
        //if (level > 1) return;

    	if (obj == _GlobalContext) {
    		typename = "Global";
    		print(typename);
    	}

    	var later = [];
        for (var prop in obj) {
            space = this.getSpace(level);

    		if (obj.hasOwnProperty(prop)) {
    			if (this.isFunction(obj[prop])) {
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
                    this.showObjProperties(obj[prop], level + 1);
                }
            } catch (e) {
                print(space + "   !!Exception!!");
            }
        }

    	for (var i=0; i<later.length; i++) {
    		var prop_obj = later[i];
    		print(prop_obj.typename);
    		if (prop_obj.value == consoleWindow) {
    			print(this.getSpace(prop_obj.level) + "-> [object Window]")
    		}
    		else {
    			this.showObjProperties(prop_obj.value, prop_obj.level, prop_obj.typename);
    		}
    	}
    };

    _pDebug.showWindowProperties = function (w)
    {
        print("Window pos: " + w.x + "," + w.y + " (" + w.width + "x" + w.height + ")");
        //print("   -> client pos: " + (m.x-w.x) + ", " + (m.y-w.y));
        print("   -> handle: " + String(w.handle).toString(16));
        print("   -> text: " + w.text);
        print("   -> class: " + w.class);
        print("   -> visible: " + w.visible);
    };


    _pDebug.getSpace = function (level)
    {
    	var space = "";
    	for (var i = 0; i < level; i++)
    		space += "   ";

    	return space;
    }

    _pDebug.log = function (str)
    {
        print(str);
    };
};

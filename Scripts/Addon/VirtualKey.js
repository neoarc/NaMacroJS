//
// add on module for virtualkey
// 2016.05.12 neoarc (neoarcturus@gmail.com)
//

var _GlobalContext = this;
if (!_GlobalContext.VirtualKey) {
    var _pVirtualKey = _GlobalContext.VirtualKey = _GlobalContext.VK = {};
    function initVirtualKey() {
        var key_table = {
            0x01: "lbutton",
            0x02: "rbutton",

            0x08: ["back", "backSpace"],
            0x09: "tab",

            0x0d: ["enter", "return"],

            0x10: "shift",
            0x11: ["control", "ctrl"],
            0x12: ["menu", "alt"],
            0x13: "pause",
            0x14: ["capital", "capsLock"],
            0x15: ["hangle", "hangul"],

            0x1b: "esc",

            32: "space",
            33: ["prior", "pageUp"],
            34: ["next", "pageDown"],
            35: "end",
            36: "home",

            37: "left",
            38: "up",
            39: "right",
            40: "down",

            0x2c: ["snapshot", "printScreen"],
            0x2d: "insert",
            0x2e: "delete",

            // 0x30 ~ 0x39 : 0 ~ 9

            // 0x41 ~ 0x5a : a ~ z

            0x5b: "lwin",
            0x5c: "rwin",
            0x5d: "apps",
            0x5f: "sleep",

            // 0x60 ~ 0x69 : num0 ~ num9

            0x6a: "multiply",
            0x6b: "add",
            0x6c: "separator",
            0x6d: "subtract",
            0x6e: "decimal",
            0x6f: "devide",

            // 0x70 ~ 0x87 : f1 ~ f24

            0x90: "numlock",
            0x91: "scrolllock",

            0xa0: "lshift",
            0xa1: "rshift",
            0xa2: "lcontrol",
            0xa3: "rcontrol",
            0xa4: ["lmenu", "lalt"],
            0xa5: ["rmenu", "ralt"],


        };

        for (value in key_table) {
            var key_name = key_table[value];
            if (key_name instanceof Array) {
                for (var i=0; i<key_name.length; i++) {
                    _pVirtualKey[key_name[i]] = value;
                }
            }
            else {
                _pVirtualKey[key_name] = value;
            }
        }

        // 0x30 ~ 0x39 : 0 ~ 9
        for (var i=0x30; i<=0x39; i++) {
            _pVirtualKey[String.fromCharCode(i)] = i;
        }

        // 0x41 ~ 0x5a : a ~ z
        for (var i=0x41; i<=0x5a; i++) {
            _pVirtualKey[String.fromCharCode(i).toLowerCase()] = i;
        }

        // 0x60 ~ 0x69 : num0 ~ num9
        for (var i=0x60; i<=0x69; i++) {
            _pVirtualKey["num" + (i-0x60)] = i;
        }

        // 0x70 ~ 0x87 : f1 ~ f24
        for (var i=0x70; i<=0x87; i++) {
            _pVirtualKey["f" + (i-0x70+1)] = i;
        }
    }
    initVirtualKey();
};

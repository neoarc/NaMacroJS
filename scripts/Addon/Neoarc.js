//
// add on module for future use
// 2016.04.27 neoarc (neoarcturus@gmail.com)
//

var _GlobalContext = this;
if (!_GlobalContext.Neoarc) {
    _GlobalContext.Neoarc = function()
    {
        this.typename = "Neoarc";
    }

    _GlobalContext.Neoarc.prototype = new Object();
    var _pNeoarc = _GlobalContext.Neoarc.prototype;

    _pNeoarc.func = function()
    {
        print("Neoarc.func()");
    }

    delete _pNeoarc;
};

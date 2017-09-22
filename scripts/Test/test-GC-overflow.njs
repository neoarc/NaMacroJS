function main()
{
    consoleWindow.visible = true;

    var MyClass = function ()
    {
        this._myProperty = new Array(1024 * 1024); // 1mb
    };

    // there is no garbage... this will crash
    this._c = {};
    for (var i=0; i<1000; i++)
    {
        var c = new MyClass();
        trace(i);
        this._c[i] = c;
    }
}

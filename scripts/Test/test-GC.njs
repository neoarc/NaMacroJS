function main()
{
    consoleWindow.visible = true;

    var MyClass = function ()
    {
        this._myProperty = new Array(1000000);
    };

    // gc - realtime garbage collect
    for (var i=0; i<1000; i++)
    {
        var c = new MyClass();
        trace(i);
    }
}

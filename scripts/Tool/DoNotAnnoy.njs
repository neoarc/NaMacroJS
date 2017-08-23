include("../Addon/Window.js")

var annoyingWindows = [
    // name, width, height, closebutton x, closebutton y, checkConditionFunction, closeFunction
    [ "원격 데스크톱 연결", 570, 173, 424, 144, null, null ],
    [ "원격 데스크톱 연결", 572, 170, 424, 144, null, null ],
    [ ":\\ - 바이러스 검사", 474, 225, 431, 206, null, function (w) {
        w.close();
    }],

	// ex) custom check function, custom close function
    /*
    [ ":\\ - 바이러스 검사", 474, 225, 431, 206,
    function (w) {
        if (w.state != "minimized")
            return true;
        return false;
    },
    function (w) {
        w.state = "minimized";
    }],
    */
];

var appConfig = {
    x: undefined,
    y: undefined,
};

function main()
{
    loadConfig();

    var c = consoleWindow;
    c.visible = true;
    c.width = 400;
    c.height = 80;
    c.move("right","bottom");

    var indicator = ["＼","│","／","ㅡ"];
    var indicatorIdx = 0;
    for ( ; ; )
    {
        c.text = indicator[indicatorIdx++] + " DNA - Do Not Annoy v0.3 ";
        if (indicatorIdx >= indicator.length)
            indicatorIdx = 0;
        checkWindows();
        sleep(500);
    }
}

function checkWindows()
{
    var i=0;
    var len = annoyingWindows.length;
    for (; i<len; i++) {
        var info = annoyingWindows[i];
        w = getFirstWindow(info[0]);
        if (w && w.width == info[1] && w.height == info[2]) {
            if (info[5] == null || info[5](w)) {
                if (info[6])
                    info[6](w);
                else {
                    w.activate();
                    system.mouse.postClick(w.x + info[3], w.y + info[4]);
                }
                trace(info[0] + " closed");
                sleep(500);
            }
        }
    }
}

function loadConfig()
{
    //... todo
}

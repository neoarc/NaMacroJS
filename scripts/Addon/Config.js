var config = {
    name: undefined,
    window: null,
    x: -1,
    y: -1,
    init: function(name, window) {
        this.name = name;
        this.window = window;
        this.restore();
        this.startMonitor();
    },
    restore: function() {
        if (this.name === undefined)
            return;
        var config_filename = this.getConfigFilename();
        var f = new File("./" + config_filename, "r");
        if (f.exist) {
            var buf = f.read();
            //trace("read: \n" + buf);
            try {
                eval("var cfg = " + buf);
                if (this.window != null) {
                    this.window.x = cfg.x;
                    this.window.y = cfg.y;
                }
            }
            catch(e) {
                //trace("restore config failed: " + e.message);
            }
            f.close();
        }
        else {
            //trace("there is no config: " + config_filename);
        }
    },
    startMonitor:  function() {
        var pThis = this;
        setInterval(1000, function() {
            //trace("monitor: " + pThis.name + " / " + pThis.window);
            if (pThis.name === undefined)
                return;
            if (pThis.window == null)
                return;
            var updated = false;
            if (pThis.window.x != pThis.x || pThis.window.y != pThis.y) {
                //trace("window position updated");
                var config_filename = pThis.getConfigFilename();
                var f = new File("./" + config_filename, "w+");
                var cfg = {
                    x: pThis.window.x,
                    y: pThis.window.y
                };
                var str = JSON.stringify(cfg);
                //trace(str);

                f.write(str);
                f.close();

                pThis.x = pThis.window.x;
                pThis.y = pThis.window.y;
            }
        })
    },
    getConfigFilename: function() {
        var config_filename = "config-" + this.name + ".json";
        return config_filename;
    },
};
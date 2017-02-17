//
// Bot API addon based on findImage
// 2017.02.16 neoarc (neoarcturus@gmail.com)
//

var _GlobalContext = this;
if (!_GlobalContext.ImageBot) {
    //
    // usage:
    // var bot = new ImageBot(["img1", "img2", "img3"], "./ImagePath/");
    // bot.run();
    _GlobalContext.ImageBot = function(image_names, image_path)
    {
        if (image_names) {
            if (image_path !== undefined)
                this.image_path = image_path;

            if (image_names.length !== undefined) {
                // it maybe array
                this.image_names = image_names;
                this.loadImages();
            }
            else {
                // ... string, etc..
            }
        }
    };

    _GlobalContext.ImageBot.prototype = new Object();
    var _pImageBot = _GlobalContext.ImageBot.prototype;

    _pImageBot.typename = "ImageBot";
    _pImageBot.image_names = [];
    _pImageBot.image_objects = [];
    _pImageBot.image_path = "./";
    _pImageBot.screen_image = null;
    _pImageBot.image_found_pos = null;
    _pImageBot.capture_target_window = null;
    _pImageBot.capture_target_rect = null;
    _pImageBot.capture_offset = [0, 0];

    _pImageBot.default_accuracy_factor = 0;
    _pImageBot.default_focus_window = null;
    _pImageBot.default_click_sleep = 2000;
    _pImageBot.default_loop_sleep = 1000;

    // base routine.. find, click, wait
    _pImageBot.run = function()
    {
        var title_text = consoleWindow.text;
        var think_count = 0;
        for ( ; ; ) {
            var ret = this.findImageFromList(this.image_objects, this.default_accuracy_factor);
            if (ret) {
                this.log(ret + " Found ==> click");
                this.clickFound(this.default_click_sleep, true);
            }
            else {
                if (this.default_focus_window)
                    this.default_focus_window.activate();

                if (++think_count > 5)
                    think_count = 0;

                var str = "";
                for (var i=0; i<think_count; i++) {
                    str += ". ";
                }
                consoleWindow.text = title_text + " " + str;
            }

            sleep(this.default_loop_sleep);
        }
    };

    // load image list & make image object list
    _pImageBot.loadImages = function()
    {
        this.log("Load Images begin.");
        this.loadImageToBuffer(this.image_names, this.image_objects);
        this.log("Load Images end.");
    };

    // load image list & make image object list helper
    _pImageBot.loadImageToBuffer = function(names, objects)
    {
        for (var i=0; i<names.length; i++)
        {
            var filename = names[i];
            var img = new Image(this.image_path + filename + ".bmp");
            img._id = filename;
            objects[i] = img;
            objects[filename] = img;
            trace("> " + img._id + " / " + img.width + " x " + img.height);
        }

        objects.length = names.length;
    };

    // capture screen
    _pImageBot.captureScreen = function()
    {
        var _s = system.screen;
        if (this.screen_image)
            this.screen_image.reset();

        //_pImageBot.capture_target_window = null;
        //_pImageBot.capture_target_rect = null;
        if (this.capture_target_window) {
            var _w = this.capture_target_window;
            this.screen_image = _s.capture(_w.x, _w.y, _w.width, _w.height);
            this.capture_offset = [_w.x, _w.y];
        }
        else if (this.capture_target_rect) {
            var _r = this.capture_target_rect;
            this.screen_image = _s.capture(r[0], r[1], r[2], r[3]);
            this.capture_offset = [r[0], r[1]];
            //this.screen_image = _s.capture(r.left, r.top, r.width, r.height);
        }
        else {
            this.screen_image = _s.capture(0, 0, _s.width, _s.height);
            this.capture_offset = [0, 0];
        }
    };

    // findImage and return point object
    _pImageBot.findImage = function(img_name, acc_factor)
    {
        if (!this.screen_image)
            throw "this.screen_image is null";

        if (acc_factor === undefined)
            acc_factor = this.default_accuracy_factor;

        // image_found_pos is null if not found
        var pos_in_image = this.screen_image.findImage(this.image_objects[img_name], acc_factor);
        if (pos_in_image) {
            this.image_found_pos = {
                x: pos_in_image.x + this.capture_offset[0],
                y: pos_in_image.y + this.capture_offset[1]
            };
        }
        else {
            this.image_found_pos = null;
        }

        return this.image_found_pos;
    };

    // wait for find image infinite, and click
    _pImageBot.findImageAndClick = function(img_name, delay_after_click, delay_when_rescan, not_found_msg)
    {
        for (; ;) {
            this.captureScreen();
            if (this.findImage(img_name, 50)) {
                this.clickFound(delay_after_click, true);
                return true;
            }
            if (not_found_msg !== undefined)
                this.log(not_found_msg);

            sleep(delay_when_rescan);
        }
    };

    // low level function
    // find all images in list
    _pImageBot.findImageFromList = function(img_objects, acc_factor)
    {
        this.captureScreen();

        var is_found = false;
        for (var i = 0; i < img_objects.length; i++) {
            if (this.findImage(img_objects[i]._id, acc_factor)) {
                return img_objects[i]._id;
            }
        }
        return null;
    };

    _pImageBot.clickFound = function(sleep_time, recapture)
    {
        if (!this.image_found_pos)
            throw "this.image_found_pos is null";

        //postClick(g_pt_found.x, g_pt_found.y);
        var _m = system.mouse;
        _m.click(this.image_found_pos.x, this.image_found_pos.y);
        sleep(sleep_time);

        if (recapture === true)
            this.captureScreen();
    };

    // Utility functions.
    _pImageBot.postClick = function(x, y)
    {
        var _m = system.mouse;
        _m.postLbuttonDown(x, y);
        sleep(10);
        _m.postLbuttonUp(x, y);
    };

    _pImageBot.log = function(str)
    {
        trace("[" + this.getTimeStamp() + "] " + str);
    };

    _pImageBot.getTimeStamp = function(str)
    {
        var currentdate = new Date();
        var datetime = currentdate.getFullYear() + "/"
                    + (currentdate.getMonth()+1)  + "/"
                    + currentdate.getDate() + " @ "
                    + currentdate.getHours() + ":"
                    + currentdate.getMinutes() + ":"
                    + currentdate.getSeconds();

        return datetime;
    };
    delete _pImageBot;
}

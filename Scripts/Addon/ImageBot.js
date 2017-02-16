//
// Bot API addon based on findImage
// 2017.02.16 neoarc (neoarcturus@gmail.com)
//

var _GlobalContext = this;
if (!_GlobalContext.ImageBot) {
    //
    // var bot = new ImageBot(["img1", "img2", "img3"], "./ImagePath");
    // bot.run();
    _GlobalContext.ImageBot = function(image_names, image_path)
    {
        if (image_names) {
            if (image_path !== undefined)
                this.image_path = image_path;

            if (image_names.length !== undefined) {
                // it's array
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

    _pImageBot.default_click_sleep = 2000;
    _pImageBot.default_loop_sleep = 1000;

    // base routine.. find, click, wait
    _pImageBot.run = function()
    {
        for ( ; ; ) {
            var ret = this.findImageFromList(this.image_objects, 0);
            if (ret) {
                this.log(ret + " Found.");
                this.clickFound(this.default_click_sleep, true);
            }
            else {
                consoleWindow.activate();
            }

            sleep(this.default_loop_sleep);
        }
    };

    _pImageBot.loadImages = function()
    {
        this.log("Load Images begin.");
        this.loadImageToBuffer(this.image_names, this.image_objects);
        this.log("Load Images end.");
    };

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

    _pImageBot.captureScreen = function()
    {
        var _s = system.screen;
        if (this.screen_image)
            this.screen_image.reset();
        this.screen_image = _s.capture(0, 0, _s.width, _s.height);
    };

    // findImage and return point object
    _pImageBot.findImage = function(img_name, acc_factor)
    {
        if (!this.screen_image)
            throw "this.screen_image is null";

        if (acc_factor === undefined)
            acc_factor = 0;

        // image_found_pos is null if not found
        this.image_found_pos = this.screen_image.findImage(
            this.image_objects[img_name], acc_factor);

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
                trace(not_found_msg);

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

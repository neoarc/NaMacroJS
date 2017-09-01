//
// for easy UI initialize
// 2017.08.25 neoarc
//
function generateUI(data)
{
    var title = data.title;
    var line_cnt = data.controls.length;

    // create window
    var w = new Window(0, 0, 300, 300);
    w.create();
    w.text = title;
    w.visible = true;

    // calc nc size
    var border_width = (w.width - w.clientWidth) / 2;
    var titlebar_height = (w.height - border_width - w.clientHeight);

    const index_id = 0;
    const index_type = 1;
    const index_width = 2;
    const index_text = 3;

    // calc correct client size
    var line_width_max = 0;
    var default_padding = 10;
    var default_line_height = 30;
    for (var line=0; line<line_cnt; line++) {
        var line_controls = data.controls[line];
        var line_controls_cnt = line_controls.length;

        // calc line width
        var line_width = default_padding;
        for (var i=0; i<line_controls_cnt; i++) {
            var control_data = line_controls[i];
            var control_data2 = control_data.split(":");
            var control_width = parseInt(control_data2[index_width]);
            line_width += control_width;
            line_width += default_padding;
        }

        if (line_width > line_width_max)
            line_width_max = line_width;
    }

    // change window size
    w.width = line_width_max + (border_width * 2);
    w.height = 
        titlebar_height + 
        border_width + 
        (default_line_height * line_cnt) + 
        (default_padding * (line_cnt + 1));

    // let's create controls
    if (data.controls === undefined)
        data.controls = [];
    
    const controls_input = data.controls;
    data.controls = {};
    data.callbacks = new Array(line_cnt);
    var current_y = default_padding;
    for (var line=0; line<line_cnt; line++) {
        var line_controls = controls_input[line];
        var line_controls_cnt = line_controls.length;

        var current_x = default_padding;
        for (var i=0; i<line_controls_cnt; i++) {
            var control_data = line_controls[i];
            var control_data2 = control_data.split(":");

            var control_type = control_data2[index_type];
            var control_width = parseInt(control_data2[index_width]);
            var control_text = control_data2.length >= 4 ? control_data2[index_text] : "";

            line_width += control_width;
            line_width += default_padding;

            var control = w.addControl(
                control_type,
                current_x, 
                current_y,
                control_width,
                default_line_height,
                control_text,
                true,
                undefined
            );
            control.id = control_data2[index_id];
            data.controls[control.id] = control;

            current_x += control_width;
            current_x += default_padding;
        }

        current_x = default_padding;
        current_y += default_line_height;
        current_y += default_padding;
    }
}
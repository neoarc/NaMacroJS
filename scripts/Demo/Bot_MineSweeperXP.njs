 //
// mine sweeper xp bot
// 2016.04.12 neoarc (neoarcturus@gmail.com)
//

var start_pos = [-1, -1];
var stage_size = [0, 0];
var block_size = [0, 0];
var stage_info;
var is_dead = false;
var is_cleared = false;

var stage_img;
var game_info = {
  life: 10, // max try
  try: 0, // tried count
  mine: 0, // remained mine
  last_chance: 0,
  last_block: {
    x: -1,
    y: -1
  },
  fast_mode: false,
};

var log = (game_info.fast_mode ? function() {} : print);

function main() {
  //game_info.fast_mode = true;
  try {
    launch_minesweeper();
    reset_minesweeper();
    calibrate();
    initialize();

    do {
      find_bomb();

      if (is_dead) {
        if (game_info.try < game_info.life) {
          reset_minesweeper();
          initialize();
        }
      } else
        break;
    } while (game_info.try < game_info.life);

    // print game log
    var str = "";
    if (game_info.life > 1)
      str += "Tried: " + game_info.try+"\n";

    if (is_dead) {
      str += "You hit " + game_info.last_chance + "% dead chance.";
      alert(str, "Sorry :p");
    } else {
      str += "Win!";
      alert(str, "Done :)");
    }
  } catch (e) {
    alert("Error: " + e + " / " + typeof(e));
  }

  exit();
}

function launch_minesweeper() {
  consoleWindow.visible = !game_info.fast_mode;

  var win_name = "Minesweeper";
  var ar;
  ar = findWindows(win_name);
  if (ar.length == 0)
    log("Please launch Winmine_xp.exe");

  for (;;) {
    ar = findWindows(win_name);
    if (ar.length > 0) {
      sleep(1000);
      break;
    }

    sleep(100);
  }

  // activate MineSweeper window
  var w = ar[0];
  if (w.x != 0 || w.y != 0) {
    w.move(0, 0);
    sleep(1000);
  }
  w.activate();
  w.topmost = true;
}

//
// let's play new game
//
function reset_minesweeper() {
  is_dead = false;

  // click Game, New
  var m = system.mouse;
  m.click(29, 38);
  m.click(74, 57);
}

//
// initialize game info
//
function initialize() {
  var xs = stage_size[0];
  var ys = stage_size[1];

  stage_info = [];
  for (var x = 0; x < xs; x++) {
    stage_info[x] = [];
    for (var y = 0; y < ys; y++) {
      stage_info[x][y] = "_";
    }
  }

  // hardcode
  if (xs == 9 && ys == 9)
    game_info.mine = 10;
  else if (xs == 16 && ys == 16)
    game_info.mine = 40;
  else if (xs == 30 && ys == 16)
    game_info.mine = 99;
}

//
// calibrating
//
function calibrate() {
  log("- Calibrating...");

  var init_pos = [
    [7, 106],
    [23, 93]
  ];
  var x, y;

  // capture screen
  var img = system.screen.capture(0, 0, 1000, 1000);

  // check start point & size of stage
  var gray, white, size;
  for (var mode = 0; mode < 2; mode++) {
    x = init_pos[mode][0];
    y = init_pos[mode][1];
    size = 0;

    log("   -> check " + ['x', 'y'][mode]);
    for (gray = 0, white = 0; x < 1000 && y < 1000;) {
      var col = img.getPixel(x, y);

      if (col == 0x808080) {
        gray++;
        size++;
      } else if (col == 0xffffff) {
        if (block_size[mode] == 0 && gray == 3) {
          size = 0;
          start_pos[mode] = (mode == 0 ? x : y);
        }
        if (block_size[mode] == 0 && gray == 2) {
          block_size[mode] = size;
          size = 0;
          mode == 0 ? x += 2 : y += 2;
          stage_size[mode] = 1;
        } else if (block_size[mode] != 0) {
          break;
        }
        gray = 0;
        white++;
        size++;
      } else {
        gray = 0;
        white = 0;
        size++;
      }

      if (block_size[mode] == 0) {
        mode == 0 ? x++ : y++;
      } else {
        var _jump = block_size[mode];
        mode == 0 ? x += _jump : y += _jump;
        stage_size[mode] += 1;
      }
    }
  }

  log("   -> start_pos: " + start_pos);
  log("   -> stage_size: " + stage_size);
  log("   -> block_size: " + block_size);

  if (stage_size[0] != 0) {
    var rc = getRealBlockRect(stage_size[0], 0);
    consoleWindow.move(rc.right, 0);

    // vertical maximize
    var x = consoleWindow.x + (consoleWindow.width / 2);
    system.mouse.click(x, 0);
    system.mouse.click(x, 0);
  }
}

function find_bomb() {
  log("- Find Bomb...");
  game_info.try++;

  // change console window text
  var str = "MineSweeperBot by NeoarcMacro";
  if (game_info.life > 1)
    str += " (Try " + game_info.try+"/" + game_info.life + ")";
  consoleWindow.text = str;

  // first step. pick random
  var x, y;
  x = parseInt((Math.random() * 1000) % stage_size[0]);
  y = parseInt((Math.random() * 1000) % stage_size[1]);
  log("   -> random: " + x + "," + y);
  pick_block(x, y);
  print_stage();

  // game loop
  var max_pick = stage_size[0] * stage_size[1];
  for (var i = 0; i < max_pick; i++) {
    if (is_dead) // oops
      return;
    find_pick();

    //sleep(1);
  }
}

//
// parse near of x,y
//
function parse_near(x, y) {
  //log("- Parse Near of " + x + "," + y);

  var list = [{
    x: x,
    y: y
  }];
  while (list.length > 0) {
    var x2, y2;
    x = list[0].x;
    y = list[0].y;

    for (x2 = x - 1; x2 <= x + 1; x2++) {
      for (y2 = y - 1; y2 <= y + 1; y2++) {
        if (x2 < 0 || y2 < 0 || x2 >= stage_size[0] || y2 > +stage_size[1])
          continue;
        if (x2 == x && y2 == y)
          continue;

        var before_val = stage_info[x2][y2];
        parse_block(x2, y2);

        if (before_val == "_" && stage_info[x2][y2] == "P")
          list.push({
            x: x2,
            y: y2
          });
      }
    }

    list.shift();
  }

  //print_stage();
}

//
// print stage status
//
function print_stage() {
  if (game_info.fast_mode)
    return;

  var is_last_block = false;
  var will_be_last_block = false;

  for (var y = 0; y < stage_size[1]; y++) {
    var str = "  ";
    for (var x = 0; x < stage_size[0]; x++) {
      if (x == game_info.last_block.x && y == game_info.last_block.y)
        is_last_block = true;
      if (x + 1 == game_info.last_block.x && y == game_info.last_block.y)
        will_be_last_block = true;
      if (x == 0)
        str += (is_last_block) ? "(" : " ";

      str += stage_info[x][y];
      str += (is_last_block) ? ")" : (will_be_last_block ? "(" : " ");

      is_last_block = false;
      will_be_last_block = false;
    }
    log(str);
  }
}

//
// find block to pick or flag
//
function find_pick() {
  if (find_fix())
    return;

  if (find_flag())
    return;

  // there is no perfect choice, infering.
  var unopened_count = count_unopened();
  if (unopened_count > 0) {
    infer_pick();
  } else {
    is_cleared = true;
  }
}

function find_flag() {
  var found = false;
  for (var y = 0; y < stage_size[1]; y++) {
    for (var x = 0; x < stage_size[0]; x++) {
      if (typeof stage_info[x][y] == "number") {
        var ar = collect_near(x, y, "_");
        var ar2 = collect_near(x, y, "v");

        // find what need flag
        var rule;
        if (ar2.length == 0 && ar.length == 1)
          rule = 1;
        if (ar2.length > 0 && ar2.length == stage_info[x][y])
        ;
        if (ar.length == 1 && stage_info[x][y] - ar2.length == 1)
          rule = 3;
        if (ar.length > 0 && ar.length + ar2.length == stage_info[x][y])
          rule = 4;

        if (rule !== undefined) {
          for (var i = 0; i < ar.length; i++) {
            var item = ar[i];
            log("   -> flag " + item.x + "," + item.y + " (from " + x + "," + y + ") (by rule #" + rule + ")");
            stage_info[item.x][item.y] = "v";

            flag_block(item.x, item.y);
          }
          print_stage();

          // recursive call
          //find_pick();
          return true;
          found = true;
        }
        ////////////////////////////////////////////////////////////////////////////////
      }
    }
  }

  /*
      if (found) {
          print_stage();
          return true;
      }
      */
}

function find_flag_old() {
  for (var y = 0; y < stage_size[1]; y++) {
    for (var x = 0; x < stage_size[0]; x++) {
      if (typeof stage_info[x][y] == "number") {
        var ar = collect_near(x, y, "_");
        var ar2 = collect_near(x, y, "v");

        // find what need flag
        var rule;
        if (ar2.length == 0 && ar.length == 1)
          rule = 1;
        if (ar2.length > 0 && ar2.length == stage_info[x][y])
        ;
        if (ar.length == 1 && stage_info[x][y] - ar2.length == 1)
          rule = 3;
        if (ar.length > 0 && ar.length + ar2.length == stage_info[x][y])
          rule = 4;

        if (rule !== undefined) {
          for (var i = 0; i < ar.length; i++) {
            var item = ar[i];
            log("   -> flag " + item.x + "," + item.y + " (from " + x + "," + y + ") (by rule #" + rule + ")");
            stage_info[item.x][item.y] = "v";

            flag_block(item.x, item.y);
          }
          print_stage();

          // recursive call
          find_pick();
          return true;
        }
        ////////////////////////////////////////////////////////////////////////////////
      }
    }
  }
}

function find_fix() {
  for (var y = 0; y < stage_size[1]; y++) {
    for (var x = 0; x < stage_size[0]; x++) {
      if (typeof stage_info[x][y] == "number") {
        var ar = collect_near(x, y, "_");
        var ar2 = collect_near(x, y, "v");

        var rule;
        ////////////////////////////////////////////////////////////////////////////////
        rule = undefined;

        // already flagged all bomb
        if (ar.length >= 1 && stage_info[x][y] == ar2.length)
          rule = 2;

        if (rule !== undefined) {
          /*
          for (var i=0; i<ar.length; i++) {
              var item = ar[i];
              log("   -> pick " + item.x + "," + item.y + " (from " + x + "," + y + ") (by rule #" + rule + ")");
              pick_block(item.x, item.y);
          }
          print_stage();
          */

          log("   -> fix " + x + "," + y + " (by rule #" + rule + ")");
          fix_block(x, y);
          print_stage();

          return true;
        }
      }
    }
  }
}

//
// count unopened block count
//
function count_unopened() {
  var unopened_count = 0;
  for (var y = 0; y < stage_size[1]; y++) {
    for (var x = 0; x < stage_size[0]; x++) {
      if (stage_info[x][y] == "_") {
        unopened_count++;
      }
    }
  }
  return unopened_count;
}

//
// inferencing, pick a block that has lower bomb chance
//
function infer_pick() {
  log("- Inferencing...");

  var unopened_blocks = [];
  for (var y = 0; y < stage_size[1]; y++) {
    for (var x = 0; x < stage_size[0]; x++) {
      // to all unopened block
      if (stage_info[x][y] == "_") {
        unopened_blocks.push({
          x: x,
          y: y
        });
      }
    }
  }

  var ubs_len = unopened_blocks.length;
  for (var i = 0; i < ubs_len; i++) {
    var ub = unopened_blocks[i];
    var nums = collect_near_number(ub.x, ub.y);
    var nums_len = nums.length;
    if (nums_len == 0) {
      ub.is_neutral = true;
      ub.chance = [game_info.mine / ubs_len];
      continue;
    }
    ub.chance = [];
    for (var j = 0; j < nums_len; j++) {
      var num = nums[j];
      var ar = collect_near(num.x, num.y, "_");
      var ar2 = collect_near(num.x, num.y, "v");

      var chance = (stage_info[num.x][num.y] - ar2.length) / ar.length;
      ub.chance.push(chance);
    }
  }

  var min_ub;
  var min_ubs = [];
  for (var i = 0; i < ubs_len; i++) {
    var ub = unopened_blocks[i];
    var ch_len = ub.chance.length;
    var average = 0;
    for (var j = 0; j < ch_len; j++) {
      var ch = ub.chance[j];
      average += ch;
    }
    ub.chance_average = average /= ch_len;

    //if (!ub.is_neutral)
    //    log((i+1) + "/" + ubs_len + ") chance of (" + ub.x + "," + ub.y + ") is " + parseInt(average*100.0) + "%");

    if (min_ub == undefined) {
      min_ub = ub;
      min_ubs = [ub];
    } else if (ub.chance_average < min_ub.chance_average) {
      min_ub = ub;
      min_ubs = [ub];
    } else if (ub.chance_average == min_ub.chance_average) {
      min_ubs.push(ub);
    }
  }

  shuffle_array(min_ubs);
  min_ub = min_ubs.pop();

  //log("neutral block's chance is " + parseInt((game_info.mine / ubs_len) * 100.0) + "% (remain: " + game_info.mine + ", unopened: " + ubs_len + ")");
  game_info.last_chance = parseInt(min_ub.chance_average * 100.0);
  log("   -> pick " + min_ub.x + "," + min_ub.y + " (chance " + game_info.last_chance + "%)");

  pick_block(min_ub.x, min_ub.y);
  print_stage();
}

function shuffle_array(a) {
  var j, x, i;
  for (i = a.length; i; i -= 1) {
    j = Math.floor(Math.random() * i);
    x = a[i - 1];
    a[i - 1] = a[j];
    a[j] = x;
  }
}

//
// collect blocks those type is number
//
function collect_near_number(x, y) {
  var ar = [];

  var x2, y2;
  for (x2 = x - 1; x2 <= x + 1; x2++) {
    for (y2 = y - 1; y2 <= y + 1; y2++) {
      if (x2 < 0 || y2 < 0 || x2 >= stage_size[0] || y2 > +stage_size[1])
        continue;
      if (x2 == x && y2 == y)
        continue;
      if (typeof stage_info[x2][y2] == "number")
        ar.push({
          x: x2,
          y: y2
        });
    }
  }

  return ar;
}

//
// collect blocks those type is 'f'
//
function collect_near(x, y, f) {
  var ar = [];

  var x2, y2;
  for (x2 = x - 1; x2 <= x + 1; x2++) {
    for (y2 = y - 1; y2 <= y + 1; y2++) {
      if (x2 < 0 || y2 < 0 || x2 >= stage_size[0] || y2 > +stage_size[1])
        continue;
      if (x2 == x && y2 == y)
        continue;
      if (stage_info[x2][y2] == f)
        ar.push({
          x: x2,
          y: y2
        });
    }
  }

  return ar;
}

//
// get count of near boundary
//
function get_near_boundary_count(x, y) {
  var ret = 0;

  var x2, y2;
  for (x2 = x - 1; x2 <= x + 1; x2++) {
    for (y2 = y - 1; y2 <= y + 1; y2++) {
      if (x2 < 0 || y2 < 0 || x2 >= stage_size[0] || y2 > +stage_size[1])
        ret++;
      if (x2 == x && y2 == y)
        continue;
    }
  }

  return ret;
}

//
// parse block if needed
//
function parse_block(x, y) {
  if (x < 0 || y < 0 || x >= stage_size[0] || y >= stage_size[1])
    return;

  var rc = getRealBlockRect(x, y);
  var col, colname;
  var result;

  // already checked
  if (stage_info[x][y] != "_")
    return;

  // pushed
  col = stage_img.getPixel(rc.left + 1, rc.top + 1);
  if (col == 0xc0c0c0) {
    result = "P";

    col = stage_img.getPixel(rc.left + rc.width / 2, rc.top + rc.height / 2);
    switch (col) {
      case 0xff0000:
        result = 1;
        break;
      case 0x008000:
        result = 2;
        break;
      case 0x0000ff:
        result = 3;
        break; // red
      case 0x800000:
        result = 4;
        break;
      case 0x000080:
        result = 5;
        break; // brown
      case 0x808000:
        result = 6;
        break;
      case 0x000000:
      case 0xffffff:
        result = "*";
        is_dead = true;
        break;
      case 0xc0c0c0:
        {
          result = "P";
          col = stage_img.getPixel(rc.left + rc.width / 2 + 1, rc.top + rc.height / 2);
          if (col == 0x000000)
            result = 7;
        }
        break;
      default:
        {
          // for debug!
          var r = parseInt(col / 256 / 256 % 256);
          var g = parseInt(col / 256 % 256);
          var b = parseInt(col % 256);
          log("#" + x + "," + y + ": " + r + "," + g + "," + b);
        }
        break;
    };
  } else if (col == 0x0000ff) {
    result = "*";
    is_dead = true;
  }

  if (result !== undefined)
    stage_info[x][y] = result;
}

//
// pick block, parse stage
//
function pick_block(x, y) {
  var m = system.mouse;
  var x2 = start_pos[0] + (x * block_size[0]) + (block_size[0] / 2);
  var y2 = start_pos[1] + (y * block_size[1]) + (block_size[1] / 2);

  m.lbuttonDown(x2, y2);
  m.lbuttonUp();
  game_info.last_block = {
    x: x,
    y: y
  };

  // stage status changed -> capture
  stage_img = system.screen.capture(0, 0, 1000, 1000);

  // parse, if block type is number then don't need parse whole stage
  parse_block(x, y);

  log("      -> " + stage_info[x][y]);
  if (!is_dead && typeof stage_info[x][y] != "number") {
    parse_near(x, y);
  }
}

//
// flag block
//
function flag_block(x, y) {
  var m = system.mouse;
  var x2 = start_pos[0] + (x * block_size[0]) + (block_size[0] / 2);
  var y2 = start_pos[1] + (y * block_size[1]) + (block_size[1] / 2);

  m.rbuttonDown(x2, y2);
  m.rbuttonUp();
  game_info.last_block = {
    x: x,
    y: y
  };

  game_info.mine--;
}

function fix_block(x, y) {
  var m = system.mouse;
  var x2 = start_pos[0] + (x * block_size[0]) + (block_size[0] / 2);
  var y2 = start_pos[1] + (y * block_size[1]) + (block_size[1] / 2);

  m.lbuttonDown(x2, y2);
  m.rbuttonDown();
  m.lbuttonUp();
  m.rbuttonUp();
  game_info.last_block = {
    x: x,
    y: y
  };

  // stage status changed -> capture
  stage_img = system.screen.capture(0, 0, 1000, 1000);

  ar = collect_near(x, y, "_");
  for (var i = 0; i < ar.length; i++) {
    parse_block(ar[i].x, ar[i].y);
    if (stage_info[ar[i].x][ar[i].y] == "P") {
      parse_near(ar[i].x, ar[i].y);
    }
  }
}

//
// get real screen rect of block
//
function getRealBlockRect(x, y) {
  var x1 = start_pos[0] + (x * block_size[0]);
  var y1 = start_pos[1] + (y * block_size[1]);
  var x2 = start_pos[0] + (x * block_size[0]) + (block_size[0]);
  var y2 = start_pos[1] + (y * block_size[1]) + (block_size[1]);

  return {
    left: x1,
    top: y1,
    right: x2,
    bottom: y2,
    width: x2 - x1,
    height: y2 - y1
  };
}

// under test

// prototype1
system.execute_ = function() {
	var exe = arguments[0];
	var args = "";
	var len = arguments.length;
	for (var i=1; i<len; i++) {
		var arg = arguments[i];
		if (arg.indexOf(" ") > 0)
			arg = "\"" + arg + "\"";

		if (args.length > 0)
			args += " ";
		args += arg;
	}

	system.execute(...)
};

// prototype2
/*
system.execute_({
	exe: "...",
	args: ["..", "...", "..."],
	visible: false
});
*/

// pressCompositeKey
// ex: control+a
// ex: control+a,control+c
// ex: down,down,right
system.keyboard.pressCompositeKey = function(keystr)
{
	var list = keystr.split(",");
	for (var i=0; i<list.length; i++)
	{
		var a = list[i];
		var keys = a.split("+");
		for (var j=0; j<keys.length; j++)
			_k.down(VK[keys[j]]);

		sleep(100);
		for (var j=keys.length-1; j>=0; j--)
			_k.up(VK[keys[j]]);

		// ,
		sleep(300);
	}
};

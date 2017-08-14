// 17.08.13 neoarc

function main()
{
    var ret = fetch("http://google.com", {
		method: "POST"
	});
    alert(ret);
    exit();
}

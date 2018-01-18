// 17.08.13 neoarc

function main()
{
	// NaMacroJS.slack / slackbot url
	var url = "https://namacrojs.slack.com/services/hooks/slackbot?token=ElG94WIcYOZdD7xHZWKTh0PL";
	url += "&channel=%23general";

    var ret = fetch(url, {
		method: "POST",
		//body: "FetchTest from NaMacro"
		body: "한글?" // #FIXME
	});

	// TODO
	// multipart prototype1 ---> from official fetch api
	// slack - image upload (multipart/form-data)
	// -F filename=test.bmp
	// -F file=@D:\private\SlackNotify\test.bmp
	// -F channels=test
	// -F initial_comment=image_upload_test
	// -F token=??
	// "https://slack.com/api/files.upload"
	/*
	var formData = new FormData();
	formData.append("filename", "test.bmp");
	formData.append("file", "d:\private\SlackNotify\test.bmp");
	var ret = fetch(url, {
		method: "POST",
		body:	formData
	});
	*/

	// multipart prototype2:
	/*
	var ret = fetch(url, {
		method: "POST",
		header: "Content-Type: multipart/form-data; boundary=CUSTOM",
		body:	"--CUSTOM\r\n"
				"Content-Type: multipart/octet-stream\r\n"
				"Content-Disposition: file; filename='test'\r\n"
				"\r\nHello World!\n"
				"--CUSTOM--"
	});
	*/

    alert(ret);
    exit();
}

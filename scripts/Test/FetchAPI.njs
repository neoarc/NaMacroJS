// 17.08.13 neoarc

function main()
{
	// NaMacroJS.slack / slackbot url
	var url = "https://namacrojs.slack.com/services/hooks/slackbot?token=ElG94WIcYOZdD7xHZWKTh0PL";
	url += "&channel=%23general";

	// TODO
	// slack - image upload (multipart/form-data)
	// -F filename=test.bmp
	// -F file=@D:\private\SlackNotify\test.bmp
	// -F channels=test
	// -F initial_comment=image_upload_test
	// -F token=??
	// "https://slack.com/api/files.upload"

	// TODO
	// multipart sample:
	// curl -i -H "Content-Type: multipart/form-data; boundary=CUSTOM" -d $'--CUSTOM\r\nContent-Type: multipart/octet-stream\r\nContent-Disposition: file; filename="test"\r\n\r\nHello World!\n--CUSTOM--' "http://localhost:5001/api/v0/add"

    var ret = fetch(url, {
		method: "POST",
		body: "FetchTest from NaMacro"
		//body: "NaMacro에서 테스트"
	});
    alert(ret);
    exit();
}

//
// tts module test
// 2016.05.03 neoarc (neoarcturus@gmail.com)
//
var c = consoleWindow;

function main() {
    try {
        test();
        alert("Done.");
    } catch(e) {
        alert(e, "Exception!");
    }
    exit();
}

function test() {
    ttsSpeak("Hello? this is TTS Module test.");
    ttsSpeak("안녕하세요? TTS 모듈 테스트 입니다.");
}

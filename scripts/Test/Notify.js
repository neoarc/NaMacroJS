//
// Notify
// 2017.01.21 neoarc (neoarcturus@gmail.com)
//
function main()
{
    /*
    notify("Hi! this is notify window.");
    alert("Click to next step.");

    notify("Long text test. 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 ");
    alert("Click to next step.");

    notify("Multiline text test.\nhaha\nhoho\n:)");
    alert("Click to next step.");

    for (var i=0; i<20; i++)
    {
        notify("Notify window test #" + i);
        sleep(100);
    }
    */

    /*
    notify("안녕하세요!");
    alert("Click to exit");
    */

    // Bug 170124
    notify("Message pump not works");
    sleep(10000);

    exit();
}

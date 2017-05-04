
include("../Addon/String.js");

function main()
{
    var c = consoleWindow;
    c.visible = true;

    ///*
    for ( ; ; ) {
        trace(getDateTimeForFilename());
        sleep(1000);
    }
    //*/

    /*
    trace("a".fillZeroForLength(4));
    trace("a".fillZeroForLength(3));
    trace("a".fillZeroForLength(2));
    trace("a".fillZeroForLength(1));
    */
}

function getDateTimeForFilename()
{
    var currentdate = new Date();
    var datetime =
        String(currentdate.getFullYear()).fillZeroForLength(4) +
        String((currentdate.getMonth()+1)).fillZeroForLength(2) +
        String(currentdate.getDate()).fillZeroForLength(2) + "_" +
        String(currentdate.getHours()).fillZeroForLength(2) +
        String(currentdate.getMinutes()).fillZeroForLength(2) +
        String(currentdate.getSeconds()).fillZeroForLength(2);
    return datetime;
}

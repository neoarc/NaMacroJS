function main()
{
    var filename = "./test.json";
    var f = new File(filename, "w");
    o = { x: 5, y: 6 }
    f.write(JSON.stringify(o));
    f.close();
    exit();
}

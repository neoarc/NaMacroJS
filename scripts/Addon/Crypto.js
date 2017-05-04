//
// Crypto Add-on
// 2016.06.18 neoarc (neoarcturus@gmail.com)
//

var _GlobalContext = this;
if (!_GlobalContext.Crypto) {
    var _pCrypto = _GlobalContext.Crypto = {};

    _pCrypto.encrypt = function(str, key, option) {
        var len = str.length;
        var keylen = key.length;
        var ret = "";
        for (var i=0; i<len; i++) {
            var a = str.charCodeAt(i);
            var b = key.charCodeAt(i % keylen);

            ret += String.fromCharCode(a ^ b);
        }

        switch (option) {
            case "csv":
                {
                    var ar = [];
                    for (var i=0; i<len; i++) {
                        var n = ret.charCodeAt(i);
                        ar.push(n);
                    }
                    ret = ar.join(",");
                }
                break;
            default:
                break;
        }

        return ret;
    };

    _pCrypto.decryptCSV = function(str, key) {
        var ar = str.split(",");
        str = "";
        for (var i=0; i<ar.length; i++) {
            str += String.fromCharCode(ar[i]);
        }

        return this.encrypt(str, key);
    };

    delete _pCrypto;
};

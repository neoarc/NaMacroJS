var _pString = String.prototype;
_pString.replaceAll = function(search, replacement) {
    var target = this;
    return target.split(search).join(replacement);
};

_pString.fillZeroForLength = function(length) {
    if (length <= 0)
        return;
    var zero = "";
    for (var i=0; i<length; i++)
        zero += "0";

    var this_string = this.toString();
    return (zero + this_string).substring(this_string.length);
};

delete _pString;

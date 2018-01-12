//alert("JS active!");
function go() {
    var code = document.getElementById("code").value;
    if (!/^[a-zA-Z]*$/g.test(code)) {
        alert("Invalid characters");
        document.getElementById("code").focus();
        return false;
    }
    var step = parseInt(document.getElementById("step").value);
    var plainT = document.getElementById("plainText").value;
    okay = cryptIt(plainT, code, step);
    if (!okay) {
        content = "Well you used something other than alphabets in code. Try again :)!"
    }
    content = "Ciphered Text is " + okay + " ."
    document.getElementById('well').innerHTML = content;
}
function go2(){
    var code = document.getElementById("code").value;
    if (!/^[a-zA-Z]*$/g.test(code)) {
        alert("Invalid characters");
        document.getElementById("code").focus();
        return false;
    }
    var step = parseInt(document.getElementById("step").value);
    var cryptT = document.getElementById("cText").value;
    okay = decryptIt(cryptT, code, step);
    if (!okay) {
        content = "Well you used something other than alphabets in code. Try again :)!"
    }
    content = "Your original text is " + okay + " ."
    document.getElementById('well').innerHTML = content;
}
function cryptIt(str, ciph, step) {
    var final = "";
    var pos = 0;
    var len = ciph.length;
    if(step >= len){
        step = step % len;
    }
    var textLen = str.length;
    for(var i = 0; i < textLen; i++ )
    {
        var ca = str[i].charCodeAt(0);
        if(ca >= 65 && ca <= 90)
        {
            //console.log(ciph[pos]);
            inc = key(ciph[pos]);
            //console.log(inc);
            if((ca + inc) > 90) {
                //console.log(ca+inc);
                final += String.fromCharCode(ca + inc - 26);
                //console.log(final);
            }
            else
                final += String.fromCharCode(ca + inc);
            pos = pos + (1 + step);
            if(pos >= len)
                pos = pos % len;
        }
        else if(ca >= 97 && ca <= 122)
        {
            inc = key(ciph[pos]);
            if((ca + inc) > 122)
                final += String.fromCharCode(ca + inc - 26);
            else
                final += String.fromCharCode(ca + inc);
            pos = pos + (1 + step);
            if(pos >= len)
                pos = pos % len;
        }
        else
        {
            final += String.fromCharCode(ca);
        }
    }
    //console.log(final);
    if(/[a-zA-Z]/.test(final))
        return final;
    else
        return false;
}
function decryptIt(str, ciph, step){
    var final = "";
    var pos = 0;
    var len = ciph.length;
    if(step >= len){
        step = step % len;
    }
    var textLen = str.length;
    for(var i = 0; i < textLen; i++ )
    {
        var ca = str[i].charCodeAt(0);
        if(ca >= 65 && ca <= 90)
        {
            //console.log(ciph[pos]);
            inc = key(ciph[pos]);
            //console.log(inc);
            if((ca - inc) < 90) {
                //console.log(ca+inc);
                final += String.fromCharCode(ca + inc + 26);
                //console.log(final);
            }
            else
                final += String.fromCharCode(ca - inc);
            pos = pos + (1 + step);
            if(pos >= len)
                pos = pos % len;
        }
        else if(ca >= 97 && ca <= 122)
        {
            inc = key(ciph[pos]);
            if((ca - inc) < 97)
                final += String.fromCharCode(ca + inc + 26);
            else
                final += String.fromCharCode(ca - inc);
            pos = pos + (1 + step);
            if(pos >= len)
                pos = pos % len;
        }
        else
        {
            final += String.fromCharCode(ca);
        }
    }
    //console.log(final);
    if(/[a-zA-Z]/.test(final))
        return final;
    else
        return false;
}
function key(c){
    var charCode = c.charCodeAt(0);
    //console.log(charCode);
    if(charCode >= 97 && charCode <= 122)
        return charCode % 97;
    else
        return charCode % 65;
}
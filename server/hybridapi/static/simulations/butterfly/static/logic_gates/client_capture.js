
 function printLog(){
    var myString = breadboard.Update();
    if(breadboard._leds.length){
        var curVal = breadboard._leds[0]._value;
        breadboard._leds[0]._Change(!curVal);
    }
    
    // document.getElementById("protocol").innerHTML = myString;
    // console.log(myString);

    parent.postMessage({
        messageType: "web2sim",
        version: "1.0",
        value: myString
    });

}

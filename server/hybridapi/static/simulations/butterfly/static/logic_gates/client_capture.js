
 function printLog(){
    var myString = breadboard.Update();
    console.log(breadboard.SaveCircuit());
    
    if(myString.includes("Error")){
        document.getElementById("protocol").innerHTML = myString;
    }
    else{
        document.getElementById("protocol").innerHTML = "Ready";
        parent.postMessage({
            messageType: "web2sim",
            version: "1.0",
            value: myString
        }, '*');

    }
    // console.log(myString);

}

window.addEventListener("message", (event) => {
    console.log(event.data.value);
    if (event.data.messageType != "sim2web")
        return;
    if (event.source != parent) {
        console.log("Message from somewhere other than the parent window", event);
        return;
    }

    // document.getElementById("sim2web").innerText = event.data.value;
    console.log(event.data.value);
    var str = event.data.value;
    var tokens = str.split('&');

    var result = {};
    for (var token of tokens) {
        var [key, value] = token.split('=');
        var ledNumber = parseInt(key.substring(3)); // extract the number after "led"
        var ledValue = parseInt(value);
        result[ledNumber] = ledValue;
    }

    console.log(result);
    if(breadboard._leds.length){
        for(const key in result){
            if(key >= breadboard._leds.length){
                break;
            }
            breadboard._leds[key]._Change(result[key]);
        }
    }
}, false);

function closeIt()
{
    var myString = "yLFd0,d1,d2,d3,d4;\n";
    parent.postMessage({
        messageType: "web2sim",
        version: "1.0",
        value: myString
    }, '*');
}
window.onbeforeunload = closeIt;

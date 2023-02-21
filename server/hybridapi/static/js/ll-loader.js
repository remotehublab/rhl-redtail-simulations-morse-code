function requestSim2DutGpios() {
    $.get(window.BASE_URL + "/gpios/sim2dut/?previous_response=" + window.SIM2DUT_STATUS).done(function (data) {
        window.SIM2DUT_STATUS = data.value;

        for (var i = 0; i < data.value.length; i++) {
            if (data.value[i] == "1") {
                $("#gpios-sim2dut-" + i).prop('checked', true);
                $("#gpios-sim2dut-" + i + "-label").text("1");
            } else if (data.value[i] == "0") {
                $("#gpios-sim2dut-" + i).prop('checked', false);
                $("#gpios-sim2dut-" + i + "-label").text("0");

            }
        }
        requestSim2DutGpios();
    });
}

function requestDut2SimGpios() {
    $.get(window.BASE_URL + "/gpios/dut2sim/?previous_response=" + window.DUT2SIM_STATUS).done(function (data) {
        window.DUT2SIM_STATUS = data.value;

        for (var i = 0; i < data.value.length; i++) {
            if (data.value[i] == "1") {
                $("#gpios-dut2sim-" + i).prop('checked', true);
            } else if (data.value[i] == "0") {
                $("#gpios-dut2sim-" + i).prop('checked', false);

            }
            $("#gpios-dut2sim-" + i).trigger("change");
        }

        requestDut2SimGpios();
    });
}

function requestWeb2SimMessages() {
    $.get(window.BASE_URL + "/messages/web2sim/?previous_response=" + encodeURIComponent(window.WEB2SIM_STATUS)).done(function (data) {
        window.WEB2SIM_STATUS = data.value;
        $("#web2sim-messages").val(data.value);
        requestWeb2SimMessages();
    });
}

function requestSim2WebMessages() {
    $.get(window.BASE_URL + "/messages/sim2web/?previous_response=" + encodeURIComponent(window.SIM2WEB_STATUS)).done(function (data) {
        if (window.SIM2WEB_STATUS != data.value) {
            console.log("Sending data with postMessage");
            $("#simulation-iframe")[0].contentWindow.postMessage({
                messageType: "sim2web",
                version: "1.0",
                value: data.value
            });
        }
        window.SIM2WEB_STATUS = data.value;
        $("#sim2web-messages").text(data.value);
        requestSim2WebMessages();
    });
}


function initializeGpios() {
    $(".gpio-dut2sim").each(function () {
        var $checkbox = $(this).find("input");
        var $label = $(this).find("label");
        var number = $checkbox.data("number");

        $checkbox.on("change", function() {
            var currentValue;
            if (this.checked) {
                $label.text("1");
                currentValue = "1";
            } else {
                $label.text("0");
                currentValue = "0";
            }
            if (currentValue != window.DUT2SIM_STATUS[number]) {
                $.ajax({
                    'url': window.BASE_URL + "/gpios/dut2sim/" + number,
                    'method': 'POST',
                    'dataType': 'json',
                    'data': JSON.stringify({value: currentValue == "1"})
                });
            }
        });
    });

    requestDut2SimGpios();
    requestSim2DutGpios();
}

function initializeMessages() {
    $("#web2sim-messages-button").click(function() {
        $.ajax({
            'url': window.BASE_URL + "/messages/web2sim/",
            'method': 'POST',
            'dataType': 'json',
            'data': JSON.stringify({
                value: $("#web2sim-messages").val() 
            })
        });
    });


    console.debug("Added event listener for messages from iframe");

    /**
     * Listen for incoming messages from the visualization iframe.
     */
    window.addEventListener("message", (event) => {

        console.debug("Received message from iframe: ", event.data);

        var simulationIframe = $("#simulation-iframe")[0];
        if (event.data.messageType !== "web2sim") {
            return;
        }

        const expectedOrigin = new URL(simulationIframe.src).origin;

        if (event.origin !== expectedOrigin) {
            console.log("Message from somewhere other than the iframe", event);
            return;
        }

        $.ajax({
            'url': window.BASE_URL + "/messages/web2sim/",
            'method': 'POST',
            'dataType': 'json',
            'data': JSON.stringify({
                value: event.data.value
            })
        });
    }, false);
    

    requestWeb2SimMessages();
    requestSim2WebMessages();
}

function loadSimulation() {
    window.sim2dut = "";
    initializeGpios();
    initializeMessages();
}

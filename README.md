# LabsLand Hybrid API

This API, licensed under GNU AGPL, allows users to develop virtual models for LabsLand remote laboratories.

## Server

The Server Flask application can be used to run a web interface for simulations development and testing.
The server is located in `server` directory and more information can be found in its README at [server/README.md](server/README.md).
Its capabilities are also explained in that README.

## Simulations

Although several components are involved in the Hybrid system, a key component are the simulations.
The simulations are the programs that run in the Pico device and that interact with the real hardware and provide
the simulation. The simulations are developed in C++ and use the hybrid API.

Note that the 3D environments are not part of what is referred to as "simulation" in this document. They are part of the
system and are loaded by the Server's web interface, but separate from the Simulation, and are instead referred to as
"Visualization" or "3D environment".

## Terminology

- *DUT*: Device Under Test. The device that the student controls. 

- *Simulation*: The program meant to run on the Pico device that interacts with the real hardware (DUT) directly. Typically developed in C++. For example,
in the Watertank model, this is the component that reads the actual inputs from the DUT, such as the GPIOs to open water pumps. It also simulates (and keeps track)
internally of the water level of the (simulated) watertank; and it is typically authoritative (while the visualization, discussed next, typically won't be).
It also periodically sends messages to the *Web* and the *Visualization* to update their state. In the case of the Watertank, to inform them of the current
water level so that it can be visualized, for example.

- *Web*: The website (either in the dev server or in the actual lab) that hosts the UI with which the user interacts. This contains the various
buttons and widgets to interact with the lab, plus, sometimes, also contains the 2D or 3D visualization. Often this visualization is not contained
directly, but is instead in an iframe.

- *Visualization* or *3D model*: The 2D or 3D application that handles the visuals and that runs within the Web, either directly or in an iframe.
In the Parking model, for example, the *visualization* is the 3D app that shows the parking. This component communicates with the *simulation* 
through the *Web* only, not directly. Since both the *visualization* and the *web* run remotely, communication is typically relatively slow
(compared to the speed at which the *simulation* can interact with the real hardware). Certain aspects of a given model can actually be simulated in the visualization side
too; and communicated through a message system to the *Simulation*. E.g., in the Parking model, the *Visualization* decides whether a car is over a presence sensor,
and communicates the presence sensor activation to the *Simulation*. Typically the Simulation is more authoritative, though.

## Compiling and running the simulations

For development, testing and debugging purposes the simulations are normally designed to be able to be run not only
on the Pico, which is its final target device, but also on a regular development computer. To compile and run them
you may follow these steps:

Go to cmake-build-debug
```
cd cmake-build-debug
```

Compile the simulations:
```
./compile.sh
```

Run one of them (will block)
```
./hybridapi watertank
```

Alternatively, run it fast simulating pauses:
```
./hybridapi watertank files run-fast
```

## Implementation details

### Visualization

The visualization will typically run in an iframe. It will typically need to communicate with the Simulation, but not directly. Instead, it will communicate through the Web.

To send a message to the Web, that will eventually be sent to the simulation:

```
parent.postMessage({
    messageType: "web2sim",
    version: "1.0",
    value: e
}, "*"),
```

It should also be ready to receive and handle sim2web messages, for which it will need to listen for similar messages from within the iframe.




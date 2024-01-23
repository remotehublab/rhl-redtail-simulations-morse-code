# LabsLand Hybrid API

This API, licensed under GNU AGPL, allows users to develop virtual models for LabsLand remote laboratories.

## Server

The Server Flask application can be used to run a web interface for simulations development and testing.
The server is located in `server` directory and more information can be found in its README at [server/README.md](server/README.md).

## Simulations

Although several components are involved in the Hybrid system, a key component are the simulations.
The simulations are the programs that run in the Pico device and that interact with the real hardware and provide
the simulation. The simulations are developed in C++ and use the hybrid API.

Note that the 3D environments are not part of what is referred to as "simulation" in this document. They are part of the
system and are loaded by the Server's web interface, but separate from the Simulation, and are instead referred to as
"Visualization" or "3D environment".

### Compiling and running the simulations

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


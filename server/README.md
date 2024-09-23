# LabsLand Hybrid API Server

## Purpose

The LabsLand Hybrid API Server is designed to help developers create and test their simulations.

The main features are:
- Runs both the *simulation* and the *visualization*.
- Allows monitoring the communication messages between different components.
- Allows sending specific messages to test & debug.


## Configuration

To test a new simulation, create file in `simulations` that contains something like:

```yaml
name: Watertank
description: This is a watertank simulation
gpios:
  sim2dut:  
    # 3 sensors (low, mid, high)  
    number: 3  
  dut2sim:  
    # 2 actuators (pump1, pump2)  
    number: 4
```

You may specify also "file", "messages -> web2sim -> file", etc.


## Communications

The basic concepts are that there are 2 types of communication: GPIO and messages.

### GPIOS

GPIOs: binary messages (0 or 1), through different input or output channels, named sim2dut and dut2sim:
  1. sim2dut: Simulation to the Device Under Test: there is a number of GPIOs (e.g., 0..2 if number=3) that go from the simulation to the Device Under Test (FPGA, microprocessor... whatever)
  1. dut2sim: Device Under Test to the simulation: there is a number of GPIOs (e.g., 0..3 if number=4) that go from the Device Under Test (FPGA, microprocessor... whatever) to the simulation.

Note 1: these numbers have nothing to do with the real GPIOs in the DUT. For example, maybe in an FPGA, the simulation gpio sim2dut #0 is the GPIO15. This way we abstract from the real board.
Note 2: these numbers are repeated between sim2dut and dut2sim, so make sure not to confuse dut2sim #0 with sim2dut #0.

In the real setup, GPIOs are physically connected from the system running the simulation to the DUT, so it is not subject to network latency issues.

### Messages

Messages are strings (simple, text messages with no newline) send and received from the web to the simulation and from the simulation to the web. Expect these messages to be subject to network latency.

  1. web2sim: from the web interface to the simulation.
  1. sim2web: from the simulation to the web interface.



## Running the simulation

To run the simulation in the Hybrid API Server, you will first need to have successfully compiled the *simulation* itself, the C++ component.
To do so, follow the README in the root folder. Then, you will need to run it in the background:

```
./hybridapi watertank
```

Now, while the simulation itself is running in the background, you may run the dev server:

```
$ . devrc
$ export SIMULATION_CONFIG_FILE=watertank-test.yml
$ flask run
```

## Specific features

In the GPIO sections:

- In the 'Simulation to Device Under Test' section you can monitor the values that the simulation (C++ component) is sending to the hardware through the GPIOs. You cannot directly modify these, they are read-only in the dev server.
- In the 'Device Under Test to Simulation' section you can edit the signals that the DUT would send to the simulation. There is no real DUT here, but it is precisely
emulated by manually setting the signals in the dev server, for testing purposes. It is write-only in this case (since there is no DUT).


In the Messages sections:
- In the 'Simulation2web' section you can monitor the message that the simulation (C++ component) is sending to the Web. That is the message that is typically received
eventually by the Visualization too. It is typically used to keep the Visualization synch'ed, so that it can show the various events at the right time, etc.
- In the 'Web2Simulation' section there is a text box that shows the message that the *Visualization* is sending to the *Simulation* (C++ component). You can also send a specific message that the *Visualization* is not actually sending, for testing purposes. Therefore, this text box is both read and write.

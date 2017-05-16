Kerbal Sim Pit Arduino library
==============================

This is an [Arduino](https://www.arduino.cc/) library and example sketches
for building hardware intended to interface with
[Kerbal Space Program](https://kerbalspaceprogram.com/) via the
[Kerbal Sim Pit](https://bitbucket.org/pjhardy/kerbalsimpit) plugin.

Getting Started
---------------

Refer to the KerbalSimPitHelloWorld example sketch. This sketch opens the
serial port, registers a callback handler, and then periodically sends
EchoRequest packets. When the flight scene is active in-game, the plugin
will respond with EchoResponse packets, which the sketch uses to blink
the LED on pin 13.

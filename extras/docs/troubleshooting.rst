Troubleshooting Guide
=====================

First check : mod installation and configuration
------------------------------------------------

Go to your *KSP/GameData* folder. You should see a *KerbalSimpit* folder if SimPit is installed (else, install it with CKAN).

Open the configuration file *KSP/GameData/KerbalSimpit/PluginData/Settings.cfg*

The line ``PortName = COM3`` should be updated with the correct port number. It should be identical to the one used with the Arduino IDE on Windows. On a Linux or a Mac, the USB port ID will look like /dev/cu.usbmodem401 OR /dev/ttyUSBO.

The line ``Verbose = False`` should be replaced by ``Verbose = True`` to increase the level of detail written in the logs. It is assumed for this guide that Verbose is set to True.

Launch KSP and check in the logs that you can see the following lines (either by opening the in-game console with Alt+F12 or by opening the file *KSP/KSP.log* :
 * ``KerbalSimpit Has put a message into the console!``
 * ``KerbalSimpit: Settings loaded.``
 * ``KerbalSimpit: Using serial polling thread for XXX`` where XXX is your port name (on Windows, it should be the same as the one set in the config file).

Make sure that you installed the SimPit Arduino library in the library folder of Arduino. You should see the *File/Examples* a category called *Kerbal Sim Pit*.

First example : receiving information from KSP
----------------------------------------------

The next step is to check the connection from KSP to the controler. For this, we will use the provided example called *KerbalSimpitAltitudeTrigger*

Open the example with the Arduino IDE, compile it and upload it to the Arduino. Make sure that KSP is not launched when you upload the code. You should see the built-in led turned off.

Then launch KSP and go to the launchpad with a rocket (for instance the 2nd Training called *Basic Flight*).

Before taking off, you should see the folowing in the KSP.log file :

 * The log line ``KerbalSimpit: Opened COM3`` meaning that the connection was started on KSP side.
 * The log line ``ACK received on port COM3. Handshake complete, Arduino library version '1.3.0`` meaning that the connection was established between KSP and the controler.
 * The log line ``KerbalSimpit: Serial port 0 subscribing to channel 24`` meaning that the controler requested an update of the altitude of the craft.
 
You should also see the built-in LED turned on when the connection is established.
 
Then activate the SAS and launch your craft straight up. As soon as its altitude is above 500m, you should see the built-in LED turned off.

After the fligth, exit KSP and go check in your log for any error related to KSP. If no error are found and if you saw the LED turning off when going higher than 500m, congratulation. Your controler can receive information from KSP.

Second example : sending information to KSP
-------------------------------------------

For this test, we will use the provided example called *KerbalSimpitStageDemo*. The objective is to launch your rocket based on an input from the controler. You will need to use a button connected to the pin 2 of the Arduino on one side and on the ground to the other side (see `this example from Arduino <https://www.arduino.cc/en/Tutorial/BuiltInExamples/InputPullupSerial>`_). You can also just plug a wire from the pin 2 to the GND pin to simulate a push on the button if you don't have one.

Start like the previous example :
 * Compile and upload the code while KSP is closed
 * Launch KSP and open the *Basic Flight* training
 * Check for the connection in the KSP log (note : there will be no log indicating a subscription from the controler to a channel).

Then activate the button (or insert the wire). The rocket should take off.

After the fligth, exit KSP and go check in your log for any error related to KSP. If no error are found and your rocket took off, congratulation !! You are now ready to start your own controler.

Common mistakes
---------------

At one point, my controler stopped working
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Please check for any error in the KSP log. If at any point an error occured, the SimPit mod will stop and no information can be shared with the controler, thus giving the impression that the controler is not working.

What does an error look like in the KSP log
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Here is an example.

  [EXC 20:48:03.683] IOException: A device attached to the system is not functioning.
	System.IO.Ports.WinSerialStream.ReportIOError (System.String optional_arg) (at <376e8c39bbab4f1193a569c8dbe4305c>:0)
	System.IO.Ports.WinSerialStream.Write (System.Byte[] buffer, System.Int32 offset, System.Int32 count) (at <376e8c39bbab4f1193a569c8dbe4305c>:0)
	System.IO.Ports.SerialPort.Write (System.Byte[] buffer, System.Int32 offset, System.Int32 count) (at <376e8c39bbab4f1193a569c8dbe4305c>:0)
	(wrapper remoting-invoke-with-check) System.IO.Ports.SerialPort.Write(byte[],int,int)
	KerbalSimpit.Serial.KSPSerialPort.<SerialWriteQueueRunner>b__29_0 () (at <d23bc766ee8645b4813da6de1d115a35>:0)
	KerbalSimpit.Serial.KSPSerialPort.SerialWriteQueueRunner () (at <d23bc766ee8645b4813da6de1d115a35>:0)
	System.Threading.ThreadHelper.ThreadStart_Context (System.Object state) (at <ad04dee02e7e4a85a1299c7ee81c79f6>:0)
	System.Threading.ExecutionContext.RunInternal (System.Threading.ExecutionContext executionContext, System.Threading.ContextCallback callback, System.Object state, System.Boolean preserveSyncCtx) (at <ad04dee02e7e4a85a1299c7ee81c79f6>:0)
	System.Threading.ExecutionContext.Run (System.Threading.ExecutionContext executionContext, System.Threading.ContextCallback callback, System.Object state, System.Boolean preserveSyncCtx) (at <ad04dee02e7e4a85a1299c7ee81c79f6>:0)
	System.Threading.ExecutionContext.Run (System.Threading.ExecutionContext executionContext, System.Threading.ContextCallback callback, System.Object state) (at <ad04dee02e7e4a85a1299c7ee81c79f6>:0)
	System.Threading.ThreadHelper.ThreadStart () (at <ad04dee02e7e4a85a1299c7ee81c79f6>:0)
	UnityEngine.UnhandledExceptionHandler:<RegisterUECatcher>m__0(Object, UnhandledExceptionEventArgs)





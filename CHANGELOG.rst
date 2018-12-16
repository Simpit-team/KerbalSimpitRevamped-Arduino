Kerbal Simpit Changelog
=======================

* **1.2.0**: 2018-12-16

  * Adds command channel for setting SAS mode.
  * Adds enums for rotation and translation masks.

* **1.1.5**: 2018-06-30

  * Better bounds checking on packets read from the game.
  * Use the right type for peri and apo.

* **1.1.2**: 2017-06-28

Fixes to keywords.txt for apsides keywords. Thanks Benji!

* **1.1.1**: 2017-06-26

Pushing a new release as 1.1.0 was incorrectly tagged.

* **1.1.0**: 2017-06-24

  Adds support for these new channels:

  * SOI. A device can request the current SoI the active vessel is in. It's
    returned as an (English-only) string.
  * Vessel rotation. A device can send rotation commands.
  * Vessel translation. A device can send translation commands.
  * Wheel control. A device can send wheel steering and throttle commands.
  * Main throttle. A device can send throttle commands.

  Adds a generic `send()` function that can accept any data type and just
  serialise and send it. No need to manually convert to a byte array.

* **1.0.1**: 2017-05-28

  Minor update changing names to bring in to line with plugin.
* **1.0.0**: 2017-05-22

  Initial 1.0 release.

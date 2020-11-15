Releasing an Arduino library
============================

To release a new version of this library:

* Update the library version in library.properties.
* Update the library version in KERBALSIMPIT_VERSION in src/KerbalSimpit.h
* Update the changelog.
* Commit those changes, and tag the commit with the version number.
  Make sure the version number in the tag is "MAJOR.MINOR.PATCH" -
  numbers separated by periods only. Check the other most recent tags.
* Push commits and then tags.


Development Environment Setup
===============================

To make it easier to have a development and stable version of the library to test with, I would recommend the following procedure:
1. Inside your Arduino Libraries folder, create a KerbalSimpitDev folder.
2. Create a symlink to "KerbalSimpitDev/src" from the "src" folder of your local copy of this repo.
3. Create a symlink to "KerbalSimpitDev/keywords.txt" from the "keywords.txt" file of your local copy of this repo.
4. Copy and paste the "library.properties" file from this repo, into the "KerbalSimpitDev" folder, and open it up.
5. Inside it, edit the name of the library to something like "Kerbal Sim Pit Development"

Then when you want to use the development version of this library in your sketch, all you need to do is include the "Kerbal Sim Pit Development" library into your project, instead of the "Kerbal Sim Pit" library that is available online. Having two libraries with the same name can cause issues, hence the reason I do not recommended symlinking the entire repo into your libraries folder, "library.properties" file and all.
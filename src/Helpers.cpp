/** @file Helpers.cpp
    Helper functions to send outgoing messages.
*/
#include "KerbalSimpit.h"

void KerbalSimpit::activateCAG(byte actiongroup)
{
  _send(CAGACTIVATE_MESSAGE, &actiongroup, 1);
}

void KerbalSimpit::deactivateCAG(byte actiongroup)
{
  _send(CAGDEACTIVATE_MESSAGE, &actiongroup, 1);
}

void KerbalSimpit::toggleCAG(byte actiongroup)
{
  _send(CAGTOGGLE_MESSAGE, &actiongroup, 1);
}

void KerbalSimpit::activateAction(byte action)
{
  _send(AGACTIVATE_MESSAGE, &action, 1);
}

void KerbalSimpit::deactivateAction(byte action)
{
  _send(AGDEACTIVATE_MESSAGE, &action, 1);
}

void KerbalSimpit::toggleAction(byte action)
{
  _send(AGTOGGLE_MESSAGE, &action, 1);
}

void KerbalSimpit::setSASMode(byte mode)
{
  _send(SAS_MODE_MESSAGE, &mode, 1);
}

void KerbalSimpit::setCameraMode(byte mode)
{
  _send(CAMERA_CONTROL_MODE, &mode, 1);
}

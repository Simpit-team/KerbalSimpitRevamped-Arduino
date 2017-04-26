#include "KerbalSimPit.h"

void KerbalSimPit::activateCAG(byte actiongroup)
{
  send(CAGACTIVATE_MESSAGE, &actiongroup, 1);
}

void KerbalSimPit::deactivateCAG(byte actiongroup)
{
  send(CAGDEACTIVATE_MESSAGE, &actiongroup, 1);
}

void KerbalSimPit::toggleCAG(byte actiongroup)
{
  send(CAGTOGGLE_MESSAGE, &actiongroup, 1);
}

void KerbalSimPit::activateAction(byte action)
{
  send(AGACTIVATE_MESSAGE, &action, 1);
}

void KerbalSimPit::deactivateAction(byte action)
{
  send(AGDEACTIVATE_MESSAGE, &action, 1);
}

void KerbalSimPit::toggleAction(byte action)
{
  send(AGTOGGLE_MESSAGE, &action, 1);
}

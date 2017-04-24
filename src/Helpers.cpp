#include "KerbalSimPit.h"

void KerbalSimPit::activateCAG(byte actiongroup)
{
  send(CAGACTIVATE_PACKET, &actiongroup, 1);
}

void KerbalSimPit::deactivateCAG(byte actiongroup)
{
  send(CAGDEACTIVATE_PACKET, &actiongroup, 1);
}

void KerbalSimPit::toggleCAG(byte actiongroup)
{
  send(CAGTOGGLE_PACKET, &actiongroup, 1);
}

void KerbalSimPit::activateAction(byte action)
{
  send(AGACTIVATE_PACKET, &action, 1);
}

void KerbalSimPit::deactivateAction(byte action)
{
  send(AGDEACTIVATE_PACKET, &action, 1);
}

void KerbalSimPit::toggleAction(byte action)
{
  send(AGTOGGLE_PACKET, &action, 1);
}

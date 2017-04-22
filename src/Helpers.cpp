#include "KerbalSimPit.h"

const byte stageFlip[] = {0x01, 0x00};

void KerbalSimPit::stageEvent()
{
  send(STAGE_PACKET, stageFlip, 2);
}

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


#include "KerbalSimPit.h"

const byte stageFlip[] = {0x01, 0x00};

void KerbalSimPit::stageEvent()
{
  send(STAGE_PACKET, stageFlip, 2);
}

void KerbalSimPit::activateCAG(byte *msg)
{
  send(CAGACTIVATE_PACKET, *msg, sizeof(msg));
}

void KerbalSimPit::deactivateCAG(byte *msg)
{
  send(CAGDEACTIVATE_PACKET, *msg, sizeof(msg));
}

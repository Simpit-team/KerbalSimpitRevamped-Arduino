#include "KerbalSimPit.h"

const byte stageFlip[] = {0x01, 0x02};

void KerbalSimPit::stageEvent()
{
  send(STAGE_PACKET, stageFlip, 2);
}

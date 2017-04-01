#include "KerbalSimPit.h"

void KerbalSimPit::stageEvent()
{
  send(STAGE_PACKET, [0x01, 0x02], 2);
}

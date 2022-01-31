#pragma once
#include <stdint.h>
#include "frc_can_defines.hpp"


void(* resetFunc) (void) = 0;

void handleCANPacket(uint8_t* data, int packetSize, APIClass apiClass, uint16_t apiIndex);


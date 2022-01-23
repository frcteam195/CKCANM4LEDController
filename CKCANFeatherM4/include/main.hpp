#pragma once
#include <stdint.h>
#include "frc_can_defines.hpp"

#define PURPLE 0x008800FF
#define RED 0x00FF0000

void(* resetFunc) (void) = 0;

void handleCANPacket(uint8_t* data, int packetSize, APIClass apiClass, uint16_t apiIndex);


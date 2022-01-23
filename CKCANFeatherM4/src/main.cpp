#include <Arduino.h>
#include <CAN.h>
#include "frc_can_defines.hpp"
#include "main.hpp"
#include <Adafruit_NeoPixel.h>

#define DEBUG

#define DEVICE_ID 1

uint32_t CK_CAN_ID;
uint32_t CK_CAN_MASK;

uint32_t counter;
APIClass prevMode;
APIClass currMode;

Adafruit_NeoPixel strip(1, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

void setup()
{
#ifdef DEBUG
	Serial.begin(9600);
#endif	
	pinMode(PIN_CAN_STANDBY, OUTPUT);
	digitalWrite(PIN_CAN_STANDBY, false); // turn off STANDBY
	pinMode(PIN_CAN_BOOSTEN, OUTPUT);
	digitalWrite(PIN_CAN_BOOSTEN, true); // turn on booster

	strip.begin();
	strip.setBrightness(255);

	// start the CAN bus at 1 Mbps
	if (!CAN.begin(1E6))
	{
#ifdef DEBUG
		Serial.println("Starting CAN failed!");
#endif
		//resetFunc();
	}

	CK_CAN_ID = (((uint32_t)DeviceType::IOBreakout << DEVICE_TYPE_OFFSET) & DEVICE_TYPE_MASK) |
				(((uint32_t)Manufacturer::TeamUse << MANUFACTURER_OFFSET) & MANUFACTURER_MASK) |
				(((uint32_t)APIClass::IDLE << API_CLASS_OFFSET) & API_CLASS_MASK) |
				((0x0 << API_INDEX_OFFSET) & API_INDEX_MASK) |
				((DEVICE_ID << DEVICE_NUMBER_OFFSET) & DEVICE_NUMBER_MASK);

	CK_CAN_MASK = DEVICE_TYPE_MASK | MANUFACTURER_MASK | DEVICE_NUMBER_MASK;

	CAN.filterExtended(CK_CAN_ID, CK_CAN_MASK);

	prevMode = APIClass::IDLE;
	currMode = APIClass::COMM_DISCONNECTED;
}

void loop()
{
#ifdef DEBUG
	if (counter++ % 100 == 0)
	{
		Serial.printf("CAN ID: %x\n", CK_CAN_ID);
	}
#endif

	int packetSize = CAN.parsePacket();
	if (packetSize)
	{
		uint8_t receivedData[packetSize];
		for (int i = 0; i < packetSize; i++) {
			receivedData[i] = CAN.read();
		}
		APIClass apiClass = (APIClass)((CAN.packetId() & API_CLASS_MASK) >> API_CLASS_OFFSET);
		uint16_t apiIndex = (uint16_t)((CAN.packetId() & API_INDEX_MASK) >> API_INDEX_OFFSET);
		handleCANPacket(receivedData, packetSize, apiClass, apiIndex);
	}

	switch (currMode)
	{
		case APIClass::IDLE:
		{
			break;
		}
		case APIClass::COMM_DISCONNECTED:
		{
			strip.fill(RED);
			strip.show();
			break;
		}
		case APIClass::COMM_CONNECTED:
		{
			strip.fill(PURPLE);
			strip.show();
			break;
		}
		case APIClass::DRIVER_SIGNAL:
		{
			break;
		}
		case APIClass::ENDGAME:
		{
			break;
		}
		default:
		{
			break;
		}
	}
}

void handleCANPacket(uint8_t* data, int packetSize, APIClass apiClass, uint16_t apiIndex)
{
	switch (apiClass)
	{
		case APIClass::IDLE:
		{
			break;
		}
		case APIClass::COMM_DISCONNECTED:
		{
			break;
		}
		case APIClass::COMM_CONNECTED:
		{
			break;
		}
		case APIClass::DRIVER_SIGNAL:
		{
			break;
		}
		case APIClass::ENDGAME:
		{
			break;
		}
		default:
		{
			break;
		}
	}

	prevMode = currMode;
	currMode = apiClass;
}
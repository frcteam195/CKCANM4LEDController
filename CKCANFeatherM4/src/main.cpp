#include <Arduino.h>
#include <CAN.h>
#include "frc_can_defines.hpp"
#include "main.hpp"
#include <Adafruit_NeoPixel.h>
#include "LEDController.hpp"
#include "drivers/RGBColor.hpp"

//#define DEBUG

#define DEVICE_ID 1

uint32_t CK_CAN_ID;
uint32_t CK_CAN_MASK;

uint32_t counter;
APIClass prevMode;
APIClass currMode;

Adafruit_NeoPixel mNeoPixelStrip(1, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

LEDController mLEDController(&mNeoPixelStrip);

void setup()
{
#ifdef DEBUG
	Serial.begin(9600);
#endif	
	pinMode(PIN_CAN_STANDBY, OUTPUT);
	digitalWrite(PIN_CAN_STANDBY, false); // turn off STANDBY
	pinMode(PIN_CAN_BOOSTEN, OUTPUT);
	digitalWrite(PIN_CAN_BOOSTEN, true); // turn on booster

	mNeoPixelStrip.begin();
	mNeoPixelStrip.setBrightness(255);

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

	prevMode = APIClass::OFF;
	currMode = APIClass::COMM_LOSS;
}

void loop()
{
#ifdef DEBUG
	if (counter++ % 100 == 0)
	{
		//Serial.printf("CAN ID: %x\n", CK_CAN_ID);
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
		APIIndex apiIndex = (APIIndex)((CAN.packetId() & API_INDEX_MASK) >> API_INDEX_OFFSET);
		handleCANPacket(receivedData, packetSize, apiClass, apiIndex);
	}
}

void handleCANPacket(uint8_t* data, int packetSize, APIClass apiClass, APIIndex apiIndex)
{
	switch (apiClass)
	{
		case APIClass::IDLE:
		{
			switch (apiIndex)
			{
				case APIIndex::SET_COLOR:
				{
					if (packetSize >= 4)
					{
						RGBColor rgb;
						rgb.white = data[0];
						rgb.red = data[1];
						rgb.green = data[2];
						rgb.blue = data[3];
#ifdef DEBUG
						Serial.printf("W,R,G,B: %x,%x,%x,%x\n", rgb.white, rgb.red, rgb.green, rgb.blue);
						Serial.printf("uint32_t: %d\n", rgb.getUInt32());
#endif
						mLEDController.setLEDColor(rgb);
					}
					break;
				}
				case APIIndex::SET_BLINK:
				{
					if (packetSize >= 3)
					{
						uint16_t blinkRateMs;
						uint8_t blinkCount;

						blinkRateMs = data[0] | ((data[1] << 8) & 0xFF00);
						blinkCount = data[2];
						mLEDController.configureBlink(blinkCount, blinkRateMs);
					}
					break;
				}
				default:
				{
					break;
				}
			}
			break;
		}
		case APIClass::OFF:
		{
			mLEDController.setLEDOff();
			break;
		}
		case APIClass::FIXED_ON:
		{
			mLEDController.setLEDOn();
			mLEDController.setRequestedState(LEDState::FIXED_ON);
			switch (apiIndex)
			{
				case APIIndex::FLOAT_PIXEL:
				{
					if (packetSize >= 8)
					{
						RGBColor rgb;
						rgb.white = data[0];
						rgb.red = data[1];
						rgb.green = data[2];
						rgb.blue = data[3];
						uint16_t pixelCount = data[4] | ((data[5] << 8) & 0xFF00);
						uint16_t pixelRepeatSpacing = data[6] | ((data[7] << 8) & 0xFF00);
						mLEDController.setFloatPixel(rgb, pixelCount, pixelRepeatSpacing);
					}
					break;
				}
				default:
				{
					break;
				}
			}
			break;
		}
		case APIClass::BLINK:
		{
			mLEDController.setRequestedState(LEDState::BLINK);
			break;
		}
		case APIClass::COMM_LOSS:
		{
			mLEDController.setRequestedState(LEDState::BLINK);
			break;
		}
		case APIClass::COMM_RESTORED:
		{
			mLEDController.setRequestedState(LEDState::FIXED_ON);
			break;
		}
		case APIClass::MORSE:
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
	}

	prevMode = currMode;
	currMode = apiClass;
}
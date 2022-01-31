#pragma once

enum class DeviceType
{
    Broadcast = 0,
    RobotController = 1,
    MotorController = 2,
    RelayController = 3,
    GyroSensor = 4,
    Accelerometer = 5,
    UltrasonicSensor = 6,
    GearToothSensor = 7,
    PowerDistributionModule = 8,
    PneumaticsController = 9,
    Miscellaneous = 10,
    IOBreakout = 11,
    FirmwareUpdate = 0x1F
};

enum class Manufacturer
{
    Broadcast = 0,
    NI = 1,
    LuminaryMicro = 2,
    DEKA = 3,
    CTRElectronics = 4,
    REVRobotics = 5,
    Grapple = 6,
    MindSensors = 7,
    TeamUse = 8,
    KauaiLabs = 9,
    Copperforge = 10,
    PlayingWithFusion = 11,
    Studica = 12,
};

enum class APIClass
{
    IDLE = 0,
    OFF = 1,
    FIXED_ON = 2,
    BLINK = 3,
    COMM_LOSS = 4,
    COMM_RESTORED = 5,
    MORSE = 6,
    DRIVER_SIGNAL = 7,
    ENDGAME =8
};

enum class APIIndex
{
    DEFAULT = 0,
    SET_COLOR = 1,
    SET_BLINK_RATE_MS = 2,
    ///////////////////////////////////////////
    //Support up to a 32 character sequenced morse string, 8 chars per packet
    MORSE_MESSAGE_1 = 1,
    MORSE_MESSAGE_2 = 2,
    MORSE_MESSAGE_3 = 3,
    MORSE_MESSAGE_4 = 4
    ///////////////////////////////////////////
};

#define DEVICE_TYPE_MASK 0x1F000000
#define DEVICE_TYPE_OFFSET 24

#define MANUFACTURER_MASK 0x00FF0000
#define MANUFACTURER_OFFSET 16

#define API_CLASS_MASK 0x0000FC00
#define API_CLASS_OFFSET 10

#define API_INDEX_MASK 0x000003C0
#define API_INDEX_OFFSET 6

#define DEVICE_NUMBER_MASK 0x0000003F
#define DEVICE_NUMBER_OFFSET 0

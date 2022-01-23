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
    COMM_DISCONNECTED = 1,
    COMM_CONNECTED = 2,
    DRIVER_SIGNAL = 3,
    ENDGAME = 4,
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

#ifndef Sensors_hpp
#define Sensors_hpp

#include <Adafruit_AM2320.h>
#include <Adafruit_Sensor.h>
#include <sps30.h>
using namespace std;
#include <vector>

/******************************************************************************
* S E T U P  B O A R D   A N D  F I E L D S
* -------------------------------------------
* please select board on platformio.ini file
******************************************************************************/
#ifdef WEMOSOLED
#define PMS_RX 13  // config for Wemos board & TTGO18650
#define PMS_TX 15  // some old TTGO18650 have PMS_RX 18 & PMS_TX 17
#elif HELTEC
#define PMS_RX 13  // config for Heltec board, ESP32Sboard & ESPDUINO-32
#define PMS_TX 12  // some old ESP32Sboard have PMS_RX 27 & PMS_TX 25
#elif TTGO_TQ
#define PMS_RX 13  // config for TTGO_TQ board
#define PMS_TX 18
#else
#define PMS_RX 17  // config for D1MIN1 board
#define PMS_TX 16
#endif

#define SENSOR_RETRY 1000  // Sensor read retry. (unit chars)

// Sensirion SPS30 sensor
#define SP30_COMMS SERIALPORT2  // UART OR I2C

typedef void (*errorCbFn)(const char *msg);
typedef void (*voidCbFn)();

class Sensors {

   public:

    /// Supported devices. Honeywell and Plantower sensors are similars
    enum SENSOR_TYPE { Honeywell, Panasonic, Sensirion };
    
    /// SPS30 values. Only for Sensirion SPS30 sensor.
    struct sps_values val;

    /// Debug mode for increase verbose.
    bool devmode;

    /// Initial sample time for all sensors
    int sample_time = 5;

    void init(int pms_type = -1, int pms_rx = PMS_RX, int pms_tx = PMS_TX);
    void loop();
    bool isDataReady();
    bool isPmSensorConfigured();
    void setSampleTime(int seconds);
    void setOnDataCallBack(voidCbFn cb);
    void setOnErrorCallBack(errorCbFn cb);
    void setDebugMode(bool enable);
    int getPmDeviceTypeSelected();
    String getPmDeviceSelected();

    uint16_t getPM1();
    uint16_t getPM25();
    uint16_t getPM10();

    float getTemperature();
    float getHumidity();
    float getPressure();
    float getAltitude();
    float getGas();

    String getFormatTemp();
    String getFormatPress();
    String getFormatHum();
    String getFormatGas();
    String getFormatAlt();

    String getStringPM1();
    String getStringPM25();
    String getStringPM10();

   private:

    /// Sensiriom library
    SPS30 sps30;
    /// Generic PM sensors Serial.
    Stream *_serial;
    /// Callback on some sensors error.
    errorCbFn _onErrorCb;
    /// Callback when sensor data is ready.
    voidCbFn _onDataCb;

    String device_selected;
    int device_type = -1;
    bool dataReady;

    uint16_t pm1;   // PM1
    uint16_t pm25;  // PM2.5
    uint16_t pm10;  // PM10

    float humi = 0.0;  // % Relative humidity
    float temp = 0.0;  // Temperature (°C)
    float pres = 0.0;  // Pressure
    float alt = 0.0;
    float gas = 0.0;

    void restart();
    void am2320Init();
    void am2320Read();
    bool pmSensorInit(int pms_type, int rx, int tx);
    bool pmSensorAutoDetect(int pms_type);
    bool pmSensorRead();
    bool pmGenericRead();
    bool pmPanasonicRead();
    bool pmSensirionRead();
    void onPmSensorError(const char *msg);
    void printValues();
    bool pmSensirionInit();
    void pmSensirionErrtoMess(char *mess, uint8_t r);
    void pmSensirionErrorloop(char *mess, uint8_t r);
    void getSensirionDeviceInfo();
    String hwSerialRead();
    void DEBUG(const char * text, const char * textb = "" );

// @todo use DEBUG_ESP_PORT ?
#ifdef WM_DEBUG_PORT
    Stream &_debugPort = WM_DEBUG_PORT;
#else
    Stream &_debugPort = Serial;  // debug output stream ref
#endif

};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SENSORSHANDLER)
extern Sensors sensors;
#endif

#endif

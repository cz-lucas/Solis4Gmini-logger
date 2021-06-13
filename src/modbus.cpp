#include "modbus.h"

// SW-Serial object
SoftwareSerial rs485(rs485_RX, rs485_TX, false); // RX, TX, Invert signal

// instantiate ModbusMaster object
ModbusMaster node;

#ifdef otherNode

ModbusMaster node2;
int node2Output[oNumReg];
bool node2Reachable = false;
#endif

void postTransmission();
void preTransmission();

float _power = 0.00;
float _energyToday = 0.00;
float _temperature = 0.00;

float _dc_u = 0.00;
float _dc_i = 0.00;

float _ac_u = 0.00;
float _ac_i = 0.00;
float _ac_f = 0.00;

bool _softRun = true;

bool reachable = false;
bool reachable_flag__lst = true;

inverter::inverter()
{
}

otherModbusDevice::otherModbusDevice()
{
}

/*
Requests data from the inverter
*/
bool inverter::request()
{
    uint8_t inverterOffCounter = 0;

    Serial.println("------------------------------------------");
    uint8_t result;
    reachable = true;
    digitalWrite(2, LOW);
    result = node.readInputRegisters(3005, 1);
    if (result == node.ku8MBSuccess)
    {
        //Serial.print("P: ");

        if (_ac_i != 0.0)
        { // Detect softrun
            _power = node.getResponseBuffer(0x00);
            _softRun = false;
        }
        else
        {
            _power = 0.0;
            _softRun = true;
        }
    }
    else
    {
        Serial.println("+ GET POWER FAILED");
        _power = -1.0;
        //reachable = false;
        Serial.println("Inverter not reachable");
        inverterOffCounter++;
        //return false;
    }
    delay(readDelay);
    digitalWrite(2, HIGH);

    result = node.readInputRegisters(3014, 2);
    if (result == node.ku8MBSuccess)
    {
        _energyToday = node.getResponseBuffer(0x00) / 10.0;
    }
    else
    {
        Serial.println("+ GET ENERGY FAILED");
        inverterOffCounter++;
    }
    delay(readDelay);
    digitalWrite(2, LOW);

    result = node.readInputRegisters(3021, 4);
    if (result == node.ku8MBSuccess)
    {
        _dc_u = node.getResponseBuffer(0x00) / 10.0;
        _dc_i = node.getResponseBuffer(0x01) / 10.0;
    }
    else
    {
        Serial.println("+ GET DC FAILED");
        inverterOffCounter++;
        _dc_u = -1;
        _dc_i = -1;
    }
    delay(readDelay);
    digitalWrite(2, HIGH);

    result = node.readInputRegisters(3035, 10);
    if (result == node.ku8MBSuccess)
    {
        _ac_u = node.getResponseBuffer(0x00) / 10.0;
        _ac_i = node.getResponseBuffer(0x03) / 10.0;
        _ac_f = node.getResponseBuffer(0x07) / 100.0;

        _temperature = node.getResponseBuffer(0x06) / 10.0;
    }
    else
    {
        Serial.println("+ GET AC FAILED");
        inverterOffCounter++;
        _ac_u = -1;
        _ac_i = -1;
        _ac_f = -1;
        _temperature = -1;
    }
    digitalWrite(2, LOW);
    delay(50);
    digitalWrite(2, HIGH);
    //    delay(400);

    Serial.println("------------------------------------------");

    if(inverterOffCounter > 2){
        reachable = false;
    }else{
        reachable = true;
    }
    return reachable;
}

/*
Returns data from AC
@param _ac_u AC voltage (float)
@param _ac_i AC current (float)
@param _ac_f AC frequency (float)
*/
void inverter::getAC(float *__ac_u, float *__ac_i, float *__ac_f)
{
    *__ac_u = _ac_u;
    *__ac_i = _ac_i;
    *__ac_f = _ac_f;
}

/*
Returns data from DC
@param _dc_u DC voltage (float)
@param _dc_i DC current (float)
*/
void inverter::getDC(float *__dc_u, float *__dc_i)
{
    *__dc_u = _dc_u;
    *__dc_i = _dc_i;
}

/*
Returns temperature
@param _temperature (float)
*/
void inverter::getTemperature(float *__temperature)
{
    *__temperature = _temperature;
}

/*
Returns power
@param _power (float)
*/
void inverter::getPower(float *__power)
{
    *__power = _power;
}

/*
Returns energy today
@param _energyToday (float)
*/
void inverter::getEnergyToday(float *__energyToday)
{
    *__energyToday = _energyToday;
}

/*
Is inverter reachable
*/
bool inverter::isInverterReachable()
{
    return reachable;
}

/*
Is inverter reachable (Last state)
*/
bool inverter::getIsInverterReachableFlagLast()
{
    return reachable_flag__lst;
}

bool inverter::setIsInverterReachableFlagLast(bool _value)
{
    reachable_flag__lst = _value;
    return reachable_flag__lst;
}

bool inverter::isSoftRun()
{
    return _softRun;
}

void inverter::begin()
{
    rs485.begin(9600);

    pinMode(MAX485_DE, OUTPUT);
    // Init in receive mode
    digitalWrite(MAX485_DE, 0);

    node.begin(slaveID_inverter, rs485); // SlaveID,Serial
    // Callbacks allow us to configure the RS485 transceiver correctly
    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);
}

/////////////////////////////////////////////////////

void preTransmission()
{
    digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
    digitalWrite(MAX485_DE, 0);
}

//////////////////////////////////////////////////////

void otherModbusDevice::begin()
{

#ifdef otherNode


    digitalWrite(MAX485_DE, 0);

    node2.begin(oSlaveID, rs485); // SlaveID,Serial
    // Callbacks allow us to configure the RS485 transceiver correctly
    node2.preTransmission(preTransmission);
    node2.postTransmission(postTransmission);

#endif
}

bool otherModbusDevice::request()
{

#ifdef otherNode
    uint8_t result;

    result = node2.readHoldingRegisters(oStartReg, oNumReg);
    if (result == node.ku8MBSuccess)
    {
        for (uint16_t x = 0; x < oNumReg; x++)
        {
            node2Output[x] = node2.getResponseBuffer(x);
            /*Serial.print("## ");
            Serial.print(x);
            Serial.print("\t");
            Serial.println(node2Output[x]);*/
        }
        node2Reachable = true;
    }
    else
    {
        node2Reachable = false;
        return false;
    }
    return true;
#else
    return true;
#endif
}

int *otherModbusDevice::getNode2Data()
{
#ifdef otherNode

    return node2Output;
#else
    return 0;
#endif
}

bool otherModbusDevice::reachable()
{
#ifdef otherNode

    return node2Reachable;
#else
    return true;
#endif
}
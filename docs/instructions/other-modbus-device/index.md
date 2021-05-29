# Reading another device over modbus
This function is useful if want to get data from another device like a powermeter or an arduino.

#### Note:
* At the moment only reading of holding registers is supported.
* The device has to use 9600 Baud.

## How to enable/configure it
1. Open the config.h file and uncomment the line ``` #define otherNode ```
2. Change the slave ID.
3. Set the start-register.
4. Set the number how many register should be read.
5. Set the interval.
6. ***(Optional)*** Set the mqtt-topic.
7. Upload it on your controller.

## HTTP API
#### Endpoint:
``` <IP-from-your-ESP>/api/otherNode/holdingRegisters```  
Example output:  
``` 0,1,2 ```

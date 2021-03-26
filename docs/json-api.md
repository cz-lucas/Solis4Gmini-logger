# Json API
You can get the data from your inverter also with this api.
The data is always updated when the inverter is polled.

## Endpoints

### All Data
```
<IP-from-your-ESP>/api/all.json
```
Response:
```
{
	"power": 270.00,
	"energyToday": 0.80,
	"isOnline": 1,
	"dc_u": 70.60,
	"dc_i": 4.60,
	"ac_u": 236.10,
	"ac_i": 1.30,
	"ac_f": 50.00,
	"temperature": 15.00
}
```
### Only Power
```
<IP-from-your-ESP>/api/power.json
```
Response:
```
{
	"power": 290.00,
	"energyToday": 0.80,
	"isOnline": 1
}
```

### Restart ESP
```
<IP-from-your-ESP>/api/restart
```

## Examples
### Curl
Get all data
```
curl -X GET <IP-from-your-ESP>/api/all.json
```

Get only the power
```
curl -X GET <IP-from-your-ESP>/api/power.json
```

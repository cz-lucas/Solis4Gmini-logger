# Json API
You can get the data from your inverter also with this api.
The data is always updated when the inverter is polled.

## Endpoints

### All Data
```
<IP-from-your-ESP>/api/all.json
```
Response:
{
	"power": 290.00,
	"energyToday": 0.80,
	"isOnline": 1
}

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
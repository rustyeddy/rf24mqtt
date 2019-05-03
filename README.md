# RF24 Radio to MQTT Gateway

This is a a simple minded RF24 radio to MQTT gateway. It expects messages 
to be strings of the form 'chan:value...' where chan is the _MQTT Topic_
separated from the value by a colon.  That makes for an easily parsable 
string.

The resulting string is an mosquitto_pub command that will send the data 
to the appropriate MQTT channel or topic.

## TODO

### Code Native MQTT Client

Code our own native MQTT client.

#ifndef __RF24MQTT_H__
#define __RF24MQTT_H__

#include <mosquitto.h>

// Make this a configurable option
#define MQTT_HOST "10.24.0.112"
#define MQTT_PORT 1883

void mqtt_setup(char *clientid);

static struct mosquitto *mosq; // a single global


//void connect_callback(struct mosquitto *mosq, void *obj, int result);

#endif // __RF24MQTT_H__

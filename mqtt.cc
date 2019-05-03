#include <signal.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "mqtt.h"

static int run = 1;

void handle_signal(int s)
{
    run = 0;
}

void connect_callback(struct mosquitto *mosq, void *obj, int result)
{
    printf("connect callback");
}

void message_callback(struct mosquitto *mosq, void *obj,
		      const struct mosquitto_message *message)
{
    printf("message callback");
}

void mqtt_setup(char *clientid)
{
    //signal(SIGINT, handle_signal);
    // signal(SIGTERM, handle_signal);
	
    mosquitto_lib_init();
    mosq = mosquitto_new(clientid, true, NULL);
    assert(mosq);
	
    mosquitto_connect_callback_set(mosq, connect_callback);
    mosquitto_message_callback_set(mosq, message_callback);

    int rc = mosquitto_connect(mosq, MQTT_HOST, MQTT_PORT, 60);
    printf("Mosquitto connect return value: %d\n", rc);
    assert(rc == MOSQ_ERR_SUCCESS);
    
    mosquitto_subscribe(mosq, NULL, "#", 0);
}

void mqtt_loop()
{
    while(run) {
	int rc = mosquitto_loop(mosq, -1, 1);
	if(run && rc) {
	    sleep(20);
	    mosquitto_reconnect(mosq);
	}
    }
    mosquitto_destroy(mosq);
}

void mqtt_cleanup()
{			    
    mosquitto_lib_cleanup();
}

void mqtt_run()
{
    mqtt_setup((char *)"chungking");
    mqtt_loop();
    mqtt_cleanup();
}

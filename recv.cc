#include <stdio.h>
#include <stdlib.h>
#include <RF24/RF24.h>
#include "mqtt.h"

void process_incoming(char *, int);


// Does the speed effect the messages?
RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24,
	   BCM2835_SPI_SPEED_8MHZ);

//const uint8_t data_pipe[6] = "00001";
const uint8_t data_pipe[6] = "nan01";

// Get the radio ready to roll
void rf24_setup(void) {
    
    radio.begin();
    radio.setRetries(15, 15);
    radio.setPALevel(RF24_PA_MAX);
    radio.openReadingPipe(1, data_pipe);
    radio.startListening();
    
}

int main(int argc, char** argv) {

    rf24_setup();

    // Accept RF channel from cmdline arg
    mqtt_setup((char *)"rpi99");

    printf("Ready to receive data ... \n");
    while (true) {
	if (radio.available()) {
	    int payload_size = radio.getDynamicPayloadSize();
	    if (payload_size > 1) {

		char* payload = new char[payload_size + 1];
		radio.read(payload, payload_size);
		payload[payload_size] = '\0';
		printf("New message size %d ", payload_size);
		printf(" -> %s\n", payload);
		process_incoming(payload, payload_size);
	    }
	}
    }
}

void print_cmds(char *cmds[], int cnt)
{
    printf("Commands cnt: %d are: \n", cnt);
    for (int i = 0; i < cnt; i++) {
	printf("\t%s\n", cmds[i]);
    }
    printf("\n");
}

void process_incoming(char *payload, int size)
{
    /*
     * Roll through the string replacing colons with string termination
     * characters.
     */
    int cmd = 0;
    char *cmds[10];
    bool newcmd = true;
    char cpy[80]; // arbitrary

    strncpy(cpy, payload, strlen(payload));

    // the string best have a null terminator, that is how we'll exit
    // this thing.  We'll put a numeric limit on it.
    for ( char *p = payload; *p && (p - payload) < size; p++) {

	// see if we need to set up a new command
	if (newcmd) {
	    // verbose logging printf("  new cmd %d -> %s\n", cmd, p);
	    cmds[cmd] = p;
	    newcmd = false;
	}

	// check for a colon as a separator and replace with a
	// line termination character.
	if ( *p == ':' ) {
	    *p = 0;
	    // printf("  Found the end of the command: %s\n", cmds[cmd]);
	    // now increate cmd command and prepare for the next cmd
	    cmd++;
	    newcmd = true;
	    cmds[cmd] = 0;
	}
    }

    char *chan = cmds[0];
    //char *mnum = cmds[1];
    //int msg = atoi(mnum);

    // replace this with a call to the library !!!
    char command[254];
    sprintf(command, "/usr/bin/mosquitto_pub -h 10.24.0.112 -t %s -m %s\n",
	    chan, cpy);

    system(command);

    if (size > 0) return;
    
    printf("MQTT publish attempt to %s - len %d payload %s\n",
	   chan, strlen(payload), payload);
    int rc = mosquitto_publish(mosq, NULL, chan,
			       strlen(payload), payload, 1, false);
    // make this a log item
    if (rc != MOSQ_ERR_SUCCESS) {
	printf("!! FAILED %d to publish string %s\n", rc, payload);
    }
}



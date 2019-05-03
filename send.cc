#include <iostream>
#include <RF24/RF24.h>
#include <string.h>

RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);
uint8_t data_pipe[6] = "00001";

void rf24_setup(const uint8_t wp[6]) {
    radio.begin();
    radio.setRetries(15, 15);
    radio.setPALevel(RF24_PA_LOW);
    radio.openWritingPipe(wp);
}

int main(int argc, char** argv) {
    
    if (argc > 1) {
	char *p = argv[1];
	strncpy((char *)data_pipe, p, 6);
    }
    
    // Setup the rf24 for writting 
    rf24_setup(data_pipe);

    if (argc != 2) {
	std::cout << "Usage: " << argv[0] << " <message to send>";
	return -1;
    }

    char* data = argv[1];
    radio.write(data, strlen(data) + 1);
    std::cout << "Data Sent" << std::endl;
}

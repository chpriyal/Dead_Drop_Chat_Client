#include "util.hpp"



int main() {
	// Initialize config and local variables
	struct config config;
	init_config(&config);
	int sending = 1;

	bool sequence[8] = {1,0,1,0,1,0,1,1};

	printf("Please type a message.\n");
	while (sending) {

		// Get a message to send from the user
		printf("< ");
		char text_buf[128];
		fgets(text_buf, sizeof(text_buf), stdin);

		// Indicate termination if input message is "exit"
		if (strcmp(text_buf, "exit\n") == 0) {
			sending = 0;
		}

		// Convert that message to binary
		char *msg = string_to_binary(text_buf);

		// Send a '10101011' bit sequence tell the receiver
		// a message is going to be sent
		for (int i = 0; i < 8; i++) {
			send_bit(sequence[i], &config);
		}

		// Send the message bit by bit
		size_t msg_len = strlen(msg);
		for (int ind = 0; ind < msg_len; ind++) {
			if (msg[ind] == '0') {
				send_bit(false, &config);
			} else {
				send_bit(true, &config);
			}
		}

	}

	printf("Sender finished\n");
	return 0;
}
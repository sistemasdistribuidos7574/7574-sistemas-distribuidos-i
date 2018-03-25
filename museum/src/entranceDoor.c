#include <stdio.h>
#include <unistd.h>
#include "message.h"
#include "include/msg.h"
#include "include/logger.h"



int main(int argc, char* argv[]) {
	safelog("Entrance door created.");
	
	int req_queue_id;
    sscanf(argv[1], "%d", &req_queue_id);
    int req_queue = getmsg(req_queue_id);

	int resp_queue_id;
    sscanf(argv[2], "%d", &resp_queue_id);
    int resp_queue = getmsg(resp_queue_id);

	if (req_queue < 0) {
		safeperror("Error getting req message queue.");
		exit(-1);
	}
	if (resp_queue < 0) {
		safeperror("Error getting resp message queue.");
		exit(-1);
	}



	while (true) {
		message_t msg;
		rcvmsg(req_queue, &msg, sizeof(message_t), 0);
		safelog("Processing request...");
		sleep(1);

		//TODO: Check shm if theres space.

		msg.type = ACCEPT;
		safelog("Finished processing request");
		sendmsg(resp_queue, &msg, sizeof(message_t));
	}

}
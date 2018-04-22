//
// Created by gaston on 22/04/18.
//

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "../common/log/log.h"
#include "../common/ipc/msg_queue.h"
#include "broker_handler.h"

void create_broker_resp_handler();

int init_msg_queue();

int main(int argc, char* argv[]) {
    log_info("Starting local broker.");

    // TODO open socket and send to resp process.
    // TODO create shm for incoming msgs and send to resp process.
    create_broker_resp_handler();

    int req_queue = init_msg_queue();

    sleep(10);
    delmsg(req_queue);
}

int init_msg_queue() {
    if (creamsg(BROKER_REQ_MSG) < 0) {
        log_error("Error creating req msg queue.");
        perror("Error");
        exit(-1);
    }
    int req_queue = getmsg(BROKER_REQ_MSG);
    if (req_queue < 0) {
        log_error("Error getting req message queue.");
        perror("Error");
        exit(-1)    ;
    }
    return req_queue;
}

void create_broker_resp_handler() {

    pid_t pid = fork();
    if (pid < 0) {
        log_error("Error forking broker resp handler");
        perror("Error");
        exit(-1);
    }
    if (pid == 0) {
        execl("./broker_resp_handler", "./broker_resp_handler", (char*)NULL);
        log_error("Error executing broker resp handler");
        perror("Error");
        exit(-1);
    }

}
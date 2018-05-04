//
// Created by gaston on 22/04/18.
//

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "../common/log/log.h"
#include "../common/message.h"
#include "../common/ipc/sig.h"


bool graceful_quit = false;

void SIGINT_handler(int signum) {
    if (signum != SIGINT) {
        log_warn("WARNING: Unknown signal received: %d.", signum);
    } else {
        log_debug("SIGINT received, aborting.");
        graceful_quit = true;
    }
}

pid_t create_req_handler(int socket) {
    pid_t pid = fork();
    if (pid < 0) {
        log_error("Error forking resp handler");
        exit(-1);
    }
    if (pid == 0) {
        char socket_str[3];
        snprintf(socket_str, 3, "%d", socket);
        execl("./resp_handler", "./resp_handler", socket_str, (char*)NULL);
        log_error("Error executing resp handler");
        exit(-1);
    }
    return pid;
}

int main(int argc, char* argv[]) {
    register_handler(SIGINT_handler);
    log_info("Starting request handler.");
    if (argc < 2) {
        log_error("No client socket parameter received.");
        exit(-1);
    }
    int client_socket;
    sscanf(argv[1], "%d", &client_socket);

    // TODO fork resp_handler
    pid_t resp_handler_pid = create_req_handler(client_socket);

    while(!graceful_quit) {
        char buffer[sizeof(msg_t) / sizeof(char)];
        // TODO check bytes read (if not all, retry).
        log_info("Waiting for messages from client...");
        ssize_t bytes = read(client_socket, buffer, sizeof(buffer) / sizeof(char));
        if(graceful_quit) {
            break;
        }
        if (bytes < 0) {
            log_error("Error reading client socket.");
            break;
        } else if (bytes == 0) {
            log_error("Lost connection from client.");
            break;
        }
        msg_t* msg = (msg_t*) buffer;
        log_info("Received type: %d", msg->type);
        // TODO send msg to worker.
    }


    log_info("Stopping request handler.");
    if (close(client_socket) < 0) {
        log_error("Error closing client socket.");
    }

}
//
// Created by gaston on 25/05/18.
//


#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "../common/ipc/sig.h"
#include "../common/log/log.h"
#include "db_message.h"
#include "../common/ipc/socket.h"

bool graceful_quit = false;

void SIGINT_handler(int signum) {
    if (signum != SIGINT) {
        log_warn("WARNING: Unknown signal received: %d.", signum);
    } else {
        log_debug("SIGINT received, aborting.");
        graceful_quit = true;
    }
}

void safe_exit(int client_socket, int error) {
    log_debug("Closing socket");
    if (close(client_socket) < 0) {
        log_error("Error closing client socket.");
    }
    exit(error);
}

void process_create(int socket, db_msg_t *msg) {
    //TODO create in db and send response to client
}

void process_subscribe(int socket, db_msg_t *msg) {
//TODO
}

void process_get_subscribed(int socket, db_msg_t *msg) {
//TODO
}

void process_delete(int socket, db_msg_t *msg) {
//TODO
}

void process_message(int client_socket, db_msg_t *msg) {
    log_info("Received type: %d", msg->type);
    if (msg->type == DB_CREATE) {
        process_create(client_socket, msg);
    } else if (msg->type == DB_SUBSCRIBE) {
        process_subscribe(client_socket, msg);
    } else if (msg->type == DB_GET_SUBSCRIBED) {
        process_get_subscribed(client_socket, msg);
    } else if (msg->type == DB_DELETE) {
        process_delete(client_socket, msg);
    } else {
        log_warn("Unexpected msg type received %d", msg->type);
    }
}

int main(int argc, char* argv[]) {
    register_handler(SIGINT_handler);
    log_info("Starting DB handler.");

    if (argc < 2) {
        log_error("No client socket parameter received.");
        exit(-1);
    }
    int client_socket;
    sscanf(argv[1], "%d", &client_socket);

    db_msg_t msg;
    log_info("Waiting for messages from client...");
    int res = socket_receive_db(client_socket, &msg);
    if(graceful_quit) {
        safe_exit(client_socket, -1);
    }
    if (res < 0) {
        log_error("Error reading client socket.");
        safe_exit(client_socket, -1);
    } else if (res == 0) {
        log_error("Lost connection from client.");
        safe_exit(client_socket, -1);
    }

    process_message(client_socket, &msg);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>

#define BUFF_SIZE 1024

// helper function for opening a socket given an address and a port. 
int open_socket(char *address, char *port) {
  struct addrinfo hints, *res;
  
  // clean up the struct
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  
  if (getaddrinfo(address, port, &hints, &res)) {
    fprintf(stderr, "Something went wrong getting the address\n");
    exit(1);
  }
   
  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  
  if (sockfd < 0) {
    fprintf(stderr, "Socket creation error\n");
    exit(1);
  }

  if (connect(sockfd, res->ai_addr, res->ai_addrlen)) {
    fprintf(stderr, "Something went wrong connecting to the socket\n");
    exit(1);
  }
  
  freeaddrinfo(res);
  return sockfd;
}

void *recieve_messages(void *arg) {
  int sockfd = *(int *)arg;
  char buffer[BUFF_SIZE];
  while (1) {
    memset(buffer, 0, sizeof(buffer));
    int bytes_recieved = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_recieved <= 0) {
      printf("Server error\n");
      exit(1);
    }
    printf("%s", buffer);
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  int sockfd = open_socket(argv[1], argv[2]);
  
  // start a thread to read in input
  pthread_t recieve;
  if (pthread_create(&recieve, NULL, recieve_messages, &sockfd)) {
    printf("Failed to create thread\n");
    exit(1);
  }
  
  char msg[BUFF_SIZE];
  printf("Enter a message\n");
  
  char username[strlen(argv[3]) + 1];
  strcpy(username, argv[3]);
  username[sizeof(username) - 1] = '\n';
  printf("Username: %s", username);
  
  send(sockfd, username, sizeof(username), 0);

  while (1) {
    memset(msg, 0, sizeof(msg));
    // get message from stdin
    if (fgets(msg, sizeof(msg), stdin) != NULL) {
    } else {
      printf("Something went wrong while reading input\n");
      continue;
    }
    if (send(sockfd, msg, strlen(msg), 0) < 0) {
      printf("Error sending message\n");
      break;
    }
  } 
  close(sockfd);
  return 0;
}


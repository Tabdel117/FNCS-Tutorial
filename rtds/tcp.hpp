#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>

#include <iostream>

int fourBytesToInt(unsigned char[4]);

float fourBytesToFloat(unsigned char[4]);

void floatToFourBytes(float, unsigned char[4]);

int connectToServer(const char*, int);

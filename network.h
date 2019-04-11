/*
 * network.h
 *
 *  Created on: Mar 10, 2019
 *      Author: EmilAliyev
 */

#ifndef NETWORK_H_
#define NETWORK_H_

static long WlanConnect();
static int set_time();
static void BoardInit(void);
static long InitializeAppVariables();
static int tls_connect();
static int connectToAccessPoint();
int http_post(int, char*);
int http_get(int);
void network_connect();
void sendMessage(char *message);

#endif /* NETWORK_H_ */

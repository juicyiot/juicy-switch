#ifndef CONNECTION_H
#define CONNECTION_H

#include "CredentialsStorage.h"

class Connection {
public:
	Connection();
	Connection(const char *ssid, const char *password);
	bool connect() const;
	bool isConnected() const;
	void persistCredentials() const;
private:
	credentials_t credentials;
};

#endif

#ifndef WIFICONNECTION_H
#define WIFICONNECTION_H

#include "CredentialsStorage.h"

class WiFiConnection {
public:
	WiFiConnection();
	WiFiConnection(const char *ssid, const char *password);
	bool connect() const;
	void disconnect(bool shouldClearCredentials);
	bool isConnected() const;
	void persistCredentials() const;
	String getIP() const;
	String getSSID() const;

	static credentials_t credentials;

private:
	void clearCredentials();
};

#endif

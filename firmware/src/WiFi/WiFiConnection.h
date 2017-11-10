#ifndef WIFICONNECTION_H
#define WIFICONNECTION_H

#include "CredentialsStorage.h"

class WiFiConnection {
public:
	WiFiConnection();
	WiFiConnection(const char *ssid, const char *password);
	bool connect(bool shouldPersistCredentials) const;
	void disconnect(bool shouldClearCredentials);
	bool isConnected() const;
	String getIP() const;
	String getSSID() const;

	static credentials_t credentials;

private:
	void persistCredentials() const;
	void clearCredentials();
};

#endif

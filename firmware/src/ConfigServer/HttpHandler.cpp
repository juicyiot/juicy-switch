#include "HttpHandler.h"

void HttpHandler::handleRoot() {
	if (captivePortal()) {
		return;
	}
	initializeConnection();
	ConfigServer::webServer.sendContent(
		"<html><head></head><body>"
		"<h1>Welcome to your Juicy Socket</h1>"
		"<p>Click <a href='/config'>here</a> to set up the socket's WiFi connection.</p>"
		"<p>Click <a href='/close'>here</a> to close the config network.</p>"
		"</body></html>"
	);
	ConfigServer::webServer.client().stop();
}

void HttpHandler::handleConfig() {
	ConfigServer::connectionStatus = none;

	initializeConnection();
	ConfigServer::webServer.sendContent(
		"<html><head></head><body>"
		"<h2>Select your WiFi network</h2>"
		"<table><tr><th>#</th><th align='left'>SSID</th></tr>"
	);
	if (ConfigServer::numAvailableNetworks > 0) {
		for (int i = 0; i < ConfigServer::numAvailableNetworks; i++) {
			ConfigServer::webServer.sendContent(String() + "<tr><td>" + i + "</td><td>" + WiFi.SSID(i) + "</td></td>");
		}
	} else {
		ConfigServer::webServer.sendContent(String() + "<tr><td>-<td><td>No WiFi network found</td></tr>");
	}
	ConfigServer::webServer.sendContent(
		"</table><br>"
		"<form method='POST' action='configsave'>"
		"SSID:"
		"<input type='text' placehoder='ssid' name='ssid'/><br>"
		"Password:"
		"<input type='password' placehoder='password' name='password'/><br>"
		"<input type='submit' value='Connect'/>"
		"</form>"
		"</body></html>"
	);
	ConfigServer::webServer.client().stop();
}

void HttpHandler::handleConfigSave() {
	initializeConnection();
	if (ConfigServer::connectionStatus == successful) {
		ConfigServer::webServer.sendContent(
			"<html><head></head><body>"
			"<h2>Connection Successful</h2>"
			"<p>IP: " + String() + toString(WiFi.localIP()) + "</p>"
			"<p>Your all set up now. <a href='/close'>Click here</a> to finish the configuration.</p>"
			"</body></html>"
		);
		ConfigServer::webServer.client().stop();
		return;
	}

	if (ConfigServer::connectionStatus == failed) {
		ConfigServer::webServer.sendContent(
			"<html><head></head><body>"
			"<h2>Connection Failed. <a href='/config'>Try again!</a></h2>"
			"</body></html>"
		);
		ConfigServer::webServer.client().stop();
		return;
	}

	ConfigServer::webServer.sendContent(
		"<html><head></head><body>"
		"<h2>Connecting...</h2>"
		"<script type='text/javascript'>setTimeout(function(){window.location.reload(1);},5000);</script>"
		"</body></html>"
	);
	ConfigServer::webServer.client().stop();

	bool twoArgs = ConfigServer::webServer.args() == 2;
	bool ssidIsThere = twoArgs ? (ConfigServer::webServer.argName(0) == "ssid") : false;
	bool passwordIsThere = twoArgs ? (ConfigServer::webServer.argName(1) == "password") : false;
	if (ssidIsThere && passwordIsThere) {
		String ssid = ConfigServer::webServer.arg("ssid");
		String password = ConfigServer::webServer.arg("password");
		strncpy(ConfigServer::networkCredentials.ssid, ssid.c_str(), CREDENTIAL_SIZE);
		strncpy(ConfigServer::networkCredentials.password, password.c_str(), CREDENTIAL_SIZE);
		CredentialsStorage::save(&ConfigServer::networkCredentials, sizeof(ConfigServer::networkCredentials));
		ConfigServer::shouldConnect = true;
	}
}

void HttpHandler::handleNotFound() {
	if(captivePortal()) {
		return;
	}
	ConfigServer::webServer.send(200, "text/plain", "juicy config: not found");
}

void HttpHandler::initializeConnection() {
	ConfigServer::webServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
	ConfigServer::webServer.sendHeader("Pragma", "no-cache");
	ConfigServer::webServer.sendHeader("Expires", "-1");
	ConfigServer::webServer.setContentLength(CONTENT_LENGTH_UNKNOWN);
	ConfigServer::webServer.send(200, "text/html", "");
	ConfigServer::webServer.sendContent("<meta name='viewport' content='width=device-width, initial-scale=1'>");
}

bool HttpHandler::captivePortal() {
	bool isRequestForIPAddress = isIPAddress(ConfigServer::webServer.hostHeader());
	bool isRequestForConfigPage = ConfigServer::webServer.hostHeader() == "juicy.local";
	if (!isRequestForIPAddress && !isRequestForConfigPage) {
		ConfigServer::webServer.sendHeader("Location", String("http://") + toString(ConfigServer::webServer.client().localIP()), true);
		ConfigServer::webServer.send(302, "text/plain", "");
		ConfigServer::webServer.client().stop();
		return true;
	}

	return false;
}

bool HttpHandler::isIPAddress(String str) {
	for (int i = 0; i < str.length(); i++) {
    	int c = str.charAt(i);
    	if (c != '.' && (c < '0' || c > '9')) {
      		return false;
    	}
  	}
  	return true;
}

String HttpHandler::toString(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
	  res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}

#include "HttpHandler.h"

#include "ConfigServer.h"

HttpHandler::HttpHandler(std::unique_ptr<ESP8266WebServer> &webServer, ConfigServer &config) : webServer(webServer), config(config) {};

void HttpHandler::handleRoot() {
	if (captivePortal()) {
		return;
	}
	webServer->send(200, "text/html",
		"<meta name='viewport' content='width=device-width, initial-scale=1'>"
		"<html><head></head><body>"
		"<h1>Welcome to your Juicy Socket</h1>"
		"<p>Click <a href='/setup'>here</a> to set up the socket's WiFi connection.</p>"
		"<p>Click <a href='/close'>here</a> to close the config network.</p>"
		"</body></html>"
	);
}

void HttpHandler::handleSetup() {
	config.status = none;

	// Todo: Scan and display available WiFi networks.

	webServer->send(200, "text/html",
		"<meta name='viewport' content='width=device-width, initial-scale=1'>"
		"<html><head></head><body>"
		"<h2>Select your WiFi network</h2>"
		"<form method='POST' action='save'>"
		"SSID:"
		"<input type='text' placehoder='ssid' name='ssid'/><br>"
		"Password:"
		"<input type='password' placehoder='password' name='password'/><br>"
		"<input type='submit' value='Connect'/>"
		"</form>"
		"</body></html>"
	);
}

void HttpHandler::handleSave() {
	webServer->send(200, "text/html",
		"<meta name='viewport' content='width=device-width, initial-scale=1'>"
		"<html><head></head><body>"
		"<h2>Connecting...</h2>"
		"<p>Click here <a href='/status'>here</a> after a couple of seconds to check the connection status.</p>"
		// "<script type='text/javascript'>setTimeout(function(){window.location.reload(1);},3000);</script>"
		"</body></html>"
	);

	bool twoArgs = webServer->args() == 2;
	bool ssidIsThere = twoArgs ? (webServer->argName(0) == "ssid" && webServer->argName(0).length() > 0) : false;
	bool passwordIsThere = twoArgs ? (webServer->argName(1) == "password" && webServer->argName(1).length() > 0) : false;
	if (ssidIsThere && passwordIsThere) {
		String ssid = webServer->arg("ssid");
		String password = webServer->arg("password");
		Serial.println("Try to connect using: " + ssid + password);
		strncpy(config.networkCredentials.ssid, ssid.c_str(), CREDENTIAL_SIZE);
		strncpy(config.networkCredentials.password, password.c_str(), CREDENTIAL_SIZE);
		config.shouldConnect = true;
	}
}

void HttpHandler::handleStatus() {
	if (config.status == successful) {
		webServer->send(200, "text/html",
			"<meta name='viewport' content='width=device-width, initial-scale=1'>"
			"<html><head></head><body>"
			"<h2>Connection Successful</h2>"
			"<p>Your all set up now. <a href='/close'>Click here</a> to finish the configuration.</p>"
			"</body></html>"
		);
	} else if (config.status == failed) {
		webServer->send(200, "text/html",
			"<meta name='viewport' content='width=device-width, initial-scale=1'>"
			"<html><head></head><body>"
			"<h2>Connection Failed. <a href='/setup'>Try again!</a></h2>"
			"</body></html>"
		);
	}
}

void HttpHandler::handleClose() {
	webServer->send(200, "text/html",
		"<meta name='viewport' content='width=device-width, initial-scale=1'>"
		"<html><head></head><body>"
		"<p>Done. You can leave now.</p>"
		"</body></html>"
	);

	config.status = done;
}

void HttpHandler::handleNotFound() {
	if(captivePortal()) {
		return;
	}
	webServer->send(200, "text/plain", "juicy config: not found");
}

void HttpHandler::initializeConnection() {
	webServer->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
	webServer->sendHeader("Pragma", "no-cache");
	webServer->sendHeader("Expires", "-1");
	webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
	webServer->send(200, "text/html", "");
	webServer->sendContent("<meta name='viewport' content='width=device-width, initial-scale=1'>");
}

bool HttpHandler::captivePortal() {
	bool isRequestForIPAddress = isIPAddress(webServer->hostHeader());
	bool isRequestForConfigPage = webServer->hostHeader() == "juicy.local";
	if (!isRequestForIPAddress && !isRequestForConfigPage) {
		webServer->sendHeader("Location", String("http://") + toString(webServer->client().localIP()), true);
		webServer->send(302, "text/plain", "");
		webServer->client().stop();
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

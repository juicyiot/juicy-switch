#ifndef WIFISCANNER_H
#define WIFISCANNER_H

#include <vector>
#include <string>

using namespace std;

class WiFiScanner {
public:
	WiFiScanner();
	vector<string> networks();
};

#endif

#include "CredentialsStorage.h"

void CredentialsStorage::save(void *data_src) {
	EEPROM.begin(CREDENTIAL_SIZE);
	for (size_t i = 0; i < CREDENTIAL_SIZE; i++) {
		char data = ((char *)data_src)[i];
		EEPROM.write(i, data);
	}
	EEPROM.commit();
}

void CredentialsStorage::load(void *data_dst) {
	EEPROM.begin(CREDENTIAL_SIZE);
	for (size_t i = 0; i< CREDENTIAL_SIZE ; i++) {
		char data = EEPROM.read(i);
		((char *)data_dst)[i] = data;
	}
	EEPROM.end();
}

void CredentialsStorage::clearMemory() {
	EEPROM.begin(CREDENTIAL_SIZE);
	for (size_t i = 0; i < CREDENTIAL_SIZE; i++) {
		EEPROM.write(i, 0);
	}
	EEPROM.end();
}

#include "CredentialsStorage.h"

void CredentialsStorage::save(void *data_src, size_t size) {
	EEPROM.begin(size);
	for (size_t i = 0; i < size; i++) {
		char data = ((char *)data_src)[i];
		EEPROM.write(i, data);
	}
	EEPROM.commit();
}

void CredentialsStorage::load(void *data_dst, size_t size) {
	EEPROM.begin(size);
	for (size_t i = 0; i< size ; i++) {
		char data = EEPROM.read(i);
		((char *)data_dst)[i] = data;
	}
	EEPROM.end();
}

void CredentialsStorage::clearMemory(size_t size) {
	EEPROM.begin(size);
	for (size_t i = 0; i < size; i++) {
		EEPROM.write(i, 0);
	}
	EEPROM.end();
}

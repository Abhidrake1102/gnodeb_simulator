
#ifndef MESSAGE_H
#define MESSAGE_H


#include <vector>
#include <stdint.h>

using namespace std;

enum MessageType{
	PRACH = 1,
	RACH_RESPONSE = 2,
	RRC_SETUP_REQUEST = 3,
	RRC_SETUP = 4,
	RRC_SETUP_COMPLETE = 5,
	END_OF_PHASE = 6
};

struct Message {
	uint16_t type;
	uint16_t length;

	vector<uint8_t> payload
		;
};
#endif

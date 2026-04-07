#include "ActivityDTO.h"

string ActivityDTO::GetType() const {
	return type;
}

int ActivityDTO::GetNumber() const {
	return number;
}

void ActivityDTO::SetNumber(int nr) {
	number = nr;
}

void ActivityDTO::operator=(ActivityDTO ot) {
	this->type = ot.GetType();
	this->number = ot.GetNumber();
}
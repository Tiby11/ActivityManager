#pragma once
#include <string>

using namespace std;
class ActivityDTO {
private:
	string type;
	int number;
public:
	ActivityDTO() = default;
	ActivityDTO(string type, int number) : type{ type }, number{ number } {};
	void operator=(ActivityDTO ot);
	string GetType() const;
	int GetNumber() const;
	void SetNumber(int nr);
};
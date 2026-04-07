#pragma once
#include "domain.h"
#include <vector>
#include <algorithm>
#include <random> 
#include <chrono>   

using std::vector;
class Program {
	friend class Service;
private:
	vector<Activity> program;
public:
	Program() = default;

	void AddProgram(const Activity& a);

	void EmptyProgram();

	void AddRandom(const vector<Activity>& activities, int n);

	const vector<Activity>& GetAllProgram();
};
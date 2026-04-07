#include "program.h"
#include <exception>
#include <iostream>

using namespace std;

void Program::AddProgram(const Activity& a) {
	this->program.push_back(a);
}
void Program::EmptyProgram() {
	this->program.clear();
}

void Program::AddRandom(const vector<Activity>& activities, int n) {
    if (activities.empty()) return;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, static_cast<int>(activities.size()) - 1);

    size_t initSize = program.size();
    while (program.size() < n + initSize) {
        int index = dist(gen);
        program.push_back(activities[index]);
    }
}

const vector<Activity>& Program::GetAllProgram() {
	return this->program;
}
#pragma once
#include <functional>
#include <string>

using std::string;
using std::function;
class Activity
{
private:
	int id;
	string title;
	string description;
	string type;
	int duration;


public:
	Activity() = default;
	int GetId() const noexcept;
	string GetTitle() const;
	string GetDescription() const;
	string GetType() const;
	int GetDuration() const noexcept;
	void SetTitle(const string title);
	void SetDescription(const string description);
	void SetType(const string type);
	void SetDuration(const int duration) noexcept;

	Activity(int id, string title, string description, string type, int duration);

};
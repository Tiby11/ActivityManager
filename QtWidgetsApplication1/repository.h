#pragma once
#include <vector>
#include "domain.h"
using std::vector;
class ValidateException {
	string msg;
public:
	ValidateException(string m) :msg{ m } {

	}
	string GetMessage() {
		return msg;
	}
};
class RepoException {
	string msg;
public:
	RepoException(string m) :msg{ m } {

	}
	string GetMessage() {
		return msg;
	}
};

class Repo {
public:
	virtual Activity SearchByTitle(const string& title) = 0;
	virtual void Store(const Activity& activity) = 0;
	virtual vector<Activity> GetAll() const = 0;
	virtual void Update(const int& id, const string& newTitle, const string& newDescription, const string& newType, const int& newTime) = 0;
	virtual void Delete(const int& id) = 0;
	virtual Activity GetActivity(const int& id) const = 0;
	virtual void Validate(const Activity& activity) const = 0;
	~Repo() = default;
};
class ActivityRepo : public Repo {
private:
	vector<Activity> activities;
public:
	virtual Activity SearchByTitle(const string& title) override;
	virtual void Store(const Activity& activity) override;
	virtual vector<Activity> GetAll() const override;
	virtual void Update(const int& id, const string& newTitle, const string& newDescription, const string& newType, const int& newTime) override;
	virtual void Delete(const int& id) override;
	virtual Activity GetActivity(const int& id) const override;
	virtual void Validate(const Activity& activity) const override;
};

class ActivityRepoFile :public ActivityRepo {
private:
	string fileN;
	void LoadFromFile();
	void WriteToFile();
public:
	ActivityRepoFile(string fileN) :fileN{ fileN } {
		LoadFromFile();
	}
	Activity SearchByTitle(const string& title) {
		return ActivityRepo::SearchByTitle(title);
	}
	void Store(const Activity& activity) {
		ActivityRepo::Store(activity);
		WriteToFile();
	}
	vector<Activity> GetAll() const {
		return ActivityRepo::GetAll();
	}
	void Update(const int& id, const string& newTitle, const string& newDescription, const string& newType, const int& newTime) {
		ActivityRepo::Update(id, newTitle, newDescription, newType, newTime);
		WriteToFile();
	}
	void Delete(const int& id) {
		ActivityRepo::Delete(id);
		WriteToFile();
	}
	Activity GetActivity(const int& id) const {
		return ActivityRepo::GetActivity(id);
	}
	void Validate(const Activity& activity) const {
		ActivityRepo::Validate(activity);
	}
};
/*
class RepoProbabilitate : public Repo {
private:
	vector<Activity> activities;
public:
	virtual Activity SearchByTitle(const string& title) override;
	virtual void Store(const Activity& activity) override;
	virtual vector<Activity> GetAll() const override;
	virtual void Update(const int& id, const string& newTitle, const string& newDescription, const string& newType, const int& newTime) override;
	virtual void Delete(const int& id) override;
	virtual Activity GetActivity(const int& id) const override;
	virtual void Validate(const Activity& activity) const override;
	void Probabilitate() const;
};
*/
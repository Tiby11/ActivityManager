#include "repository.h"
#include <fstream>
#include <sstream>
#include <random>
using namespace std;

/**
 * Adauga o activitate in repository.
 * Arunca exceptie daca activitatea exista deja.
 * @param activity Activitatea de adaugat
 */
void ActivityRepo::Store(const Activity& activity) {
	for (unsigned long long i = 0; i < activities.size(); i++)
		if (activities[i].GetId() == activity.GetId())
			throw RepoException("Activity already exist!");
	activities.push_back(activity);
}

/**
 * Returneaza toate activitatile din repository.
 * @return Vectorul de activitati
 */
vector<Activity> ActivityRepo::GetAll() const {
	return activities;
}

/**
 * Modifica o activitate existenta identificata prin ID.
 * Arunca exceptie daca activitatea nu exista.
 * @param id ID-ul activitatii de modificat
 * @param newTitle Titlul nou
 * @param newDescription Descrierea noua
 * @param newType Tipul nou
 * @param newDuration Durata noua
 */
void ActivityRepo::Update(const int& id, const string& newTitle, const string& newDescription, const string& newType, const int& newDuration) {
	int ok = 0;
	for (unsigned long long i = 0; i < activities.size(); i++)
		if (activities[i].GetId() == id) {
			activities[i].SetTitle(newTitle);
			activities[i].SetDescription(newDescription);
			activities[i].SetType(newType);
			activities[i].SetDuration(newDuration);
			ok = 1;
		}
	if (!ok)
		throw RepoException("Activity does not exist!");
}

/**
 * Sterge o activitate identificata prin ID.
 * Arunca exceptie daca activitatea nu exista.
 * @param id ID-ul activitatii de sters
 */
void ActivityRepo::Delete(const int& id) {
	int ok = 0;
	for (unsigned long long i = 0; i < activities.size(); i++)
		if (activities[i].GetId() == id) {
			activities.erase(activities.begin() + i);
			ok = 1;
		}
	if (!ok)
		throw RepoException("Activity does not exist!");
}

/**
 * Returneaza activitatea cu un ID specific.
 * Arunca exceptie daca activitatea nu exista.
 * @param id ID-ul activitatii cautate
 * @return Activitatea gasita
 */
Activity ActivityRepo::GetActivity(const int& id) const {
	for (unsigned long long i = 0; i < activities.size(); i++)
		if (activities[i].GetId() == id) {
			return activities[i];
		}
	throw RepoException("Activity does not exist!");
}

/**
 * Valideaza o activitate inainte de adaugare/modificare.
 * Arunca exceptie daca datele activitatii nu sunt valide.
 * @param a Activitatea de validat
 */
void ActivityRepo::Validate(const Activity& a) const {
	string msg = "";
	if (a.GetId() < 0)
		msg += "Invalid Id!\n";
	if (a.GetTitle().empty())
		msg += "Invalid Title!\n";
	if (a.GetDescription().empty())
		msg += "Invalid Description!\n";
	if (a.GetType().empty())
		msg += "Invalid Type!\n";
	if (a.GetDuration() < 0)
		msg += "Invalid Duration!\n";
	if (!msg.empty())
		throw ValidateException(msg);
}

Activity ActivityRepo::SearchByTitle(const string& title) {
	for (auto el : activities) {
		if (el.GetTitle() == title) {
			return el;
		}
	}
	throw RepoException("No activity with this title!");
}

void ActivityRepoFile::LoadFromFile() {
	std::ifstream inFile(fileN);
	std::string title, descr, type;
	int id, time;
	while (inFile>>id>>title>>descr>>type>>time) {
		Activity a{ id, title, descr, type, time };
		ActivityRepo::Store(a);
	}
}

void ActivityRepoFile::WriteToFile() {
	ofstream fout(fileN);
	for (const auto& a : ActivityRepo::GetAll()) {
		fout << a.GetId() << " "
			<< a.GetTitle() << " "
			<< a.GetDescription() << " "
			<< a.GetType() << " "
			<< a.GetDuration() << "\n";
	}
	fout.close();
}
/*
void RepoProbabilitate::Probabilitate() const {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<> dis(0.0, 1.0);

	double numar = dis(gen);

	if (numar < 0.4) {
		throw RepoException("Exceptie aruncata!");
	}
}

void RepoProbabilitate::Store(const Activity& a) {
	Probabilitate();
	activities.push_back(a);
}

Activity RepoProbabilitate::GetActivity(const int& id) const {
	Probabilitate();
	for (unsigned long long i = 0; i < activities.size(); i++)
		if (activities[i].GetId() == id) {
			return activities[i];
		}
	throw RepoException("Activity does not exist!");
}

void RepoProbabilitate::Validate(const Activity& a) const {
	Probabilitate();
	string msg = "";
	if (a.GetId() < 0)
		msg += "Invalid Id!\n";
	if (a.GetTitle().empty())
		msg += "Invalid Title!\n";
	if (a.GetDescription().empty())
		msg += "Invalid Description!\n";
	if (a.GetType().empty())
		msg += "Invalid Type!\n";
	if (a.GetDuration() < 0)
		msg += "Invalid Duration!\n";
	if (!msg.empty())
		throw ValidateException(msg);
}

Activity RepoProbabilitate::SearchByTitle(const string& title) {
	Probabilitate();
	for (auto el : activities) {
		if (el.GetTitle() == title) {
			return el;
		}
	}
	throw RepoException("No activity with this title!");
}

vector<Activity> RepoProbabilitate::GetAll() const {
	Probabilitate();
	return activities;
}

void RepoProbabilitate::Update(const int& id, const string& newTitle, const string& newDescription, const string& newType, const int& newDuration) {
	int ok = 0;
	Probabilitate();
	for (unsigned long long i = 0; i < activities.size(); i++)
		if (activities[i].GetId() == id) {
			activities[i].SetTitle(newTitle);
			activities[i].SetDescription(newDescription);
			activities[i].SetType(newType);
			activities[i].SetDuration(newDuration);
			ok = 1;
		}
	if (!ok)
		throw RepoException("Activity does not exist!");
}

void RepoProbabilitate::Delete(const int& id) {
	int ok = 0;
	Probabilitate();
	for (unsigned long long i = 0; i < activities.size(); i++)
		if (activities[i].GetId() == id) {
			activities.erase(activities.begin() + i);
			ok = 1;
		}
	if (!ok)
		throw RepoException("Activity does not exist!");
}
*/
#pragma once
#include "undo.h"
#include <functional>
#include <vector>
#include <unordered_map>
#include "repository.h"
#include "program.h"
#include "domain.h"
#include "ActivityDTO.h"
#include "Observer.h"
using std::string;
using std::function;
using namespace std;

class ActivityStore: public Observable {
private:
	Repo& rep;
	Program programList;
	vector<ActiuneUndo*> undoList;
public:
	/**
 * Comparator pentru sortarea dupa titlu.
 * @param a - prima activitate
 * @param b - a doua activitate
 * @return true daca a este mai mic sau egal cu b dupa titlu
 */
	static bool CmpTitle(const Activity& a, const Activity& b) {
		return a.GetTitle() < b.GetTitle();
	}

	/**
	 * Comparator pentru sortarea dupa descriere.
	 * @param a - prima activitate
	 * @param b - a doua activitate
	 * @return true daca a este mai mic sau egal cu b dupa descriere
	 */
	static bool CmpDescription(const Activity& a, const Activity& b) {
		return a.GetDescription() < b.GetDescription();
	}

	/**
	 * Comparator pentru sortarea dupa tip si durata.
	 * @param a - prima activitate
	 * @param b - a doua activitate
	 * @return true daca (tipA, durataA) este mai mic sau egal cu (tipB, durataB)
	 */
	static bool CmpTypeAndDuration(const Activity& a, const Activity& b) {
		string typeA = a.GetType();
		int durationA = a.GetDuration();
		string typeB = b.GetType();
		int durationB = b.GetDuration();
		return tie(typeA, durationA) < tie(typeB, durationB);
	}
	ActivityStore(Repo& rep) :rep{ rep } {

	}
	void Add(const int& id, const string& title, const string& description, const string& type, const int& duration);
	void Delete(const int& id);
	void Update(const int& id, const string& title, const string& description, const string& type, const int& duration);
	Activity GetActivity(const int& id)const;
	vector<Activity> GetAll();
	vector<Activity> FilteredByDescription(const string& description);
	vector<Activity> FilteredByType(const string& type);
	vector<Activity> SortedByTitle();
	vector<Activity> SortedByDescription();
	vector<Activity> SortedByTypeAndDuration();
	void AddProgramSrv(string title);
	size_t AddRandomSrv(int n);
	void EmptyProgramSrv();
	const vector<Activity>& GetAllProgramsSrv();
	unordered_map<string, ActivityDTO> Report() const;
	void Undo();
};

#include "test.h"
#include <assert.h>
#include "domain.h"
#include "repository.h"
#include "service.h"
#include "program.h"
#include <iostream>
using namespace std;


void Tests::ProgramTests() {
	TestAddProgram();
	TestAddRandom();
	TestEmpty();
	TestReport();
}

void Tests::TestAddProgram() {
	ActivityRepo rep;
	ActivityStore serv(rep);
	serv.Add(1, "Title", "Desc", "Type", 30);
	serv.AddProgramSrv("Title");
	assert(serv.GetAllProgramsSrv().size() == 1);
}

void Tests::TestAddRandom() {
	ActivityRepo rep;
	ActivityStore serv(rep);
	serv.Add(1, "Title", "Desc", "Type", 30);
	serv.AddRandomSrv(2);
	assert(serv.GetAllProgramsSrv().size() == 2);
}

void Tests::TestReport() {
	ActivityRepo rep;
	ActivityStore serv(rep);
	serv.Add(1, "Title", "Desc", "Type", 30);
	serv.Add(2, "a", "b", "Type", 40);
	unordered_map<string, ActivityDTO> map = serv.Report();
	assert(map["Type"].GetNumber() == 2);
}

void Tests::TestEmpty() {
	ActivityRepo rep;
	ActivityStore serv(rep);
	serv.Add(1, "Title", "Desc", "Type", 30);
	serv.AddRandomSrv(2);
	assert(serv.GetAllProgramsSrv().size() == 2);
	serv.EmptyProgramSrv();
	assert(serv.GetAllProgramsSrv().size() == 0);
}

void Tests::TestSearchByTitle() {
	Activity activity{ 1, "Title", "Description", "Type", 30 };
	Activity activity1{ 2, "a", "a", "a", 1 };
	ActivityRepo rep;
	rep.Store(activity);
	rep.Store(activity1);
	Activity searched = rep.SearchByTitle("Title");
	assert(searched.GetId() == 1);
	try
	{
		searched = rep.SearchByTitle("Invalid");
		assert(false);
	}
	catch (RepoException e) {
		assert(e.GetMessage() == "No activity with this title!");
	}
}

void Tests::AllTests() {
	DomainTests();
	RepoTests();
	ServiceTests();
	ProgramTests();
}

void Tests::TestGet() {
	Activity activity{ 1, "Title", "Description", "Type", 30 };
	assert(activity.GetId() == 1);
	assert(activity.GetTitle() == "Title");
	assert(activity.GetDescription() == "Description");
	assert(activity.GetType() == "Type");
	assert(activity.GetDuration() == 30);
}

void Tests::TestSet() {
	Activity activity{ 1, "a", "a", "a", 1 };
	activity.SetTitle("Title");
	activity.SetDescription("Description");
	activity.SetType("Type");
	activity.SetDuration(30);
	assert(activity.GetId() == 1);
	assert(activity.GetTitle() == "Title");
	assert(activity.GetDescription() == "Description");
	assert(activity.GetType() == "Type");
	assert(activity.GetDuration() == 30);
}

void Tests::DomainTests() {
	TestGet();
	TestSet();
}

void Tests::TestStore() {
	ActivityRepo rep;
	Activity a{ 1, "A", "A", "A", 1 };
	rep.Store(a);
	auto activities = rep.GetAll();
	assert(activities.size() == 1);
	try {
		rep.Store(a);
		assert(false);
	}
	catch (RepoException e) {
		assert(e.GetMessage() == "Activity already exist!");
		assert(true);
	}
}

void Tests::TestUpdateRepo() {
	ActivityRepo rep;
	Activity a{ 1, "a", "a", "a", 1 };
	rep.Store(a);
	rep.Update(1, "b", "b", "b", 2);
	auto activities = rep.GetAll();
	assert(activities.size() == 1);
	assert(activities[0].GetId() == 1);
	assert(activities[0].GetTitle() == "b");
	assert(activities[0].GetDescription() == "b");
	assert(activities[0].GetType() == "b");
	assert(activities[0].GetDuration() == 2);
	try {
		rep.Update(2, "b", "b", "b", 2);
		assert(false);
	}
	catch (RepoException e) {
		assert(e.GetMessage() == "Activity does not exist!");
		assert(true);
	}
}

void Tests::TestDeleteRepo() {
	ActivityRepo rep;
	Activity a{ 1, "a", "a", "a", 1 };
	Activity b{ 2, "b", "b", "b", 2 };
	rep.Store(a);
	rep.Store(b);
	auto activities = rep.GetAll();
	assert(activities.size() == 2);
	rep.Delete(1);
	activities = rep.GetAll();
	assert(activities.size() == 1);
	assert(activities[0].GetId() == 2);
	assert(activities[0].GetTitle() == "b");
	assert(activities[0].GetDescription() == "b");
	assert(activities[0].GetType() == "b");
	assert(activities[0].GetDuration() == 2);
	try {
		rep.Delete(1);
		assert(false);
	}
	catch (RepoException e) {
		assert(e.GetMessage() == "Activity does not exist!");
		assert(true);
	}
}

void Tests::TestGetActivity() {
	ActivityRepo rep;
	Activity a{ 1, "a", "a", "a", 1 };
	Activity b{ 2, "b", "b", "b", 2 };
	rep.Store(a);
	rep.Store(b);
	auto activities = rep.GetAll();
	assert(activities.size() == 2);
	Activity c = rep.GetActivity(2);
	assert(c.GetId() == 2);
	assert(c.GetTitle() == "b");
	assert(c.GetDescription() == "b");
	assert(c.GetType() == "b");
	assert(c.GetDuration() == 2);
	try {
		rep.GetActivity(3);
		assert(false);
	}
	catch (RepoException e) {
		assert(e.GetMessage() == "Activity does not exist!");
		assert(true);
	}
}

void Tests::TestValidate() {
	ActivityRepo rep;
	Activity invalidActivity{ -1, "\0", "\0", "\0", -1 };
	Activity a{ 1, "a", "a", "a", 1 };
	rep.Validate(a);
	assert(true);
	try {
		rep.Validate(invalidActivity);
		assert(false);
	}
	catch (ValidateException e) {
		assert(e.GetMessage() == "Invalid Id!\nInvalid Title!\nInvalid Description!\nInvalid Type!\nInvalid Duration!\n");
		assert(true);
	}
}

void Tests::RepoTests() {
	TestValidate();
	TestGetActivity();
	TestDeleteRepo();
	TestUpdateRepo();
	TestStore();
	TestSearchByTitle();
}

void Tests::TestAdd() {
	ActivityRepo rep;
	ActivityStore serv{ rep };

	serv.Add(1, "a", "a", "a", 1);
	serv.Add(2, "b", "b", "b", 2);
	auto activities = serv.GetAll();
	assert(activities.size() == 2);
	try {
		serv.Add(-1, "\0", "\0", "\0", -1);
		assert(false);
	}
	catch (ValidateException e) {
		assert(e.GetMessage() == "Invalid Id!\nInvalid Title!\nInvalid Description!\nInvalid Type!\nInvalid Duration!\n");
		assert(true);
	}
	try {
		serv.Add(1, "C", "C", "C", 3);
		assert(false);
	}
	catch (RepoException e) {
		assert(e.GetMessage() == "Activity already exist!");
		assert(true);
	}
}

void Tests::TestDelete() {
	ActivityRepo rep;
	ActivityStore serv{ rep };

	serv.Add(1, "a", "a", "a", 1);
	serv.Add(2, "b", "b", "b", 2);
	auto activities = serv.GetAll();
	assert(activities.size() == 2);

	serv.Delete(1);
	activities = serv.GetAll();
	assert(activities.size() == 1);
	assert(activities[0].GetId() == 2);
	assert(activities[0].GetTitle() == "b");
	assert(activities[0].GetDescription() == "b");
	assert(activities[0].GetType() == "b");
	assert(activities[0].GetDuration() == 2);

	try {
		serv.Delete(1);
		assert(false);
	}
	catch (RepoException e) {
		assert(e.GetMessage() == "Activity does not exist!");
		assert(true);
	}
}

void Tests::TestUpdate() {
	ActivityRepo rep;
	ActivityStore serv{ rep };

	serv.Add(1, "a", "a", "a", 1);
	serv.Add(2, "b", "b", "b", 2);
	auto activities = serv.GetAll();
	assert(activities.size() == 2);
	try {
		serv.Update(1, "\0", "\0", "\0", -1);
		assert(false);
	}
	catch (ValidateException e) {
		assert(e.GetMessage() == "Invalid Title!\nInvalid Description!\nInvalid Type!\nInvalid Duration!\n");
		assert(true);
	}
	try {
		serv.Update(3, "c", "c", "c", 3);
		assert(false);
	}
	catch (RepoException e) {
		assert(e.GetMessage() == "Activity does not exist!");
		assert(true);
	}
	serv.Update(2, "c", "c", "c", 3);
	activities = serv.GetAll();
	assert(activities[1].GetId() == 2);
	assert(activities[1].GetTitle() == "c");
	assert(activities[1].GetDescription() == "c");
	assert(activities[1].GetType() == "c");
	assert(activities[1].GetDuration() == 3);
}

void Tests::TestFilteredDescription() {
	ActivityRepo rep;
	ActivityStore serv{ rep };

	serv.Add(1, "a", "a", "a", 1);
	serv.Add(2, "b", "b", "b", 2);
	serv.Add(3, "a", "d", "3", 3);
	serv.Add(4, "b", "b", "b", 4);
	serv.Add(5, "a", "b", "c", 5);

	auto filtered = serv.FilteredByDescription("b");
	assert(filtered.size() == 3);

	assert(filtered[0].GetId() == 2);
	assert(filtered[0].GetTitle() == "b");
	assert(filtered[0].GetDescription() == "b");
	assert(filtered[0].GetType() == "b");
	assert(filtered[0].GetDuration() == 2);

	assert(filtered[1].GetId() == 4);
	assert(filtered[1].GetTitle() == "b");
	assert(filtered[1].GetDescription() == "b");
	assert(filtered[1].GetType() == "b");
	assert(filtered[1].GetDuration() == 4);

	assert(filtered[2].GetId() == 5);
	assert(filtered[2].GetTitle() == "a");
	assert(filtered[2].GetDescription() == "b");
	assert(filtered[2].GetType() == "c");
	assert(filtered[2].GetDuration() == 5);

	filtered = serv.FilteredByDescription("c");
	assert(filtered.size() == 0);
}

void Tests::TestFilteredType() {
	ActivityRepo rep;
	ActivityStore serv{ rep };

	serv.Add(1, "a", "a", "a", 1);
	serv.Add(2, "b", "b", "b", 2);
	serv.Add(3, "a", "d", "a", 3);
	serv.Add(4, "b", "b", "b", 4);
	serv.Add(5, "a", "b", "a", 5);

	auto filtered = serv.FilteredByType("a");
	assert(filtered.size() == 3);

	assert(filtered[0].GetId() == 1);
	assert(filtered[0].GetTitle() == "a");
	assert(filtered[0].GetDescription() == "a");
	assert(filtered[0].GetType() == "a");
	assert(filtered[0].GetDuration() == 1);

	assert(filtered[1].GetId() == 3);
	assert(filtered[1].GetTitle() == "a");
	assert(filtered[1].GetDescription() == "d");
	assert(filtered[1].GetType() == "a");
	assert(filtered[1].GetDuration() == 3);

	assert(filtered[2].GetId() == 5);
	assert(filtered[2].GetTitle() == "a");
	assert(filtered[2].GetDescription() == "b");
	assert(filtered[2].GetType() == "a");
	assert(filtered[2].GetDuration() == 5);

	filtered = serv.FilteredByType("c");
	assert(filtered.size() == 0);
}

void Tests::TestCmpTitle() {
	ActivityRepo rep;
	ActivityStore serv{ rep };
	Activity a{ 1, "a", "a", "a", 1 };
	Activity b{ 2, "b", "b", "b", 2 };
	assert(serv.CmpTitle(a, b));
	assert(!serv.CmpTitle(b, a));
}

void Tests::TestCmpDescription() {
	ActivityRepo rep;
	ActivityStore serv{ rep };
	Activity a{ 1, "a", "a", "a", 1 };
	Activity b{ 2, "b", "b", "b", 2 };
	assert(serv.CmpDescription(a, b));
	assert(!serv.CmpDescription(b, a));
}

void Tests::TestCmpTitleAndDuration() {
	ActivityRepo rep;
	ActivityStore serv{ rep };
	Activity a{ 1, "a", "a", "a", 1 };
	Activity b{ 2, "a", "b", "b", 2 };
	assert(serv.CmpTypeAndDuration(a, b));
	assert(!serv.CmpTypeAndDuration(b, a));
}

void Tests::TestSortedBy() {
	ActivityRepo rep;
	ActivityStore serv{ rep };

	serv.Add(1, "a", "a", "a", 5);
	serv.Add(2, "b", "b", "b", 4);
	serv.Add(3, "a", "d", "a", 3);
	serv.Add(4, "b", "b", "b", 2);
	serv.Add(5, "a", "b", "a", 1);

	auto sorted = serv.SortedByTitle();
	assert(sorted.size() == 5);
	assert(sorted[0].GetId() == 1);
	assert(sorted[1].GetId() == 3);
	assert(sorted[2].GetId() == 5);
	assert(sorted[3].GetId() == 2);
	assert(sorted[4].GetId() == 4);

	sorted = serv.SortedByDescription();
	assert(sorted[0].GetId() == 1);
	assert(sorted[1].GetId() == 2);
	assert(sorted[2].GetId() == 4);
	assert(sorted[3].GetId() == 5);
	assert(sorted[4].GetId() == 3);

	sorted = serv.SortedByTypeAndDuration();
	assert(sorted[0].GetId() == 5);
	assert(sorted[1].GetId() == 3);
	assert(sorted[2].GetId() == 1);
	assert(sorted[3].GetId() == 4);
	assert(sorted[4].GetId() == 2);
}

void Tests::TestGetActivityServ() {
	ActivityRepo rep;
	ActivityStore serv{ rep };
	serv.Add(1, "a", "a", "a", 1);
	Activity a = serv.GetActivity(1);
	assert(a.GetId() == 1);
	try {
		a = serv.GetActivity(2);
		assert(false);
	}
	catch (RepoException e) {
		assert(e.GetMessage() == "Activity does not exist!");
	}
}

void Tests::TestRepoFile() {
	ActivityRepoFile rep{ "test.txt" };
	Activity a{ 2, "B", "A", "A", 20 };
	Activity invalid{ -1,"\0","\0","\0", -1 };
	rep.Store(a);
	rep.Update(2, "B", "B", "B", 30);
	assert(rep.GetActivity(2).GetType() == "B");
	assert(rep.GetAll().size() == 2);
	assert(rep.SearchByTitle("B").GetId() == 2);
	try {
		rep.Validate(invalid);
		assert(false);
	}
	catch (ValidateException v) {
		assert(true);
	}
	rep.Validate(a);
	rep.Delete(2);
	assert(rep.GetAll().size() == 1);
}

void Tests::TestUndo() {
	ActivityRepo rep;
	ActivityStore serv{ rep };

	serv.Add(1, "a", "a", "a", 5);
	serv.Add(2, "b", "b", "b", 4);
	serv.Add(3, "a", "d", "a", 3);
	assert(serv.GetAll().size() == 3);
	serv.Undo();
	assert(serv.GetAll().size() == 2);
	serv.Undo();
	assert(serv.GetAll().size() == 1);
	serv.Update(1, "b", "b", "b", 2);
	serv.Undo();
	assert(serv.GetActivity(1).GetTitle() == "a");
	serv.Delete(1);
	serv.Undo();
	assert(serv.GetAll().size() == 1);
	serv.Undo();
	assert(serv.GetAll().size() == 0);
	try {
		serv.Undo();
		assert(false);
	}
	catch (RepoException e) {
		assert(true);
	}
}

void Tests::ServiceTests() {
	TestGetActivityServ();
	TestAdd();
	TestDelete();
	TestUpdate();
	TestFilteredDescription();
	TestFilteredType();
	TestCmpTitle();
	TestCmpDescription();
	TestCmpTitleAndDuration();
	TestSortedBy();
	TestRepoFile();
	TestUndo();
}


#include "service.h"
#include <algorithm>
#include <vector>
#include <fstream>
#include <unordered_map>
using std::sort;
using std::tie;
using namespace std;

/**
 * Adauga o activitate in repository.
 * @param id - identificator unic al activitatii
 * @param title - titlul activitatii
 * @param description - descrierea activitatii
 * @param type - tipul activitatii
 * @param duration - durata activitatii
 */
void ActivityStore::Add(const int& id, const string& title, const string& description, const string& type, const int& duration) {
    Activity a{ id, title, description, type, duration };
    rep.Validate(a);
    rep.Store(a);
    undoList.push_back(new AddUndo(rep, a));
}

/**
 * Sterge o activitate din repository.
 * @param id - identificatorul activitatii de sters
 */
void ActivityStore::Delete(const int& id) {
    Activity a = rep.GetActivity(id);
    rep.Delete(id);
    undoList.push_back(new DeleteUndo(rep, a));
}

Activity ActivityStore::GetActivity(const int& id) const {
    return rep.GetActivity(id);
}

/**
 * Actualizeaza o activitate existenta.
 * @param id - identificatorul activitatii
 * @param title - noul titlu
 * @param description - noua descriere
 * @param type - noul tip
 * @param duration - noua durata
 */
void ActivityStore::Update(const int& id, const string& title, const string& description, const string& type, const int& duration) {
    Activity undoA = rep.GetActivity(id);
    Activity a{ id, title, description, type, duration };
    rep.Validate(a);
    rep.Update(id, title, description, type, duration);
    undoList.push_back(new UpdateUndo(rep, undoA));
}

/**
 * Returneaza toate activitatile din repository.
 * @return vector cu toate activitatile
 */
vector<Activity> ActivityStore::GetAll() {
    return rep.GetAll();
}

/**
 * Filtreaza activitatile dupa descriere.
 * @param description - descrierea dupa care se filtreaza
 * @return vector cu activitatile filtrate
 */
vector<Activity> ActivityStore::FilteredByDescription(const string& description) {
    vector<Activity> filteredVector;
    auto activities = rep.GetAll();
    copy_if(activities.begin(), activities.end(), back_inserter(filteredVector), [description](Activity& a) {
        return a.GetDescription() == description;
        });
    return filteredVector;
}

/**
 * Filtreaza activitatile dupa tip.
 * @param type - tipul dupa care se filtreaza
 * @return vector cu activitatile filtrate
 */
vector<Activity> ActivityStore::FilteredByType(const string& type) {
    vector<Activity> filteredVector;
    auto activities = rep.GetAll();
    copy_if(activities.begin(), activities.end(), back_inserter(filteredVector), [type](Activity& a) {
        return a.GetType() == type;
        });
    return filteredVector;
}

/**
 * Sorteaza activitatile dupa titlu in ordine crescatoare.
 * @return vector cu activitatile sortate dupa titlu
 */
vector<Activity> ActivityStore::SortedByTitle() {
    auto sorted = rep.GetAll();
    sort(sorted.begin(), sorted.end(), CmpTitle);
    return sorted;
}

/**
 * Sorteaza activitatile dupa descriere in ordine crescatoare.
 * @return vector cu activitatile sortate dupa descriere
 */
vector<Activity> ActivityStore::SortedByDescription() {
    auto sorted = rep.GetAll();
    sort(sorted.begin(), sorted.end(), CmpDescription);
    return sorted;
}

/**
 * Sorteaza activitatile dupa tip si durata in ordine crescatoare.
 * @return vector cu activitatile sortate dupa tip si durata
 */
vector<Activity> ActivityStore::SortedByTypeAndDuration() {
    auto sorted = rep.GetAll();
    sort(sorted.begin(), sorted.end(), CmpTypeAndDuration);
    return sorted;
}

/**
 * Adauga un program in lista.
 * @param title - titlul programului
 */
void ActivityStore::AddProgramSrv(string title) {
    Activity a = rep.SearchByTitle(title);
    programList.AddProgram(a);
    notify();
}

/**
 * Adauga activitati aleatorii in program.
 * @param n - numarul de activitati aleatorii
 * @return dimensiunea programului
 */
size_t ActivityStore::AddRandomSrv(int n) {
    programList.AddRandom(this->rep.GetAll(), n);
    notify();
    return programList.GetAllProgram().size();
}

/**
 * Goleste programul.
 */
void ActivityStore::EmptyProgramSrv() {
    programList.EmptyProgram();
    notify();
}

/**
 * Returneaza toate activitatile din program.
 * @return vector cu activitatile din program
 */
const vector<Activity>& ActivityStore::GetAllProgramsSrv() {
    return programList.GetAllProgram();
}

/**
 * Genereaza un raport cu numarul de activitati pe tip.
 * @return un map cu tipul si numarul de activitati
 */
unordered_map <string, ActivityDTO> ActivityStore::Report() const {
    unordered_map<string, ActivityDTO> map;
    for (const auto& el : rep.GetAll()) {
        string type = el.GetType();
        if (map.find(type) == map.end()) {
            map[type] = ActivityDTO(type, 1);
        }
        else {
            map[type].SetNumber(map[type].GetNumber() + 1);
        }
    }
    return map;
}

/**
 * Aplica ultima actiune de undo.
 */
void ActivityStore::Undo() {
    if (undoList.empty()) {
        throw RepoException("Nothing to undo!");
    }
    undoList.back()->DoUndo();
    delete undoList.back();
    undoList.pop_back();
}

#include "domain.h"
#include <iostream>
/**
 * Returneaza titlul activitatii.
 * @return Titlul activitatii.
 */
string Activity::GetTitle() const {
    return title;
}

/**
 * Returneaza descrierea activitatii.
 * @return Descrierea activitatii.
 */
string Activity::GetDescription() const {
    return description;
}

/**
 * Returneaza tipul activitatii.
 * @return Tipul activitatii.
 */
string Activity::GetType() const {
    return type;
}

/**
 * Returneaza durata activitatii.
 * @return Durata activitatii (in minute).
 */
int Activity::GetDuration() const noexcept {
    return duration;
}

/**
 * Returneaza ID-ul activitatii.
 * @return ID-ul activitatii.
 */
int Activity::GetId() const noexcept {
    return id;
}

/**
 * Seteaza titlul activitatii.
 * @param t Noua valoare pentru titlu.
 */
void Activity::SetTitle(const string t) {
    title = t;
}

/**
 * Seteaza descrierea activitatii.
 * @param d Noua valoare pentru descriere.
 */
void Activity::SetDescription(const string d) {
    description = d;
}

/**
 * Seteaza tipul activitatii.
 * @param t Noua valoare pentru tip.
 */
void Activity::SetType(const string t) {
    type = t;
}

/**
 * Seteaza durata activitatii.
 * @param d Noua valoare pentru durata.
 */
void Activity::SetDuration(const int d) noexcept {
    duration = d;
}

Activity::Activity(int id, string title, string description, string type, int duration)
{
    this->id = id;
    this->title = title;
    this->description = description;
    this->type = type;
    this->duration = duration;
}
#pragma once
#include "domain.h"
#include "repository.h"

class ActiuneUndo {
public:
    virtual void DoUndo() = 0;
    virtual ~ActiuneUndo() = default;
};

class AddUndo : public ActiuneUndo {
    Repo& rep;
    Activity addedActivity;

public:
    AddUndo(Repo& rep, const Activity& addedActivity)
        : rep{ rep }, addedActivity{ addedActivity } {}

    void DoUndo() override {
        rep.Delete(addedActivity.GetId());
    }
};

class DeleteUndo : public ActiuneUndo {
    Repo& rep;
    Activity deletedActivity;

public:
    DeleteUndo(Repo& rep, const Activity& deletedActivity)
        : rep{ rep }, deletedActivity{ deletedActivity } {}

    void DoUndo() override {
        rep.Store(deletedActivity);
    }
};

class UpdateUndo : public ActiuneUndo {
    Repo& rep;
    Activity updatedActivity;

public:
    UpdateUndo(Repo& rep, const Activity& updatedActivity)
        : rep{ rep }, updatedActivity{ updatedActivity } {}

    void DoUndo() override {
        rep.Update(updatedActivity.GetId(), updatedActivity.GetTitle(),
            updatedActivity.GetDescription(), updatedActivity.GetType(),
            updatedActivity.GetDuration());
    }
};

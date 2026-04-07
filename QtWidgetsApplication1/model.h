#pragma once
#include <QWidget>
#include <QListView>
#include <QTableView>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QMessageBox>
#include <QAbstractTableModel>
#include <QAbstractListModel>
#include <QVector>
#include "service.h"

class ActivityListModel : public QAbstractListModel {
private:
    QVector<Activity> activities;
public:
    ActivityListModel(QObject* parent = nullptr) : QAbstractListModel(parent) {}

    void setActivities(const QVector<Activity>& newActivities) {
        activities = newActivities;
        emit layoutChanged();
    }

    int rowCount(const QModelIndex & = QModelIndex()) const override {
        return activities.size();
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (role == Qt::DisplayRole && index.isValid() && index.row() < activities.size()) {
            const auto& act = activities[index.row()];
            return QString::fromStdString(act.GetTitle());
        }
        return {};
    }

    const Activity& getActivity(int row) const {
        return activities[row];
    }
};

class ActivityTableModel : public   QAbstractTableModel {
private:
    QVector<Activity> activities;
public:
    ActivityTableModel(QObject* parent = nullptr) : QAbstractTableModel(parent) {}

    void setActivities(const QVector<Activity>& newActivities) {
        activities = newActivities;
        emit layoutChanged();
    }

    int rowCount(const QModelIndex & = QModelIndex()) const override {
        return activities.size();
    }

    int columnCount(const QModelIndex & = QModelIndex()) const override {
        return 5; 
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (role == Qt::DisplayRole && index.isValid() && index.row() < activities.size()) {
            const auto& act = activities[index.row()];
            switch (index.column()) {
            case 0: return act.GetId();
            case 1: return QString::fromStdString(act.GetTitle());
            case 2: return QString::fromStdString(act.GetDescription());
            case 3: return QString::fromStdString(act.GetType());
            case 4: return act.GetDuration();
            }
        }
        return {};
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            switch (section) {
            case 0: return "Id";
            case 1: return "Titlu";
            case 2: return "Descriere";
            case 3: return "Tip";
            case 4: return "Durata";
            }
        }
        return {};
    }

    const Activity& getActivity(int row) const {
        return activities[row];
    }
};

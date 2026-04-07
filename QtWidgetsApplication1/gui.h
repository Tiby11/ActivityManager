#pragma once
#include <QTWidgets/qwidget.h>
#include <qboxlayout.h>
#include <qlistwidget.h>
#include <qformlayout.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qtablewidget.h>
#include "service.h"
#include <QVector>
#include "programGUI.h"
#include "model.h"

class ActivityGUI : public QWidget {
public:
	ActivityGUI(ActivityStore &serv) :serv{ serv } {
		initGui();
		list->setModel(listModel);
		table->setModel(tableModel);
		loadData(serv.GetAll());
		initConnect();
	}
private:
	QTableView* table = new QTableView;
	ActivityTableModel* tableModel = new ActivityTableModel{ this };
	QListView* list = new QListView;
	ActivityListModel* listModel = new ActivityListModel{ this };
	ActivityStore& serv;
	QVBoxLayout* dynamicButtonsLayout = new QVBoxLayout;
	QPushButton* exitBtn = new QPushButton{ "&Exit" };
	QPushButton* addBtn = new QPushButton{ "&Add" };
	QPushButton* deleteBtn = new QPushButton{ "&Delete" };
	QPushButton* updateBtn = new QPushButton{ "&Update" };
	QPushButton* filtrDescrBtn = new QPushButton{ "&FilterDescr" };
	QPushButton* filtrTypeBtn = new QPushButton{ "&FilterType" };
	QPushButton* sortTitleBtn = new QPushButton{ "&SortTitle" };
	QPushButton* sortDescrBtn = new QPushButton{ "&SortDescr" };
	QPushButton* sortTypeDurBtn = new QPushButton{ "&SortTypeDur" };
	QPushButton* reload = new QPushButton{ "&Reload" };
	QPushButton* newProgramWndBtn = new QPushButton{ "OpenProgram" };
	QPushButton* newProgramColorBtn = new QPushButton{ "ColoredProgram" };
	QPushButton* addProgramBtn = new QPushButton{ "AddToProgram" };
	QPushButton* emptyProgramBtn = new QPushButton{ "EmptyProgram" };
	QPushButton* generateProgramBtn = new QPushButton{ "GenerateProgram" };
	QLineEdit* txtId = new QLineEdit;
	QLineEdit* txtTitle = new QLineEdit;
	QLineEdit* txtDescr = new QLineEdit;
	QLineEdit* txtType = new QLineEdit;
	QLineEdit* txtDur = new QLineEdit;
	QLineEdit* txtFilter = new QLineEdit;
	QLineEdit* txtNumber = new QLineEdit;
	void initGui() {
		QHBoxLayout* lyMain = new QHBoxLayout;
		setLayout(lyMain);
		lyMain->addWidget(table);
		lyMain->addWidget(list);
		auto rightLy = new QVBoxLayout;
		auto programLy = new QHBoxLayout;
		auto formLy = new QFormLayout;
		formLy->addRow("Id", txtId);
		formLy->addRow("Titlu", txtTitle);
		formLy->addRow("Descriere", txtDescr);
		formLy->addRow("Tip", txtType);
		formLy->addRow("Durata", txtDur);
		formLy->addRow("Filtrare", txtFilter);
		formLy->addRow("Number", txtNumber);
		rightLy->addLayout(formLy);
		auto btnLy = new QHBoxLayout;
		btnLy->addWidget(reload);
		btnLy->addWidget(addBtn);
		btnLy->addWidget(deleteBtn);
		btnLy->addWidget(updateBtn);
		btnLy->addWidget(sortTitleBtn);
		btnLy->addWidget(sortDescrBtn);
		btnLy->addWidget(sortTypeDurBtn);
		btnLy->addWidget(filtrDescrBtn);
		btnLy->addWidget(filtrTypeBtn);
		btnLy->addWidget(exitBtn);
		rightLy->addLayout(btnLy);
		programLy->addWidget(newProgramWndBtn);
		programLy->addWidget(generateProgramBtn);
		programLy->addWidget(addProgramBtn);
		programLy->addWidget(emptyProgramBtn);
		programLy->addWidget(newProgramColorBtn);
		rightLy->addLayout(programLy);
		lyMain->addLayout(rightLy);
		lyMain->addLayout(dynamicButtonsLayout);
		reloadTypeButtons();
	}
	void reloadTypeButtons() {
		QLayoutItem* oldBtns;
		while ((oldBtns = dynamicButtonsLayout->takeAt(0)) != nullptr) {
			delete oldBtns->widget();
			delete oldBtns;
		}
		auto typeMap = serv.Report();
		for (const auto& pair : typeMap) {
			const auto& dto = pair.second;
			auto* btn = new QPushButton(QString::fromStdString(dto.GetType()));
			dynamicButtonsLayout->addWidget(btn);
			QObject::connect(btn, &QPushButton::clicked, [&,dto]() {
				QString message = QString("Numar activitati de tip %1: %2")
				.arg(QString::fromStdString(dto.GetType()))
				.arg(dto.GetNumber());
				QMessageBox::information(this, "Raport activitate", message);
			});
		}
	}
	void loadData(vector<Activity> activities) {
		QVector<Activity> qVect;
		for (const auto& act : activities) {
			qVect.append(act);
		}
		listModel->setActivities(qVect);
		tableModel->setActivities(qVect);
	}
	void initConnect() {
		QObject::connect(generateProgramBtn, &QPushButton::clicked, [&]() {
			auto number = txtNumber->text();
			if (number.isEmpty()) {
				QMessageBox::warning(this, "Error", "Empty fields!");
				return;
			}
			serv.AddRandomSrv(number.toInt());
		});
		QObject::connect(addProgramBtn, &QPushButton::clicked, [&]() {
			auto title = txtTitle->text();
			if (title.isEmpty()) {
				QMessageBox::warning(this, "Error", "Empty fields!");
				return;
			}
			serv.AddProgramSrv(title.toStdString());
		});
		QObject::connect(emptyProgramBtn, &QPushButton::clicked, [&]() {
			serv.EmptyProgramSrv();
		});
		QObject::connect(newProgramWndBtn, &QPushButton::clicked, [&]() {
			auto newProgramWnd = new CosCRUDGUI{ serv };
			newProgramWnd->show();
		});
		QObject::connect(newProgramColorBtn, &QPushButton::clicked, [&]() {
			auto newProgramWnd = new CosReadOnlyGUI{ serv };
			newProgramWnd->show();
			});
		QObject::connect(list, &QListView::clicked, [&](const QModelIndex& index) {
			const auto& act = listModel->getActivity(index.row());
			txtId->setText(QString::number(act.GetId()));
			txtTitle->setText(QString::fromStdString(act.GetTitle()));
			txtDescr->setText(QString::fromStdString(act.GetDescription()));
			txtType->setText(QString::fromStdString(act.GetType()));
			txtDur->setText(QString::number(act.GetDuration()));
		});

		QObject::connect(table, &QTableView::clicked, [&](const QModelIndex& index) {
			const auto& act = tableModel->getActivity(index.row());
			txtId->setText(QString::number(act.GetId()));
			txtTitle->setText(QString::fromStdString(act.GetTitle()));
			txtDescr->setText(QString::fromStdString(act.GetDescription()));
			txtType->setText(QString::fromStdString(act.GetType()));
			txtDur->setText(QString::number(act.GetDuration()));
		});
		QObject::connect(exitBtn, &QPushButton::clicked, [&]() {
			close();
			});
		QObject::connect(sortDescrBtn, &QPushButton::clicked, [&]() {
			loadData(serv.SortedByDescription());
			});
		QObject::connect(sortTitleBtn, &QPushButton::clicked, [&]() {
			loadData(serv.SortedByTitle());
			});
		QObject::connect(sortTypeDurBtn, &QPushButton::clicked, [&]() {
			loadData(serv.SortedByTypeAndDuration());
			});
		QObject::connect(reload, &QPushButton::clicked, [&]() {
			loadData(serv.GetAll());
			});
		QObject::connect(addBtn, &QPushButton::clicked, [&]() {
			auto id = txtId->text();
			auto title = txtTitle->text();
			auto descr = txtDescr->text();
			auto type = txtType->text();
			auto dur = txtDur->text();
			if (id.isEmpty() || title.isEmpty() || descr.isEmpty() || type.isEmpty() || dur.isEmpty()) {
				QMessageBox::warning(this, "Error", "Empty fields!");
				return; 
			}
			try {
				serv.Add(id.toInt(), title.toStdString(), descr.toStdString(), type.toStdString(), dur.toInt());
				loadData(serv.GetAll());
				reloadTypeButtons();
			}
			catch (RepoException& e) {
				QMessageBox::warning(nullptr, QString::fromStdString("Exception"), QString::fromStdString(e.GetMessage()));
			}
			});
		QObject::connect(deleteBtn, &QPushButton::clicked, [&]() {
			auto id = txtId->text();
			if (id.isEmpty()) {
				QMessageBox::warning(this, "Error", "Empty fields!");
				return;
			}
			try {
				serv.Delete(id.toInt());
				loadData(serv.GetAll());
				reloadTypeButtons();
			}
			catch (RepoException& e) {
				QMessageBox::warning(nullptr, QString::fromStdString("Exception"), QString::fromStdString(e.GetMessage()));
			}
		});
		QObject::connect(filtrDescrBtn, &QPushButton::clicked, [&]() {
			auto filter = txtFilter->text();
			if (filter.isEmpty()) {
				QMessageBox::warning(this, "Error", "Empty fields!");
				return;
			}
			loadData(serv.FilteredByDescription(filter.toStdString()));
		});
		QObject::connect(filtrTypeBtn, &QPushButton::clicked, [&]() {
			auto filter = txtFilter->text();
			if (filter.isEmpty()) {
				QMessageBox::warning(this, "Error", "Empty fields!");
				return;
			}
			loadData(serv.FilteredByType(filter.toStdString()));
			});
		QObject::connect(updateBtn, &QPushButton::clicked, [&]() {
			auto id = txtId->text();
			auto title = txtTitle->text();
			auto descr = txtDescr->text();
			auto type = txtType->text();
			auto dur = txtDur->text();
			if (id.isEmpty() || title.isEmpty() || descr.isEmpty() || type.isEmpty() || dur.isEmpty()) {
				QMessageBox::warning(this, "Error", "Empty fields!");
				return;
			}
			try {
				serv.Update(id.toInt(), title.toStdString(), descr.toStdString(), type.toStdString(), dur.toInt());
				loadData(serv.GetAll());
				reloadTypeButtons();
			}
			catch (RepoException& e) {
				QMessageBox::warning(nullptr, QString::fromStdString("Exception"), QString::fromStdString(e.GetMessage()));
			}
		});
	}
};

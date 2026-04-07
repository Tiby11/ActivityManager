#include <qwidget.h>
#include "service.h"
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qlistwidget.h>
#include <qformlayout.h>
#include <qmessagebox.h>
#include <qpainter.h>
class CosCRUDGUI : public QWidget, public Observer {
public:
	CosCRUDGUI(ActivityStore& serv) :serv{ serv } {
		initGui();
		initConnect();
		loadData(serv.GetAllProgramsSrv());
	}
private:
	QListWidget* program = new QListWidget;
	QPushButton* generateProgramBtn = new QPushButton{ "GenerateProgram" };
	QPushButton* emptyProgramBtn = new QPushButton{ "EmptyProgram" };
	QLineEdit* inputTxt = new QLineEdit;
	ActivityStore& serv;
	void initGui() {
		QHBoxLayout* lyMain = new QHBoxLayout;
		setLayout(lyMain);
		lyMain->addWidget(program);
		auto rightLy = new QVBoxLayout;
		auto topLy = new QVBoxLayout;
		auto formLy = new QFormLayout;
		formLy->addRow("Number", inputTxt);
		topLy->addLayout(formLy);
		topLy->addWidget(generateProgramBtn);
		topLy->addWidget(emptyProgramBtn);
		lyMain->addLayout(topLy);
	}
	void update() override {
		loadData(serv.GetAllProgramsSrv());
	}
	void loadData(vector<Activity> activities) {
		program->clear();
		for (const auto& a : activities) {
			auto* item = new QListWidgetItem(QString::fromStdString(a.GetTitle()));
			item->setData(Qt::UserRole, a.GetId());
			item->setData(Qt::UserRole + 1, QString::fromStdString(a.GetDescription()));
			item->setData(Qt::UserRole + 2, QString::fromStdString(a.GetType()));
			item->setData(Qt::UserRole + 3, a.GetDuration());
			program->addItem(item);
		}
	}
	void initConnect() {
		serv.addObserver(this);
		QObject::connect(generateProgramBtn, &QPushButton::clicked, [&]() {
			auto number = inputTxt->text();
		if (number.isEmpty()) {
			QMessageBox::warning(this, "Error", "Empty fields!");
			return;
		}
		serv.AddRandomSrv(number.toInt());
			});

		QObject::connect(emptyProgramBtn, &QPushButton::clicked, [&]() {
			serv.EmptyProgramSrv();
		program->clear();
			});
	}
};

class CosReadOnlyGUI : public QWidget, public Observer {
private:
	ActivityStore& serv;
public:
	CosReadOnlyGUI(ActivityStore& serv) :serv{ serv } {
		serv.addObserver(this);
	}
	void update() override {
		repaint();
	}
	void paintEvent(QPaintEvent* ev) override {
		QPainter p{ this };
		std::random_device rd;  
		std::mt19937 gen(rd()); 
		std::uniform_int_distribution<> distr(0, 300);
		for (const auto a : serv.GetAllProgramsSrv()) {
			int x = distr(gen);
			int y = distr(gen);
			p.drawRect(x, y, 10, 20);
		}
	}
};
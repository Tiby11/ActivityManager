#include "gui.h"
#include "test.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    Tests test;
    test.AllTests();
    QApplication a(argc, argv);
    ActivityRepoFile rep{ "file.txt" };
    ActivityStore serv{ rep };
    ActivityGUI w {serv};
    w.show();

    return a.exec();
}

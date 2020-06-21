#include "mainwindow.h"
#include "network.h"

#include <QApplication>

int main(int argc, char * argv[]) {
	QApplication a(argc, argv);
	MainWindow w;
	Network network;
	w.show();
	return a.exec();
}

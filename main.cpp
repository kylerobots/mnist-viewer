#include "mainwindow.h"
#include "network.h"

#include <QApplication>

int main(int argc, char * argv[]) {
	QApplication a(argc, argv);
	MainWindow w;
	Network network;
	// Connect the front end to the back end.
	a.connect(&w, &MainWindow::iterateImage, &network, &Network::changeImage);
	a.connect(&network, &Network::example, &w, &MainWindow::displayExample);
	// The image is only sent when a change is triggered, so manually trigger the first one.
	network.changeImage(0);
	w.show();
	return a.exec();
}

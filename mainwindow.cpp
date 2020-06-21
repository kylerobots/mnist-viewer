#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget * parent) :
		QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	connect(ui->next_button, &QPushButton::clicked, this, &MainWindow::nextButtonClicked);
	connect(ui->previous_button, &QPushButton::clicked, this, &MainWindow::previousButtonClicked);
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::displayExample(const QImage & image, int label, int prediction) {
	ui->image_label->setPixmap(QPixmap::fromImage(image.scaledToHeight(800), Qt::ImageConversionFlag::NoFormatConversion));
	ui->truth_label->setText(QString::number(label));
	ui->prediction_label->setText(QString::number(prediction));
}

void MainWindow::nextButtonClicked(bool checked) {
	emit iterateImage(1);
	Q_UNUSED(checked);
}

void MainWindow::previousButtonClicked(bool checked) {
	emit iterateImage(-1);
	Q_UNUSED(checked);
}

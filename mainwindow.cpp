#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget * parent) :
		QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	connect(ui->next_button, &QPushButton::clicked, this, &MainWindow::nextButtonClicked);
	connect(ui->previous_button, &QPushButton::clicked, this, &MainWindow::previousButtonClicked);
	connect(ui->train_button, &QPushButton::clicked, this, &MainWindow::trainButtonClicked);
	ui->truth_label->setText("");
	ui->prediction_label->setText("");
}

MainWindow::~MainWindow() {
	delete ui;
	delete training_window;
}

void MainWindow::displayExample(int index, const QImage & image, int label, int prediction) {
	ui->index_label->setText(QString("Image #") + QString::number(index));
	ui->image_label->setPixmap(QPixmap::fromImage(image.scaledToHeight(800), Qt::ImageConversionFlag::NoFormatConversion));
	ui->truth_label->setText(QString::number(label));
	ui->prediction_label->setText(QString::number(prediction));
	Q_UNUSED(index);
}

void MainWindow::trainingUpdate(unsigned int batch, unsigned int total_batches, unsigned int epoch, unsigned int total_epochs, float latest_loss) {
	if (training_window == nullptr) {
		training_window = new TrainingProgress(this);
		training_window->show();
	}

	if (batch == total_batches && epoch == total_epochs) {
		training_window->hide();
		training_window->deleteLater();
	}
}

void MainWindow::nextButtonClicked(bool checked) {
	emit iterateImage(1);
	Q_UNUSED(checked);
}

void MainWindow::previousButtonClicked(bool checked) {
	emit iterateImage(-1);
	Q_UNUSED(checked);
}

void MainWindow::trainButtonClicked(bool checked) {
	emit startTraining(1, 1);
}

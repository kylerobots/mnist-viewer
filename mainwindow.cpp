#include "mainwindow.h"

#include "./ui_mainwindow.h"

#include <QRandomGenerator>

MainWindow::MainWindow(QWidget * parent) :
		QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	connect(ui->next_button, &QPushButton::clicked, this, &MainWindow::nextButtonClicked);
	connect(ui->previous_button, &QPushButton::clicked, this, &MainWindow::previousButtonClicked);
	// For now, just make a single image appear.
	torch::data::datasets::MNIST dataset("C:/Users/kylem/Documents/dev/src/mnist-viewer/data/test", torch::data::datasets::MNIST::Mode::kTest);
	torch::data::Example<torch::Tensor, torch::Tensor> example = dataset.get(1);
	torch::Tensor flat_data = example.data.flatten();
	float * raw_data = flat_data.data<float>();
	int64_t pixel_count = flat_data.size(0);
	//    QSharedPointer< raw_pixels = QSharedPointer::create(new uchar[pixel_count]);
	raw_pixels = new uchar[pixel_count];
	uint64_t sum = 0;
	QRandomGenerator generator;
	for (int i = 0; i < pixel_count; ++i) {
		float weight = raw_data[i] * 255.0;
		uchar pixel = static_cast<uchar>(qRound(weight));
		sum += pixel;
		pixel = qBound(uchar(0), pixel, uchar(255));
		raw_pixels[i] = 255 - pixel;

		//		raw_pixels[i] = 128;
	}
	QImage image(raw_pixels, 28, 28, QImage::Format_Grayscale8);
	ui->image_label->setPixmap(QPixmap::fromImage(image.scaledToHeight(800, Qt::FastTransformation)));
	ui->prediction_label->setText(QString::number(example.data.sum().item<float>()));
	ui->truth_label->setText(QString::number(example.target.item<int>()));
	//	ui->truth_label->setText(QString::number(sum));

	//	QString str = "";
	//	for (int i = 0; i < flat_data.size(0); ++i) {
	//		str += QString::number(raw_data[i]);
	//		str += " ";
	//	}
	//	ui->image_label->setText(str);
	//	ui->image_label->setWordWrap(true);

	//	torch::IntArrayRef sizes = flat_data.sizes();
	//	QString string_sizes = "";
	//	for (auto size : sizes) {
	//		string_sizes.append(QString::number(size));
	//		string_sizes.append(" ");
	//	}
	//	ui->prediction_label->setText(string_sizes);
	//	torch::IntArrayRef new_sizes = example.target.sizes();
	//	QString new_string_sizes = "";
	//	for (auto size : new_sizes) {
	//		new_string_sizes.append(QString::number(size));
	//		new_string_sizes.append(" ");
	//	}
	//	//    flat_data.data()
	//	ui->prediction_label->setText(QString::number(flat_data[0].item<float>()));
	//	ui->truth_label->setText(QString::number(example.target.dim()));
}

MainWindow::~MainWindow() {
	delete ui;
	delete raw_pixels;
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

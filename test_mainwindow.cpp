#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCoreApplication>
#include <QLineEdit>
#include <QSignalSpy>
#include <QtTest>

class TestMainWindow : public QObject {
	Q_OBJECT
public:
	TestMainWindow();
	~TestMainWindow();

private:
	const int IMAGE_HEIGHT = 28;
	const int IMAGE_WIDTH = 28;

private slots:
	void initTestCase();
	void cleanupTestCase();
	void cycleForward();
	void cycleBackward();
	void updateImage_data();
	void updateImage();
};

TestMainWindow::TestMainWindow() {
}

TestMainWindow::~TestMainWindow() {
}

void TestMainWindow::initTestCase() {
	MainWindow test;
}

void TestMainWindow::cleanupTestCase() {
}

void TestMainWindow::cycleForward() {
	MainWindow test;
	QSignalSpy spy(&test, SIGNAL(iterateImage(int)));
	QVERIFY(spy.isValid());
	QTest::mouseClick(test.ui->next_button, Qt::LeftButton);
	if (spy.size() == 0) {
		QVERIFY(spy.wait());
	}
	QList<QVariant> arguments = spy.takeFirst();
	QVERIFY(arguments.at(0).type() == QVariant::Int);
	QVERIFY(arguments.at(0) == 1);
}

void TestMainWindow::cycleBackward() {
	MainWindow test;
	QSignalSpy spy(&test, SIGNAL(iterateImage(int)));
	QVERIFY(spy.isValid());
	QTest::mouseClick(test.ui->previous_button, Qt::LeftButton);
	if (spy.size() == 0) {
		QVERIFY(spy.wait());
	}
	QList<QVariant> arguments = spy.takeFirst();
	QVERIFY(arguments.at(0).type() == QVariant::Int);
	QVERIFY(arguments.at(0) == -1);
}

void TestMainWindow::updateImage_data() {
	QTest::addColumn<QImage>("image");
	QTest::addColumn<int>("truth");
	QTest::addColumn<int>("prediction");

	uchar * fake_data_first = new uchar[IMAGE_WIDTH * IMAGE_HEIGHT];
	uchar * fake_data_second = new uchar[IMAGE_WIDTH * IMAGE_HEIGHT];
	for (int i = 0; i < IMAGE_WIDTH * IMAGE_HEIGHT; ++i) {
		fake_data_first[i] = 0;
		fake_data_second[i] = 128;
	}
	QImage first_image(fake_data_first, IMAGE_WIDTH, IMAGE_HEIGHT, QImage::Format_Grayscale8);
	QImage second_image(fake_data_second, IMAGE_WIDTH, IMAGE_HEIGHT, QImage::Format_Grayscale8);

	QTest::newRow("First") << first_image << 1 << 0;
	QTest::newRow("Second") << second_image << 4 << 4;
}

void TestMainWindow::updateImage() {
	MainWindow test;

	QFETCH(QImage, image);
	QFETCH(int, truth);
	QFETCH(int, prediction);

	test.displayExample(image, truth, prediction);
	QCOMPARE(test.ui->truth_label->text(), QString::number(truth));
	QCOMPARE(test.ui->prediction_label->text(), QString::number(prediction));
	// Extract and scale the image back its original size.
	QImage result_image = test.ui->image_label->pixmap()->toImage();
	result_image = result_image.scaled(IMAGE_WIDTH, IMAGE_HEIGHT);
	QVERIFY(!result_image.isNull());
	QVERIFY(result_image.isGrayscale());
	QVERIFY(result_image == image);
}

QTEST_MAIN(TestMainWindow)
#include "test_mainwindow.moc"

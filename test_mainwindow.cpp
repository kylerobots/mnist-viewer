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

private slots:
	void initTestCase();
	void cleanupTestCase();
	void cycleForward();
	void cycleBackward();
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

void TestMainWindow::updateImage() {
	MainWindow test;
	// Create fake data to display.
	int width = 28;
	int height = 28;
	uchar * fake_data_first = new uchar[width * height];
	uchar * fake_data_second = new uchar[width * height];
	for (int i = 0; i < width * height; ++i) {
		fake_data_first[i] = 0;
		fake_data_second[i] = 128;
	}
	QImage first_image(fake_data_first, width, height, QImage::Format_Grayscale8);
	int first_prediction = 0;
	int first_truth = 1;
	QImage second_image(fake_data_second, width, height, QImage::Format_Grayscale8);
	int second_prediction = 4;
	int second_truth = 4;
	// Send the first data and check the labels.
	test.displayExample(first_image, first_truth, first_prediction);
	QCOMPARE(test.ui->truth_label->text(), QString::number(first_truth));
	QCOMPARE(test.ui->prediction_label->text(), QString::number(first_prediction));
	// Images are implicitly shared, so there is a chance this is still pointing to the orignal.
	QImage first_label_image = test.ui->image_label->pixmap()->toImage();
	QVERIFY(!first_label_image.isNull());
	QVERIFY(first_label_image.isGrayscale());
	QVERIFY(first_label_image == first_image);
	// Send the second data and check the labels.
	test.displayExample(second_image, second_truth, second_prediction);
	QCOMPARE(test.ui->truth_label->text(), QString::number(second_truth));
	QCOMPARE(test.ui->prediction_label->text(), QString::number(second_prediction));
	QImage second_label_image = test.ui->image_label->pixmap()->toImage();
	QVERIFY(!second_label_image.isNull());
	QVERIFY(second_label_image.isGrayscale());
	QVERIFY(second_label_image == first_image);
}

QTEST_MAIN(TestMainWindow)
#include "test_mainwindow.moc"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCoreApplication>
#include <QSignalSpy>
#include <QtTest>

/*!
 * \brief The TestMainWindow class is used to test \ref MainWindow and its GUI.
 */
class TestMainWindow : public QObject {
	Q_OBJECT
public:
	TestMainWindow();
	~TestMainWindow();

private:
	/*!
	 * \brief The assumed image height for all data points.
	 */
	const int IMAGE_HEIGHT = 28;
	/*!
	 * \brief The assumed image width for all data points.
	 */
	const int IMAGE_WIDTH = 28;
	/*!
	 * \brief The class under test.
	 */
	MainWindow test;

private slots:
	/*!
	 * \brief initTestCase runs at the start of the test.
	 */
	void initTestCase();
	/*!
	 * \brief cleanupTestCase runs at the end of the test.
	 */
	void cleanupTestCase();
	/*!
	 * \test This test clicks the next button on the GUI and makes sure that an
	 * iteration value of 1 is emitted on the \ref MainWindow::iterateImage signal.
	 */
	void cycleForward();
	/*!
	 * \test This test clicks the previous button on the GUI and makes sure that an
	 * iteration value of -1 is emitted on the \ref MainWindow::iterateImage signal.
	 */
	void cycleBackward();
	/*!
	 * \test This tests that when an example is received on \ref MainWindow::displayExample,
	 * all the appropriate labels are updated.
	 * 
	 * This includes the:
	 * - Image label
	 * - Truth label
	 * - Prediction label
	 * - Index label
	 * 
	 * The test sends two images to make sure that all the values update each time, particularly
	 * the image, since it will implicitly share its data.
	 * 
	 * This uses the logic in \ref updateImage.
	 */
	void updateImage_data();
	/*!
	 * \brief The method that actually makes the comparisions in \ref updateImage_data.
	 */
	void updateImage();
	/*!
     * \test This tests that the GUI sends the right batch size and number of epochs to train on.
     * */
	void callTraining();
};

TestMainWindow::TestMainWindow() {
}

TestMainWindow::~TestMainWindow() {
}

void TestMainWindow::initTestCase() {
}

void TestMainWindow::cleanupTestCase() {
}

void TestMainWindow::cycleForward() {
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
	QTest::addColumn<int>("index");
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

	QTest::newRow("First") << 0 << first_image << 1 << 0;
	QTest::newRow("Second") << 0 << second_image << 4 << 4;
}

void TestMainWindow::updateImage() {
	QFETCH(int, index);
	QFETCH(QImage, image);
	QFETCH(int, truth);
	QFETCH(int, prediction);

	test.displayExample(index, image, truth, prediction);
	QCOMPARE(test.ui->truth_label->text(), QString::number(truth));
	QCOMPARE(test.ui->prediction_label->text(), QString::number(prediction));
	QCOMPARE(test.ui->index_label->text(), QString("Image #") + QString::number(index));
	// Extract and scale the image back its original size.
	QImage result_image = test.ui->image_label->pixmap(Qt::ReturnByValueConstant()).toImage();
	result_image = result_image.scaled(IMAGE_WIDTH, IMAGE_HEIGHT);
	QVERIFY(!result_image.isNull());
	QVERIFY(result_image.isGrayscale());
	QVERIFY(result_image == image);
}

void TestMainWindow::callTraining() {
	QSignalSpy spy(&test, SIGNAL(startTraining(unsigned int, unsigned int)));
	QVERIFY(spy.isValid());
	QTest::mouseClick(test.ui->train_button, Qt::LeftButton);
	if (spy.size() == 0) {
		QVERIFY(spy.wait());
	}
	QList<QVariant> arguments = spy.takeFirst();
	QVERIFY(arguments.at(0).type() == QVariant::UInt);
	QVERIFY(arguments.at(1).type() == QVariant::UInt);
	QVERIFY(arguments.at(0) == 1);
	QVERIFY(arguments.at(1) == 1);
}

QTEST_MAIN(TestMainWindow)
#include "test_mainwindow.moc"

#include "network.h"

#include <QSignalSpy>
#include <QTest>
#include <QtGui/QImage>
#include <limits>

class TestNetwork : public QObject {
	Q_OBJECT
public:
	TestNetwork();
	~TestNetwork();

private:
	Network network;
	QImage image;

private slots:
	void initTestCase();
	void cleanupTestCase();
	void cycleImage_data();
	void cycleImage();
};
TestNetwork::TestNetwork() {
}

TestNetwork::~TestNetwork() {
}

void TestNetwork::initTestCase() {
}

void TestNetwork::cleanupTestCase() {
}

void TestNetwork::cycleImage_data() {
	QTest::addColumn<int>("increment");

	QTest::newRow("Initialize") << 1;
	QTest::newRow("+1") << 1;
	QTest::newRow("-1") << -1;
	QTest::newRow("0") << 0;
	QTest::newRow("-3") << -3;
	QTest::newRow("+20000") << 15000;
	QTest::newRow("Max Int") << std::numeric_limits<int>::max();
	QTest::newRow("Min Int") << std::numeric_limits<int>::min();
}

void TestNetwork::cycleImage() {
	QFETCH(int, increment);
	QSignalSpy spy(&network, SIGNAL(example(QImage, int, int)));
	QVERIFY(spy.isValid());
	network.changeImage(increment);
	if (spy.size() == 0) {
		QVERIFY(spy.wait());
	}
	QList<QVariant> arguments = spy.takeFirst();
	// Verify the types received
	QVERIFY(arguments.at(0).type() == QVariant::Image);
	QVERIFY(arguments.at(1).type() == QVariant::Int);
	QVERIFY(arguments.at(2).type() == QVariant::Int);
	// Both Ints should be between 0 and 9.
	QVERIFY(arguments.at(1) >= 0 && arguments.at(1) <= 9);
	QVERIFY(arguments.at(2) >= 0 && arguments.at(2) <= 9);
	// Check image properties.
	QImage image = arguments.at(0).value<QImage>();
	QVERIFY(!image.isNull());
	QVERIFY(image.height() == 28);
	QVERIFY(image.width() == 28);
	QVERIFY(image.isGrayscale());
	// The image should be a new one or the same, depending on if the
	// increment is nonzero or zero.
	if (increment == 0) {
		QVERIFY(image == this->image);
	} else {
		QVERIFY(image != this->image);
	}
	// Update the stored image.
	this->image = image.copy();
}

QTEST_MAIN(TestNetwork)
#include "test_network.moc"

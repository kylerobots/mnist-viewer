#include "network.h"

#include <QSignalSpy>
#include <QTest>
#include <QtGui/QImage>

class TestNetwork : public QObject {
	Q_OBJECT
public:
	TestNetwork();
	~TestNetwork();

private:
	Network network;

private slots:
	void initTestCase();
	void cleanupTestCase();
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

void TestNetwork::cycleImage() {
	QSignalSpy spy(&network, SIGNAL(example(QImage, int, int)));
	QVERIFY(spy.isValid());
	network.changeImage(1);
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
	// I don't know what image to expect, so just check some properties.
	QImage image = arguments.at(0).value<QImage>();
	QVERIFY(!image.isNull());
	QVERIFY(image.height() == 28);
	QVERIFY(image.width() == 28);
	QVERIFY(image.isGrayscale());
}

QTEST_MAIN(TestNetwork)
#include "test_network.moc"

#include <QTest>

class TestNetwork : public QObject {
	Q_OBJECT
public:
	TestNetwork();
	~TestNetwork();

private slots:
	void initTestCase();
	void cleanupTestCase();
	void test1();
};
TestNetwork::TestNetwork() {
}

TestNetwork::~TestNetwork() {
}

void TestNetwork::initTestCase() {
}

void TestNetwork::cleanupTestCase() {
}

void TestNetwork::test1() {
}

QTEST_MAIN(TestNetwork)
#include "test_network.moc"

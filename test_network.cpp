#include "network.h"

#include <QSignalSpy>
#include <QTest>
#include <QtGui/QImage>
#include <limits>

/*!
 * \brief The TestNetwork class is used to test \ref Network.
 */
class TestNetwork : public QObject {
	Q_OBJECT
public:
	TestNetwork();
	~TestNetwork();

private:
	/*!
	 * \brief The \ref Network under test.
	 */
	Network network;
	/*!
	 * \brief A QImage member to store example images between
	 * iterations to ensure they are updating.
	 */
	QImage image;

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
	 * \test This tests that the example is incremented to the
	 * correct index when the \ref Network::changeImage slot is
	 * called.
	 * 
	 * The tests are:
	 * 1. Just initialize the image, since that isn't guaranteed.
	 * 2. Increment forward one.
	 * 3. Decrement backward one.
	 * 4. Make no change.
	 * 5. Decrement enough to wrap around to the end of the dataset.
	 * 6. Increment a whole dataset's worth so the index is unchanged.
	 * 7. Increment some arbitrarily large number.
	 * 8. Increment the maximum value of an int.
	 * 9. Decrement the minimum value of an int.
	 * 10. Increment enough to be the exact size of the data, which should wrap around to 0.
	 * 
	 * In each case, the properties of the emitted \ref Network::example signal are checked
	 * for the right data types. Additionally, some properties about the image are checked as
	 * well. Lastly, the image is checked against the previous one to see if it should or
	 * should not be different.
	 * 
	 * This uses the logic in \ref cycleImage.
	 */
	void cycleImage_data();
	/*!
	 * \brief The method that actually makes the comparisions in \ref cycleImage_data.
	 */
	void cycleImage();
	/*!
     * \test This tests that the network properly trains the itself and emits progress
     * updates.
     * 
     * The test cases are:
     * 1. **No data size:** The network will immediately return without training.
     * 2. **No epochs:** The network will immediately return without training.
     * 3. **One datapoint and one epoch:** The network will train as expected.
     * 4. **>Max datapoints and one epoch:** The network will use the full dataset to train.
     * 
     * This uses the logic in \ref train.
     */
	void train_data();
	/*!
     * \brief The method that actually makes the comparisons in \ref train_data.
     */
	void train();
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
	QTest::addColumn<int>("expected_index");

	QTest::newRow("Initialize") << 1 << 1;
	QTest::newRow("+1") << 1 << 2;
	QTest::newRow("-1") << -1 << 1;
	QTest::newRow("0") << 0 << 1;
	QTest::newRow("-3") << -3 << 9998;
	QTest::newRow("+10000") << 10000 << 9998;
	QTest::newRow("+20000") << 15000 << 4998;
	QTest::newRow("Max Int") << std::numeric_limits<int>::max() << 8645;
	QTest::newRow("Min Int") << std::numeric_limits<int>::min() << 4997;
	QTest::newRow("Exactly 10000 (so 0)") << 5003 << 0;
}

void TestNetwork::cycleImage() {
	QFETCH(int, increment);
	QFETCH(int, expected_index);
	QSignalSpy spy(&network, SIGNAL(example(int, QImage, int, int)));
	QVERIFY(spy.isValid());
	network.changeImage(increment);
	if (spy.size() == 0) {
		QVERIFY(spy.wait());
	}
	QList<QVariant> arguments = spy.takeFirst();
	// Verify the types received
	QVERIFY(arguments.at(0).type() == QVariant::Int);
	QVERIFY(arguments.at(1).type() == QVariant::Image);
	QVERIFY(arguments.at(2).type() == QVariant::Int);
	QVERIFY(arguments.at(3).type() == QVariant::Int);
	// The index should match the expected value.
	QCOMPARE(arguments.at(0).toInt(), expected_index);
	// Both labels should be between 0 and 9.
	int truth_label = arguments.at(2).toInt();
	int prediction_label = arguments.at(3).toInt();
	QVERIFY(truth_label >= 0 && truth_label <= 9);
	QVERIFY(prediction_label >= 0 && prediction_label <= 9);
	// Check image properties.
	QImage image = arguments.at(1).value<QImage>();
	QVERIFY(!image.isNull());
	QVERIFY(image.height() == 28);
	QVERIFY(image.width() == 28);
	QVERIFY(image.isGrayscale());
	// The image should be a new one or the same, depending on if the
	// increment is nonzero or zero.
	if (increment % 10000 == 0) {
		QVERIFY(image == this->image);
	} else {
		QVERIFY(image != this->image);
	}
	// Update the stored image.
	this->image = image.copy();
}

void TestNetwork::train_data() {
	QTest::addColumn<unsigned int>("data_size");
	QTest::addColumn<unsigned int>("epoch_number");
	QTest::addColumn<bool>("is_expected_loss_zero");

	QTest::newRow("No data") << static_cast<uint>(0) << static_cast<uint>(1) << true;
	QTest::newRow("No epochs") << static_cast<uint>(1) << static_cast<uint>(0) << true;
	QTest::newRow("One each") << static_cast<uint>(1) << static_cast<uint>(1) << false;
	QTest::newRow("Max data") << static_cast<uint>(15000) << static_cast<uint>(1) << false;
}

void TestNetwork::train() {
	QFETCH(unsigned int, data_size);
	QFETCH(unsigned int, epoch_number);
	QFETCH(bool, is_expected_loss_zero);

	QSignalSpy spy(&network, SIGNAL(trainingUpdate(unsigned int, unsigned int, float)));
	QVERIFY(spy.isValid());
	QBENCHMARK {
		network.startTraining(data_size, epoch_number);
	}
	if (spy.size() == 0) {
		// Training can take some time, so increase the wait.
		QVERIFY(spy.wait(30000));
	}
	QList<QVariant> arguments = spy.takeFirst();
	// The total epochs should match.
	QCOMPARE(arguments.at(1).toUInt(), epoch_number);
	// The value of the loss function will depend on the case.
	// This assumes that if training runs, it is impossible to have
	// a perfect training session with loss of zero.
	if (is_expected_loss_zero) {
		QCOMPARE(arguments.at(2).toFloat(), 0.0);
	} else {
		QVERIFY(arguments.at(2).toFloat() != 0.0);
	}
}

QTEST_MAIN(TestNetwork)
#include "test_network.moc"

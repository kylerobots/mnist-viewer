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

QTEST_MAIN(TestMainWindow)
#include "test_mainwindow.moc"

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "torch/torch.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
	Q_OBJECT
	friend class TestMainWindow;

public:
	MainWindow(QWidget * parent = nullptr);
	~MainWindow();

public slots:
	void displayExample(const QImage & image, int label, int prediction);

private:
	void nextButtonClicked(bool checked);
	void previousButtonClicked(bool checked);

private:
	Ui::MainWindow * ui;
	uchar * raw_pixels;

signals:
	void iterateImage(int difference);
};
#endif // MAINWINDOW_H
#ifndef NETWORK_H
#define NETWORK_H

#include "torch/torch.h"

#include <QImage>
#include <QObject>

class Network : public QObject {
	Q_OBJECT
public:
	explicit Network(QObject * parent = nullptr);
	~Network();

public slots:
	void changeImage(int increment);

private:
	void sendImage();

private:
	size_t current_displayed_example;
	QScopedPointer<torch::data::datasets::MNIST> data;
	const QString DATA_PATH = "C:/Users/kylem/Documents/dev/src/mnist-viewer/data/test";
	uchar * raw_pixels;

signals:
	void example(QImage image, int truth, int prediction);
};

#endif // NETWORK_H

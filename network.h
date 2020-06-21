#ifndef NETWORK_H
#define NETWORK_H

// Qt and Libtorch both use the slots definition, cause conflicts.
// This is a workaround suggested here:
// https://github.com/pytorch/pytorch/issues/19405
#undef slots
#include "torch/torch.h"
#define slots Q_SLOTS

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
	size_t current_index;
	QScopedPointer<torch::data::datasets::MNIST> data;
	const QString DATA_PATH = "C:/Users/kylem/Documents/dev/src/mnist-viewer/data/test";
	uchar * raw_pixels;

signals:
	void example(int index, QImage image, int truth, int prediction);
};

#endif // NETWORK_H

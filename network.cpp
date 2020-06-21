#include "network.h"

#include <iostream>

Network::Network(QObject * parent) :
		QObject(parent) {
	data.reset(new torch::data::datasets::MNIST(DATA_PATH.toStdString(), torch::data::datasets::MNIST::Mode::kTest));
	current_index = 0;
	sendImage();
	raw_pixels = new uchar[28 * 28];
}

Network::~Network() {
	delete raw_pixels;
}

void Network::changeImage(int increment) {
	// To avoid overflow, this uses a series of while loops to bring the
	// requested increment into the range of data values. The assumption
	// is that the data size won't approach the max value of int.
	// There are probably ways to do this better, but I was having
	// difficulty with handling negative increments.
	size_t data_size = data->size().value();
	while (increment < 0) {
		increment += data_size;
	}
	while (increment >= data_size) {
		increment -= data_size;
	}
	current_index += increment;
	while (current_index > data_size) {
		current_index -= data_size;
	}
	sendImage();
}

void Network::sendImage() {
	torch::data::Example<torch::Tensor, torch::Tensor> current_example = data->get(current_index);
	torch::Tensor flat_data = current_example.data.flatten();
	float * raw_data = flat_data.data<float>();
	int64_t pixel_count = flat_data.size(0);
	raw_pixels = new uchar[pixel_count];
	for (int i = 0; i < pixel_count; ++i) {
		float weight = raw_data[i] * 255.0;
		uchar pixel = static_cast<uchar>(qRound(weight));
		pixel = qBound(uchar(0), pixel, uchar(255));
		raw_pixels[i] = 255 - pixel;
	}
	QImage image(raw_pixels, 28, 28, QImage::Format_Grayscale8);
	int label = current_example.target.item<int>();
	emit example(current_index, image, label, label);
}

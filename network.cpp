#include "network.h"

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
	// Update the index counter, accounting for int rollover.

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

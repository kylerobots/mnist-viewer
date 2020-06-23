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
	while (current_index >= data_size) {
		current_index -= data_size;
	}
	sendImage();
}

QImage Network::convertToImage(at::Tensor tensor) {
	torch::Tensor flat_tensor = tensor.flatten();
	float * raw_data = flat_tensor.data<float>();
	int64_t pixel_count = flat_tensor.size(0);
	uchar * pixel_values = new uchar[pixel_count];
	for (int i = 0; i < pixel_count; ++i) {
		float pixel_float_value = raw_data[i] * 255.0;
		uchar pixel_value = static_cast<uchar>(qRound(pixel_float_value));
		pixel_value = qBound(uchar(0), pixel_value, uchar(255));
		// Invert the color.
		pixel_values[i] = 255 - pixel_value;
	}
	QImage image(pixel_values, 28, 28, QImage::Format_Grayscale8);
	return image.copy();
}

void Network::sendImage() {
	// Get whichever iteration is requested.
	torch::data::Example<torch::Tensor, torch::Tensor> current_example = data->get(current_index);
	// The inputs to the network have to be of the shape {batch_size, channels, width, height}
	// so add an additional dimension into the tensor.
	torch::Tensor input = current_example.data.unsqueeze(0);
	std::cout << input.sizes() << "\n$$$$$$$$$$$$$$" << std::endl;
	//    torch::Tensor prediction = network->forward(current_example.data);
	std::cout << "Done PRediction!!!!" << std::endl;
	QImage image = convertToImage(current_example.data);
	int label = current_example.target.item<int>();
	emit example(current_index, image, label, label);
}

#include "network.h"

#include <iostream>

Network::Network(QObject * parent) :
		QObject(parent),

		network(
				torch::nn::Conv2d(torch::nn::Conv2dOptions(1, 32, 3)),
				torch::nn::ReLU(torch::nn::ReLUOptions()),
				torch::nn::MaxPool2d(torch::nn::MaxPool2dOptions(2)),

				torch::nn::Conv2d(torch::nn::Conv2dOptions(32, 64, 3)),
				torch::nn::ReLU(torch::nn::ReLUOptions()),
				torch::nn::MaxPool2d(torch::nn::MaxPool2dOptions(2)),

				torch::nn::Conv2d(torch::nn::Conv2dOptions(64, 64, 3)),
				torch::nn::ReLU(torch::nn::ReLUOptions()),

				torch::nn::Flatten(torch::nn::FlattenOptions()),

				torch::nn::Linear(torch::nn::LinearOptions(576, 64)),
				torch::nn::ReLU(torch::nn::ReLU()),

				torch::nn::Linear(torch::nn::LinearOptions(64, 10)),
				torch::nn::Softmax(torch::nn::SoftmaxOptions(1))) {
	network->train(false);
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

void Network::startTraining(unsigned int batch_size, unsigned int epochs) {
	emit trainingUpdate(static_cast<uint>(0), static_cast<uint>(0), 0.0);
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
	torch::Tensor prediction_distribution = network->forward(input);
	// Whichever index has the max value is the predicted value.
	int prediction = prediction_distribution.argmax(1).item<int>();
	QImage image = convertToImage(current_example.data);
	int label = current_example.target.item<int>();
	emit example(current_index, image, label, prediction);
}

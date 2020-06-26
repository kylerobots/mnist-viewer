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

/*!
\class Network
\brief The backend class that handles data flow into and out of the network.

This class not only defines the network. It also sets up the slots and signals
to receive and send out data to it. It will perform all the right type
conversions to extract images, labels, and predictions from the example set.

In particular, the \ref network has the following shape:
Layer Type      | Details
----------------|----------------
2D Convolution  |32 3x3 Filters
ReLU Activation |None
2D Max Pool     |2x2 Filter
2D Convolution  |2 3x3 Filters
ReLU Activation |None
2D Max Pool     |2x2 Filter
2D Convolution  |1 3x3 Filter
ReLU Activation |None
Flatten         |None
Full Layer      |576 x 64
ReLU Activation |None
Full Layer      |64 x 10
Softmax Activation|None

*/
class Network : public QObject {
	Q_OBJECT
public:
	/*!
	 * \brief The default constructor.
	 * \param parent A parent QObject class, as is usual for any class that inherits it.
	 */
	explicit Network(QObject * parent = nullptr);
	~Network();

public slots:
	/*!
	 * \brief The slot that records when a new image should be predicted and displayed.
	 * 
	 * This will update all the internal indices and ultimate result in a new \ref example
	 * signal to be emitted. If the end of the dataset has been reached, it will wrap around
	 * to the start. Likewise for traveling backwards.
	 * \param increment The number of places to offset when selecting the new image. For instance,
	 * a value of 1 will change the image from index *N* to index *N+1*. If zero is sent, a new
	 * prediction is still made and an image emitted.
	 */
	void changeImage(int increment);

private:
	/*!
	 * \brief Takes a Tensor and converts it into a QImage.
	 * 
	 * This currently assumes this will be a 28x28 pixel image. It flattens the tensor
	 * and then fills in the QImage.
	 * \param tensor The input tensor, assumed to have the dimensions of 28x28.
	 * \return The resulting image. It is 28x28 and 8-bit grayscale. The image is copied from the
	 * tensor, so there is no issue with implicit sharing.
	 */
	QImage convertToImage(torch::Tensor tensor);
	/*!
	 * \brief The method to predict the image and output a \ref example signal.
	 * 
	 * This loads the image at index value \ref current_index, then uses \ref network
	 * to make a prediction about the data. It also calls \ref convertToImage to create
	 * a QImage. All this information is then sent out on the signal.
	 */
	void sendImage();

private:
	/*!
	 * \brief The index of the current image that should be displayed.
	 */
	size_t current_index;
	/*!
	 * \brief A pointer to the loaded dataset.
	 */
	QScopedPointer<torch::data::datasets::MNIST> data;
	/*!
	 * \brief The hard coded location of the dataset file.
	 */
	const QString DATA_PATH = "C:/Users/kylem/Documents/dev/src/mnist-viewer/data/test";
	/*!
	 * \brief The MNIST network that does the prediction.
	 */
	torch::nn::Sequential network;
	/*!
	 * \brief A pointer used in \ref convertToImage to translate from Tensor values to QImage.
	 */
	uchar * raw_pixels;

signals:
	/*!
	 * \brief A signal containing a new example and associated labels.
	 * \param index The index number where this example can be found in the dataset.
	 * \param image The image itself.
	 * \param truth The actual label from the dataset.
	 * \param prediction The predicted label from \ref network.
	 */
	void example(int index, QImage image, int truth, int prediction);
};

#endif // NETWORK_H

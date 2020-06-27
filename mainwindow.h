#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QImage>
#include <QMainWindow>
#include <QPixmap>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE

/*!
 * \class MainWindow
 * \extends QMainWindow
 * \brief The MainWindow class is used to hold the GUI elements of the project.
 * 
 * It displays the image, prediction, and label values. It also allows the user
 * to scroll through images and begin training.
 */
class MainWindow : public QMainWindow {
	Q_OBJECT
	/*!
     * \brief \ref TestMainWindow is a friend class so that it can properly test
     * \ref ui elements of this class.
     */
	friend class TestMainWindow;

public:
	/*!
	 * \brief The default constructor.
	 * \param parent The QWidget that owns this object.
	 */
	MainWindow(QWidget * parent = nullptr);
	~MainWindow();

public slots:
	/*!
	 * \brief Receive an image, its actual label, and its predicted label for publishing
	 * to the screen.
	 * 
	 * This slot will put the values in the right spots on the GUI when new images are
	 * received.
	 * \param index The index value associated with the example.
	 * \param image The actual image to display. It is assumed to be a 28x28 8-bit grayscale.
	 * \param label The real label for the image.
	 * \param prediction The network predicted label.
	 */
	void displayExample(int index, const QImage & image, int label, int prediction);

private:
	/*!
	 * \brief Called when the next button emits a clicked event.
	 * 
	 * This method will emit a \ref iterateImage signal with a value of 1.
	 * \param checked *Unused* If the button is depressed or not.
	 */
	void nextButtonClicked(bool checked);
	/*!
	 * \brief Called when the previous button emits a clicked event.
	 * 
	 * This method will emit a \ref iterateImage signal with a value of -1.
	 * \param checked *Unused* If the button is deperessed or not.
	 */
	void previousButtonClicked(bool checked);
	/*!
     * \brief Called when the training button emits a clicked event.
     * 
     * This method will emit a \ref startTraining signal. Currently, it
     * will only request a batch size of one with an epoch of one.
     * \param checked *Unused* If the button is depressed or not.
     */
	void trainButtonClicked(bool checked);

private:
	/*!
	 * \brief The pointer to the GUI elements generated from the mainwindow.ui file.
	 */
	Ui::MainWindow * ui;

signals:
	/*!
	 * \brief Signal the connected \ref Network object to change the example displayed.
	 * 
	 * It will advance or retreat the amount specified in difference. This could be
	 * any integer, but in practice is either -1, 0, or 1.
	 * \param difference The amount to increment the displayed image by.
	 */
	void iterateImage(int difference);
	/*!
     * \brief Signals the connected \ref Network object to train the network.
     * \param batch_size The number of examples in each batch.
     * \param epochs The number of iterations to train on.
     */
	void startTraining(unsigned int batch_size, unsigned int epochs);
};
#endif // MAINWINDOW_H

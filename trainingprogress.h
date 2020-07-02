#ifndef TRAININGPROGRESS_H
#define TRAININGPROGRESS_H

#include <QDialog>

namespace Ui {
	class TrainingProgress;
}

/*!
 * \brief The TrainingProgress class is used to update the user on the status
 * of training.
 * 
 * It contains two progress bars, one for batch and one for epoch, as well as a label
 * to indicate loss. The progress bars are automatically scaled between 0% and 100% based
 * on the max values provided at construction and the values provided at each update.
 * 
 * It is left to the containing class to determine when to hide this display.
 */
class TrainingProgress : public QDialog {
	Q_OBJECT

public:
	/*!
	 * \brief The constructor for the class.
	 * \param total_batches The total number of batches that will be displayed. Used to 
	 * set what value should be 100%.
	 * \param total_epochs The total number of epochs that will be displayed. Used to
	 * set what value should be 100%.
	 * \param parent The parent QWidget.
	 */
	explicit TrainingProgress(uint total_batches, uint total_epochs, QWidget * parent = nullptr);
	~TrainingProgress();
	/*!
	 * \brief Update the widgets displayed on the GUI.
	 * \param batch The current batch. This will be used to calculate the percentage to display.
	 * \param epoch The current epoch. This will be used to calculate the percentage to display.
	 * \param loss The current loss to display.
	 */
	void update(uint batch, uint epoch, float loss);

private:
	/*!
	 * \brief The pointer to the UI elements.
	 */
	Ui::TrainingProgress * ui;
};

#endif // TRAININGPROGRESS_H

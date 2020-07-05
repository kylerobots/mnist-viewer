#include "trainingprogress.h"

#include "ui_trainingprogress.h"

TrainingProgress::TrainingProgress(uint total_batches, uint total_epochs, QWidget * parent) :
		QDialog(parent),
		ui(new Ui::TrainingProgress) {
	ui->setupUi(this);
	ui->batch_progress_bar->setMinimum(0);
	ui->epoch_progress_bar->setMinimum(0);
	ui->batch_progress_bar->setMaximum(total_batches);
	ui->epoch_progress_bar->setMaximum(total_epochs);
}

TrainingProgress::~TrainingProgress() {
	delete ui;
}

void TrainingProgress::update(uint batch, uint epoch, float loss) {
	// Set these to one larger than the max because the network reports the
	// batch and epoch the system is currently training on. So if it is the
	// last one, it will show as 100% already, which is not strictly correct.
	ui->batch_progress_bar->setValue(batch - 1);
	ui->epoch_progress_bar->setValue(epoch - 1);
	ui->loss_display->setText(QString::number(loss));
}

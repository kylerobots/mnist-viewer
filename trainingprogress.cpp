#include "trainingprogress.h"

#include "ui_trainingprogress.h"

TrainingProgress::TrainingProgress(uint total_batches, uint total_epochs, QWidget * parent) :
		QDialog(parent),
		ui(new Ui::TrainingProgress) {
	ui->setupUi(this);
	ui->batch_progress_bar->setMinimum(1);
	ui->batch_progress_bar->setMaximum(total_batches);
	ui->epoch_progress_bar->setMinimum(1);
	ui->epoch_progress_bar->setMaximum(total_epochs);
}

TrainingProgress::~TrainingProgress() {
	delete ui;
}

void TrainingProgress::update(uint batch, uint epoch, float loss) {
	ui->batch_progress_bar->setValue(batch);
	ui->epoch_progress_bar->setValue(epoch);
	ui->loss_display->setText(QString::number(loss));
}

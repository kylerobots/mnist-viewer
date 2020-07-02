#include "trainingprogress.h"
#include "ui_trainingprogress.h"

TrainingProgress::TrainingProgress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrainingProgress)
{
    ui->setupUi(this);
}

TrainingProgress::~TrainingProgress()
{
    delete ui;
}

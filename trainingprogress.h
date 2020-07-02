#ifndef TRAININGPROGRESS_H
#define TRAININGPROGRESS_H

#include <QDialog>

namespace Ui {
class TrainingProgress;
}

class TrainingProgress : public QDialog
{
    Q_OBJECT
    
public:
    explicit TrainingProgress(QWidget *parent = nullptr);
    ~TrainingProgress();
    
private:
    Ui::TrainingProgress *ui;
};

#endif // TRAININGPROGRESS_H

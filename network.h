#ifndef NETWORK_H
#define NETWORK_H

#include <QImage>
#include <QObject>

class Network : public QObject {
	Q_OBJECT
public:
	explicit Network(QObject * parent = nullptr);

public slots:
	void changeImage(int increment);

signals:
	void example(QImage image, int truth, int prediction);
};

#endif // NETWORK_H

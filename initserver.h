#ifndef INITSERVER_H
#define INITSERVER_H

#include <QObject>

class initServer : public QObject
{
    Q_OBJECT
public:
    explicit initServer(QObject *parent = nullptr);
    void start();
private:
signals:

};

#endif // INITSERVER_H

#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <fstream>
#include <cstring>
struct log{
    char user[100]={}, password[100]={};
};
class MyThread : public QThread
{
    Q_OBJECT
public:
    std::ifstream fin;
    std::ofstream fout;
    explicit MyThread(int ID, QObject *parent=0);
    void run();
    bool search(char*);
    void addNew(char*);
    void getList();
signals:
    void error(QTcpSocket::SocketError socketerror);
public slots:
    void readyRead();
    void disconnected();
private:
    QTcpSocket *socket;
    int socketDescriptor;
};

#endif // MYTHREAD_H

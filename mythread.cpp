#include "mythread.h"

MyThread::MyThread(int ID, QObject *parent):
    QThread(parent)
{
    this->socketDescriptor=ID;
}

void MyThread::run()
{
    //thread starts here
    qDebug()<<"URA";
    qDebug()<<socketDescriptor<<" Starting thread ";
    socket=new QTcpSocket();
    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(socket->error());
        return;
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::DirectConnection);
    qDebug()<<socket->socketDescriptor()<<"client conected";
    exec();
}
void MyThread::readyRead()
{
    std::ifstream fin("date.in");
    std::ofstream fout("date.txt");
    QByteArray Data=socket->readAll();
    //char da[100];
    char *client_data;
    //QByteArray::replace(Data, data);
    client_data=Data.data();

    if(MyThread::search(client_data))
    {
        qDebug()<<"User connected!";
        socket->write("okey");
    }
    else
    {
        qDebug()<<"User not connected, he send in: "<<Data;
        socket->write("Do you want to register? [y/n]");
        QByteArray Data2=socket->readAll();
        char* client_data2;
        client_data=Data2.data();
        if(Data=="y")
        {
            //socket->write()
            MyThread::addNew(client_data2);
        }
        else
        {
            socket->write("okey then :)");
        }
    }
}
void MyThread::disconnected()
{
     qDebug()<<socketDescriptor<<"Disconnected";
    socket->deleteLater();
    exit(0);
}
bool MyThread::search(char* logs)
{
    char* login;
    login=strtok(logs," ");
    char* pass;
    pass=strtok(NULL, " ");
    while(!fin.eof())
    {
        char c[100];
        fin.getline(c, 100);
        char* user;
        user=strtok(c, " ");
        char* passcode;
        passcode=strtok(NULL, " ");
        if(!strcmp(user, login))
        {
            socket->write("User correct!");
            if(!strcmp(pass, passcode))
            {
                socket->write("You're in!");
                return 1;
            }
        }

    }
    fin.close();
    fin.open("date.txt");
    socket->write("Date not found");
    return 0;
}

void MyThread::addNew(char* logs)
{
    char* login;
    login=strtok(logs, " ");
    char* pass;
    pass=strtok(NULL, " ");
    fout<<'\n'<<login<<" "<<pass<<" ";
    socket->write("New user added");
}

void MyThread::getList()
{
    socket->write("Getting list: '\n");
    while(fin.eof())
    {
        char user[100], code[100], scor[100];
        fin>>user>>code>>scor;
        socket->write(user);
        socket->write("-");
        socket->write(scor);
        socket->write("\n");
    }

}

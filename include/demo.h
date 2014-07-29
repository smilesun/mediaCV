#ifndef DEMO_H
#define DEMO_H


/*
 * this file demos the usage of certain classes
//thread method 1
class Worker : public QObject
{
    Q_OBJECT
private:
    CDetectTrack cd;
public slots:
    void doWork(const QString &parameter) {

        QString result;
        // ... here is the expensive or blocking operation ...
        //.......................................
        emit resultReady(result);
    }

signals:
    void resultReady(const QString &result);//notify the controller to handle the event
};

class CThreadController : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    CThreadController() {
        Worker *worker = new Worker;
        worker->moveToThread(&workerThread);
        connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &Controller::operate, worker, &Worker::doWork);
        connect(worker, &Worker::resultReady, this, &Controller::handleResults);
        workerThread.start();
    }
    ~CThreadController() {
        workerThread.quit();
        workerThread.wait();
    }
public slots:
    void handleResults(const QString &);
signals:
    void operate(const QString &);
};


//demo code:thread method 2
class WorkerThread : public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE {
        QString result;
        // ... here is the expensive or blocking operation ...
        emit resultReady(result);
    }
signals:
    void resultReady(const QString &s);
};
/*

void MyObject::startWorkInAThread()
{
    WorkerThread *workerThread = new WorkerThread(this);
    connect(workerThread, &WorkerThread::resultReady, this, &MyObject::handleResults);
    connect(workerThread, &WorkerThread::finished, workerThread, &QObject::deleteLater);
    workerThread->start();
}


*/

#endif // DEMO_H

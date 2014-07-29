#include <QtCore>

#include <stdio.h>
#include <stdlib.h>

const int DataSize = 100000;

const int BufferSize = 8192;
char buffer[BufferSize];

QSemaphore freeBytes(BufferSize);
QSemaphore usedBytes;

class Producer : public QThread
{
public:
    void run()
    {
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));


        for (int i = 0; i < DataSize; ++i)
        {

            freeBytes.acquire();
            buffer[i % BufferSize] = "ACGT"[(int)qrand() % 4];
            usedBytes.release();
        }

    }
};

class Consumer : public QThread
{
    Q_OBJECT
public:
    void run()
    {
        for (int i = 0; i < DataSize; ++i) {
            usedBytes.acquire();
            fprintf(stderr, "%c", buffer[i % BufferSize]);
            freeBytes.release();
        }
        fprintf(stderr, "\n");
    }

signals:
    void stringConsumed(const QString &text);

protected:
    bool finish;
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    Producer producer;
    Consumer consumer;
    producer.start();
    consumer.start();
    producer.wait();
    consumer.wait();
    return 0;
}

#include "semaphores.moc"

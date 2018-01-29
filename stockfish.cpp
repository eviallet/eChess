#include "stockfish.h"

Stockfish::Stockfish(QObject* parent) {
    engine = new QProcess(parent);

    connect(engine,SIGNAL(started()),
            this,SLOT(engineStarted()));
    connect(engine,SIGNAL(error(QProcess::ProcessError)),
            this,SLOT(error(QProcess::ProcessError)));
    connect(engine,SIGNAL(readyRead()),
            this,SLOT(dataAvailable()));


    engine->start("C:/stockfish.exe");
}


void Stockfish::send(QString cmd) {
    cmd.append("\n");
    engine->write(cmd.toLocal8Bit().data());
}

void Stockfish::setDifficulty(int level) {
    send("setoption name Skill Level value "+QString::number(level));
}

void Stockfish::engineStarted() {
    qDebug() << "Started";
}

void Stockfish::dataAvailable() {
    QByteArray array = engine->readAll();
    QString received = QString::fromStdString(array.toStdString()).replace("\r","\n");
    for(int i=0; i<received.count("\n");i+=2) {
        QString line = received.section("\n",i,i+1).replace("\n","");
        //qDebug() << line;
        if(line.contains("bestmove"))
            emit(bestMove(line));
        if(line.contains("readyok"))
            emit(readyok());
        if(line.contains("uciok"))
            emit(uciok());
        if(line.contains("info"))
            emit(info(line));
    }
}

void Stockfish::error(QProcess::ProcessError) {
    qDebug() << engine->error();
}


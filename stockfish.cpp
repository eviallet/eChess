#include "stockfish.h"

Stockfish::Stockfish(QObject* parent) {
    engine = new QProcess(parent);

    connect(engine, SIGNAL(started()), this, SLOT(engineStarted()));
    connect(engine, SIGNAL(error(QProcess::ProcessError)), this, SLOT(error(QProcess::ProcessError)));
    connect(engine, SIGNAL(readyRead()), this, SLOT(dataAvailable()));

    engine->start("C:/stockfish.exe");
}

/// =========================================================================
///                                COMMANDS
/// =========================================================================



void Stockfish::send(QString cmd) {
    //qDebug() << "Sending to Stockfish : " << cmd;
    cmd.append("\n");
    engine->write(cmd.toLocal8Bit().data());
}

void Stockfish::setDifficulty(int level) {
    send("setoption name Skill Level value "+QString::number(level));
}

void Stockfish::setELO(int elo) {
    if(elo<1350)
        elo = 1350;
    else if(elo>2850)
        elo = 2850;
    send("setoption name UCI_LimitStrength value 1");
    send("setoption name UCI_ELO value "+QString::number(elo));
}

/// =========================================================================
///                          SIGNALS FROM ENGINE
/// =========================================================================

void Stockfish::dataAvailable() {
    QByteArray array = engine->readAll();
    QString received = QString::fromStdString(array.toStdString()).replace("\r","\n");
    for(int i=0; i<received.count("\n");i+=2) {
        QString line = received.section("\n",i,i+1).replace("\n","");
        //qDebug() << line;
        if(line.contains("bestmove"))
            emit(bestMove(line));
        else if(line.contains("readyok"))
            emit(readyok());
        else if(line.contains("uciok"))
            emit(uciok());
        else if(line.contains("info"))
            emit(info(line));
        else if(line.right(3) == ": 1")
            emit(move(line.left(4)));
        else if(line.left(15) == "Nodes searched:")
            emit(endMove(line.replace("Nodes searched: ","").toInt()));
        else if(line.left(4) == "Fen:")
            emit(fen(line.replace("Fen: ","")));
        else if(line.left(9) == "Checkers:")
            emit(check(line.replace("Checkers: ","")));
    }
}

/// =========================================================================
///                             PROCESS MANAGEMENT
/// =========================================================================

void Stockfish::engineStarted() {
    qDebug() << "Started";
}

void Stockfish::error(QProcess::ProcessError) {
    qDebug() << engine->error();
}

void Stockfish::kill() {
    engine->terminate();
}


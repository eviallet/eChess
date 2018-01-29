#ifndef STOCKFISH_H
#define STOCKFISH_H

#include <QCoreApplication>
#include <QProcess>
#include <QDebug>


static QString cmd_text[] = {
    "isready",
    "go",
    "ucinewgame"
};

class Stockfish : public QObject {
    Q_OBJECT
    public:
        Stockfish(QObject *parent);

        void send(QString cmd);
        void setDifficulty(int level);
    signals:
        void uciok();
        void readyok();
        void bestMove(QString move);
        void info(QString infos);
    private slots:
        void dataAvailable();
        void engineStarted();
        void error(QProcess::ProcessError);
    private :
        QProcess *engine;
};

#endif // STOCKFISH_H

#ifndef STOCKFISH_H
#define STOCKFISH_H

#include <QCoreApplication>
#include <QProcess>
#include <QDebug>


class Stockfish : public QObject {
    Q_OBJECT
    public:
        Stockfish(QObject *parent);

        void send(QString cmd);
        void setDifficulty(int level);
        void setELO(int elo);
        void kill();
signals:
        void uciok();
        void readyok();
        void bestMove(QString move);
        void info(QString infos);
        void move(QString move);
        void endMove();
        void fen(QString fen);
    private slots:
        void dataAvailable();
        void engineStarted();
        void error(QProcess::ProcessError);
    private :
        QProcess *engine;
};

#endif // STOCKFISH_H

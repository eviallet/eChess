#include "boardmodel.h"
#include <QDebug>


BoardModel::BoardModel() {
    /*
    for(int c=0; c<COLS; c++)
        for(int r=0; r<ROWS; r++)
            for(int role=0; role<3; role++) {
                if(role==getRole(Qt::BackgroundRole)) {
                    if ((c+r)%2!=0)
                        board[c][r][role] = QVariant(QColor(Qt::gray));
                    else
                        board[c][r][role] = QVariant(QColor(Qt::white));
                } else
                    board[c][r][role] = QVariant();
            }
            */
}

int BoardModel::rowCount(const QModelIndex &parent) const {
    return ROWS;
}

int BoardModel::columnCount(const QModelIndex &parent) const {
    return COLS;
}

bool BoardModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    board[index.column()][index.row()][getRole(role)] = value;
    emit(dataChanged(index,index));
    return true;
}

QVariant BoardModel::data(const QModelIndex &index, int role) const {
    if(board[index.column()][index.row()][getRole(role)].isValid())
        return board[index.column()][index.row()][getRole(role)];
    else
        return QVariant();
}
/*
int BoardModel::getRole(int role) const {
    switch(role) {
        case Qt::DecorationRole:
            return 0;
        case Qt::BackgroundRole:
            return 1;
        default:
            return 2;
    }
}
*/


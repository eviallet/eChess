#include "boardmodel.h"
#include <QDebug>


BoardModel::BoardModel() {
    for(int r=0; r<ROWS; r++) {
        for(int c=0; c<COLS; c++) {
            if((c+r)%2!=0)
                board[c][r][ROLE_BKG] = QVariant(QBrush(QImage(":/wood_b.jpg").scaled(SQUARE_SIZE,SQUARE_SIZE)));
            else
                board[c][r][ROLE_BKG] = QVariant(QBrush(QImage(":/wood_w.jpg").scaled(SQUARE_SIZE,SQUARE_SIZE)));
        }
    }
}

int BoardModel::rowCount(const QModelIndex &parent) const {
    return ROWS;
}

int BoardModel::columnCount(const QModelIndex &parent) const {
    return COLS;
}

bool BoardModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    board[index.column()][index.row()][ROLE_DEC] = value;

    emit(dataChanged(index,index));
    return true;
}

QVariant BoardModel::data(const QModelIndex &index, int role) const {
    if(role==Qt::BackgroundRole)
        return board[index.column()][index.row()][ROLE_BKG];
    else if(role==Qt::DecorationRole)
        return board[index.column()][index.row()][ROLE_DEC];
    else
        return QVariant();
}

/// TODO
/*
QString Analyzer::toFEN(const Square board[COLS][ROWS]) {

}
*/

#include "boardmodel.h"

BoardModel::BoardModel() {

}

int BoardModel::rowCount(const QModelIndex &parent) const {
    return ROWS;
}

int BoardModel::columnCount(const QModelIndex &parent) const {
    return COLS;
}

bool BoardModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    board[index.column()][index.row()] = value;
    emit(dataChanged(index,index));
    return true;
}

QVariant BoardModel::data(const QModelIndex &index, int role) const {
    switch(role) {
        case Qt::DecorationRole :
            return board[index.column()][index.row()];
        case Qt::BackgroundColorRole:
            if ((index.column()+index.row())%2!=0)
                return QVariant(QColor(Qt::gray));
            else
                return QVariant(QColor(Qt::white));
        case Qt::BackgroundRole :
            return QVariant();
        case Qt::ForegroundRole :
            return QVariant();
        default:
            return QVariant();
    }
}


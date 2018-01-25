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
    return true;
}

QVariant BoardModel::data(const QModelIndex &index, int role) const {
    return board[index.column()][index.row()];
}


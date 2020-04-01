#ifndef BOARDMODEL_H
#define BOARDMODEL_H

#include <QAbstractTableModel>
#include <QModelIndex>
#include "square.h"

#define ROLE_BKG 0
#define ROLE_DEC 1

#define SQUARE_SIZE 80
#define CIRCLE_SIZE SQUARE_SIZE/4

class BoardModel : public QAbstractTableModel {
    public:
        BoardModel();
        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant data(const QModelIndex &index, int role = Qt::DecorationRole) const;
        bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    private:
        QVariant board[COLS][ROWS][2];
};

#endif // BOARDMODEL_H

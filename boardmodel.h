#ifndef BOARDMODEL_H
#define BOARDMODEL_H

#include <QAbstractTableModel>
#include <QModelIndex>
#include "square.h"

#define SQUARE_SIZE 80

class BoardModel : public QAbstractTableModel {
    public:
        BoardModel();
        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    private:
        QVariant board[COLS][ROWS];
};

#endif // BOARDMODEL_H

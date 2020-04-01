#ifndef PROMOTIONDIALOG_H
#define PROMOTIONDIALOG_H

#include <QDialog>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QPushButton>
#include "constants.h"
#include "square.h"

#define BTN_SIZE 100

class PromotionDialog : public QDialog {
    Q_OBJECT
public:
    PromotionDialog(QWidget* parent = 0, Turn turn = Turn::WHITE);
signals:
    void pieceSelected(char);
public slots:
    void accept();
    void reject();
private slots:
    void onItemSelected();
    QPushButton *createRoundButton(QIcon icon);
private:
    Turn turn;
};

#endif // PROMOTIONDIALOG_H

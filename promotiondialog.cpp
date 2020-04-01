#include "promotiondialog.h"

PromotionDialog::PromotionDialog(QWidget *parent, Turn turn) : QDialog(parent), turn(turn) {
    setWindowFlag(Qt::FramelessWindowHint);

    QHBoxLayout *mainLayout = new QHBoxLayout();

    QPushButton *queen;
    QPushButton *rook;
    QPushButton *knight;
    QPushButton *bishop;

    if(turn == Turn::WHITE) {
        queen = createRoundButton(Square::getPic(Piece::W_QUEEN));
        rook = createRoundButton(Square::getPic(Piece::W_ROOK));
        knight = createRoundButton(Square::getPic(Piece::W_KNIGHT));
        bishop = createRoundButton(Square::getPic(Piece::W_BISHOP));
    } else {
        queen = createRoundButton(Square::getPic(Piece::B_QUEEN));
        rook = createRoundButton(Square::getPic(Piece::B_ROOK));
        knight = createRoundButton(Square::getPic(Piece::B_KNIGHT));
        bishop = createRoundButton(Square::getPic(Piece::B_BISHOP));
    }

    queen->setAccessibleName("q");
    rook->setAccessibleName("r");
    knight->setAccessibleName("n");
    bishop->setAccessibleName("b");

    mainLayout->addWidget(queen);
    mainLayout->addWidget(rook);
    mainLayout->addWidget(knight);
    mainLayout->addWidget(bishop);

    setLayout(mainLayout);
}

QPushButton* PromotionDialog::createRoundButton(QIcon icon) {
    QPushButton *btn = new QPushButton(icon, "");
    btn->setFixedSize(QSize(BTN_SIZE, BTN_SIZE));
    QRect rect(0,0,BTN_SIZE*0.95,BTN_SIZE*0.95);
    QRegion region(rect, QRegion::Ellipse);
    btn->setMask(region);
    btn->setIconSize(QSize(BTN_SIZE*0.95,BTN_SIZE*0.95));
    btn->setFlat(true);

    PromotionDialog::connect(btn, &QPushButton::clicked, this, &PromotionDialog::onItemSelected);

    return btn;
}

void PromotionDialog::onItemSelected() {
    QPushButton* s = (QPushButton*)sender();
    char piece = s->accessibleName()[0].toLatin1();

    if(turn == Turn::WHITE)
        piece += ('A' - 'a');

    emit(pieceSelected(piece));
    QDialog::accept();
}


/// PREVENT FROM CLOSING WITHOUT CHOSING AN OPTION

void PromotionDialog::accept() {
    return;
}
void PromotionDialog::reject() {
    return;
}


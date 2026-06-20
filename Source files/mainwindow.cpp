#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QTimer>
#include <QSizePolicy>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    choix(0),
    selLigne(-1),
    selCol(-1),
    jeuEnCours(false)
{
    srand(time(0));

    setWindowTitle("Jeu d'Echecs");
    setMinimumSize(700, 750);

    initialiserUI();
    afficherMenu();
}

MainWindow::~MainWindow()
{
}

// ─────────────────────────────────────────────
//  Construction de l'interface
// ─────────────────────────────────────────────
void MainWindow::initialiserUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QGridLayout(centralWidget);
    mainLayout->setSpacing(5);

    // ── Menu ──────────────────────────────────
    menuWidget = new QWidget();
    QVBoxLayout *menuLayout = new QVBoxLayout(menuWidget);
    menuLayout->setAlignment(Qt::AlignCenter);

    QLabel *titre = new QLabel("♟  JEU D'ÉCHECS  ♟");
    titre->setAlignment(Qt::AlignCenter);
    titre->setFont(QFont("Arial", 22, QFont::Bold));
    titre->setStyleSheet("color: #2c3e50; margin: 20px;");

    btnHvsH = new QPushButton("1 - Joueur vs Joueur");
    btnHvsM = new QPushButton("2 - Joueur vs Machine");

    QString btnStyle =
        "QPushButton {"
        "  background-color: #2c3e50;"
        "  color: white;"
        "  border-radius: 8px;"
        "  padding: 15px 30px;"
        "  font-size: 16px;"
        "  margin: 8px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #3d5166;"
        "}";

    btnHvsH->setStyleSheet(btnStyle);
    btnHvsM->setStyleSheet(btnStyle);
    btnHvsH->setFixedSize(280, 60);
    btnHvsM->setFixedSize(280, 60);

    menuLayout->addWidget(titre);
    menuLayout->addSpacing(30);
    menuLayout->addWidget(btnHvsH, 0, Qt::AlignCenter);
    menuLayout->addWidget(btnHvsM, 0, Qt::AlignCenter);

    connect(btnHvsH, &QPushButton::clicked, this, &MainWindow::lancerHvsH);
    connect(btnHvsM, &QPushButton::clicked, this, &MainWindow::lancerHvsM);

    // ── Plateau ───────────────────────────────
    boardWidget = new QWidget();
    QVBoxLayout *boardLayout = new QVBoxLayout(boardWidget);
    boardLayout->setSpacing(4);

    // Barre d'info
    lblTour = new QLabel("Tour : Joueur Blanc");
    lblTour->setAlignment(Qt::AlignCenter);
    lblTour->setFont(QFont("Arial", 14, QFont::Bold));
    lblTour->setStyleSheet(
        "background-color: #2c3e50; color: white;"
        "border-radius: 6px; padding: 8px;");

    lblInfo = new QLabel("Cliquez sur une pièce pour la sélectionner");
    lblInfo->setAlignment(Qt::AlignCenter);
    lblInfo->setFont(QFont("Arial", 11));
    lblInfo->setStyleSheet("color: #555; padding: 4px;");

    // Grille 8x8
    QWidget *gridWrapper = new QWidget();
    gridLayout = new QGridLayout(gridWrapper);
    gridLayout->setSpacing(1);
    gridLayout->setContentsMargins(2,2,2,2);

    // Numéros de colonnes (0-7)
    for(int j = 0; j < 8; j++)
    {
        QLabel *num = new QLabel(QString::number(j));
        num->setAlignment(Qt::AlignCenter);
        num->setFont(QFont("Arial", 10, QFont::Bold));
        num->setStyleSheet("color: #2c3e50;");
        gridLayout->addWidget(num, 0, j+1);
    }

    // Cases + numéros de lignes
    for(int i = 0; i < 8; i++)
    {
        QLabel *numL = new QLabel(QString::number(i));
        numL->setAlignment(Qt::AlignCenter);
        numL->setFont(QFont("Arial", 10, QFont::Bold));
        numL->setStyleSheet("color: #2c3e50;");
        gridLayout->addWidget(numL, i+1, 0);

        for(int j = 0; j < 8; j++)
        {
            cases[i][j] = new QPushButton();
            cases[i][j]->setFixedSize(72, 72);
            cases[i][j]->setFont(QFont("Arial", 11, QFont::Bold));

            // Connexion avec capture de i,j
            connect(cases[i][j], &QPushButton::clicked,
                    this, [this, i, j](){ caseCliquee(i, j); });

            gridLayout->addWidget(cases[i][j], i+1, j+1);
        }
    }

    // Bouton Reset
    btnReset = new QPushButton("🔄  Nouvelle partie");
    btnReset->setStyleSheet(
        "QPushButton {"
        "  background-color: #e74c3c; color: white;"
        "  border-radius: 6px; padding: 8px 20px;"
        "  font-size: 13px;"
        "}"
        "QPushButton:hover { background-color: #c0392b; }");
    btnReset->setFixedHeight(40);
    connect(btnReset, &QPushButton::clicked, this, &MainWindow::resetJeu);

    boardLayout->addWidget(lblTour);
    boardLayout->addWidget(lblInfo);
    boardLayout->addWidget(gridWrapper, 0, Qt::AlignCenter);
    boardLayout->addWidget(btnReset);

    mainLayout->addWidget(menuWidget, 0, 0);
    mainLayout->addWidget(boardWidget, 0, 0);

    // Timer pour l'IA
    timerIA = new QTimer(this);
    timerIA->setSingleShot(true);
    connect(timerIA, &QTimer::timeout, this, &MainWindow::tourIA);

    boardWidget->hide();
}


//  Affichage menu / jeu

void MainWindow::afficherMenu()
{
    boardWidget->hide();
    menuWidget->show();
    jeuEnCours = false;
}

void MainWindow::afficherJeu()
{
    menuWidget->hide();
    boardWidget->show();
    jeuEnCours = true;
    selLigne = -1;
    selCol   = -1;
    afficherPlateau();
    mettreAJourInfo();
}

//  Lancement des modes de jeu

void MainWindow::lancerHvsH()
{
    choix = 1;
    p = plateau();          // Réinitialise le plateau
    afficherJeu();
}

void MainWindow::lancerHvsM()
{
    choix = 2;
    p = plateau();
    afficherJeu();
}

void MainWindow::resetJeu()
{
    timerIA->stop();
    afficherMenu();
}

// ─────────────────────────────────────────────
//  Affichage du plateau
// ─────────────────────────────────────────────
QString MainWindow::getPieceTexte(int valeur)
{
    switch(valeur)
    {
    case 1:  return QString::fromUtf8("\u2659"); // pion blanc
    case 2:  return QString::fromUtf8("\u2656"); // tour blanche
    case 3:  return QString::fromUtf8("\u2658"); // cavalier blanc
    case 4:  return QString::fromUtf8("\u2657"); // fou blanc
    case 5:  return QString::fromUtf8("\u2655"); // dame blanche
    case 6:  return QString::fromUtf8("\u2654"); // roi blanc
    case -1: return QString::fromUtf8("\u265F"); // pion noir
    case -2: return QString::fromUtf8("\u265C"); // tour noire
    case -3: return QString::fromUtf8("\u265E"); // cavalier noir
    case -4: return QString::fromUtf8("\u265D"); // fou noir
    case -5: return QString::fromUtf8("\u265B"); // dame noire
    case -6: return QString::fromUtf8("\u265A"); // roi noir
    default: return QString();
    }
}

QString MainWindow::getCouleurCase(int i, int j,
                                   bool selectionnee,
                                   bool estDepart)
{
    QString couleurBase;

    if(selectionnee && estDepart)
    {
        // Case sélectionnée (départ)
        couleurBase = "#f1c40f";        // jaune
    }
    else if(selectionnee)
    {
        // Case de destination possible (surbrillance légère)
        couleurBase = "#a9cce3";
    }
    else if((i + j) % 2 == 0)
    {
        couleurBase = "#f0d9b5";        // beige clair
    }
    else
    {
        couleurBase = "#b58863";        // brun foncé
    }
    return couleurBase;
}

void MainWindow::afficherPlateau()
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            int val = p.getPiece(i, j);

            // Détermine si la case est sélectionnée
            bool estDepart = (selLigne == i && selCol == j);

            // Détermine si c'est une destination possible
            bool estDest = false;
            if(selLigne != -1 && !estDepart)
            {
                if(p.evaluer(selLigne, selCol, i, j) == 1)
                {
                    // Vérifie que la destination n'est pas une pièce alliée
                    int pieceDepart = p.getPiece(selLigne, selCol);
                    int pieceDest   = p.getPiece(i, j);

                    bool memecamp = (pieceDepart > 0 && pieceDest > 0) ||
                                    (pieceDepart < 0 && pieceDest < 0);
                    if(!memecamp)
                        estDest = true;
                }
            }

            QString bg = getCouleurCase(i, j, estDepart || estDest, estDepart);

            // Couleur du texte selon la couleur de la pièce
            QString couleurTexte;
            if(val > 0)       couleurTexte = "#ffffff";   // Blanc : texte blanc
            else if(val < 0)  couleurTexte = "#1a1a1a";   // Noir  : texte sombre
            else              couleurTexte = "transparent";

            cases[i][j]->setText(getPieceTexte(val));
            cases[i][j]->setStyleSheet(
                QString(
                    "QPushButton {"
                    "  background-color: %1;"
                    "  color: %2;"
                    "  border: 1px solid #888;"
                    "  border-radius: 0px;"
                    "  font-size: 40px;"
                    "  font-weight: bold;"
                    "}"
                    "QPushButton:hover {"
                    "  background-color: #a8d8a8;"
                    "  border: 2px solid #27ae60;"
                    "}"
                    ).arg(bg).arg(couleurTexte)
                );
        }
    }
}

// ─────────────────────────────────────────────
//  Mise à jour du label d'info
// ─────────────────────────────────────────────
void MainWindow::mettreAJourInfo()
{
    if(p.getTour() % 2 == 0)
    {
        lblTour->setText("Tour : Joueur Noir");
        lblTour->setStyleSheet(
            "background-color: #1a1a1a; color: white;"
            "border-radius: 6px; padding: 8px;");
    }
    else
    {
        lblTour->setText("Tour : Joueur Blanc");
        lblTour->setStyleSheet(
            "background-color: #f5f5f5; color: #2c3e50;"
            "border-radius: 6px; padding: 8px;"
            "border: 1px solid #ccc;");
    }

    if(choix == 2 && p.getTour() % 2 == 0)
    {
        lblInfo->setText("L'IA réfléchit...");
    }
    else
    {
        lblInfo->setText("Cliquez sur une pièce pour la sélectionner");
    }
}

// ─────────────────────────────────────────────
//  Gestion du clic sur une case
// ─────────────────────────────────────────────
void MainWindow::caseCliquee(int ligne, int col)
{
    if(!jeuEnCours) return;

    // Bloque si c'est le tour de l'IA
    if(choix == 2 && p.getTour() % 2 == 0) return;

    int val = p.getPiece(ligne, col);

    // ── Aucune pièce sélectionnée ──────────────
    if(selLigne == -1)
    {
        // Tour pair = Noir joue / Tour impair = Blanc joue
        bool tourBlanc = (p.getTour() % 2 != 0);

        if(tourBlanc && val > 0)
        {
            selLigne = ligne;
            selCol   = col;
            lblInfo->setText(
                QString("Pièce sélectionnée en (%1,%2) — Choisissez la destination")
                    .arg(ligne).arg(col));
            afficherPlateau();
        }
        else if(!tourBlanc && val < 0)
        {
            selLigne = ligne;
            selCol   = col;
            lblInfo->setText(
                QString("Pièce sélectionnée en (%1,%2) — Choisissez la destination")
                    .arg(ligne).arg(col));
            afficherPlateau();
        }
        else
        {
            lblInfo->setText("Sélectionnez une de vos pièces !");
        }
        return;
    }


    // Clic sur la même case → déselectionner
    if(selLigne == ligne && selCol == col)
    {
        selLigne = -1;
        selCol   = -1;
        lblInfo->setText("Sélection annulée");
        afficherPlateau();
        return;
    }

    // Tenter le déplacement
    int res = p.deplacer(selLigne, selCol, ligne, col);

    if(res == 1)
    {
        selLigne = -1;
        selCol   = -1;
        afficherPlateau();
        verifierFinPartie();

        if(jeuEnCours)
        {
            mettreAJourInfo();

            // Mode HvsM : lancer l'IA avec délai
            if(choix == 2 && p.getTour() % 2 == 0)
            {
                timerIA->start(600);
            }
        }
    }
    else
    {
        // Mouvement invalide : peut-être re-sélectionner une autre pièce
        bool tourBlanc = (p.getTour() % 2 != 0);

        if((tourBlanc && val > 0) || (!tourBlanc && val < 0))
        {
            selLigne = ligne;
            selCol   = col;
            lblInfo->setText(
                QString("Nouvelle sélection : (%1,%2)").arg(ligne).arg(col));
            afficherPlateau();
        }
        else
        {
            lblInfo->setText("Mouvement invalide ! Réessayez.");
        }
    }
}

// ─────────────────────────────────────────────
//  Tour de l'IA
// ─────────────────────────────────────────────
void MainWindow::tourIA()
{
    if(!jeuEnCours) return;

    p.deplacerIA();
    afficherPlateau();
    verifierFinPartie();

    if(jeuEnCours)
        mettreAJourInfo();
}

// ─────────────────────────────────────────────
//  Vérification fin de partie
// ─────────────────────────────────────────────
void MainWindow::verifierFinPartie()
{
    int fin = p.finPartie();

    if(fin == 1)
    {
        jeuEnCours = false;
        QMessageBox::information(this,
                                 "Fin de partie",
                                 "👑  Le joueur BLANC gagne !\nBravo !");
        afficherMenu();
    }
    else if(fin == 2)
    {
        jeuEnCours = false;
        QMessageBox::information(this,
                                 "Fin de partie",
                                 "👑  Le joueur NOIR gagne !\nBravo !");
        afficherMenu();
    }
}
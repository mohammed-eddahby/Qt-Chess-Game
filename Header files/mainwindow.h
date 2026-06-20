#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QWidget>
#include <QMessageBox>
#include <QTimer>
#include "plateau.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void caseCliquee(int ligne, int col);
    void lancerHvsH();
    void lancerHvsM();
    void resetJeu();
    void tourIA();

private:
    // ---- Widgets ----
    QWidget     *centralWidget;
    QGridLayout *mainLayout;
    QGridLayout *gridLayout;
    QWidget     *boardWidget;
    QWidget     *menuWidget;

    QPushButton *btnHvsH;
    QPushButton *btnHvsM;
    QPushButton *btnReset;
    QLabel      *lblTour;
    QLabel      *lblInfo;

    // Boutons cases (8x8)
    QPushButton *cases[8][8];

    // ---- Logique ----
    plateau     p;
    int         choix;        // 1=HvH  2=HvM
    int         selLigne;     // case sélectionnée (-1 si aucune)
    int         selCol;
    bool        jeuEnCours;

    QTimer      *timerIA;

    // ---- Méthodes ----
    void        afficherPlateau();
    void        mettreAJourInfo();
    QString     getPieceTexte(int valeur);
    QString     getCouleurCase(int i, int j, bool selectionnee, bool estDepart);
    void        verifierFinPartie();
    void        initialiserUI();
    void        afficherMenu();
    void        afficherJeu();
};

#endif // MAINWINDOW_H
#ifndef PLATEAU_H
#define PLATEAU_H

struct mov
{
    int ld,cd,la,ca,poid;
};

class plateau
{
public:
    plateau();
    ~plateau();
    int deplacer(int ld, int cd, int la, int ca);
    void initPlateau();
    int getPiece(int i,int j);
    int evaluer(int ld,int cd,int la,int ca);
    int evalCavalier(int ld,int cd,int la,int ca);
    int finPartie();
    int getTour();
    int evalPion(int ld,int cd,int la,int ca);
    int evalTour(int ld,int cd,int la,int ca);
    int evalDame(int ld,int cd,int la,int ca);
    int evalFou(int ld,int cd,int la,int ca);
    int evalRoi(int ld,int cd,int la,int ca);
    int roiSainApresCoup(int ld, int cd, int la, int ca);
    void deplacerIA();
    void evalIA();
    void mouvementIA();
private:
    int ech[8][8];
    int tour;
    mov tMov[200];
    int tailleIA;
};

#endif
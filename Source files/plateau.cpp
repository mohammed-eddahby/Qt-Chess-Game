#include "plateau.h"
#include <cstdlib>
#include <ctime>
using namespace std;

plateau::plateau()
{
    tour=1;
    initPlateau();
}

plateau::~plateau()
{
}

void plateau::mouvementIA()
{
    tailleIA = 0;
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(ech[i][j] < 0)
            {
                for(int k = 0; k < 8; k++)
                {
                    for(int l = 0; l < 8; l++)
                    {
                        if(ech[k][l] >= 0)
                        {
                            if(evaluer(i, j, k, l) == 1)
                            {
                                tMov[tailleIA].ld = i;
                                tMov[tailleIA].cd = j;
                                tMov[tailleIA].la = k;
                                tMov[tailleIA].ca = l;
                                tMov[tailleIA].poid = 0;
                                tailleIA++;
                            }
                        }
                    }
                }
            }
        }
    }
}

void plateau::deplacerIA()
{
    tailleIA = 0;
    mouvementIA();

    if(tailleIA > 0)
    {
        int tMakla[200];
        int tailleMakla = 0;

        for(int i = 0; i < tailleIA; i++)
        {
            if(ech[tMov[i].la][tMov[i].ca] > 0)
            {
                tMakla[tailleMakla] = i;
                tailleMakla++;
            }
        }

        int indexChoisi = 0;

        if(tailleMakla > 0)
        {
            int randMakla = rand() % tailleMakla;
            indexChoisi = tMakla[randMakla];
        }
        else
        {
            indexChoisi = rand() % tailleIA;
        }

        ech[tMov[indexChoisi].la][tMov[indexChoisi].ca] =
            ech[tMov[indexChoisi].ld][tMov[indexChoisi].cd];
        ech[tMov[indexChoisi].ld][tMov[indexChoisi].cd] = 0;
        tour++;
    }
}

void plateau::evalIA()
{
    for(int i=0;i<tailleIA;i++)
    {
        tMov[i].poid = 0;
        switch(ech[tMov[i].la][tMov[i].ca])
        {
        case 0:  tMov[i].poid += 0;    break;
        case 1:  tMov[i].poid += 10;   break;
        case 2:  tMov[i].poid += 50;   break;
        case 3:  tMov[i].poid += 30;   break;
        case 4:  tMov[i].poid += 30;   break;
        case 5:  tMov[i].poid += 100;  break;
        case 6:  tMov[i].poid += 1000; break;
        }
    }

    for(int i=0;i<tailleIA;i++)
    {
        for(int j=i+1;j<tailleIA;j++)
        {
            if(tMov[i].poid < tMov[j].poid)
            {
                mov temp = tMov[i];
                tMov[i] = tMov[j];
                tMov[j] = temp;
            }
        }
    }
}

int plateau::getTour()
{
    return tour;
}

int plateau::evalCavalier(int ld, int cd, int la, int ca)
{
    if(abs(la-ld) == 2 && abs(ca-cd) == 1) return 1;
    if(abs(la-ld) == 1 && abs(ca-cd) == 2) return 1;
    return 0;
}

int plateau::evalPion(int ld, int cd, int la, int ca)
{
    // Blanc (Piece > 0)
    if(ech[ld][cd] > 0)
    {
        // Déplacement simple
        if(cd == ca && ech[la][ca] == 0)
        {
            if(ld == 6 && la == 4 && ech[5][cd] == 0) return 1;
            if(ld == 6 && la == 5) return 1;
            if(la == ld - 1) return 1;
        }
        // Capture
        if((ca == cd+1 || ca == cd-1) && la == ld-1 && ech[la][ca] < 0)
            return 1;
    }

    // Noir (Piece < 0)
    if(ech[ld][cd] < 0)
    {
        // Déplacement simple
        if(cd == ca && ech[la][ca] == 0)
        {
            if(ld == 1 && la == 3 && ech[2][cd] == 0) return 1;
            if(ld == 1 && la == 2) return 1;
            if(la == ld + 1) return 1;
        }
        // Capture
        if((ca == cd+1 || ca == cd-1) && la == ld+1 && ech[la][ca] > 0)
            return 1;
    }
    return 0;
}

int plateau::evalDame(int ld,int cd,int la,int ca)
{
    if(evalTour(ld,cd,la,ca) == 1) return 1;
    if(evalFou(ld,cd,la,ca) == 1)  return 1;
    return 0;
}

int plateau::evalTour(int ld,int cd,int la,int ca)
{
    if(ld == la)
    {
        int step = (ca > cd) ? 1 : -1;
        for(int j = cd + step; j != ca; j = j + step)
        {
            if(ech[ld][j] != 0) return 0;
        }
        return 1;
    }
    else if(cd == ca)
    {
        int step = (la > ld) ? 1 : -1;
        for(int i = ld + step; i != la; i = i + step)
        {
            if(ech[i][cd] != 0) return 0;
        }
        return 1;
    }
    return 0;
}

int plateau::evalFou(int ld, int cd, int la, int ca)
{
    if(abs(la-ld) != abs(ca-cd)) return 0;

    if(la-ld == 0) return 0; // Même ligne

    int stepL = (la > ld) ? 1 : -1;
    int stepC = (ca > cd) ? 1 : -1;

    int i = ld + stepL;
    int j = cd + stepC;

    while(i != la && j != ca)
    {
        if(ech[i][j] != 0) return 0;
        i += stepL;
        j += stepC;
    }
    return 1;
}

int plateau::evalRoi(int ld, int cd, int la, int ca)
{
    // Vérifie le déplacement d'une case dans toutes les directions
    if(abs(la - ld) <= 1 && abs(ca - cd) <= 1)
    {
        // Ne pas rester sur la même case
        if(ld == la && cd == ca) return 0;

        // Vérifie que la destination n'est pas occupée par une pièce alliée
        int pieceSource = ech[ld][cd];
        int pieceDest = ech[la][ca];

        // Pièces alliées : même signe (blanc > 0, noir < 0)
        if((pieceSource > 0 && pieceDest > 0) || (pieceSource < 0 && pieceDest < 0))
            return 0;

        return 1;
    }
    return 0;
}

int plateau::roiSainApresCoup(int ld, int cd, int la, int ca)
{
    int piece = ech[ld][cd];
    int dest  = ech[la][ca];

    ech[la][ca] = piece;
    ech[ld][cd] = 0;

    int joueur = (piece > 0) ? 1 : 2;

    // check if king is in danger
    bool danger = false;

    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(joueur == 1 && ech[i][j] < 0)
            {
                for(int k=0;k<8;k++)
                    for(int l=0;l<8;l++)
                        if(ech[k][l] == 6 && evaluer(i,j,k,l))
                            danger = true;
            }

            if(joueur == 2 && ech[i][j] > 0)
            {
                for(int k=0;k<8;k++)
                    for(int l=0;l<8;l++)
                        if(ech[k][l] == -6 && evaluer(i,j,k,l))
                            danger = true;
            }
        }
    }

    // undo move
    ech[ld][cd] = piece;
    ech[la][ca] = dest;

    return !danger;
}

int plateau::evaluer(int ld,int cd,int la,int ca)
{

    int pieceSource = ech[ld][cd];
    int pieceDest = ech[la][ca];

    if((pieceSource > 0 && pieceDest > 0) || (pieceSource < 0 && pieceDest < 0))
        return 0;

    int result = 0;

    switch(ech[ld][cd])
    {
    case  1: result = evalPion(ld,cd,la,ca); break;
    case -1: result = evalPion(ld,cd,la,ca); break;
    case  2: result = evalTour(ld,cd,la,ca); break;
    case -2: result = evalTour(ld,cd,la,ca); break;
    case  3: result = evalCavalier(ld,cd,la,ca); break;
    case -3: result = evalCavalier(ld,cd,la,ca); break;
    case  4: result = evalFou(ld,cd,la,ca); break;
    case -4: result = evalFou(ld,cd,la,ca); break;
    case  5: result = evalDame(ld,cd,la,ca); break;
    case -5: result = evalDame(ld,cd,la,ca); break;
    case  6: result = evalRoi(ld,cd,la,ca); break;
    case -6: result = evalRoi(ld,cd,la,ca); break;
    default: return 0;
    }

    if(result == 1 && !roiSainApresCoup(ld,cd,la,ca))
        return 0;

    return result;
}

int plateau::deplacer(int ld, int cd, int la, int ca)
{
    // Vérification supplémentaire : ne pas capturer une pièce alliée
    int pieceSource = ech[ld][cd];
    int pieceDest = ech[la][ca];

    if((pieceSource > 0 && pieceDest > 0) || (pieceSource < 0 && pieceDest < 0))
        return 0;

    if(evaluer(ld,cd,la,ca)==1)
    {
        ech[la][ca]=ech[ld][cd];
        ech[ld][cd]=0;
        tour++;
        return 1;
    }
    return 0;
}

void plateau::initPlateau()
{
    for(int i=2;i<6;i++)
        for(int j=0;j<8;j++)
            ech[i][j]=0;

    for(int j=0;j<8;j++)
    {
        ech[1][j]=-1;
        ech[6][j]=1;
    }

    ech[0][0]=-2; ech[0][1]=-3; ech[0][2]=-4; ech[0][3]=-5;
    ech[0][4]=-6; ech[0][5]=-4; ech[0][6]=-3; ech[0][7]=-2;

    ech[7][0]=2;  ech[7][1]=3;  ech[7][2]=4;  ech[7][3]=5;
    ech[7][4]=6;  ech[7][5]=4;  ech[7][6]=3;  ech[7][7]=2;
}

int plateau::getPiece(int i,int j)
{
    return ech[i][j];
}

int plateau::finPartie()
{
    bool roiBlanc = false, roiNoir = false;
    bool coupBlanc = false, coupNoir = false;

    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(ech[i][j] == 6) roiBlanc = true;
            if(ech[i][j] == -6) roiNoir = true;

            if(ech[i][j] > 0)
                for(int k=0;k<8;k++)
                    for(int l=0;l<8;l++)
                        if(evaluer(i,j,k,l)) coupBlanc = true;

            if(ech[i][j] < 0)
                for(int k=0;k<8;k++)
                    for(int l=0;l<8;l++)
                        if(evaluer(i,j,k,l)) coupNoir = true;
        }
    }

    if(!roiBlanc || !coupBlanc) return 2;
    if(!roiNoir  || !coupNoir)  return 1;

    return 0;
}
#include <QtGui>
#include "redim.h"
#include <qimage.h>
#include <qcolor.h>

Redim::Redim(QImage base) {
    image = base;


    pair=true;
    f=new Filtres();
}

void Redim::maj(QImage img) {
    image = img;
}
QImage Redim::decouper(int sX0, int sY0,int sX1,int sY1) {
    QImage resImage(sX1,sY1,image.format());

    int x =0, y=0;

    for(int i=sX0; i<sX0 + sX1; i++) {
        for(int j=sY0; j<sY0 + sY1; j++) {
            if (i < 0 || i > image.width()-1 || j < 0 || j > image.height()-1)
                resImage.setPixel(QPoint(x,y++),qRgba(255,255,255,0));
            else
                resImage.setPixel(QPoint(x,y++),image.pixel(i,j));
        }
        x++;
        y=0;
    }


    return resImage;
}

QImage Redim::down(int l, int h) {

    QImage res(l,h,image.format());
    double ratioL, ratioH;
    double compteurH = 0, compteurL = 0;
    int iR = 0, jR = 0;

    //Reduction de la taille de l'image :
    ratioL = (double) image.width() / (double) l;
    ratioH = (double) image.height() / (double) h ;

    int cptL = 0, cptH = 0;


    for(int i = 0; i < l; i++) {
        for(int j = 0; j < h; j++) {
            int r = 0, g = 0, b = 0;
            compteurL += ratioL;
            compteurH += ratioH;

            for(cptL = 0; cptL < (int) compteurL; cptL++) {
                for(cptH = 0; cptH < (int) compteurH; cptH++) {
                    r += ((QColor) image.pixel(cptL+iR, cptH+jR)).red();
                    g += ((QColor) image.pixel(cptL+iR, cptH+jR)).green();
                    b += ((QColor) image.pixel(cptL+iR, cptH+jR)).blue();
                }
            }

            jR = (int) ((double)  j*(ratioH)) ;
            res.setPixel(QPoint(i,j), (QColor(r/(cptL*cptH),g/(cptL*cptH),b/(cptL*cptH),255).rgb()));

            compteurL -= (int) compteurL;
            compteurH -= (int) compteurH;
        }

        iR = (int) ((double) i*(ratioL ));
        jR = 0;


    }

    return res;
}

QImage Redim::up(int l, int h) {

    QImage res(l,h,image.format());
    double ratioL, ratioH;
    double compteurH = 0, compteurL = 0;
    int iR = 0, jR = 0;

    //Augmentation de la taille de l'image :
    ratioH = (double) image.height() / ((double) h - image.height());
    ratioL = (double) image.width() / ((double) l - image.width());
    for(int iB=0; iB < image.width(); iB++) {
        for(int jB=0; jB<image.height(); jB++) {
            // On parcours l'image et copie chaque pixel
            res.setPixel(QPoint(iR,jR++),image.pixel(iB,jB));
            //S'il y a un redimensionnement sur la hauteur on rajoute les pixels necessaire
            if (h > image.height()) {
                compteurH ++;
                while (compteurH >= ratioH) {
                    res.setPixel(QPoint(iR,jR++),image.pixel(iB,jB));
                    compteurH -= ratioH;
                }
            }
        }
        //Dans le cas d'un nombre de pixel impair la hauteur de l'image ne sera pas atteinte
        if (jR != res.height()) {
            res.setPixel(QPoint(iR,jR),res.pixel(iR,jR-1));
            jR++;
        }
        //S'il y a un redimensionnement sur la largeur on rajoute les pixels necessaire
        if (l > image.width()) {
            compteurL ++;
            while (compteurL >= ratioL) {
                iR++;
                for(int j=0 ; j < res.height(); j++)
                    res.setPixel(QPoint(iR,j),res.pixel(iR-1,j));
                compteurL -= ratioL;
            }
        }
        iR++;
        jR = 0;
        compteurH = 0;
    }
    //Dans le cas d'un nombre de pixel impaire la largeur de l'image ne sera pas atteinte
    if (iR != res.width()) {
        for(int j=0 ; j < res.height(); j++)
            res.setPixel(QPoint(iR,j),res.pixel(iR-1,j));
    }

    return res;
}


int cstA = -0.5;

double C0(double t) {
    return -cstA*t*t*t + cstA*t*t;
}
double C1(double t) {
    return -(cstA+2)*t*t*t + (2*cstA+3)*t*t - cstA*t;
}
double C2(double t) {
    return (cstA+2)*t*t*t - (cstA+3)*t*t + 1;
}
double C3(double t) {
    return cstA*t*t*t - 2*cstA*t*t + cstA*t;
}
double Redim::fRed(int i, int j) {
    return (((QColor) image.pixel(i,j)).red());
}
double Redim::fGreen(int i, int j) {
    return (((QColor) image.pixel(i,j)).green());
}
double Redim::fBlue(int i, int j) {

    return (((QColor) image.pixel(i,j)).blue());
}
double Redim::H(int j, double x, int c) {
    int i = (int)x;
    double res = 0;

    double c0 = C0(x-i);
    double c1 = C1(x-i);
    double c2 = C2(x-i);
    double c3 = C3(x-i);


    if(c == 1) {
        res += fRed(i,j)*c2;

        if (i != 0 )
            res += fRed(i-1,j)*c3;
        else
            res += fRed(i,j)*c3;

        if(i+1 < image.width())
            res += fRed(i+1,j)*c1;
        else
            res += fRed(i,j)*c1;

        if (i+2 < image.width())
            res += fRed(i+2,j)*c0;
        else if (i+1 < image.width())
            res += fRed(i+1,j)*c0;
        else
            res += fRed(i,j)*c0;
    }
    if (c == 2) {
        res += fGreen(i,j)*c2;

        if (i != 0 )
            res += fGreen(i-1,j)*c3;
        else
            res += fGreen(i,j)*c3;

        if(i+1 < image.width())
            res += fGreen(i+1,j)*c1;
        else
            res += fGreen(i,j)*c1;

        if (i+2 < image.width())
            res += fGreen(i+2,j)*c0;
        else if (i+1 < image.width())
            res += fGreen(i+1,j)*c0;
        else
            res += fGreen(i,j)*c0;
    }

    if (c == 3) {
        res += fBlue(i,j)*c2;

        if (i != 0 )
            res += fBlue(i-1,j)*c3;
        else
            res += fBlue(i,j)*c3;

        if(i+1 < image.width())
            res += fBlue(i+1,j)*c1;
        else
            res += fBlue(i,j)*c1;

        if (i+2 < image.width())
            res += fBlue(i+2,j)*c0;
        else if (i+1 < image.width())
            res += fBlue(i+1,j)*c0;
        else
            res += fBlue(i,j)*c0;

    }

    return res;
}

QColor Redim::value(double x, double y) {
    int j = (int)y;
    QColor res;
    double h0r, h0g, h0b, h1r, h1b, h1g;
    double c0 = C0(y-j);
    double c1 = C1(y-j);
    double c2 = C2(y-j);
    double c3 = C3(y-j);

    double r = 0, g = 0, b = 0;
    h0r = H(j,x,1);
    h0g = H(j,x,2);
    h0b = H(j,x,3);

    if (j != 0) {
        r +=  H(j-1,x,1)*c3;
        g +=  H(j-1,x,2)*c3;
        b +=  H(j-1,x,3)*c3;
    } else {
        r += h0r*c3;
        g += h0g*c3;
        b += h0b*c3;
    }
    if (j+1 < image.height()) {
        h1r = H(j+1,x,1);
        h1g = H(j+1,x,2);
        h1b = H(j+1,x,3);

        r += h1r*c1;
        g += h1g*c1;
        b += h1b*c1;
    } else {
        r += h0r*c1;
        g += h0g*c1;
        b += h0b*c1;
    }
    r += h0r*c2;
    g += h0g*c2;
    b += h0b*c2;

    if (j+2 < image.height()) {
        r += H(j+2,x,1)*c0;
        g += H(j+2,x,2)*c0;
        b += H(j+2,x,3)*c0;
    } else if (j+1 < image.height()) {
        r += h1r*c0;
        g += h1g*c0;
        b += h1b*c0;
    } else {
        r += h0r*c0;
        g += h0g*c0;
        b += h0b*c0;
    }

    res.setRed(r);
    res.setGreen(g);
    res.setBlue(b);

    //qDebug() << res.red() << res.blue() << res.green();
    return res;
}


QImage Redim::interpolationBilineaire(int l, int h) {

    QImage res(l,h,image.format());

    double iRatioL = (double) image.width()/(double) l;
    double iRatioH = (double) image.height()/(double) h;

    for(int i = 0; i < l; i++) {
        for(int j = 0; j < h; j++) {
            QColor r = value(i*iRatioL,j*iRatioH);
            res.setPixel(i,j,r.rgb());
        }
    }
    return res;
}

void Redim::evaluationScL() {
    matriceL = new int *[image.width()];
    for (int i=0; i <image.width(); i++) {
        matriceL[i] = new int [image.height()];
    }
    imgGrad=f->norme_4gradients(GRADIENT_SOBEL, image);

    for(int j = 0; j < image.height(); j++) {
        for(int i = 0; i < image.width(); i++) {

            if(j==0)
                matriceL[i][j]=((QColor) imgGrad.pixel(i,j)).red();
            else {
                int m;
                if(i>=(image.width()-1)) { //iTemp+1 non atteignable
                    m=minM(matriceL[i-1][j-1],matriceL[i][j-1],-1);
                }
                else if(i<=0) { //iTemp-1 non atteignable
                    m=minM(-1,matriceL[i][j-1],matriceL[i+1][j-1]);
                }
                else {
                    m=minM(matriceL[i-1][j],matriceL[i][j-1],matriceL[i+1][j-1]);
                }

                matriceL[i][j]=m+((QColor) imgGrad.pixel(i,j)).red();
            }
        }
    }
}

void Redim::evaluationScH() {
    matriceH = new int *[image.width()];
    for (int i=0; i <image.width(); i++) {
        matriceH[i] = new int [image.height()];
    }
    imgGrad=f->norme_4gradients(GRADIENT_SOBEL, image);


    for(int i = 0; i < image.width(); i++) {
        for(int j = 0; j < image.height(); j++) {

            if(i==0)
                matriceH[i][j]=((QColor) imgGrad.pixel(i,j)).red();
            else {
                int m;
                if(j>=(image.height()-1)) { //iTemp+1 non atteignable
                    m=minM(matriceH[i-1][j-1],matriceH[i-1][j-1],-1);
                }
                else if(j<=0) { //iTemp-1 non atteignable
                    m=minM(-1,matriceH[i-1][j],matriceH[i-1][j+1]);
                }
                else {
                    m=minM(matriceH[i-1][j-1],matriceH[i-1][j],matriceH[i-1][j+1]);
                }

                matriceH[i][j]=m+((QColor) imgGrad.pixel(i,j)).red();
            }
        }
    }
}

int Redim::min(int i1, int i2) {
    if (i1>=i2)
        return i2;
    else
        return i1;
}

int Redim::minM(int i1, int i2, int i3) {
    int res;

    if (i1<0)
        res=min(i3,i2);
    else if(i2<0)
        res=min(i1,i3);
    else if(i3<0)
        res=min(i1,i2);
    else
        res=min(i1,min(i2,i3));

    return res;
}

int Redim::min3(int i1, int i2, int i3) {
    int res;

    res=minM(i1,i2,i3);

    if (res==i2)
        return 0;
    else if(res==i1)
        return -1;
    else
        return 1;
}


void Redim::majCheminL() {
    int pMin=-1;

    for(int i=0;i<image.width();i++) {

        if(matriceL[i][image.height()-1]<pMin||pMin==-1){
            pMin=matriceL[i][image.height()-1];
        }
    }
    for(int i=0;i<image.width();i++) {
        if(matriceL[i][image.height()-1]==pMin) {
            Chemin *ch;
            int iTemp=i;
            for(int j=image.height()-1;j>=0;j--) {
                if(j==image.height()-1) {
                    ch = new Chemin();
                    ch->chemin = new int[image.height()];
                    ch->chemin[j]=i;
                }
                else {
                    if(iTemp>=(image.width()-1)) { //iTemp+1 non atteignable
                        if(minM(matriceL[iTemp-1][j],matriceL[iTemp][j],-1) == matriceL[iTemp-1][j])
                            iTemp=iTemp-1;
                    }
                    else if(iTemp<=0) { //iTemp-1 non atteignable
                        if(minM(-1,matriceL[iTemp][j],matriceL[iTemp+1][j]) == matriceL[iTemp+1][j])
                            iTemp=iTemp+1;
                    }
                    else {
                        if(minM(matriceL[iTemp-1][j],matriceL[iTemp][j],matriceL[iTemp+1][j]) == matriceL[iTemp-1][j])
                            iTemp=iTemp-1;
                        else if(minM(matriceL[iTemp-1][j],matriceL[iTemp][j],matriceL[iTemp+1][j]) == matriceL[iTemp+1][j])
                            iTemp=iTemp+1;
                    }


                    ch->chemin[j]=iTemp;// on affecte le chemin
                }

            }
            listeLigne.append(*ch);
        }

    }
}

void Redim::majCheminH() {
    int pMin=-1;

    for(int j=0;j<image.height();j++) {

        if(matriceH[image.width()-1][j]<pMin||pMin==-1){
            pMin=matriceH[image.width()-1][j];
        }
    }
    for(int j=0;j<image.height();j++) {
        if(matriceH[image.width()-1][j]==pMin) {
            Chemin *ch;
            int jTemp=j;
            for(int i=image.width()-1;i>=0;i--) {
                if(i==image.width()-1) {
                    ch = new Chemin();
                    ch->chemin = new int[image.width()];
                    ch->chemin[i]=j;
                }
                else {
                    if(jTemp>=(image.height()-1)) { //iTemp+1 non atteignable
                        if(minM(matriceH[i][jTemp-1],matriceH[i][jTemp],-1) == matriceH[i][jTemp-1])
                            jTemp=jTemp-1;
                    }
                    else if(jTemp<=0) { //iTemp-1 non atteignable
                        if(minM(-1,matriceH[i][jTemp],matriceH[i][jTemp+1]) == matriceH[i][jTemp+1])
                            jTemp=jTemp+1;
                    }
                    else {
                        if(minM(matriceH[i][jTemp-1],matriceH[i][jTemp],matriceH[i][jTemp+1]) == matriceH[i][jTemp-1])
                            jTemp=jTemp-1;
                        else if(minM(matriceH[i][jTemp-1],matriceH[i][jTemp],matriceH[i][jTemp+1]) == matriceH[i][jTemp+1])
                            jTemp=jTemp+1;
                    }


                    ch->chemin[i]=jTemp;// on affecte le chemin
                }

            }
            listeLigne.append(*ch);
        }

    }
}



QImage Redim::redimSeamCarving(int l, int h) {
    //l et h représente la quantité de lignes et de colones a supprimer de l'image redimensionnée

    QImage res(image);
    QImage resF;
    listeLigne.clear();

    if(l>0) {
        evaluationScL();
        majCheminL();

        res = redScUpL(image);
        resF = res;
        for(int cpt=1; cpt<l;cpt++) {
            listeLigne.clear();
            image = resF;
            evaluationScL();
            majCheminL();
            QImage temp(redScUpL(resF));
            resF = temp;
        }
    }
    else
        resF = image;
    if(h>0) {
        evaluationScH();
        majCheminH();

        res = redScUpH(resF);
        resF = res;
        for(int cpt=1; cpt<h;cpt++) {
            listeLigne.clear();
            image = resF;
            evaluationScH();
            majCheminH();
            QImage temp(redScUpH(resF));
            resF = temp;
        }
    }
    return resF;
}

QImage Redim::redScUpL(QImage base){
    QImage res(base.width()-1, base.height(), base.format());
    Chemin c;
    if(pair)
        c = listeLigne.takeFirst();
    else
        c = listeLigne.takeLast();
    pair = !pair;

    int jRes[res.height()];
    for(int i=0; i<res.width();i++) {
        for(int j=0; j<res.height(); j++) {
            if(i==0)
                jRes[j] = 0;
            if(c.chemin[j+jRes[j]]==i)
                jRes[j]=1;

            res.setPixel(i,j,base.pixel(i+jRes[j],j));

        }
    }
    return res;
}

QImage Redim::redScUpH(QImage base){
    QImage res(base.width(), base.height()-1, base.format());
    Chemin c;
    if(pair)
        c = listeLigne.takeFirst();
    else
        c = listeLigne.takeLast();
    pair = !pair;

    int iRes[res.width()];
    for(int j=0; j<res.height(); j++) {
        for(int i=0; i<res.width();i++) {
            if(j==0)
                iRes[i]=0;
            if(c.chemin[i+iRes[i]]==j)
                iRes[i]=1;

            res.setPixel(i,j,base.pixel(i,j+iRes[i]));

        }
    }
    return res;
}


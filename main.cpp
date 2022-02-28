#include <iostream>
#include <ostream>
#include "Matrice3D.h"
#include "Matrice2D.h"
#include <typeinfo>


struct point {
	int x; ///< coordinata x del punto
	int y; ///< coordinata y del punto

	point(int xx, int yy) : x(xx), y(yy) {}

    point(int value) : x(value), y(value) {}
};


/**
	Ridefinizione dell'operatore di stream << per un point.
	Necessario per l'operatore di stream della classe Matrice3D.
*/
std::ostream &operator<<(std::ostream &os, const point &p) {
	std::cout<<"("<<p.x<<","<<p.y<<")";
	return os;
}

typedef Matrice3D<int> Matrix3Dint;
typedef Matrice3D<double> Matrix3Ddouble;
typedef Matrice3D<char> Matrix3Dchar;

typedef Matrice2D<int> Matrix2Dint;
typedef Matrice2D<double> Matrix2Ddouble;
typedef Matrice2D<char> Matrix2Dchar;
typedef Matrice3D<point> Matrix3DPonit;
typedef Matrice2D<point> Matrix2DPonit;

struct plus{
    plus(int value) : i(value){}
    int operator()(int ele) const{return ele+i;}
    private:
        int i;
};

struct minus{
    minus(int value) : i(value){}
    int operator()(int ele) const{return ele-i;}
    private:
        int i;
};

struct multiplies{
    multiplies(int value) : i(value){}
    int operator()(int ele) const{return ele*i;}
    private:
        int i;
};

struct divide{
    divide(int value) {
        if(value != 0)
            i = value;
        else{
            std::cout << "Impossiible dividere per 0" << std::endl;
            i = 1;
        }
    }
    int operator()(int ele) const{return ele/i;}
    private:
        int i;
};


/**
	Test per la creazione della matrice3d ed aggiuna di valori
	@brief Test per la creazione della matrice3d ed aggiuna di valori
*/
void test_matrice3D() {

	std::cout << "\033[1;31mTEST MATRICE3D\033[0m" << std::endl;

	Matrix3Dint M3Dint1(2,2,2); // ctor default x,y,z
    
    M3Dint1.add(10,0,0,0);
    std::cout << "Provo ad aggiungere 100 in posizione 0,0,20. Dimensione matrice (2,2,2)" << std::endl;
    M3Dint1.add(100,0,0,20);

    Matrix3Dint M3Dint2(M3Dint1);
    M3Dint2.add(22,0,0,1);
    M3Dint2.add(25,1,1,1);
    M3Dint2.add(166,1,1,1);
    std::cout << "Provo ad aggiungere 166 in posizione 1,144,1. Dimensione matrice (2,2,2)" << std::endl;
    M3Dint2.add(166,1,144,1);

    std::cout << std::endl <<"Matrice M3Dint1 di tipo " << typeid(M3Dint1).name() << " " << std::endl << M3Dint1 << std::endl;
    std::cout << "Matrice M3Dint2 di tipo " << typeid(M3Dint2).name() << " " << std::endl << M3Dint2 << std::endl;

    Matrix3Ddouble M3Ddouble1(5,5,5);
    M3Ddouble1.add(12,0,0,0);
    Matrix3Ddouble M3Ddouble2(M3Ddouble1);
    M3Ddouble2.add(220.2,1,1,1);

    std::cout << "Matrice M3Ddouble1 di tipo " << typeid(M3Ddouble1).name() << " " << std::endl << M3Ddouble1 << std::endl;
    std::cout << "Matrice M3Ddouble2 di tipo " << typeid(M3Ddouble2).name() << " " << std::endl << M3Ddouble2 << std::endl;

    M3Dint1.clear();
    M3Dint2.clear();
    M3Ddouble1.clear();
    M3Ddouble2.clear();
}

void test_tipoCustom() {
    std::cout << "\033[1;31mTEST TIPO CUSTOM (PUNTO)\033[0m" << std::endl;
    Matrix3DPonit M3DP(2,2,2);
    std::cout << "Aggiungo il punto (1,1) in posizione 0,0,3. Dimensione matrice (2,2,2) : " << std::endl;
    M3DP.add(point(1,1),0,0,3);
    M3DP.add(point(2,4),0,0,0);
    M3DP.add(point(1,5),1,2,1);
    M3DP.add(point(5,1),0,2,1);

    std::cout << "Matrice3D di tipo punto : " << std::endl << M3DP << std::endl;
    
    Matrix2DPonit M2DP(M3DP.slice(1));
    std::cout << "Eseguo la Slice su z = 1 : " << std::endl << M2DP << std::endl;

    std::cout << "La cella in M3DP(0,0,0) e' : " << M3DP(0,0,0) << std::endl << std::endl;
    M3DP(0,0,0) = M3DP(0,2,1);

    std::cout << "Eseguo M3DP(0,0,0) = M3DP(0,2,1)" << std::endl << "La nuova cella M3DP(0,0,0) e' : " << M3DP(0,0,0) << std::endl << std::endl;

    plus plus5(5);
    Matrix3Dint M3Di(2,2,2);
    M3Di.add(1,0,0,0);
    M3Di.add(2,2,1,2);
    M3Di.add(3,1,2,2);
    Matrix3DPonit nM3DP(3,3,3);
    std::cout << "La matrice M3Di : " << std::endl << M3Di << std::endl;
    std::cout << "Eseguo nM3DP = transform(plus5,M3Di)" << std::endl;
    nM3DP = transform(plus5, M3Di);
    std::cout << "Nuova matrice nM3DP : " << std::endl << nM3DP << std::endl;

    M3DP.clear();
    M2DP.clear();
    M3Di.clear();
}


/**
	Test per il cast della matrice3d definita su un tipo U a una amtrice3d definita su tipo T

	@brief Test per il cast della matrice3d definita su un tipo U a una amtrice3d definita su tipo T
*/
void test_cast(){
    std::cout << "\033[1;31mTEST CAST\033[0m" << std::endl;

    Matrix3Ddouble M3Ddouble(2,2,2);
    M3Ddouble.add(126.2,1,1,1);
    Matrix3Dint mati(M3Ddouble);
    std::cout << "Matrice mati di tipo " << typeid(mati).name() << " " << std::endl << mati << std::endl;
    Matrix3Dchar matc(M3Ddouble);
    std::cout << "Matrice matc di tipo " << typeid(matc).name() << " " << std::endl << matc << std::endl;

    M3Ddouble.clear();
    mati.clear();
    matc.clear();
}

/**
	Test per ottenere un piano della matrice3d quindi ottenendo una matrice2d contenente tuttle le celle del piano richiesto

	@brief Test per ottenere un piano della matrice3d quindi ottenendo una matrice2d contenente tuttle le celle del piano richiesto
*/
void test_slice(){
    std::cout << "\033[1;31mTEST SLICE\033[0m" << std::endl;

    Matrix3Dint M3Dint(3,3,3); // ctor default x,y,z
    M3Dint.add(1,0,0,0);
    M3Dint.add(10,0,0,1);
    M3Dint.add(100,0,0,2);
    M3Dint.add(200,0,1,1);

    Matrix2Dint M2Dint(M3Dint.slice(1));
    
    std::cout << "La matrice 3D e' : " << std::endl << M3Dint << std::endl;
    std::cout << "Eseguita la slice su z=1, la matrice 2D e' : " << std::endl;
    std::cout << M2Dint << std::endl;

    M3Dint.clear();
    M2Dint.clear();
}

/**
	Test dell'operatore() per richiedere e scrivere un valore su una cella (x,y,z)

	@brief Test dell'operatore() per richiedere e scrivere un valore su una cella (x,y,z)
*/
void test_operatore(){
    std::cout << "\033[1;31mTEST OPERATORE() G(0,0,0) = G(0,0,1)\033[0m" << std::endl;
    Matrix3Dint G(3,3,3); // ctor default x,y,z
    G.add(1,0,0,0);
    G.add(10,0,0,1);
    G.add(100,0,0,2);
    G.add(200,0,1,1);
    std::cout << "La cella in G(0,0,0) e' : " << G(0,0,0) << std::endl;
    G(0,0,0) = G(0,0,1);

    std::cout << "Eseguo G(0,0,0) = G(0,0,1)" << std::endl << "La nuova cella G(0,0,0) e' : " << G(0,0,0) << std::endl << std::endl;

    G.clear();
}

/**
	Test per la funzione generica transform che data una matricer3d A di tipo T e un funtore F ritorna una matrice3d B di tipo Q applicando il funtore

	@brief Test d'uso della lista ordinata
*/
void test_funtore(){
    std::cout << "\033[1;31mTEST TRANSFORM B(i,j,k) = F(A(i,j,k))\033[0m" << std::endl;
    Matrix3Dint A(3,3,3); // ctor default x,y,z
    A.add(121,0,0,0);
    A.add(131,1,0,0);
    Matrix3Dint B(3,3,3); // ctor default x,y,z

    Matrix3Dchar C(3,3,3); // ctor default x,y,z

    std::cout << "La matrice di partenza : " << std::endl << A << std::endl;
    std::cout << " INIZIO TEST SU MATRICE INT" << std::endl;
    plus plus5(5);
    std::cout << "Eseguo B = transform(plus5,A)" << std::endl;
    B = transform(plus5, A);
    std::cout << "Nuova matrice B : " << std::endl << B << std::endl;

    minus minus5(5);
    std::cout << "Eseguo B = transform(minus5,A)" << std::endl;
    B = transform(minus5, A);
    std::cout << "Nuova matrice B : " << std::endl << B << std::endl;

    multiplies multiplies5(5);
    std::cout << "Eseguo B = transform(multiplies5,A)" << std::endl;
    B = transform(multiplies5, A);
    std::cout << "Nuova matrice B : " << std::endl << B << std::endl;

    divide divide5(5);
    std::cout << "Eseguo B = transform(divide5,A)" << std::endl;
    B = transform(divide5, A);
    std::cout << "Nuova matrice B : " << std::endl << B << std::endl;

    std::cout << " INIZIO TEST SU MATRICE CHAR" << std::endl;

    std::cout << "Eseguo C = transform(plus5,A)" << std::endl;
    C = transform(plus5, A);
    std::cout << "Nuova matrice C : " << std::endl << C << std::endl;

    std::cout << "Eseguo C = transform(minus5,A)" << std::endl;
    C = transform(minus5, A);
    std::cout << "Nuova matrice C : " << std::endl << C << std::endl;

    std::cout << "Eseguo C = transform(multiplies5,A)" << std::endl;
    C = transform(multiplies5, A);
    std::cout << "Nuova matrice C : " << std::endl << C << std::endl;

    std::cout << "Eseguo C = transform(divide5,A)" << std::endl;
    C = transform(divide5, A);
    std::cout << "Nuova matrice C : " << std::endl << C << std::endl;

    

    A.clear();
    B.clear();
    C.clear();
}

int main() {
    test_matrice3D();
    test_tipoCustom();
    test_cast();
    test_slice();
    test_operatore();
    test_funtore();
    return 0;
}
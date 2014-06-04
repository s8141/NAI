#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>   
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <ctype.h>

using namespace std ;

class neuron{
 private :
    int stan ; // aktywny : stan=1; nieaktywny : stan=-1
    int sygnal ;
    double x , y ;
    double progSygnalu ;
 public :
    void WpiszStan ( int s ){ stan=s ; }
    void WpiszPolozenia (double xx , double yy ){ x=xx ; y=yy ; }
    int PodajStan () { return stan ; }

 } ;

class synapsa {
 private :
    int silabond ;
 public :
    void ResetSynapsy ( ) { silabond =0;}
    void WprowadzWartoscSynapsy ( int s ){ silabond +=s ; }
    int PodajWartoscSynapsy ( ) { return silabond ; }
 } ;

typedef std :: vector<neuron*> Komorki ;
 Komorki Neur ;
 typedef std :: vector<synapsa*> Polaczenia ;
 Polaczenia Synap ;


 void AlokujNeurony (Komorki&Neur , int LLx , int LLy){
 int L=LLx*LLy ;

 for ( int k=0;k<L; k++) Neur.push_back (new neuron ) ;
 }

 void AlokujSynapsy ( Polaczenia&Synap , int LLx , int LLy){
int LL=LLx*LLy ;

 for ( int k=0;k<LL; k++) Synap . push_back (new synapsa ) ;
 }

 void ResetujNeurony (Komorki &Neur ){
 cout<<"Reset neuronow ... "<<endl ;
 int L=Neur.size ( ) ;
 for ( int k=0;k<L; k++) Neur [ k]->WpiszStan (-1);
 }

 void ResetujSynapsy ( Polaczenia&Synap ){
 cout<<"Reset synaps ... "<<endl ;
 int L=Synap.size();
 for ( int k=0;k<L; k++) Synap [ k]->ResetSynapsy ( ) ;
 }

 void WczytajParametryObrazka ( char * nazwapliku , int &Lx , int &Ly){
 fstream WE;

 WE.open ( nazwapliku , ios::in ) ;
if (WE.good()==false ){
 cout<<"Nie moge otworzyc pliku "<<nazwapliku<<endl ;
 cout<<"Wychodze do systemu "<<endl ;
 exit ( 1 ) ;
}

string linia ;
 int dlug ;
 Lx=0,Ly=0; // wa r t o s c i poczatkowe
 // k t o r e zos tana wy l i c z one z p l i k u z obrazkiem

 while (1){
 getline (WE, linia ) ;
 if (WE.eof () ) break ;

 dlug=linia.size ( ) ;
 if (Lx<dlug )Lx=dlug ; // lx przyjmuje wartosc maksymalna
 // parametru dlug
 Ly++; //numeruje kolejne line az do wa r t o s c i maksymalnej
 }


 WE.close ( ) ;
 }

 void WczytajObrazek (Komorki &Neur , char * nazwapliku ){
 fstream WE;
 WE.open ( nazwapliku, ios::in ) ;  
 if (WE. good()==false ){
cout<<"Nie moge otworzyc pliku "<<nazwapliku<<endl ;
cout<<"Wychodze do systemu "<<endl ;
 exit ( 1 ) ;
}

string linia ;
 int dlug ;
 int LICZNIK=0;
 char znak ;

 while (1){
 getline (WE, linia ) ;
 if (WE.eof ( ) ) break ;

 dlug=linia.size ( ) ;
for ( int k=0;k<dlug ; k++){
 znak=linia[k] ;
 if ( znak == '*'){
 Neur [LICZNIK]->WpiszStan ( 1 ) ;
 }

 LICZNIK++;

 }
 }

 WE.close ( ) ;

 }

 void PokazStanyNeuronow(Komorki&Neur , int Lx , int Ly){
 int j ;
 int L=Neur.size ( ) ;

 for ( int k=0;k<L; k++){
j=k%Lx ;
 if (Neur [k]->PodajStan ()==1) cout<< "+" ;
else cout<<" " ;
 if ( j==Lx - 1) cout<<endl ;
 }

 }

 void WczytajSynapsy (Komorki &Neur , Polaczenia&Synap , int Lx , int Ly){
 int polacz=0;
 int LL=Lx*Ly ;
int ij ;

for ( int i =0; i<LL; i++){
for ( int j =0; j<LL; j++){
if ( i==j ) continue ;
ij=i *LL+j ;
 if (Neur [ i ]->PodajStan()== Neur [ j ]->PodajStan( ) )
 Synap [ ij ]->WprowadzWartoscSynapsy ( 1 ) ;
 else Synap [ ij ]->WprowadzWartoscSynapsy (-1);
 }
 }
 }

 void WczytajDowolnyObrazek (Komorki &Neur , char * nazwapliku , int Lx , int Ly){
 fstream WE;

 WE.open ( nazwapliku , ios :: in ) ;
 if (WE.good()==false ){
 cout<<"Nie moge otworzyc pliku "<<nazwapliku<<endl ;
 cout <<"Wychodze do systemu "<<endl ;
 exit (1) ;
 }

 string linia ;
 int dlug ;
 int LICZNIK=0;
 char znak ;
 int numerlinii=0;

while (1){
 getline (WE, linia ) ;
if (WE.eof( ) ) break ;

dlug=linia.size( ) ;

 if ( dlug>Lx){
 cout<<"Wczytany obrazek jest obciety w linii "<<Lx*Ly/LICZNIK<<endl ;
 }

 for ( int k=0;k<dlug ; k++){
 if (k>=Lx) continue ; // o b c i e c i e h o r y z ont a lne
znak=linia [ k ] ;
 if ( znak =='*'){
 Neur [ LICZNIK]->WpiszStan(1) ;
 }

 LICZNIK++;

 }
 numerlinii++;
 if ( numerlinii>=Ly) break ; // o b c i e c i e w p i oni e
 }

 WE.close() ;

 }


 int Rozpoznaj (Komorki&Neur , Polaczenia&Synap , int Lx , int Ly){
 int LL=Lx*Ly ;
 int ij ;
 int irand ;
 int iteruj =0;
int sila ; // p o t e n c j a l aktywujacy neuron

for ( int i =0; i<LL; i++){
 sila =0; // zerujemy d l a kazdego neuronu i
 irand=(int ) (LL*( rand ( )/(1.0+RAND_MAX) ) ) ;

 for ( int j =0; j<LL; j++){ //wyliczamy s i l e od p o z o s t a l y c h neuronow
if ( irand==j ) continue ;
 ij=irand*LL+j ;
 sila+=Neur [ j ]->PodajStan ( ) * Synap [ ij ]->PodajWartoscSynapsy ( ) ;
 }
if ( sila <0) {
 if (Neur [ irand]->PodajStan ()==1) iteruj =1;
 Neur [ irand]->WpiszStan (-1);
}
 else {
 if (Neur [ irand]->PodajStan()==-1) iteruj =1;
 Neur [ irand]->WpiszStan ( 1 ) ;
}
 }



 return iteruj ;
 }


 int main ( int argc , char ** argv ){
 if ( argc != 4){
 cout<<"Poprawny format polecenia: "<<endl ;
 cout<<argv [0]<<" PlikWEObrazek1 PlikWEObrazek2 PlikWE3"<<endl ;
 return 0 ;
 }

 /* ****************************************************
 ETAP I : NAUKA WZORCOW
 *************************************************** */
 cout<<endl<<endl<<"Siec neuronowa uczy sie wzorcow ..."<<endl ;

int Lx , Ly ; // rozmiar s i a t k i kwadratowej Lx x Ly wezlow
 int Lx_temp , Ly_temp ;

// pi e rws z y ob raz e k
WczytajParametryObrazka ( argv [1] , Lx , Ly ) ;
 Lx_temp=Lx ; Ly_temp=Ly ;
 // drug i ob raz e k
WczytajParametryObrazka ( argv [2] , Lx , Ly ) ;
  if ( Lx_temp!=Lx || Ly_temp!=Ly){
 cout<<"Rozne rozmiary siatki pikseli"<<endl ;
 cout<<"Wychodze do systemu ..."<<endl ;
 exit ( 1 ) ;
 }

 cout<<"Odczytane wartosci Lx="<<Lx<<"\t"<<"Ly="<<Ly<<endl ;
 // alokujemy kontener Neur z neuronami
 AlokujNeurony (Neur , Lx , Ly ) ; // s i a t k a LxLy neuronow w kont ene r z e ;
 // alokujemy synapsy
AlokujSynapsy ( Synap , Lx*Ly , Lx*Ly ) ; // kontener z (LxLy )^2 synapsami


 //Reset neuronow i synaps przed wczytaniem 1 obrazka
 ResetujNeurony (Neur ) ;
 ResetujSynapsy ( Synap ) ;

 WczytajObrazek (Neur , argv [ 1 ] ) ; // wc zytani e 1 obrazka
WczytajSynapsy (Neur , Synap , Lx , Ly ) ;
 cout<<"Obrazek 1:"<<endl ;
PokazStanyNeuronow(Neur , Lx , Ly ) ;

 ResetujNeurony (Neur ) ; // r e s e tuj emy neurony przed wczytaniem 2 obrazka
 WczytajObrazek (Neur , argv [ 2 ] ) ; // wc zytani e 2 obrazka
WczytajSynapsy (Neur , Synap , Lx , Ly ) ; // u z u p e l n i e n i e synaps drugim obrazkiem
cout<<"Obrazek 2:"<<endl ;
PokazStanyNeuronow(Neur , Lx , Ly ) ;

 /* ****************************************************
 ETAP I I : Czytanie s y gnalu
 *************************************************** */
 //uwaga c a l a pamiec obrazkow j e s t w synapsach
 //Wczytujemy t e r a z j a k i s inny ob raz e k k t o r y t r o c h e r o zni s i e od jednego z wzorcow

ResetujNeurony (Neur) ; // r e s e tuj emy neurony przed wczytaniem obrazka
 WczytajDowolnyObrazek (Neur, argv [ 3 ] , Lx , Ly ) ; //wczytujemy obraz3
 cout<<"Obrazek przeznaczony do rozpoznania:"<<endl ;
PokazStanyNeuronow(Neur , Lx , Ly ) ;
 /* ****************************************************
 ETAPU I I I : Rozpoznawanie s y gnalu
 *************************************************** */
 int iteruj =1;
 int krok=1;
 while(iteruj){
 iteruj=Rozpoznaj (Neur , Synap , Lx , Ly ) ;

  cout<<"................................................"<<endl ;
 cout<<"Rozpoznanie - krok "<<krok<<endl ;

 PokazStanyNeuronow(Neur , Lx , Ly ) ;
 krok++;
 }


 return 0 ;
}
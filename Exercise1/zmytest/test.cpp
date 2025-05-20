
// #include "..."


/* ************************************************************************** */
#include <iostream>
#include "../vector/vector.hpp"
#include "test.hpp"





using namespace std;

/* ************************************************************************** */



void testVector() {
  std::cout << "\n--- Inizio Test Vector ---" << std::endl;

  cout << "\n-Provo a creare un vettore e a stampare i suoi elementi-\n" << std::endl;
  //creo il vettore
  lasd::Vector<int> vettoreProva(3);
  //vettoreProva[4] = 10; lancia un out_of_range : corretto

  vettoreProva[0] = 1;
  vettoreProva[1] = 2;
  vettoreProva[2] = 3;
  //stampo il primo elemento
  cout << "Primo elemento : " << vettoreProva.Front() << std::endl;

  //stampo con operator[]
  cout << "Secondo elemento : " << vettoreProva[1] << std::endl;


  //stampo l'ultimo elemento 
  cout <<"Ultimo elemento : " << vettoreProva.Back() << std::endl; 

  //costruisco e copio il vettore
  lasd::Vector<int> vettoreCopia(vettoreProva);
  cout << "Vettore Copia con la stessa dimensione: " << vettoreCopia.Size() << std::endl; 
  
  //resize
  vettoreProva.Resize(2);
  cout << "Vettore Copia Resize: " << vettoreProva.Size() << std::endl; // accesso diretto 
  // cout << vettoreProva[2] << std::endl;    ora il terzo elemento da out_of_range



  cout << "\n-Provo una copia tra vettori-\n" << std::endl;

  //copia tra vettori
  lasd::SortableVector<int> vec1(3);
  vec1[0] = 10;
  vec1[1] = 20;
  vec1[2] = 30;

  lasd::SortableVector<int> vec2(1); // copia
  cout << "Dimensione di vec2 prima della copia : " << vec2.Size() << endl;

  vec2 = vec1; // chiamato vec2.operator=(vec1)copia

  cout << "Dimensione di vec2 dopo la copia : " << vec2.Size() << endl;

  // Qui usiamo l'operatore ==
  cout << "Sono uguali ? : " << (vec1 == vec2) << endl; // true



  cout << "\n- Provo ad ordinare un elemento-\n" << std::endl;
  lasd::SortableVector<int> vettoreDaOrdinare(4);
  vettoreDaOrdinare[0] = 4;
  vettoreDaOrdinare[1] = 2;
  vettoreDaOrdinare[2] = 3;
  vettoreDaOrdinare[3] = 1;

  for (int i = 0; i < vettoreDaOrdinare.Size(); i++) {
    cout << "Elemento " << i << " : " << vettoreDaOrdinare[i] << endl;
  }

  //ora ordino
  vettoreDaOrdinare.Sort();
  cout << "Dopo l'ordinamento : " << endl;
  for (int i = 0; i < vettoreDaOrdinare.Size(); i++) {
    cout << "Elemento " << i << " : " << vettoreDaOrdinare[i] << endl;
  }
  





 
  //Vedo un po di throw
  lasd::Vector<char> vettoreChar;
 // vettoreChar.Front();  lancia un length_error
 // vettoreChar[4] = 'a';    lancia un out_of_range










  std::cout << "--- Fine Test Vector ---" << std::endl;
}

void mytest() {
  std::cout << "== Esecuzione di mytest() iniziata  ==" << std::endl;
  

  testVector();
  

  std::cout << "\n== Esecuzione di mytest() terminata ==" << std::endl;
}

//il prof ha detto che in mutablelinearocontainer i preorder si implementano nello stesso modo.
//il quick sort con partition lo cancelliamo e dobbiamo usare solo l'insertion sort. 
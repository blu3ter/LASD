
// #include "..."


/* ************************************************************************** */
#include <iostream>
#include "../vector/vector.hpp"
#include "test.hpp"





using namespace std;

/* ************************************************************************** */

void testVectorConstructorsAccessResize() {
  std::cout << "\n--- Inizio Test Costruttori, operator[] e Resize Vector ---" << std::endl;

  // Test costruttore di default e Size()
  lasd::Vector<int> v1;
  std::cout << "v1 (costruttore default): Size = " << v1.Size() << std::endl;

  // Test costruttore con dimensione
  lasd::Vector<int> v2(5);
  std::cout << "v2 (costruttore con size 5): Size = " << v2.Size() << std::endl;

  // Test operator[] per scrittura e lettura
  if (v2.Size() >= 3) {
    v2[0] = 10;
    v2[1] = 20;
    v2[2] = 30;
    std::cout << "v2[0] = " << v2[0] << ", v2[1] = " << v2[1] << ", v2[2] = " << v2[2] << std::endl;
  } else {
    std::cout << "Errore: v2 troppo piccolo per test operator[]" << std::endl;
  }

  // Test Resize: ingrandimento
  std::cout << "Resize v2 a 8" << std::endl;
  v2.Resize(8);
  std::cout << "v2 (dopo resize a 8): Size = " << v2.Size() << std::endl;
  if (v2.Size() >= 1 && v2.Size() <= 8) {
      std::cout << "v2[0] (dopo resize) = " << v2[0] << std::endl; // Dovrebbe essere 10 se il resize preserva i dati
  }
  if (v2.Size() >= 7) {
    v2[6] = 70; // Assegna a un nuovo elemento
    std::cout << "v2[6] (nuovo elemento) = " << v2[6] << std::endl;
  }


  // Test Resize: riduzione
  std::cout << "Resize v2 a 2" << std::endl;
  v2.Resize(2);
  std::cout << "v2 (dopo resize a 2): Size = " << v2.Size() << std::endl;
  if (v2.Size() >= 1) {
      std::cout << "v2[0] (dopo resize a 2) = " << v2[0] << std::endl; // Dovrebbe essere 10
  }
  
  // Esempio di come potresti testare il costruttore di copia che avevi iniziato
  lasd::Vector<int> v_orig(3);
  if (v_orig.Size() >=3) {
    v_orig[0] = 1; v_orig[1] = 2; v_orig[2] = 3;
  }
  lasd::Vector<int> v_copia(v_orig); // Costruttore di copia
  std::cout << "v_copia (da v_orig): Size = " << v_copia.Size() << std::endl;
  if (v_copia.Size() >= 1) {
    std::cout << "v_copia[0] = " << v_copia[0] << std::endl; // Dovrebbe essere 1
  }
  // Modifica l'originale per assicurarsi che la copia sia profonda
  if (v_orig.Size() >= 1) {
    v_orig[0] = 99;
  }
  std::cout << "v_copia[0] (dopo modifica v_orig[0]) = " << v_copia[0] << std::endl; // Dovrebbe essere ancora 1
  std::cout << "v_orig[0] (dopo modifica) = " << v_orig[0] << std::endl; // Dovrebbe essere 99


  std::cout << "--- Fine Test Costruttori, operator[] e Resize Vector ---" << std::endl;
}

void testVector() {
  std::cout << "\n--- Inizio Test Vector ---" << std::endl;
  //creo il vettore
  lasd::Vector<int> vettoreProva(3);
  //vettoreProva[4] = 10; lancia un out_of_range : corretto

  vettoreProva[0] = 1;
  //stampo il primo elemento
  cout << "Primo elemento : " << vettoreProva.Front() << std::endl;


  //stampo l'ultimo elemento 
  cout <<"Ultimo elemento : " << vettoreProva.Back() << std::endl; 

  //copio il vettore
  lasd::Vector<int> vettoreCopia(vettoreProva);
  cout << "Vettore Copia: " << vettoreCopia[0] << std::endl; 
  
  //resize
  vettoreProva.Resize(2);
  cout << "Vettore Resize: " << vettoreProva.Size() << std::endl; // accesso diretto 


  
 
  //Vedo un po di throw
  lasd::Vector<char> vettoreChar;
 // vettoreChar.Front();  lancia un length_error
 // vettoreChar[4] = 'a';    lancia un out_of_range










  std::cout << "--- Fine Test Vector ---" << std::endl;
}

void mytest() {
  std::cout << "== Esecuzione di mytest() iniziata  ==" << std::endl;
  
 // testVectorConstructorsAccessResize();
  testVector();
  

  std::cout << "\n== Esecuzione di mytest() terminata ==" << std::endl;
}

//il prof ha detto che in mutablelinearocontainer i preorder si implementano nello stesso modo.
//il quick sort con partition lo cancelliamo e dobbiamo usare solo l'insertion sort. 
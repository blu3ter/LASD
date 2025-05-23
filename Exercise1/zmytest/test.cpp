// #include "..."


/* ************************************************************************** */
#include <iostream>
#include <utility> // For std::pair
#include "../vector/vector.hpp"
#include "../list/list.hpp"
#include "../set/vec/setvec.hpp"
#include "../set/lst/setlst.hpp" // Including setlst.hpp
#include "test.hpp"
#include "../zlasdtest/container/container.hpp"
#include "../zlasdtest/container/traversable.hpp"
#include "../zlasdtest/container/mappable.hpp"
#include "../zlasdtest/container/linear.hpp"
#include "../zlasdtest/container/testable.hpp"
#include "../zlasdtest/container/dictionary.hpp" // Added for Min, Max, etc.


using namespace std;
using namespace lasd; // To directly use TraversePrint, FoldAdd, etc.
/* ************************************************************************** */

std::pair<unsigned int, unsigned int> testVector() {
  unsigned int testnum = 0; // Total tests counter for this function
  unsigned int testerr = 0; // Error counter for this function

  std::cout << "\n--- Inizio Test Vector ---" << std::endl;

  cout << "\n- Test di base su Vector<int> e utilizzo di Empty/Size da zlasdtest -" << std::endl;
  lasd::Vector<int> vettoreVuoto;
  Empty(testnum, testerr, vettoreVuoto, true);
  Size(testnum, testerr, vettoreVuoto, true, 0);

  lasd::Vector<int> vettoreProva(3);
  Empty(testnum, testerr, vettoreProva, false);
  Size(testnum, testerr, vettoreProva, true, 3);

  vettoreProva[0] = 1;
  vettoreProva[1] = 2;
  vettoreProva[2] = 3;
  
  cout << "Elementi di vettoreProva:" << endl;
  for(unsigned long i = 0; i < vettoreProva.Size(); ++i) {
    cout << "vettoreProva[" << i << "] = " << vettoreProva[i] << endl;
  }
  cout << "Primo elemento (Front): " << vettoreProva.Front() << std::endl;
  cout << "Ultimo elemento (Back): " << vettoreProva.Back() << std::endl; 

  lasd::Vector<int> vettoreCopia(vettoreProva);
  cout << "Test su vettoreCopia (copia di vettoreProva):" << endl;
  Empty(testnum, testerr, vettoreCopia, false);
  Size(testnum, testerr, vettoreCopia, true, 3);
  // Add a test here to verify that the elements are equal
  bool copiaCorretta = true;
  if (vettoreCopia.Size() == vettoreProva.Size()) {
    for (unsigned long i = 0; i < vettoreProva.Size(); ++i) {
      if (vettoreCopia[i] != vettoreProva[i]) {
        copiaCorretta = false;
        break;
      }
    }
  } else {
    copiaCorretta = false;
  }
  testnum++;
  cout << " " << testnum << " (" << testerr << ") Contenuto di vettoreCopia corrisponde a vettoreProva: " << (copiaCorretta ? "Correct" : "Error") << "!" << endl;
  if (!copiaCorretta) testerr++;


  cout << "Test Resize su vettoreProva:" << endl;
  vettoreProva.Resize(2);
  Empty(testnum, testerr, vettoreProva, false); // Should not be empty
  Size(testnum, testerr, vettoreProva, true, 2);
  // cout << vettoreProva[2] << std::endl; // now the third element gives out_of_range (manual test)

  cout << "\n- Test su SortableVector<int> e assegnamento -" << std::endl;
  lasd::SortableVector<int> vec1(3);
  vec1[0] = 10;
  vec1[1] = 20;
  vec1[2] = 30;
  Size(testnum, testerr, vec1, true, 3);

  lasd::SortableVector<int> vec2(1);
  Size(testnum, testerr, vec2, true, 1);
  vec2 = vec1; // Copy assignment
  cout << "Dopo l'assegnamento vec2 = vec1:" << endl;
  Size(testnum, testerr, vec2, true, 3);
  testnum++;
  bool assegnamentoCorretto = (vec1 == vec2);
  cout << " " << testnum << " (" << testerr << ") vec1 == vec2 dopo assegnamento: " << (assegnamentoCorretto ? "Correct" : "Error") << "!" << endl;
  if (!assegnamentoCorretto) testerr++;


  cout << "\n- Test ordinamento su SortableVector<int> -" << std::endl;
  lasd::SortableVector<int> vettoreDaOrdinare(4);
  vettoreDaOrdinare[0] = 4;
  vettoreDaOrdinare[1] = 2;
  vettoreDaOrdinare[2] = 3;
  vettoreDaOrdinare[3] = 1;
  Size(testnum, testerr, vettoreDaOrdinare, true, 4);

  cout << "Elementi prima dell'ordinamento:" << endl;
  for (unsigned long i = 0; i < vettoreDaOrdinare.Size(); i++) {
    cout << "vettoreDaOrdinare[" << i << "] = " << vettoreDaOrdinare[i] << endl;
  }

  vettoreDaOrdinare.Sort();
  cout << "Elementi dopo l'ordinamento:" << endl;
  bool ordinamentoCorretto = true;
  if (vettoreDaOrdinare.Size() == 4) {
    if (vettoreDaOrdinare[0] != 1 || vettoreDaOrdinare[1] != 2 || vettoreDaOrdinare[2] != 3 || vettoreDaOrdinare[3] != 4) {
      ordinamentoCorretto = false;
    }
  } else {
    ordinamentoCorretto = false;
  }
  for (unsigned long i = 0; i < vettoreDaOrdinare.Size(); i++) {
    cout << "vettoreDaOrdinare[" << i << "] = " << vettoreDaOrdinare[i] << endl;
  }
  testnum++;
  cout << " " << testnum << " (" << testerr << ") Ordinamento di vettoreDaOrdinare: " << (ordinamentoCorretto ? "Correct" : "Error") << "!" << endl;
  if (!ordinamentoCorretto) testerr++;

  cout << "\n- Test interfaccia TestableContainer (Exists) -" << std::endl;
  Exists(testnum, testerr, vettoreDaOrdinare, true, 3); // 3 should exist
  Exists(testnum, testerr, vettoreDaOrdinare, false, 5); // 5 should not exist
  Exists(testnum, testerr, vettoreVuoto, false, 0); // 0 should not exist in an empty vector

  cout << "\n- Test interfaccia TraversableContainer (Traverse, Fold) su Vector<int> -" << std::endl;
  lasd::Vector<int> vecTrav(3);
  vecTrav[0] = 10; vecTrav[1] = 20; vecTrav[2] = 30;
  cout << "Elementi di vecTrav: ";
  TraversePreOrder(testnum, testerr, vecTrav, true, &TraversePrint<int>); // Prints the elements
  cout << endl;
  FoldPreOrder(testnum, testerr, vecTrav, true, &FoldAdd<int>, 0, 60); // Sum: 0+10+20+30 = 60
  FoldPostOrder(testnum, testerr, vecTrav, true, &FoldMultiply<int>, 1, 6000); // Product: 1*10*20*30 = 6000

  cout << "\n- Test interfaccia MappableContainer (Map) su Vector<int> -" << std::endl;
  lasd::Vector<int> vecMap(vecTrav); // Copy of {10, 20, 30}
  MapPreOrder(testnum, testerr, vecMap, true, &MapIncrement<int>); // Increments: {11, 21, 31}
  cout << "vecMap dopo MapIncrement: ";
  TraversePreOrder(testnum, testerr, vecMap, true, &TraversePrint<int>);
  cout << endl;
  FoldPreOrder(testnum, testerr, vecMap, true, &FoldAdd<int>, 0, 63); // Sum: 11+21+31 = 63

  MapPostOrder(testnum, testerr, vecMap, true, &MapDouble<int>); // Doubles: {22, 42, 62}
  cout << "vecMap dopo MapDouble: ";
  TraversePreOrder(testnum, testerr, vecMap, true, &TraversePrint<int>);
  cout << endl;
  FoldPreOrder(testnum, testerr, vecMap, true, &FoldAdd<int>, 0, 126); // Sum: 22+42+62 = 126

  cout << "\n- Test interfaccia LinearContainer su Vector<std::string> -" << std::endl;
  lasd::Vector<std::string> vecLin(2);
  Empty(testnum, testerr, vecLin, false);
  Size(testnum, testerr, vecLin, true, 2);

  SetAt(testnum, testerr, vecLin, true, 0, std::string("Hello"));
  SetAt(testnum, testerr, vecLin, true, 1, std::string("World"));
  GetAt(testnum, testerr, vecLin, true, 0, std::string("Hello"));
  GetAt(testnum, testerr, vecLin, true, 1, std::string("World"));

  GetFront(testnum, testerr, vecLin, true, std::string("Hello"));
  GetBack(testnum, testerr, vecLin, true, std::string("World"));

  lasd::Vector<std::string> vecLin2(vecLin);
  EqualLinear(testnum, testerr, vecLin, vecLin2, true);

  SetFront(testnum, testerr, vecLin2, true, std::string("Hi"));
  NonEqualLinear(testnum, testerr, vecLin, vecLin2, true);
  GetFront(testnum, testerr, vecLin2, true, std::string("Hi"));

  // Exception tests (already present in commented form, I make them active with try-catch)
  cout << "\n- Test gestione eccezioni -" << std::endl;
  lasd::Vector<char> vettoreChar; // Empty vector
  testnum++;
  try {
    vettoreChar.Front(); // Should throw length_error
    cout << " " << testnum << " (" << testerr << ") Accesso a Front() su vettore vuoto: Error (eccezione non lanciata)!" << endl;
    testerr++;
  } catch (const std::length_error& e) {
    cout << " " << testnum << " (" << testerr << ") Accesso a Front() su vettore vuoto (std::length_error): Correct!" << endl;
  } catch (...) {
    cout << " " << testnum << " (" << testerr << ") Accesso a Front() su vettore vuoto: Error (eccezione errata)!" << endl;
    testerr++;
  }

  testnum++;
  try {
    vettoreChar[0] = 'a'; // Should throw out_of_range
    cout << " " << testnum << " (" << testerr << ") Accesso a [0] su vettore vuoto: Error (eccezione non lanciata)!" << endl;
    testerr++;
  } catch (const std::out_of_range& e) {
    cout << " " << testnum << " (" << testerr << ") Accesso a [0] su vettore vuoto (std::out_of_range): Correct!" << endl;
  } catch (...) {
    cout << " " << testnum << " (" << testerr << ") Accesso a [0] su vettore vuoto: Error (eccezione errata)!" << endl;
    testerr++;
  }


  std::cout << "\n--- Risultati Finali Test Vector (Errori/Test): " << testerr << "/" << testnum << " ---" << std::endl;
  std::cout << "--- Fine Test Vector ---" << std::endl;
  return {testnum, testerr};
}


std::pair<unsigned int, unsigned int> testList() {
  unsigned int testnum = 0; // Total tests counter for this function
  unsigned int testerr = 0; // Error counter for this function

  std::cout << "\n\n--- Inizio Test List ---" << std::endl;

  cout << "\n- Test di base su List<int> e utilizzo di Empty/Size da zlasdtest -" << std::endl;
  lasd::List<int> listaVuota;
  Empty(testnum, testerr, listaVuota, true);
  Size(testnum, testerr, listaVuota, true, 0);

  cout << "\n- Test inserimento e accesso base -" << std::endl;
  lasd::List<int> listaProva;
  listaProva.InsertAtBack(10);
  listaProva.InsertAtFront(5);
  listaProva.InsertAtBack(20); // List: 5, 10, 20
  Empty(testnum, testerr, listaProva, false);
  Size(testnum, testerr, listaProva, true, 3);

  GetFront(testnum, testerr, listaProva, true, 5);
  GetBack(testnum, testerr, listaProva, true, 20);
  GetAt(testnum, testerr, listaProva, true, 1, 10); // Middle element

  cout << "Elementi di listaProva: ";
  TraversePreOrder(testnum, testerr, listaProva, true, &TraversePrint<int>);
  cout << endl;

  cout << "\n- Test copia e assegnamento -" << std::endl;
  lasd::List<int> listaCopia(listaProva);
  Empty(testnum, testerr, listaCopia, false);
  Size(testnum, testerr, listaCopia, true, 3);
  EqualLinear(testnum, testerr, listaProva, listaCopia, true);

  lasd::List<int> listaAssegnata;
  listaAssegnata.InsertAtBack(100);
  listaAssegnata = listaProva;
  Size(testnum, testerr, listaAssegnata, true, 3);
  EqualLinear(testnum, testerr, listaProva, listaAssegnata, true);
  
  listaAssegnata.InsertAtFront(1); // Modifying listaAssegnata
  NonEqualLinear(testnum, testerr, listaProva, listaAssegnata, true);


  cout << "\n- Test rimozione -" << std::endl;
  // listaProva: 5, 10, 20
  listaProva.RemoveFromFront(); // Now: 10, 20
  Size(testnum, testerr, listaProva, true, 2);
  GetFront(testnum, testerr, listaProva, true, 10);
  
  int valRimossa = listaProva.FrontNRemove(); // Removes 10, returns 10. Now: 20
  testnum++;
  cout << " " << testnum << " (" << testerr << ") Valore rimosso con FrontNRemove: " << valRimossa << ". Atteso: 10. ";
  if (valRimossa == 10) { // Should be 10, not 20, because FrontNRemove removes the current front
    cout << "Correct!" << endl;
  } else {
    cout << "Error! (Ottenuto: " << valRimossa << ")" << endl;
    testerr++;
  }
  Size(testnum, testerr, listaProva, true, 1); // After removing 10, 20 remains
  GetFront(testnum, testerr, listaProva, true, 20);

  listaProva.Clear();
  Empty(testnum, testerr, listaProva, true);
  Size(testnum, testerr, listaProva, true, 0);

  cout << "\n- Test interfaccia TestableContainer (Exists) su List<int> -" << std::endl;
  listaCopia.InsertAtBack(30); // listaCopia was 5, 10, 20. Now: 5, 10, 20, 30
  Exists(testnum, testerr, listaCopia, true, 20);
  Exists(testnum, testerr, listaCopia, false, 15);
  Exists(testnum, testerr, listaVuota, false, 0);

  cout << "\n- Test interfaccia TraversableContainer (Traverse, Fold) su List<int> -" << std::endl;
  // listaCopia: 5, 10, 20, 30
  cout << "Elementi di listaCopia: ";
  TraversePreOrder(testnum, testerr, listaCopia, true, &TraversePrint<int>);
  cout << endl;
  FoldPreOrder(testnum, testerr, listaCopia, true, &FoldAdd<int>, 0, 65); // 5+10+20+30 = 65
  
  // Test with PostOrder (for List, PreOrder and PostOrder Traverse/Fold are often identical in effect)
  cout << "Elementi di listaCopia (PostOrder Traverse): ";
  TraversePostOrder(testnum, testerr, listaCopia, true, &TraversePrint<int>);
  cout << endl;
  FoldPostOrder(testnum, testerr, listaCopia, true, &FoldMultiply<int>, 1, 30000); // 5*10*20*30 = 30000

  cout << "\n- Test interfaccia MappableContainer (Map) su List<int> -" << std::endl;
  lasd::List<int> listaMap(listaCopia); // Copy of {5, 10, 20, 30}
  MapPreOrder(testnum, testerr, listaMap, true, &MapIncrement<int>); // {6, 11, 21, 31}
  cout << "listaMap dopo MapIncrement: ";
  TraversePreOrder(testnum, testerr, listaMap, true, &TraversePrint<int>);
  cout << endl;
  FoldPreOrder(testnum, testerr, listaMap, true, &FoldAdd<int>, 0, 69); // 6+11+21+31 = 69

  MapPostOrder(testnum, testerr, listaMap, true, &MapDouble<int>); // {12, 22, 42, 62}
  cout << "listaMap dopo MapDouble: ";
  TraversePreOrder(testnum, testerr, listaMap, true, &TraversePrint<int>);
  cout << endl;
  FoldPreOrder(testnum, testerr, listaMap, true, &FoldAdd<int>, 0, 138); // 12+22+42+62 = 138

  cout << "\n- Test interfaccia LinearContainer su List<std::string> -" << std::endl;
  lasd::List<std::string> listLin;
  listLin.InsertAtBack(std::string("Alpha"));
  listLin.InsertAtBack(std::string("Beta"));
  listLin.InsertAtFront(std::string("Gamma")); // Gamma, Alpha, Beta
  
  Empty(testnum, testerr, listLin, false);
  Size(testnum, testerr, listLin, true, 3);

  SetAt(testnum, testerr, listLin, true, 1, std::string("Omega")); // Gamma, Omega, Beta
  GetAt(testnum, testerr, listLin, true, 0, std::string("Gamma"));
  GetAt(testnum, testerr, listLin, true, 1, std::string("Omega"));
  GetAt(testnum, testerr, listLin, true, 2, std::string("Beta"));

  GetFront(testnum, testerr, listLin, true, std::string("Gamma"));
  GetBack(testnum, testerr, listLin, true, std::string("Beta"));

  lasd::List<std::string> listLin2(listLin);
  EqualLinear(testnum, testerr, listLin, listLin2, true);

  SetFront(testnum, testerr, listLin2, true, std::string("Delta")); // Delta, Omega, Beta
  NonEqualLinear(testnum, testerr, listLin, listLin2, true);
  GetFront(testnum, testerr, listLin2, true, std::string("Delta"));

  cout << "\n- Test gestione eccezioni per List -" << std::endl;
  lasd::List<char> listaCharVuota;
  testnum++;
  try {
    listaCharVuota.Front();
    cout << " " << testnum << " (" << testerr << ") Accesso a Front() su lista vuota: Error (eccezione non lanciata)!" << endl;
    testerr++;
  } catch (const std::length_error& e) {
    cout << " " << testnum << " (" << testerr << ") Accesso a Front() su lista vuota (std::length_error): Correct!" << endl;
  } catch (...) {
    cout << " " << testnum << " (" << testerr << ") Accesso a Front() su lista vuota: Error (eccezione errata)!" << endl;
    testerr++;
  }

  testnum++;
  try {
    listaCharVuota.RemoveFromFront();
    cout << " " << testnum << " (" << testerr << ") RemoveFromFront() su lista vuota: Error (eccezione non lanciata)!" << endl;
    testerr++;
  } catch (const std::length_error& e) {
    cout << " " << testnum << " (" << testerr << ") RemoveFromFront() su lista vuota (std::length_error): Correct!" << endl;
  } catch (...) {
    cout << " " << testnum << " (" << testerr << ") RemoveFromFront() su lista vuota: Error (eccezione errata)!" << endl;
    testerr++;
  }
  
  testnum++;
  try {
    listaCharVuota[0] = 'a';
    cout << " " << testnum << " (" << testerr << ") Accesso a [0] su lista vuota: Error (eccezione non lanciata)!" << endl;
    testerr++;
  } catch (const std::out_of_range& e) {
    cout << " " << testnum << " (" << testerr << ") Accesso a [0] su lista vuota (std::out_of_range): Correct!" << endl;
  } catch (...) {
    cout << " " << testnum << " (" << testerr << ") Accesso a [0] su lista vuota: Error (eccezione errata)!" << endl;
    testerr++;
  }

  lasd::List<int> listSingle;
  listSingle.InsertAtFront(100);
  // Test SetAt with invalid index (should throw out_of_range)
  // The test SetAt(testnum, testerr, listSingle, false, 1, 200); is correct
  // because we expect the set attempt to fail (chk = false)
  // due to the out_of_range exception.
  SetAt(testnum, testerr, listSingle, false, 1, 200);


  std::cout << "\n--- Risultati Finali Test List (Errori/Test): " << testerr << "/" << testnum << " ---" << std::endl;
  std::cout << "--- Fine Test List ---" << std::endl;
  return {testnum, testerr};
}


std::pair<unsigned int, unsigned int> testSetVec() {
  unsigned int testnum = 0;
  unsigned int testerr = 0;

  std::cout << "\n\n--- Inizio Test SetVec ---" << std::endl;

  cout << "\n- Test di base su SetVec<int> -" << std::endl;
  lasd::SetVec<int> setVuoto;
  Empty(testnum, testerr, setVuoto, true);
  Size(testnum, testerr, setVuoto, true, 0);

  cout << "\n- Test Insert e unicità -" << std::endl;
  lasd::SetVec<int> setProva;
  testnum++;
  cout << " " << testnum << " (" << testerr << ") Inserimento 10 (atteso true): ";
  if (setProva.Insert(10)) { cout << "Correct!" << endl; } else { cout << "Error!" << endl; testerr++; }
  Size(testnum, testerr, setProva, true, 1);

  testnum++;
  cout << " " << testnum << " (" << testerr << ") Inserimento 5 (atteso true): ";
  if (setProva.Insert(5)) { cout << "Correct!" << endl; } else { cout << "Error!" << endl; testerr++; }
  Size(testnum, testerr, setProva, true, 2); // Elements: 5, 10 (sorted)

  testnum++;
  cout << " " << testnum << " (" << testerr << ") Inserimento 20 (atteso true): ";
  if (setProva.Insert(20)) { cout << "Correct!" << endl; } else { cout << "Error!" << endl; testerr++; }
  Size(testnum, testerr, setProva, true, 3); // Elements: 5, 10, 20

  testnum++;
  cout << " " << testnum << " (" << testerr << ") Inserimento 10 (duplicato, atteso false): ";
  if (!setProva.Insert(10)) { cout << "Correct!" << endl; } else { cout << "Error!" << endl; testerr++; }
  Size(testnum, testerr, setProva, true, 3); // Size does not change

  cout << "Elementi di setProva (dovrebbero essere ordinati): ";
  TraversePreOrder(testnum, testerr, setProva, true, &TraversePrint<int>);
  cout << endl;
  GetFront(testnum, testerr, setProva, true, 5); // Minimum
  GetBack(testnum, testerr, setProva, true, 20);  // Maximum
  GetAt(testnum, testerr, setProva, true, 1, 10); // Middle element

  cout << "\n- Test Exists e Remove -" << std::endl;
  Exists(testnum, testerr, setProva, true, 10);
  Exists(testnum, testerr, setProva, false, 15);

  testnum++;
  cout << " " << testnum << " (" << testerr << ") Rimozione 15 (non esistente, atteso false): ";
  if (!setProva.Remove(15)) { cout << "Correct!" << endl; } else { cout << "Error!" << endl; testerr++; }
  Size(testnum, testerr, setProva, true, 3);

  testnum++;
  cout << " " << testnum << " (" << testerr << ") Rimozione 10 (esistente, atteso true): ";
  if (setProva.Remove(10)) { cout << "Correct!" << endl; } else { cout << "Error!" << endl; testerr++; }
  Size(testnum, testerr, setProva, true, 2); // Elements: 5, 20
  Exists(testnum, testerr, setProva, false, 10);
  cout << "Elementi dopo rimozione di 10: ";
  TraversePreOrder(testnum, testerr, setProva, true, &TraversePrint<int>);
  cout << endl;

  cout << "\n- Test copia e assegnamento -" << std::endl;
  lasd::SetVec<int> setCopia(setProva); // setProva is {5, 20}
  Empty(testnum, testerr, setCopia, false);
  Size(testnum, testerr, setCopia, true, 2);
  EqualLinear(testnum, testerr, setProva, setCopia, true);

  lasd::SetVec<int> setAssegnato;
  setAssegnato.Insert(100);
  setAssegnato = setProva;
  Size(testnum, testerr, setAssegnato, true, 2);
  EqualLinear(testnum, testerr, setProva, setAssegnato, true);
  
  setAssegnato.Insert(1); // Modifying setAssegnato: {1, 5, 20}
  NonEqualLinear(testnum, testerr, setProva, setAssegnato, true);

  cout << "\n- Test OrderedDictionaryContainer: Min, Max, Predecessor, Successor -" << std::endl;
  lasd::SetVec<int> setOrd;
  setOrd.Insert(50); setOrd.Insert(30); setOrd.Insert(70); setOrd.Insert(20); setOrd.Insert(40); setOrd.Insert(60); setOrd.Insert(80);
  // Set: 20, 30, 40, 50, 60, 70, 80
  cout << "Elementi di setOrd: "; TraversePreOrder(testnum, testerr, setOrd, true, &TraversePrint<int>); cout << endl;
  
  Min(testnum, testerr, setOrd, true, 20);
  Max(testnum, testerr, setOrd, true, 80);

  Predecessor(testnum, testerr, setOrd, true, 50, 40);
  Successor(testnum, testerr, setOrd, true, 50, 60);
  
  // Boundary test: Predecessor of the minimum (throws exception) -> chk false
  // The Predecessor test function expects the predecessor to exist if chk is true.
  // If we expect an exception, chk must be false.
  Predecessor(testnum, testerr, setOrd, false, 20, 0); // The expected value (0) is irrelevant when chk is false for an exception
  Successor(testnum, testerr, setOrd, false, 80, 0);   // Boundary test: Successor of the maximum (throws exception) -> chk false

  // Test Predecessor/Successor for non-existent values
  Predecessor(testnum, testerr, setOrd, true, 55, 50); // Predecessor of 55 is 50
  Successor(testnum, testerr, setOrd, true, 55, 60);   // Successor of 55 is 60

  // Line 473 is empty in the current file

  int valPred = setOrd.PredecessorNRemove(50); // Removes 40 - Correct: -> int
  testnum++;
  cout << " " << testnum << " (" << testerr << ") PredecessorNRemove(50) (rimosso 40, restituito 40): ";
  if (valPred == 40 && !setOrd.Exists(40) && setOrd.Size() == 6) { cout << "Correct!" << endl; } else { cout << "Error!" << endl; testerr++; }
  // Set: 20, 30, 50, 60, 70, 80

  int valSucc = setOrd.SuccessorNRemove(50); // Removes 60 - Correct: Data -> int
  testnum++;
  cout << " " << testnum << " (" << testerr << ") SuccessorNRemove(50) (rimosso 60, restituito 60): ";
  if (valSucc == 60 && !setOrd.Exists(60) && setOrd.Size() == 5) { cout << "Correct!" << endl; } else { cout << "Error!" << endl; testerr++; }
  // Set: 20, 30, 50, 70, 80
  cout << "Elementi di setOrd dopo PredecessorNRemove e SuccessorNRemove: "; TraversePreOrder(testnum, testerr, setOrd, true, &TraversePrint<int>); cout << endl;

  int minVal = setOrd.MinNRemove(); // Removes 20 - Correct: Data -> int
  testnum++;
  cout << " " << testnum << " (" << testerr << ") MinNRemove (rimosso 20, restituito 20): ";
  if (minVal == 20 && !setOrd.Exists(20) && setOrd.Size() == 4) { cout << "Correct!" << endl; } else { cout << "Error!" << endl; testerr++; }
  // Set: 30, 50, 70, 80

  int maxVal = setOrd.MaxNRemove(); // Removes 80 (already correct in the file)
  testnum++;
  cout << " " << testnum << " (" << testerr << ") MaxNRemove (rimosso 80, restituito 80): ";
  if (maxVal == 80 && !setOrd.Exists(80) && setOrd.Size() == 3) { cout << "Correct!" << endl; } else { cout << "Error!" << endl; testerr++; }
  // Set: 30, 50, 70
  cout << "Elementi di setOrd dopo MinNRemove e MaxNRemove: "; TraversePreOrder(testnum, testerr, setOrd, true, &TraversePrint<int>); cout << endl;

  setOrd.Clear();
  Empty(testnum, testerr, setOrd, true);

  cout << "\n- Test Traversable e Mappable su SetVec<std::string> -" << std::endl;
  lasd::SetVec<std::string> setStr;
  setStr.Insert("Charlie"); setStr.Insert("Alpha"); setStr.Insert("Bravo"); // Alpha, Bravo, Charlie
  
  cout << "Elementi di setStr: ";
  TraversePreOrder(testnum, testerr, setStr, true, &TraversePrint<std::string>); cout << endl;
  FoldPreOrder(testnum, testerr, setStr, true, &FoldStringConcatenate, std::string(""), std::string("AlphaBravoCharlie"));
  
  cout << "\n- Test gestione eccezioni per SetVec -" << std::endl;
  lasd::SetVec<char> setCharVuoto;
  Min(testnum, testerr, setCharVuoto, false, ' ');
  Max(testnum, testerr, setCharVuoto, false, ' ');
  Predecessor(testnum, testerr, setCharVuoto, false, 'a', ' ');
  Successor(testnum, testerr, setCharVuoto, false, 'a', ' ');

  lasd::SetVec<int> setSingle;
  setSingle.Insert(100);
  Predecessor(testnum, testerr, setSingle, false, 100, 0);
  Successor(testnum, testerr, setSingle, false, 100, 0);
  GetAt(testnum, testerr, setSingle, false, 1, 0); // Out of range access

  std::cout << "\n--- Risultati Finali Test SetVec (Errori/Test): " << testerr << "/" << testnum << " ---" << std::endl;
  std::cout << "--- Fine Test SetVec ---" << std::endl;
  return {testnum, testerr};
}


std::pair<unsigned int, unsigned int> testSetLst() {
  unsigned int testnum = 0;
  unsigned int testerr = 0;

  std::cout << "\n\n--- Inizio Test SetLst ---" << std::endl;

  cout << "\n- Test di base su SetLst<int> -" << std::endl;
  lasd::SetLst<int> setVuoto;
  Empty(testnum, testerr, setVuoto, true);
  Size(testnum, testerr, setVuoto, true, 0);

  cout << "\n- Test Insert e unicità -" << std::endl;
  lasd::SetLst<int> setProva;
  testnum++;
  cout << " " << testnum << " (" << testerr << ") Inserimento 10 (atteso true): ";
  if (setProva.Insert(10)) { cout << "Correct!" << endl; } else { cout << "Error!" << endl; testerr++; }
  Size(testnum, testerr, setProva, true, 1);

  testnum++;
  cout << " " << testnum << " (" << testerr << ") Inserimento 5 (atteso true): ";
  if (setProva.Insert(5)) { cout << "Correct!" << endl; } else { cout << "Error!" << endl; testerr++; }
  Size(testnum, testerr, setProva, true, 2); // Elements: 5, 10 (sorted)

  testnum++;
  cout << " " << testnum << " (" << testerr << ") Inserimento 20 (atteso true): ";
  if (setProva.Insert(20)) { cout << "Correct!" << endl; } else { cout << "Error!" << endl; testerr++; }
  Size(testnum, testerr, setProva, true, 3); // Elements: 5, 10, 20

  testnum++;
  cout << " " << testnum << " (" << testerr << ") Inserimento 10 (duplicato, atteso false): ";
  if (!setProva.Insert(10)) { cout << "Correct!" << endl; } else { cout << "Error!" << endl; testerr++; }
  Size(testnum, testerr, setProva, true, 3); // Size does not change

  cout << "Elementi di setProva (dovrebbero essere ordinati): ";
  TraversePreOrder(testnum, testerr, setProva, true, &TraversePrint<int>);
  cout << endl;
  GetFront(testnum, testerr, setProva, true, 5); // Minimum
  GetBack(testnum, testerr, setProva, true, 20);  // Maximum
  GetAt(testnum, testerr, setProva, true, 1, 10); // Middle element

  cout << "\n- Test Exists e Remove -" << std::endl;
  Exists(testnum, testerr, setProva, true, 10);
  Exists(testnum, testerr, setProva, false, 15);

  testnum++;
  cout << " " << testnum << " (" << testerr << ") Rimozione 15 (non esistente, atteso false): ";
  if (!setProva.Remove(15)) { cout << "Correct!" << endl; } else { cout << "Error!" << endl; testerr++; }
  Size(testnum, testerr, setProva, true, 3);

  testnum++;
  cout << " " << testnum << " (" << testerr << ") Rimozione 10 (esistente, atteso true): ";
  if (setProva.Remove(10)) { cout << "Correct!" << endl; } else { cout << "Error!" << endl; testerr++; }
  Size(testnum, testerr, setProva, true, 2); // Elements: 5, 20
  Exists(testnum, testerr, setProva, false, 10);
  cout << "Elementi dopo rimozione di 10: ";
  TraversePreOrder(testnum, testerr, setProva, true, &TraversePrint<int>);
  cout << endl;

  cout << "\n- Test copia e assegnamento -" << std::endl;
  lasd::SetLst<int> setCopia(setProva); // setProva is {5, 20}
  Empty(testnum, testerr, setCopia, false);
  Size(testnum, testerr, setCopia, true, 2);
  EqualLinear(testnum, testerr, setProva, setCopia, true);

  lasd::SetLst<int> setAssegnato;
  setAssegnato.Insert(100);
  setAssegnato = setProva;
  Size(testnum, testerr, setAssegnato, true, 2);
  EqualLinear(testnum, testerr, setProva, setAssegnato, true);
  
  setAssegnato.Insert(1); // Modifying setAssegnato: {1, 5, 20}
  NonEqualLinear(testnum, testerr, setProva, setAssegnato, true);

  cout << "\n- Test OrderedDictionaryContainer: Min, Max, Predecessor, Successor -" << std::endl;
  lasd::SetLst<int> setOrd;
  setOrd.Insert(50); setOrd.Insert(30); setOrd.Insert(70); setOrd.Insert(20); setOrd.Insert(40); setOrd.Insert(60); setOrd.Insert(80);
  // Set: 20, 30, 40, 50, 60, 70, 80
  cout << "Elementi di setOrd: "; TraversePreOrder(testnum, testerr, setOrd, true, &TraversePrint<int>); cout << endl;
  
  Min(testnum, testerr, setOrd, true, 20);
  Max(testnum, testerr, setOrd, true, 80);

  Predecessor(testnum, testerr, setOrd, true, 50, 40);
  Successor(testnum, testerr, setOrd, true, 50, 60);
  
  Predecessor(testnum, testerr, setOrd, false, 20, 0);
  Successor(testnum, testerr, setOrd, false, 80, 0);

  Predecessor(testnum, testerr, setOrd, true, 55, 50);
  Successor(testnum, testerr, setOrd, true, 55, 60);

  int valPred = setOrd.PredecessorNRemove(50);
  testnum++;
  cout << " " << testnum << " (" << testerr << ") PredecessorNRemove(50) (rimosso 40, restituito 40): ";
  if (valPred == 40 && !setOrd.Exists(40) && setOrd.Size() == 6) { cout << "Correct!" << endl; } else { cout << "Error!" << endl; testerr++; }
  
  int valSucc = setOrd.SuccessorNRemove(50);
  testnum++;
  cout << " " << testnum << " (" << testerr << ") SuccessorNRemove(50) (rimosso 60, restituito 60): ";
  if (valSucc == 60 && !setOrd.Exists(60) && setOrd.Size() == 5) { cout << "Correct!" << endl; } else { cout << "Error!" << endl; testerr++; }
  cout << "Elementi di setOrd dopo PredecessorNRemove e SuccessorNRemove: "; TraversePreOrder(testnum, testerr, setOrd, true, &TraversePrint<int>); cout << endl;

  int minVal = setOrd.MinNRemove();
  testnum++;
  cout << " " << testnum << " (" << testerr << ") MinNRemove (rimosso 20, restituito 20): ";
  if (minVal == 20 && !setOrd.Exists(20) && setOrd.Size() == 4) { cout << "Correct!" << endl; } else { cout << "Error!" << endl; testerr++; }
  
  int maxVal = setOrd.MaxNRemove();
  testnum++;
  cout << " " << testnum << " (" << testerr << ") MaxNRemove (rimosso 80, restituito 80): ";
  if (maxVal == 80 && !setOrd.Exists(80) && setOrd.Size() == 3) { cout << "Correct!" << endl; } else { cout << "Error!" << endl; testerr++; }
  cout << "Elementi di setOrd dopo MinNRemove e MaxNRemove: "; TraversePreOrder(testnum, testerr, setOrd, true, &TraversePrint<int>); cout << endl;

  setOrd.Clear();
  Empty(testnum, testerr, setOrd, true);

  cout << "\n- Test Traversable e Mappable su SetLst<std::string> -" << std::endl;
  lasd::SetLst<std::string> setStr;
  setStr.Insert("Charlie"); setStr.Insert("Alpha"); setStr.Insert("Bravo"); // Alpha, Bravo, Charlie
  
  cout << "Elementi di setStr: ";
  TraversePreOrder(testnum, testerr, setStr, true, &TraversePrint<std::string>); cout << endl;
  FoldPreOrder(testnum, testerr, setStr, true, &FoldStringConcatenate, std::string(""), std::string("AlphaBravoCharlie"));
  
  cout << "\n- Test gestione eccezioni per SetLst -" << std::endl;
  lasd::SetLst<char> setCharVuoto;
  Min(testnum, testerr, setCharVuoto, false, ' ');
  Max(testnum, testerr, setCharVuoto, false, ' ');
  Predecessor(testnum, testerr, setCharVuoto, false, 'a', ' ');
  Successor(testnum, testerr, setCharVuoto, false, 'a', ' ');

  lasd::SetLst<int> setSingle;
  setSingle.Insert(100);
  Predecessor(testnum, testerr, setSingle, false, 100, 0);
  Successor(testnum, testerr, setSingle, false, 100, 0);
  GetAt(testnum, testerr, setSingle, false, 1, 0); // Out of range access

  std::cout << "\n--- Risultati Finali Test SetLst (Errori/Test): " << testerr << "/" << testnum << " ---" << std::endl;
  std::cout << "--- Fine Test SetLst ---" << std::endl;
  return {testnum, testerr};
}


void mytest() {
  std::cout << "== Esecuzione di mytest() iniziata  ==" << std::endl;
  
  unsigned int totalTestNum = 0;
  unsigned int totalTestErr = 0;
  std::pair<unsigned int, unsigned int> results;

  results = testVector();
  totalTestNum += results.first;
  totalTestErr += results.second;

  results = testList();
  totalTestNum += results.first;
  totalTestErr += results.second;
  
  results = testSetVec();
  totalTestNum += results.first;
  totalTestErr += results.second;

  results = testSetLst();
  totalTestNum += results.first;
  totalTestErr += results.second;
  

  std::cout << "\n\n===== RIEPILOGO TEST TOTALI =====" << std::endl;
  std::cout << "Numero totale di test eseguiti: " << totalTestNum << std::endl;
  std::cout << "Numero totale di errori riscontrati: " << totalTestErr << std::endl;
  std::cout << "Risultato finale complessivo (Errori/Test): " << totalTestErr << "/" << totalTestNum << std::endl;
  std::cout << "===================================" << std::endl;
  std::cout << "\n== Esecuzione di mytest() terminata ==" << std::endl;
}

//the professor said that in mutablelinearcontainer preorders are implemented in the same way.
//we delete quick sort with partition and we must use only insertion sort.
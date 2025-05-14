
// #include "..."


/* ************************************************************************** */
#include <iostream>
#include "../vector/vector.hpp"
#include "test.hpp"





using namespace std;

/* ************************************************************************** */

void mytest() {
  // ...
  lasd::Vector<int> v(10);

  for (ulong i = 0; i < 10; ++i) {
    v[i] = i;
  }
}
//il prof ha detto che in mutablelinearocontainer i preorder si implementano nello stesso modo.
//il quick sort con partition lo cancelliamo e dobbiamo usare solo l'insertion sort. 
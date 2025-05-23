

namespace lasd {

/* ************************************************************************** */

// TraversableContainer 

template<typename Data>
bool TraversableContainer<Data>::Exists(const Data& dat) const noexcept {
    bool found = false;
    Traverse([&found, &dat](const Data& curr) {
        if (curr == dat) {
            found = true;
        }
    });
    return found;
}

template<typename Data>
template<typename Accumulator>
Accumulator TraversableContainer<Data>::Fold(typename TraversableContainer<Data>::FoldFun<Accumulator> foldFun, Accumulator acc) {
    Accumulator result = acc;
    Traverse([&foldFun, &result](const Data& curr) {
        result = foldFun(curr, result);
    });
    return result;
} 

/* ************************************************************************** */

// PreOrderTraversableContainer 

template<typename Data>
void PreOrderTraversableContainer<Data>::Traverse(typename TraversableContainer<Data>::TraverseFun fun) const {
    PreOrderTraverse(fun);
}

template<typename Data>
template<typename Accumulator>
Accumulator PreOrderTraversableContainer<Data>::PreOrderFold(typename TraversableContainer<Data>::FoldFun<Accumulator> fold, Accumulator acc) const {
    Accumulator result = acc;
    PreOrderTraverse([&fold, &result](const Data& curr) {
        result = fold(curr, result);
    });
    return result;
}

/* ************************************************************************** */

// PostOrderTraversableContainer 

template<typename Data>
void PostOrderTraversableContainer<Data>::Traverse(typename TraversableContainer<Data>::TraverseFun fun) const {
    PostOrderTraverse(fun);
}

template<typename Data>
template<typename Accumulator>
Accumulator PostOrderTraversableContainer<Data>::PostOrderFold(typename TraversableContainer<Data>::FoldFun<Accumulator> fold, Accumulator acc) const {
    Accumulator result = acc;
    PostOrderTraverse([&fold, &result](const Data& curr) {
        result = fold(curr, result);
    });
    return result;
}

/* ************************************************************************** */

}



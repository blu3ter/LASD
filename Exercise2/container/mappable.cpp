
namespace lasd {

/* ************************************************************************** */

// PreOrderMappableContainer implementations

template<typename Data>
void PreOrderMappableContainer<Data>::Map(typename MappableContainer<Data>::MapFun fun) {
    PreOrderMap(fun);
}

/* ************************************************************************** */

// PostOrderMappableContainer implementations

template<typename Data>
void PostOrderMappableContainer<Data>::Map(typename MappableContainer<Data>::MapFun fun) {
    PostOrderMap(fun);
}

/* ************************************************************************** */

}



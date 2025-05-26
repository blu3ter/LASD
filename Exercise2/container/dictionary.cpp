namespace lasd {

/* ************************************************************************** */

template<typename Data>
bool DictionaryContainer<Data>::InsertAll(const TraversableContainer<Data>& container) {
    bool inserted = true;
    container.Traverse([this, &inserted](const Data& data) {
        if(!Insert(data)) {
            inserted = false;
        }
    });
    return inserted;
}

template<typename Data>
bool DictionaryContainer<Data>::InsertAll(MappableContainer<Data>&& container) {
    bool inserted = true;
    container.Map([this, &inserted](Data& data) {
        if(!Insert(std::move(data))) {
            inserted = false;
        }
    });
    return inserted;
}

template<typename Data>
bool DictionaryContainer<Data>::RemoveAll(TraversableContainer<Data>&& container) {
    bool removed = true;
    container.Traverse([this, &removed](const Data& data) {
        if(!Remove(data)) {
            removed = false;
        }
    });
    return removed;
}

template<typename Data>
bool DictionaryContainer<Data>::InsertSome(const TraversableContainer<Data>& container) {
    bool inserted = false;
    container.Traverse([this, &inserted](const Data& data) {
        if(Insert(data)) {
            inserted = true;
        }
    });
    return inserted;
}

template<typename Data>
bool DictionaryContainer<Data>::InsertSome(MappableContainer<Data>&& container) {
    bool inserted = false;
    container.Map([this, &inserted](Data& data) {
        if(Insert(std::move(data))) {
            inserted = true;
        }
    });
    return inserted;
}

template<typename Data>
bool DictionaryContainer<Data>::RemoveSome(TraversableContainer<Data>&& container) {
    bool removed = false;
    container.Traverse([this, &removed](const Data& data) {
        if(Remove(data)) {
            removed = true;
        }
    });
    return removed;
}

/* ************************************************************************** */

}



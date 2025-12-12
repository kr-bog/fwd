#ifndef FWD_CONTAINER_IMPL_H
#define FWD_CONTAINER_IMPL_H

//реализация iterator

//создает пустой итератор
template <typename T>
fwd_container<T>::iterator::iterator() : ptr(nullptr) {}

//создает инетратор обертку вокург p
template <typename T>
fwd_container<T>::iterator::iterator(iterator_base* p) : ptr(p) {}

//диструктор
template <typename T>
fwd_container<T>::iterator::~iterator() { delete ptr; }

//копирующий конструкто
template <typename T>
fwd_container<T>::iterator::iterator(const iterator& o)
    : ptr(o.ptr ? o.ptr->clone() : nullptr) {}

//перемещающий конструктор
template <typename T>
fwd_container<T>::iterator::iterator(iterator&& o) noexcept
    : ptr(o.ptr) { o.ptr = nullptr; }

//копирующее присваивание
template <typename T>
typename fwd_container<T>::iterator&
fwd_container<T>::iterator::operator=(const iterator& o) {
    if (this != &o) {
        delete ptr;
        ptr = o.ptr ? o.ptr->clone() : nullptr;
    }
    return *this;
}

//перемещающее присваивание
template <typename T>
typename fwd_container<T>::iterator&
fwd_container<T>::iterator::operator=(iterator&& o) noexcept {
    if (this != &o) {
        delete ptr;
        ptr = o.ptr;
        o.ptr = nullptr;
    }
    return *this;
}

//возвращает элемент
template <typename T>
typename fwd_container<T>::iterator::reference
fwd_container<T>::iterator::operator*() { return **ptr; }

//доступ к полю элемента
template <typename T>
typename fwd_container<T>::iterator::pointer
fwd_container<T>::iterator::operator->() { return ptr->operator->(); }

//шаг вперед
template <typename T>
typename fwd_container<T>::iterator&
fwd_container<T>::iterator::operator++() { ++(*ptr); return *this; }

//возвращает старое значение
template <typename T>
typename fwd_container<T>::iterator
fwd_container<T>::iterator::operator++(int) {
    iterator t(*this);      //копия текущего
    ++(*this);              //сдвигаем себя
    return t;               //возвращаем копию
}

//сравнение двух итераторов
template <typename T>
bool fwd_container<T>::iterator::operator==(const iterator& o) const {
    if (!ptr && !o.ptr) return true;    //оба пустые true
    if (!ptr || !o.ptr) return false;   //один пустой false
    return *ptr == *o.ptr;              //сравниваем внутренние итераторы
}

template <typename T>
bool fwd_container<T>::iterator::operator!=(const iterator& o) const {
    return !(*this == o);
}

//сравнение с константным итератором
template <typename T>
bool fwd_container<T>::iterator::operator==(const const_iterator& o) const {
    if (!ptr && !o.get()) return true;
    if (!ptr || !o.get()) return false;
    return *ptr == *o.get();
}

template <typename T>
bool fwd_container<T>::iterator::operator!=(const const_iterator& o) const {
    return !(*this == o);
}

//получить внутренний указатель
template <typename T>
typename fwd_container<T>::iterator_base*
fwd_container<T>::iterator::get() const { return ptr; }

//реализация const_iterator

//создает пустой константный итератор
template <typename T>
fwd_container<T>::const_iterator::const_iterator() : ptr(nullptr) {}

//создает обертку для p
template <typename T>
fwd_container<T>::const_iterator::const_iterator(const_iterator_base* p) : ptr(p) {}

//дистркуктор
template <typename T>
fwd_container<T>::const_iterator::~const_iterator() { delete ptr; }

//копирующий конструктор
template <typename T>
fwd_container<T>::const_iterator::const_iterator(const const_iterator& o)
    : ptr(o.ptr ? o.ptr->clone() : nullptr) {}

//перемещающий конструктор
template <typename T>
fwd_container<T>::const_iterator::const_iterator(const_iterator&& o) noexcept
    : ptr(o.ptr) { o.ptr = nullptr; }

//делает константную версию
template <typename T>
fwd_container<T>::const_iterator::const_iterator(const iterator& o)
    : ptr(o.get() ? o.get()->make_const() : nullptr) {}

//копирующее присваивание
template <typename T>
typename fwd_container<T>::const_iterator&
fwd_container<T>::const_iterator::operator=(const const_iterator& o) {
    if (this != &o) {
        delete ptr;
        ptr = o.ptr ? o.ptr->clone() : nullptr;
    }
    return *this;
}

//перемещающее присваивание
template <typename T>
typename fwd_container<T>::const_iterator&
fwd_container<T>::const_iterator::operator=(const_iterator&& o) noexcept {
    if (this != &o) {
        delete ptr;
        ptr = o.ptr;
        o.ptr = nullptr;
    }
    return *this;
}

//присваивание из обычного итератора
template <typename T>
typename fwd_container<T>::const_iterator&
fwd_container<T>::const_iterator::operator=(const iterator& o) {
    delete ptr;
    ptr = o.get() ? o.get()->make_const() : nullptr;
    return *this;
}

//возвращает константную ссылку
template <typename T>
typename fwd_container<T>::const_iterator::reference
fwd_container<T>::const_iterator::operator*() const { return **ptr; }

//доступ к полю элемента
template <typename T>
typename fwd_container<T>::const_iterator::pointer
fwd_container<T>::const_iterator::operator->() const { return ptr->operator->(); }

// шаг вперед
template <typename T>
typename fwd_container<T>::const_iterator&
fwd_container<T>::const_iterator::operator++() { ++(*ptr); return *this; }

//возвращает старое значение
template <typename T>
typename fwd_container<T>::const_iterator
fwd_container<T>::const_iterator::operator++(int) {
    const_iterator t(*this);
    ++(*this);
    return t;
}

//сравнение константных итераторов
template <typename T>
bool fwd_container<T>::const_iterator::operator==(const const_iterator& o) const {
    if (!ptr && !o.ptr) return true;
    if (!ptr || !o.ptr) return false;
    return *ptr == *o.ptr;
}

template <typename T>
bool fwd_container<T>::const_iterator::operator!=(const const_iterator& o) const {
    return !(*this == o);
}

//сравнение с обычным итератором
template <typename T>
bool fwd_container<T>::const_iterator::operator==(const iterator& o) const {
    if (!ptr && !o.get()) return true;
    if (!ptr || !o.get()) return false;
    return *ptr == *o.get();
}

template <typename T>
bool fwd_container<T>::const_iterator::operator!=(const iterator& o) const {
    return !(*this == o);
}

//получить внутренний указатель
template <typename T>
typename fwd_container<T>::const_iterator_base*
fwd_container<T>::const_iterator::get() const { return ptr; }

//реализация operator= контейнера
//позволяет присваивать любой контейнер любому (stack = queue)
template <typename T>
fwd_container<T>& fwd_container<T>::operator=(const fwd_container& o) {
    if (this != &o) {
        while (!is_empty()) pop();
        std::size_t n = o.size();
        if (n > 0) {
            T* arr = new T[n];
            std::size_t i = 0;
            for (auto it = o.cbegin(); it != o.cend(); ++it) arr[i++] = *it;
            for (std::size_t j = n; j > 0; --j) push(std::move(arr[j-1]));
            delete[] arr;
        }
    }
    return *this;
}

//ввод
template <typename T>
std::istream& operator>>(std::istream& is, fwd_container<T>& c) {
    T val;
    while (is >> val) c.push(val);
    return is;
}

//вывод
template <typename T>
std::ostream& operator<<(std::ostream& os, const fwd_container<T>& c) {
    bool first = true;
    for (auto it = c.cbegin(); it != c.cend(); ++it) {
        if (!first) os << ' ';
        os << *it;
        first = false;
    }
    return os;
}

#endif

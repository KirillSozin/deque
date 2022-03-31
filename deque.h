#include <iostream>
#include <memory>
const int SIZE = 512;
//----------------------------------------------------------------------------------------------------------------------
template<class T, typename ref = T &, typename ptr = T *>
class deque_iterator {
 public:
  typedef deque_iterator<T, T &, T *> iterator;
  typedef deque_iterator<T, const T &, const T *> const_iterator;

  T *first;
  T *current;
  T *last;
  T **node;

  deque_iterator() : first(nullptr), current(nullptr), last(nullptr), node(nullptr) {}

  deque_iterator(const iterator &other)
      : first(other.first), current(other.current), last(other.last), node(other.node) {}

  deque_iterator<T, ref, ptr> &operator++();
  deque_iterator<T, ref, ptr> operator++(int);

  deque_iterator<T, ref, ptr> &operator--();
  deque_iterator<T, ref, ptr> operator--(int);

  deque_iterator<T, ref, ptr> &operator+=(int N);
  deque_iterator<T, ref, ptr> &operator-=(int N);

  ref operator*() const { return *current; }

  ptr operator->() const { return current; }

  ref operator[](int N) const { return *((*this + N).current); }

  bool operator==(const deque_iterator<T, ref, ptr> &other) const { return current == other.current; }

  bool operator!=(const deque_iterator &other) const;

  bool operator<(const deque_iterator &other) const;

  bool operator>(const deque_iterator &other) const;

  bool operator<=(const deque_iterator &other) const;

  bool operator>=(const deque_iterator &other) const;

  void set_node(T **&other);

};
//----------------------------------------------------------------------------------------------------------------------

template<class T, typename ref, typename ptr>
bool deque_iterator<T, ref, ptr>::operator!=(const deque_iterator<T, ref, ptr> &other) const {
  return current != other.current;
}

template<class T, typename ref, typename ptr>
bool deque_iterator<T, ref, ptr>::operator<(const deque_iterator<T, ref, ptr> &other) const {
  return node == other.node ? current < other.current : node < other.node;
}

template<class T, typename ref, typename ptr>
bool deque_iterator<T, ref, ptr>::operator>(const deque_iterator<T, ref, ptr> &other) const {
  return !(current < other.current || current == other.current);
}

template<class T, typename ref, typename ptr>
bool deque_iterator<T, ref, ptr>::operator<=(const deque_iterator<T, ref, ptr> &other) const {
  return (current < other.current) || (current == other.current);
}

template<class T, typename ref, typename ptr>
bool deque_iterator<T, ref, ptr>::operator>=(const deque_iterator<T, ref, ptr> &other) const {
  return !(current < other.current);
}

template<class T, typename ref, typename ptr>
deque_iterator<T, ref, ptr> &deque_iterator<T, ref, ptr>::operator++() {
  current++;
  if (current == last) {
    node++;
    first = *node;
    current = first;
    last = first + SIZE;
  }
  return *this;
}

template<class T, typename ref, typename ptr>
deque_iterator<T, ref, ptr> deque_iterator<T, ref, ptr>::operator++(int) {
  deque_iterator<T, ref, ptr> tmp = *this;
  ++*this;
  return tmp;
}

template<class T, typename ref, typename ptr>
deque_iterator<T, ref, ptr> &deque_iterator<T, ref, ptr>::operator--() {
  current--;
  if (current < first) {
    node--;
    first = *node;
    last = first + SIZE;
    current = last - 1;

  }
  return *this;
}

template<class T, typename ref, typename ptr>
deque_iterator<T, ref, ptr> deque_iterator<T, ref, ptr>::operator--(int) {
  deque_iterator<T, ref, ptr> tmp = *this;
  --*this;
  return tmp;
}

template<class T, typename ref, typename ptr>
deque_iterator<T, ref, ptr> &deque_iterator<T, ref, ptr>::operator+=(int N) {
  int new_pos = current - first + N;
  if (new_pos >= 0 && new_pos < SIZE) {
    current += N;
  }
  else {
    int new_node;
    if (new_pos > 0) {
      new_node = new_pos / SIZE;
    }
    else {
      new_node = -((-new_pos - 1) / SIZE)  - 1;
    }
    T** tmp_node = node + new_node;
    set_node(tmp_node);
    current = first + (new_pos - new_node * SIZE);
  }
  return *this;
}

template<class T, typename ref, typename ptr>
deque_iterator<T, ref, ptr> &deque_iterator<T, ref, ptr>::operator-=(int N) {
  return *this += -N;
}

template<class T, typename ref, typename ptr>
void deque_iterator<T, ref, ptr>::set_node(T **&other) {
  node = other;
  first = *node;
  last = first + SIZE;
}

template<class T, typename ref, typename ptr>
deque_iterator<T, ref, ptr> operator+(const deque_iterator<T, ref, ptr> &other, int N) {
  deque_iterator<T, ref, ptr> tmp = other;
  tmp += N;
  return tmp;
}

template<class T, typename ref, typename ptr>
deque_iterator<T, ref, ptr> operator+(int N, const deque_iterator<T, ref, ptr> &other) {
  deque_iterator<T, ref, ptr> tmp = other;
  tmp += N;
  return tmp;
}

template<class T, typename ref, typename ptr>
deque_iterator<T, ref, ptr> operator-(const deque_iterator<T, ref, ptr> &other, int N) {
  deque_iterator<T, ref, ptr> tmp = other;
  tmp -= N;
  return tmp;
}

template<class T, typename ref, typename ptr>
deque_iterator<T, ref, ptr> operator-(int N, const deque_iterator<T, ref, ptr> &other) {
  deque_iterator<T, ref, ptr> tmp = other;
  tmp -= N;
  return tmp;
}

template<class T, typename ref, typename ptr, class T2, typename ref2, typename ptr2>
int operator-(const deque_iterator<T, ref, ptr> &first, const deque_iterator<T2, ref2, ptr2> &second) {
  return (first.node - second.node) * SIZE + (first.current - first.first) - (second.current - second.first);
}

//----------------------------------------------------------------------------------------------------------------------

template<class T>
class Deque {
 public:
  typedef deque_iterator<T, T &, T *> iterator;
  typedef deque_iterator<T, const T &, const T *> const_iterator;
 private:
  iterator start;
  iterator finish;
  T **map;
  int capacity;

  void swap(Deque<T> &other);

 public:
  Deque();
  Deque(const Deque<T> &other);
  Deque(int N) : capacity{} { create_map_and_nodes(N); }
  Deque(int N, const T &value) : capacity{} { fill_initialize(N, value); }

  void fill_initialize(int N, const T &value);
  void create_map_and_nodes(int N);
  void destroy_nodes(int N);

  Deque<T> &operator=(Deque<T> other);

  T &operator[](int N) { return start[N]; }
  const T &operator[](int N) const { return start[N]; }

  T &at(int N);
  const T &at(int N) const;

  void push_back(const T &value);
  void push_front(const T &value);

  void push_back_aux();
  void push_front_aux(const T &value);

  void pop_back();
  void pop_front();

  iterator begin() { return start; }
  iterator end() { return finish; }

  const_iterator begin() const { return start; }
  const_iterator end() const { return finish; }

  const_iterator cbegin() const { return start; }
  const_iterator cend() const { return finish; }

  size_t size() const { return finish - start; }

  void insert(const iterator &pos, const T &value);
  void erase(const iterator &pos);

  ~Deque();
};
//----------------------------------------------------------------------------------------------------------------------
template<class T>
void Deque<T>::fill_initialize(int N, const T &value) {
  create_map_and_nodes(N);

  for (auto cur = start.node; cur < finish.node; ++cur) {
    std::uninitialized_fill(*cur, *cur + SIZE, value);
  }
  std::uninitialized_fill(finish.first, finish.current, value);

}

template<class T>
void Deque<T>::create_map_and_nodes(int num_elements) {
  int num_nodes = num_elements / SIZE + 1;
  capacity = num_nodes * 3;
  map = (T **) malloc(capacity * sizeof(T *));

  T **tmp_start = map + (capacity - num_nodes) / 2;
  T **tmp_finish = tmp_start + num_nodes - 1;

  for (T **current = tmp_start; current <= tmp_finish; current++) {
    *current = (T *) malloc(SIZE * sizeof(T));
  }

  start.set_node(tmp_start);
  start.current = start.first;

  finish.set_node(tmp_finish);
  finish.current = finish.first + num_elements % SIZE;
}

template<class T>
Deque<T>::Deque(const Deque<T> &other) : Deque(other.size()) {
  try {
    for (size_t i = 0; i < other.size(); i++) {
      start[i] = other[i];
    }
  }
  catch (...) {
    throw;
  }
}

template<class T>
Deque<T>::~Deque() {
  for (auto i = start.node; i <= finish.node; i++) {
    free(*i);
  }
  free(map);
}

template<class T>
T &Deque<T>::at(int N) {
  if (N >= static_cast<int>(size()) || N < 0) {
    throw std::out_of_range("out of range");
  }
  return start[N];
}

template<class T>
const T &Deque<T>::at(int N) const {
  if (N >= static_cast<int>(size()) || N < 0) {
    throw std::out_of_range("out of range");
  }
  return start[N];
}

template<class T>
void Deque<T>::swap(Deque<T> &other) {
  std::swap(map, other.map);
  std::swap(start, other.start);
  std::swap(finish, other.finish);
  std::swap(capacity, other.capacity);
}

template<class T>
Deque<T> &Deque<T>::operator=(Deque<T> other) {
  Deque<T> tmp = *this;
  try {
    tmp.swap(other);
  }
  catch (...) {
    throw;
  }
  swap(tmp);
  return *this;
}

template<class T>
void Deque<T>::push_back(const T &value) {
  *finish.current = value;
  if (finish.current < finish.last - 1) {
    finish.current++;
  } else push_back_aux();
}

template<class T>
void Deque<T>::push_back_aux() {
  if (finish.node == map + capacity - 1) {
    T **tmp_map = (T **) malloc(capacity * 3 * sizeof(T *));
    for (int i = 0; i < capacity; i++) {
      std::uninitialized_fill(tmp_map + i + capacity, tmp_map + i + capacity + 1, map[i]);
    }
    std::swap(map, tmp_map);
    free(tmp_map);
    int tmp_length = finish.node - start.node;
    finish.node = map + 2 * capacity - 1;
    start.node = finish.node - tmp_length;
    capacity *= 3;
  }
  finish.node++;
  *finish.node = (T *) malloc(SIZE * sizeof(T));
  finish.first = *finish.node;
  finish.last = finish.first + SIZE;
  finish.current = finish.first;
}

template<class T>
void Deque<T>::push_front(const T &value) {
  if (start.current != start.first) {
    --start.current;
    *start.current = value;
  } else push_front_aux(value);
}

template<class T>
void Deque<T>::push_front_aux(const T &value) {
  if (start.node == map) {
    T **tmp_map = (T **) malloc(capacity * 3 * sizeof(T *));
    for (int i = 0; i < capacity; i++) {
      std::uninitialized_fill(tmp_map + i + capacity, tmp_map + i + capacity + 1, map[i]);
    }
    std::swap(map, tmp_map);
    free(tmp_map);
    int tmp_length = finish.node - start.node;
    start.node = map + capacity;
    finish.node = start.node + tmp_length;
    capacity *= 3;
  }
  start.node--;
  *start.node = (T *) malloc(SIZE * sizeof(T));
  start.first = *start.node;
  start.last = start.first + SIZE;
  start.current = start.last - 1;

  *start.current = value;
}

template<class T>
void Deque<T>::pop_back() {
  if (size() > 0) {
    if (finish.current == finish.first) {
      free(finish.first);
    }
    finish--;
  }
}

template<class T>
void Deque<T>::pop_front() {
  if (size() > 0) {
    if (start.current == start.last - 1) {
      free(start.first);
    }
    start++;
  }
}

template<class T>
void Deque<T>::insert(const Deque::iterator &pos, const T &value) {
  push_back(value);
  int pos_el = pos - start;
  pos_el = std::min(static_cast<int>(size()) - 1, pos_el);
  for (int i = static_cast<int>(size()) - 2; i >= pos_el; i--) {
    start[i + 1] = start[i];
  }
  start[pos_el] = value;
}

template<class T>
void Deque<T>::erase(const Deque::iterator &pos) {
  if (size() <= 0) return;
  int pos_el = pos - start;
  pos_el = std::min(static_cast<int>(size()) - 1, pos_el);
  for (int i = pos_el; i < static_cast<int>(size()) - 1; i++) {
    start[i] = start[i + 1];
  }
  pop_back();
}

template<class T>
Deque<T>::Deque() : capacity{} {
  create_map_and_nodes(1);
  finish = start;
}

#ifndef HASH_MAP_HPP
#define HASH_MAP_HPP
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::hash;
using std::pair;
using std::make_pair;
using std::runtime_error;
using std::exception;

#define INITIAL_SIZE 0
#define INITIAL_CAPACITY 16
#define MIN_CAPACITY 1
#define UPPER_LOAD_FACTOR 0.75
#define LOWER_LOAD_FACTOR 0.25

#define PAIR_T pair<KeyT, ValueT>
#define INVALID_KEY_MSG "Key not found"
#define INVALID_VECTOR_LENGTHS_ERR_MSG "Error: The length of the " \
                                          "received vectors is not equal"

template<typename KeyT, typename ValueT>
class HashMap {
 protected:
  /**
  * get the hash index for given key
  * @param key: key to check
  */
  int get_hash_index (KeyT key) const;

  /**
  * resize the hash map's capacity
  * @param increase: should the map capacity be increased or decreased
  */
  void handle_resize (bool increase);

  /**
  * creates a new hash map and passes all the elements from the old map
  * @param old_size: the capacity of the map before the resize
  */
  void resize_map (int old_size);

  /**
  * go over the previous map and move the elements to the current map.
  * @param old_map: pointer to the previous map
  * @param old_size: the capacity of the map before the resize
  */
  bool move_elements_from (vector<PAIR_T > *old_map, int old_size);

  /**
  * receives an index and returns the next non-empty index in the map.
  * @param curr_index: the index to start searching from
  */
  int get_next_index_with_elements (int curr_index) const;

  hash<KeyT> hash_func;
  vector<PAIR_T > *_map;
  int _size;
  int _capacity;
 public:
  /**
  * constructor
  */
  HashMap ();

  /**
  * constructor which receives 2 vectors, one with keys and one with values.
  * @param keys: keys of the hash map
  * @param values: values of the hash map
  */
  HashMap (vector<KeyT> keys, vector<ValueT> values);

  /**
  * copy constructor.
  * @param hm: the hash map to copy from
  */
  HashMap (const HashMap<KeyT, ValueT> &hm);

  /**
  * HashMap destructor
  */
  virtual ~HashMap ();

  /**
  * @return number of elements in the HashMap.
  */
  int size () const;

  /**
  * @return capacity of the HashMap.
  */
  int capacity () const;

  /**
  * @return true if there are no elements in the Hashmap, false otherwise.
  */
  bool empty () const;

  /**
  * inserts a new pair of key and value to the map.
  * @param keys: key to insert
  * @param values: value to insert
  * @return true if the insertion was successful, false otherwise.
  */
  bool insert (KeyT key, ValueT value);

  /**
  * checks if the HashMap contains the given key.
  * @param keys: key to check
  * @return true if the key was found. false otherwise
  */
  bool contains_key (KeyT key) const;

  /**
  * find the value of the matching key.
  * @param keys: key to find the matching value
  * @return value in the index of the key. Throws otherwise
  */
  ValueT &at (KeyT key) const;

  /**
  * removes an element with given key from the HashMap.
  * @param key: key of the element to remove.
  * @return true if the key was removed. false otherwise
  */
  virtual bool erase (KeyT key);

  /**
  * @return the load factor of the HashMap (size / capacity)
  */
  double get_load_factor () const;

  /**
  * @return the size of the bucket of the given key. Throws if key not found.
  */
  int bucket_size (KeyT key) const;

  /**
  * @return the index of the bucket with the given key.
  *         Throws if key not found.
  */
  int bucket_index (KeyT key) const;

  /**
  * removes all elements from the HashMap
  */
  void clear ();

  /**
  * Receives a HashMap and assigns it to this HashMap.
  * @param hm a HashMap.
  * @return reference to the assigned HashMap.
  */
  HashMap &operator= (const HashMap &hm);

  /**
  * Receives key and returns the requested value in the HashMap.
  * Creates a key with default value if it doesn't exist.
  * @param key key to find the matching value
  * @return reference to the value in the map
  */
  ValueT &operator[] (KeyT key);

  /**
  * Receives key and returns the requested value in the HashMap.
  * @param key key to find the matching value
  * @return reference to the value in the map
  */
  const ValueT &operator[] (KeyT key) const;

  /**
  * Receives HashMap and checks if it's identical to
  * this map (keys and values).
  * @param rhs: right hand side
  * @return true if maps are equal, false otherwise
  */
  bool operator== (const HashMap &rhs) const;

  /**
  * Receives HashMap and checks if it's different from
  * this map (keys and values).
  * @param rhs: right hand side
  * @return true if maps are not equal, false otherwise
  */
  bool operator!= (const HashMap &rhs) const;

  class ConstIterator {
   private:
    const HashMap *hash_map;
    int map_index;
    int element_index;
   public:
    // Iterator traits
    typedef PAIR_T value_type;
    typedef const PAIR_T &reference;
    typedef const PAIR_T *pointer;
    typedef int difference_type;
    typedef std::forward_iterator_tag iterator_category;

    /**
    * ConstIterator default constructor
    * @param hash_map: the map to iterate
    * @param map_index: right hand side
    * @param rhs: right hand side
    * @return true if maps are not equal, false otherwise
    */
    ConstIterator (const HashMap *hash_map,
                   int map_index,
                   int element_index)
        : hash_map (hash_map),
          map_index (map_index),
          element_index (element_index)
    {}

    /**
    * ConstIterator move one element forward
    */
    ConstIterator *operator++ ()
    {
      vector<PAIR_T > cur_element = hash_map->_map[map_index];
      if ((element_index + 1) >= (int) hash_map->_map[map_index].size ())
        {
          element_index = 0;
          map_index =
              hash_map->get_next_index_with_elements (map_index);
        }
      else
        {
          element_index++;
        }

      return this;
    }

    /**
    * ConstIterator move one element forward
    */
    ConstIterator operator++ (int)
    {
      ConstIterator it (*this);
      this->operator++ ();
      return it;
    }

    /**
    * ConstIterator check if equal to other iterator
    */
    bool operator== (const ConstIterator &rhs) const
    {
      return (hash_map == rhs.hash_map)
             && (element_index == rhs.element_index)
             && (map_index == rhs.map_index);
    }

    /**
    * ConstIterator check if not equal to other iterator
    */
    bool operator!= (const ConstIterator &rhs) const
    {
      return !(operator== (rhs));
    }

    /**
    * ConstIterator return reference to element
    */
    reference operator* () const
    {
      return hash_map->_map[map_index][element_index];
    }

    /**
    * ConstIterator return pointer to element
    */
    pointer operator-> () const
    {
      return &(operator* ());
    }
  };

  using const_iterator = ConstIterator;

  const_iterator cbegin () const
  {
    int map_index = get_next_index_with_elements (0);
    return ConstIterator (this, map_index, 0);
  }
  const_iterator cend () const
  {
    return ConstIterator (this, _capacity, 0);
  }
  const_iterator begin () const
  {
    return cbegin ();
  }
  const_iterator end () const
  {
    return cend ();
  }
};

template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::HashMap ()
{
  _size = INITIAL_SIZE;
  _capacity = INITIAL_CAPACITY;
  _map = new vector<PAIR_T >[_capacity];
}

template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::HashMap (vector<KeyT> keys, vector<ValueT> values)
{
  _size = INITIAL_SIZE;
  _capacity = INITIAL_CAPACITY;
  _map = new vector<PAIR_T >[_capacity];

  if (keys.size () != values.size ())
    {
      delete[] _map;
      throw runtime_error (INVALID_VECTOR_LENGTHS_ERR_MSG);
    }
  for (int i = 0; i < (int) keys.size (); ++i)
    {
      int index = get_hash_index (keys[i]);
      for (auto &element: _map[index])
        {
          if (keys[i] == element.first)
            {
              if (values[i] != element.second)
                {
                  element.second = values[i];
                }
              continue;
            }
        }
      insert (keys[i], values[i]);
    }
}

template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::HashMap (const HashMap &hm)
{
  _size = INITIAL_SIZE;
  _capacity = hm._capacity;
  _map = new vector<PAIR_T >[_capacity];

  for (const auto &element: hm)
    {
      insert (element.first, element.second);
    }
}

template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT>::~HashMap ()
{
  delete[] _map;
}

template<typename KeyT, typename ValueT>
int HashMap<KeyT, ValueT>::size () const
{
  return _size;
}

template<typename KeyT, typename ValueT>
int HashMap<KeyT, ValueT>::capacity () const
{
  return _capacity;
}

template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::empty () const
{
  return (_size == INITIAL_SIZE);
}

template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::insert (KeyT key, ValueT value)
{
  int index = get_hash_index (key);
  for (auto &element: _map[index])
    {
      if (key == element.first)
        {
          return false;
        }
    }

  PAIR_T element_to_insert = make_pair (key, value);
  _map[index].push_back (element_to_insert);
  _size++;
  handle_resize (true);

  return true;
}

template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::contains_key (KeyT key) const
{
  int index = get_hash_index (key);
  for (const auto &element: _map[index])
    {
      if (key == element.first)
        {
          return true;
        }
    }
  return false;
}

template<typename KeyT, typename ValueT>
ValueT &HashMap<KeyT, ValueT>::at (KeyT key) const
{
  int index = get_hash_index (key);
  for (auto &element: _map[index])
    {
      if (key == element.first)
        {
          return element.second;
        }
    }
  throw runtime_error (INVALID_KEY_MSG);
}

template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::erase (KeyT key)
{
  int index = get_hash_index (key);
  for (int j = 0; j < (int) _map[index].size (); ++j)
    {
      if (key == _map[index][j].first)
        {
          _map[index].erase (_map[index].begin () + j);
          _size--;
          handle_resize (false);
          return true;
        }
    }

  return false;
}

template<typename KeyT, typename ValueT>
double HashMap<KeyT, ValueT>::get_load_factor () const
{
  return ((double) _size / _capacity);
}

template<typename KeyT, typename ValueT>
int HashMap<KeyT, ValueT>::bucket_size (KeyT key) const
{
  at (key);
  int index = get_hash_index (key);
  return (int) _map[index].size ();
}

template<typename KeyT, typename ValueT>
int HashMap<KeyT, ValueT>::bucket_index (KeyT key) const
{
  at (key);
  return get_hash_index (key);
}

template<typename KeyT, typename ValueT>
void HashMap<KeyT, ValueT>::clear ()
{
  delete[] _map;
  _size = INITIAL_SIZE;
  _map = new vector<PAIR_T >[_capacity];
}

template<typename KeyT, typename ValueT>
HashMap<KeyT, ValueT> &HashMap<KeyT, ValueT>::operator= (const HashMap &hm)
{
  if (this == &hm)
    {
      return *this;
    }
  clear ();
  for (auto &element: hm)
    {
      insert (element.first, element.second);
    }
  return *this;
}

template<typename KeyT, typename ValueT>
ValueT &HashMap<KeyT, ValueT>::operator[] (KeyT key)
{
  try
    {
      ValueT &element = at (key);
      return element;
    }
  catch (const exception &)
    {
      insert (key, ValueT ());
      ValueT &element = at (key);
      return element;
    }
}

template<typename KeyT, typename ValueT>
const ValueT &HashMap<KeyT, ValueT>::operator[] (KeyT key) const
{
  try
    {
      ValueT &element = at (key);
      return element;
    }
  catch (const exception &)
    {
      return nullptr;
    }
}

template<typename KeyT, typename ValueT>
bool
HashMap<KeyT, ValueT>::operator== (const HashMap<KeyT, ValueT> &rhs) const
{
  try
    {
      if (_size != rhs._size)
        {
          return false;
        }
      for (auto &elem: *this)
        {
          if (rhs.at (elem.first) != elem.second)
            {
              return false;
            }
        }
      return true;
    }
  catch (const exception &)
    {
      return false;
    }
}

template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::operator!= (const HashMap &rhs) const
{
  return !(operator== (rhs));
}

// protected HashMap members
template<typename KeyT, typename ValueT>
int HashMap<KeyT, ValueT>::get_hash_index (KeyT key) const
{
  return (int) (hash_func (key) & (_capacity - 1));
}

template<typename KeyT, typename ValueT>
void HashMap<KeyT, ValueT>::handle_resize (bool increase)
{
  int old_size;
  while ((get_load_factor () > UPPER_LOAD_FACTOR) && increase)
    {
      old_size = _capacity;
      _capacity *= 2;
      resize_map (old_size);
    }

  while ((get_load_factor () < LOWER_LOAD_FACTOR) && !increase
         && _capacity > MIN_CAPACITY)
    {
      old_size = _capacity;
      _capacity /= 2;
      resize_map (old_size);
    }
}

template<typename KeyT, typename ValueT>
void HashMap<KeyT, ValueT>::resize_map (int old_size)
{
  auto *old_map = _map;
  _map = new vector<PAIR_T >[_capacity];
  move_elements_from (old_map, old_size);

  delete[] old_map;
}

template<typename KeyT, typename ValueT>
bool HashMap<KeyT, ValueT>::move_elements_from (vector<PAIR_T > *old_map,
                                                int old_size)
{
  for (int i = 0; i < old_size; ++i)
    {
      for (const auto &element: old_map[i])
        {
          int index = get_hash_index (element.first);
          _map[index].push_back (element);
        }
    }
  return true;
}

template<typename KeyT, typename ValueT>
int HashMap<KeyT, ValueT>::get_next_index_with_elements (int curr_index) const
{
  for (int i = curr_index + 1; i < _capacity; ++i)
    {
      if (!_map[i].empty ())
        {
          return i;
        }
    }
  return _capacity;
}

#endif

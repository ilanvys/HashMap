#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include "HashMap.hpp"

using std::invalid_argument;
using std::exception;

#define INVALID_KEY "invalid key"

class InvalidKey : public invalid_argument {
 protected:
  string _err_msg;
 public:
  /**
  * constructor
  */
  InvalidKey ()
      : invalid_argument (INVALID_KEY), _err_msg (INVALID_KEY)
  {}

  /**
  * constructor that receives a pointer to char as the error message.
  */
  explicit InvalidKey (const char *msg)
      : invalid_argument (msg), _err_msg (msg)
  {}

  /**
  * constructor that receives a reference to string as the error message.
  */
  explicit InvalidKey (const string &msg)
      : invalid_argument (msg), _err_msg (msg)
  {}

  /**
  * @return the error message.
  */
  const char *what () const noexcept override
  {
    return _err_msg.c_str ();
  }
};

class Dictionary : public HashMap<string, string> {
 public:
  /**
  * constructor
  */
  Dictionary () = default;

  /**
  * constructor which receives 2 vectors, one with keys and one with values.
  * @param keys: keys of the dictionary
  * @param values: values of the dictionary
  */
  Dictionary (const vector<string> &keys, const vector<string> &values)
      : HashMap (keys, values)
  {};

  /**
  * copy constructor.
  * @param hm: the dictionary to copy from
  */
  Dictionary (const HashMap<string, string> &hm)
      : HashMap (hm)
  {};

  /**
  * removes an element with given key from the Dictionary.
  * @param key: key of the element to remove.
  * @return true if the key was removed. Throws an error otherwise
  */
  bool erase (string key) override;

  /**
  * receives an iterator and adds all it's elements to the Dictionary.
  * @param it: iterator to start from.
  * @param it_end: iterator end.
  */
  template<typename Iterator>
  void update (Iterator it, Iterator it_end);
};

bool Dictionary::erase (string key)
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
  throw InvalidKey (INVALID_KEY);
}

template<typename Iterator>
void Dictionary::update (Iterator it, Iterator it_end)
{
  while (it != it_end)
    {
      int index = get_hash_index (it->first);
      for (auto &element: _map[index])
        {
          if (it->first == element.first)
            {
              if (it->second != element.second)
                {
                  element.second = it->second;
                }
              continue;
            }
        }
      insert (it->first, it->second);
      it++;
    }
}

#endif

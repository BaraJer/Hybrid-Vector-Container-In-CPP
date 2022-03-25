//
// Created by Bara on 1/4/2022.
//
#include <vector>
#include <algorithm>
#include <cmath>
#ifndef _VL_VECTOR_H_
#define _VL_VECTOR_H_
#define INIT_CAP 16UL
#define OFR "ERROR: index out of range\n"
#define CAP_C(x, y) std::floor((3*(x+y))/2)
template<class T, size_t StackCapacity = INIT_CAP>
class vl_vector {
 protected:
  size_t stack_cap{}; // given const memory size
  size_t curr_cap{}; // current capacity available
  size_t curr_size{}; // current size of the vector
  bool is_dynamic{}; // if the vector is using heap
  T *v_heap; // heap memory array
  T v_stack[StackCapacity]{}; // stack memory array
 public:
//----------------------- typedefs -----------------------

  typedef T *iterator;
  typedef const T *const_iterator;
  typedef std::reverse_iterator<T *> reverse_iterator;
  typedef std::reverse_iterator<const T *> const_reverse_iterator;

//----------------------- Constructors -----------------------
/**
 * default constructor
 */
  vl_vector ()
      : stack_cap (StackCapacity), curr_cap (stack_cap), curr_size (0),
        is_dynamic (false), v_heap (nullptr)
  {}
/**
 * receives a value (member) and a count, initializes a vector the
 * given value stored count times
 * @param count
 * @param member
 */
  vl_vector (size_t count, T member) : stack_cap (StackCapacity)
  {
    if (count > stack_cap)
      {
        is_dynamic = true;
        curr_cap = CAP_C(0, count);
        v_heap = new T[curr_cap];
        for (size_t i = 0; i < count; ++i)
          {
            v_heap[i] = member;
          }
      }
    else
      {
        is_dynamic = false;
        v_heap = nullptr;
        for (size_t i = 0; i < count; ++i)
          {
            v_stack[i] = member;
          }
        curr_cap = stack_cap;
      }
    curr_size = count;
  }
/**
 * initials a vector of a given iterators data
 * @tparam InputIterator
 * @param first
 * @param last
 */
  template<class InputIterator>
  vl_vector (InputIterator first, InputIterator last):
      stack_cap (StackCapacity)
  {
    size_t given_data_size = std::distance (first, last);
    if (given_data_size > stack_cap)
      {
        is_dynamic = true;
        curr_cap = CAP_C(0, given_data_size);
        v_heap = new T[curr_cap];
        std::copy (first, last, v_heap);

      }
    else
      {
        is_dynamic = false;
        v_heap = nullptr;
        std::copy (first, last, v_stack);
        curr_cap = stack_cap;
      }
    curr_size = given_data_size;
  }
  /**
   * assign copy constructor
   * @param rhs
   * @return
   */
  vl_vector<T, StackCapacity> &operator=
      (const vl_vector<T, StackCapacity> &rhs)
  {
    if (this != &rhs)
      {

        if (rhs.empty ())
          {
            this->clear ();
            return *this;
          }
        if (is_dynamic)
          {
            delete[] v_heap;
            v_heap = nullptr;
          }
        stack_cap = rhs.stack_cap;
        curr_cap = rhs.curr_cap;
        curr_size = rhs.curr_size;
        is_dynamic = rhs.is_dynamic;

        if (is_dynamic)
          {
            v_heap = new T[curr_cap];
            std::copy (rhs.v_heap, rhs.v_heap + curr_size, v_heap);
          }
        else
          {
            std::copy (rhs.v_stack, rhs.v_stack + curr_size, v_stack);
            v_heap = nullptr;
          }
      }
    return *this;
  }
  /**
   * copy Ctor
   * @param rhs
   */
  vl_vector (const vl_vector<T, StackCapacity> &rhs)
  {

    stack_cap = rhs.stack_cap;
    curr_cap = rhs.curr_cap;
    curr_size = rhs.curr_size;
    is_dynamic = rhs.is_dynamic;

    if (is_dynamic)
      {
        v_heap = new T[curr_cap];
        std::copy (rhs.v_heap, rhs.v_heap + curr_size, v_heap);
      }
    else
      {
        std::copy (rhs.v_stack, rhs.v_stack + curr_size, v_stack);
        v_heap = nullptr;
      }

  }
/**
 * frees the dynamic allocated memory
 */
  virtual ~vl_vector () noexcept
  {
    delete[] v_heap;
  }

//----------------------- Methods -----------------------

/**
 * returns the current size of the vector
 * @return
 */
  virtual size_t size () const noexcept
  {
    return curr_size;
  }

  virtual size_t capacity () const noexcept
  {
    return curr_cap;
  }
/**
 * returns true if the vector is empty and false otherwise
 * @return
 */
  virtual bool empty () const noexcept
  {
    return curr_size == 0;
  }

  /**
   * returns the value at the given index
   * @param idx
   * @return
   */
  virtual T at (const size_t &idx) const
  {
    if (idx >= curr_size)
      {
        this->~vl_vector();
        std::exit (0);
      }
    return is_dynamic ? v_heap[idx] : v_stack[idx];
  }
  virtual T &at (const size_t &idx)
  {
    if (idx >= curr_size)
      {
//        this->~vl_vector();
        throw std::exception();
      }
    return is_dynamic ? v_heap[idx] : v_stack[idx];
  }
/**
 * checks if the vector needs a capacity increase
 * and if memory allocation is needed
 * @param k
 */
  virtual void increase_cap_check (const size_t &k)
  {

    if (curr_size + k <= curr_cap)
      {
        return;
      }

    size_t new_cap = (3 * (curr_size + k)) / 2;
    T *temp = new (std::nothrow) T[new_cap];
    if (!temp)
      {
        return;
      }

    T *v_curr = is_dynamic ? v_heap : v_stack;
    std::copy (v_curr, v_curr + curr_size, temp);
    if (is_dynamic)
      {
        delete[] v_heap;
      }
    v_heap = temp;
    curr_cap = new_cap;
    is_dynamic = true;

  }
/**
 * pushes a new member to the head of the vector
 * @param member
 */
  virtual void push_back (const T member) noexcept
  {
    increase_cap_check (1);
    is_dynamic ?
        v_heap[curr_size] = member :
        v_stack[curr_size] = member;
    curr_size += 1;

  }

/**
 * adds the given element to the vector before the element that the given
 * iterator points at
 * @param position iterator points to a member in the vector
 * @param member new member to add to the vector
 * @return
 */
  virtual iterator insert (iterator position, const T member)
  {
    if (position == nullptr)
      {
//        this->~vl_vector ();
//        std::exit (0);;
        throw std::exception();
      }
    T *v_curr = is_dynamic ? v_heap : v_stack;
    size_t pos_idx = std::distance (v_curr, position);
    if (curr_size + 1 > curr_cap)
      {
        curr_cap = CAP_C(curr_size, 1);
        T *temp = new T[curr_cap];
        std::copy (v_curr, v_curr + pos_idx, temp);
        std::copy (
            v_curr + pos_idx, v_curr + curr_size, temp + pos_idx + 1);
        temp[pos_idx] = member;
        if (is_dynamic)
          {
            delete[] v_heap;
          }
        v_heap = temp;
        is_dynamic = true;
      }
    else
      {
        T *temp = new T[curr_size];
        std::copy (v_curr, v_curr + curr_size, temp);
        std::copy (temp, temp + pos_idx, v_curr);
        std::copy (temp + pos_idx, temp + curr_size, v_curr + pos_idx + 1);
        v_curr[pos_idx] = member;
        delete[] temp;
      }
    curr_size += 1;
    return is_dynamic ? &v_heap[pos_idx] : &v_stack[pos_idx];
  }
/**

 * adds the given elements to the vector before the element that the given
 * iterator points at
 * @tparam InputIterator given iterator type
 * @param position given element iterator pointer
 * @param first first iterator
 * @param last last iterator
 * @return
 */

  template<class InputIterator>
  iterator insert (iterator position,
                   const InputIterator &first,
                   const InputIterator &last)
  {
    if (position == nullptr)
      {
        return nullptr;
      }
    T *v_curr = is_dynamic ? v_heap : v_stack;
    size_t pos_idx = std::distance (v_curr, position);
    size_t data_size = std::distance (first, last);
    if (curr_size + data_size > curr_cap)
      {
        curr_cap = CAP_C(curr_size, data_size);
        T *temp = new T[curr_cap];
        std::copy (v_curr, v_curr + pos_idx, temp);
        std::copy (
            v_curr + pos_idx,
            v_curr + curr_size, temp + pos_idx + data_size);
        std::copy (first, last, temp + pos_idx);
        if (is_dynamic)
          {
            delete[] v_heap;
          }
        v_heap = temp;
        is_dynamic = true;
      }
    else
      {
        T *temp = new T[curr_size];
        std::copy (v_curr, v_curr + curr_size, temp);
        std::copy (temp, temp + pos_idx, v_curr);
        std::copy (
            temp + pos_idx, temp + curr_size, v_curr + pos_idx + data_size);
        std::copy (first, last, v_curr+pos_idx);
        delete[] temp;
      }
    curr_size += data_size;
    return is_dynamic ? &v_heap[pos_idx] : &v_stack[pos_idx];
  }
/**
 * removes the last member of the vector
 */
  virtual void pop_back ()
  {
    if (!curr_size)
      {
        return;
      }

    if ((is_dynamic) && (curr_size - 1 <= stack_cap))
      {
        std::copy (v_heap, v_heap + curr_size - 1, v_stack);
        delete[] v_heap;
        v_heap = nullptr;
        curr_cap = stack_cap;
        is_dynamic = false;
      }

    curr_size -= 1;

  }
/**
 * recieves an iterator that points to an element in the vector and
 * erases it
 * @param position
 * @return a pointer to the same position was pointed at by the given
 * iterator
 */
  virtual iterator erase (iterator position)
  {
    if (position == nullptr || position == end ())
      {
        return nullptr;
      }

    size_t pos_idx;
    if ((is_dynamic) && (curr_size - 1 <= stack_cap))
      {
        pos_idx = std::distance (v_heap, position);
        std::copy (v_heap, v_heap + pos_idx, v_stack);
        std::copy (v_heap + pos_idx + 1, v_heap + curr_size,
                   v_stack + pos_idx);
        delete[] v_heap;
        v_heap = nullptr;
        curr_cap = stack_cap;
        is_dynamic = false;
      }
    else if (is_dynamic)
      {
        pos_idx = std::distance (v_heap, position);
        std::copy (
            v_heap + pos_idx + 1, v_heap + curr_size, v_heap + pos_idx);
      }
    else
      {
        pos_idx = std::distance (v_stack, position);
        std::copy (
            v_stack + pos_idx + 1, v_stack + curr_size, v_stack + pos_idx);
      }
    curr_size -= 1;
    return is_dynamic ? &v_heap[pos_idx] : &v_stack[pos_idx];
  }
/**
 * receives two iterator pointing at two elements at the vector s.t
 * first <= last, erases the field [first,last)
 * @param first
 * @param last
 * @return
 */
  virtual iterator erase (iterator first, iterator last)
  {

    size_t given_data_l = std::distance (first, last);
    if (given_data_l > curr_size)
      {
        return nullptr;
      }
    size_t first_idx;
    if ((is_dynamic) && (curr_size - given_data_l <= stack_cap))
      {
        first_idx = std::distance (v_heap, first);
        std::copy (v_heap, v_heap + first_idx, v_stack);
        std::copy (last, v_heap + curr_size, v_stack + first_idx);
        delete[] v_heap;
        v_heap = nullptr;
        is_dynamic = false;
        curr_cap = stack_cap;
      }
    else if (is_dynamic)
      {
        first_idx = std::distance (v_heap, first);
        size_t last_idx = std::distance (v_heap, last);
        std::copy (
            v_heap + last_idx, v_heap + curr_size, v_heap + first_idx);
      }
    else
      {
        first_idx = std::distance (v_stack, first);
        size_t last_idx = std::distance (v_stack, last);
        std::copy (
            v_stack + last_idx, v_stack + curr_size, v_stack + first_idx);
      }
    curr_size -= given_data_l;
    return is_dynamic ? &v_heap[first_idx] : &v_stack[first_idx];
  }

/**
 * deletes the whole data of the vector
 */
  virtual void clear ()
  {
    if (is_dynamic && v_heap != nullptr)
      {
        delete[] v_heap;
        v_heap = nullptr;
        is_dynamic = false;
      }
    curr_size = 0;
    curr_cap = stack_cap;
  }
/**
 * returns a points of where is the data of the vector starts
 * @return
 */
  virtual T *data () noexcept
  {
    return is_dynamic ? v_heap : v_stack;
  }

  virtual bool contains (const T &member) noexcept
  {
    T *check_v = is_dynamic ? v_heap : v_stack;
    for (size_t i = 0; i < curr_size; ++i)
      {
        if (check_v[i] == member)
          {
            return true;
          }
      }
    return false;
  }
//----------------------- Operators -----------------------
/**
 * returns the value in the given index
 * @param idx
 * @return
 */
  T operator[] (size_t idx) const noexcept
  {
    return is_dynamic ? v_heap[idx] : v_stack[idx];
  }
  T &operator[] (size_t idx) noexcept
  {
    return is_dynamic ? v_heap[idx] : v_stack[idx];
  }
/**
 *
 * @param rhs
 * @return true if the given vector is identical the this false otherwise
 */
  bool operator== (const vl_vector<T, StackCapacity> &rhs) noexcept
  {
    if (curr_size != rhs.curr_size || is_dynamic != rhs.is_dynamic)
      {
        return false;
      }
    const T *v_curr = is_dynamic ? v_heap : v_stack;
    for (size_t i = 0; i < curr_size; ++i)
      {
        if (v_curr[i] != rhs[i])
          {
            return false;
          }
      }
    return true;

  }
/**
 *
 * @param rhs
 * @return false if the given vector is identical the this true otherwise
 */
  bool operator!= (const vl_vector<T, StackCapacity> &rhs) noexcept
  {
    bool res = operator== (rhs);
    return !res;
  }
//----------------------- Iterators -----------------------

  virtual iterator begin () noexcept
  {
    return is_dynamic ? v_heap : v_stack;
  }
  virtual const_iterator cbegin () const noexcept
  {
    return is_dynamic ? v_heap : v_stack;
  }
  virtual const_iterator begin () const noexcept
  {
    return is_dynamic ? v_heap : v_stack;
  }
//----------------------- Const Iterators -----------------------
  virtual iterator end () noexcept
  {
    return is_dynamic ? v_heap + curr_size : v_stack + curr_size;
  }

  virtual const_iterator cend () const noexcept
  {
    return is_dynamic ? v_heap + curr_size : v_stack + curr_size;
  }
  virtual const_iterator end () const noexcept
  {
    return is_dynamic ? v_heap + curr_size : v_stack + curr_size;
  }
//----------------------- Reverse Iterators -----------------------
  virtual reverse_iterator rbegin () noexcept
  {
    return reverse_iterator (end ());
  }

  virtual const_reverse_iterator crbegin () const noexcept
  {
    return const_reverse_iterator (end ());
  }

  virtual const_reverse_iterator rbegin () const noexcept
  {
    return const_reverse_iterator (end ());

  }

  virtual reverse_iterator rend () noexcept
  {
    return reverse_iterator (begin ());
  }

  virtual const_reverse_iterator crend () const noexcept
  {
    return const_reverse_iterator (begin ());
  }

  virtual const_reverse_iterator rend () const noexcept
  {
    return const_reverse_iterator (begin ());
  }
};

#endif //_VL_VECTOR_H_

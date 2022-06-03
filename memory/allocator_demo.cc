#include "zy_allocator.h"
#include <iostream>
#include <cstdlib>
#include <cstdlib>
#include <list>
using namespace std;
using namespace zy;

class TestAllocator : public Allocator
{
 private:
  const int ALLOC_HEAD_SIZE = sizeof(size_t);
  const int READ_ALLOC_SIZE_OFFSET = -(ALLOC_HEAD_SIZE);

 public:
  char* alloc(std::size_t size)
  {
    size_t _alloc_size = size + ALLOC_HEAD_SIZE;
    void *ptr = ::malloc(_alloc_size);
    *static_cast<size_t*>(ptr) = _alloc_size;
    return static_cast<char*>(ptr) + ALLOC_HEAD_SIZE;
  }

  char* realloc(void *ptr, size_t size)
  {
    if (ptr != nullptr)
    {
      ptr = static_cast<char*>(ptr) + READ_ALLOC_SIZE_OFFSET;
    }

    size_t _realloc_size = size + ALLOC_HEAD_SIZE;
    ptr = ::realloc(ptr, _realloc_size);
    *static_cast<size_t*>(ptr) = _realloc_size;
    return static_cast<char*>(ptr) + ALLOC_HEAD_SIZE;
  }

  void free(void *ptr)
  {
    ::free(static_cast<char*>(ptr) + READ_ALLOC_SIZE_OFFSET);
  }

};

TestAllocator *g_allocator = new TestAllocator();


class Test_ref
{
 public:
  std::list<char> &m_lst_ref;
  Test_ref(std::list<char> &lst_ref)
    : m_lst_ref(lst_ref)
  {
   }
};

class Test
{
 private:
  std::list<char> m_lst;
  std::list<char> *m_lst_ptr;
  std::list<char> &m_lst_ref;
  std::list<char> *m_lst_ptr_alloc;
  Test_ref *m_test_ref_ptr;
 public:
  Test()
    : m_lst_ptr(&m_lst),
      m_lst_ref(m_lst),
      m_lst_ptr_alloc(g_new<std::list<char> >(g_allocator)),
      m_test_ref_ptr(g_new<Test_ref>(g_allocator, m_lst))
  {

    cout
      << "Test::Test, "
      << (void*)(&m_lst)
      << " "
      << (void*)(m_lst_ptr)
      << " "
      << (void*)(&m_lst_ref)
      << " "
      << (void*)(&(m_test_ref_ptr->m_lst_ref))
      << endl;
  }

  Test(int a)
    : m_lst_ptr(&m_lst),
      m_lst_ref(m_lst)

  {
    cout << "Test::Test(int)"
         << (void*)(&m_lst)
         << " "
         << (void*)(m_lst_ptr)
         << (void*)(&m_lst_ref)
         << endl;
  }

  Test(int a, int b)
    : m_lst_ptr(&m_lst),
      m_lst_ref(m_lst)
  {
    cout << "Test::Test(int,int)"
         << (void*)(&m_lst)
         << " "
         << (void*)(m_lst_ptr)
         << (void*)(&m_lst_ref)
         << endl;
  }

  Test(Allocator *a, int b)
    : m_lst_ptr(&m_lst),
      m_lst_ref(m_lst)
  {
    cout << "Test::Test(Allocator*,int)"
         << (void*)(&m_lst)
         << " "
         << (void*)(m_lst_ptr)
         << (void*)(&m_lst_ref)
         << endl;
  }

  ~Test()
  {
    cout << "Test::~Test()" << endl;
  }
};


template <typename T>
class Ref_test
{
 public:
  T &m_ref;
  Ref_test(T &ref)
    :m_ref(ref)
  {
    cout << "m_ref " << (void*)(&m_ref) << " value " << m_ref << endl;
  }
};

int main()
{
  Test *t2 = g_new<Test, 10>(g_allocator);
  g_delete(t2);

  int ref = 1;
  cout << "ref " << (void*)(&ref) << endl;
  Ref_test<int> *_ref = g_new<Ref_test<int>, 3>(g_allocator, ref);
  g_delete(_ref);
  return 0;
}

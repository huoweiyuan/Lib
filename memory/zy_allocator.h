#ifndef _ZY_ALLOCATOR_
#define _ZY_ALLOCATOR_

#include <cstddef>
#include <utility>

namespace zy
{
class Allocator
{
 private:
   static const short OBJ_NUM_HEAD_SIZE = sizeof(size_t);
   static const short OBJ_PTR_HEAD_SIZE = sizeof(void*);
   static const short READ_OBJ_NUM_OFFSET = -(OBJ_NUM_HEAD_SIZE) -(OBJ_PTR_HEAD_SIZE);
   static const short READ_OBJ_PTR_OFFSET = -(OBJ_PTR_HEAD_SIZE);

 private:
   virtual char* alloc_memory(std::size_t size) = 0;
   virtual char* realloc_memory(void *ptr, size_t size) = 0;
   virtual void free_memory(void *ptr) = 0;

 public:
   // |obj num|obj pointer|xxxx
   template<typename ClassName, size_t Num, typename... P> 
     ClassName* create_obj(P&&... args)
     {
       const size_t _class_size = sizeof(ClassName);
       char *_ptr = 
         alloc_memory(_class_size * Num + OBJ_NUM_HEAD_SIZE + OBJ_PTR_HEAD_SIZE);
       if (_ptr == nullptr)
         return nullptr;

       *(reinterpret_cast<size_t*>(_ptr)) = Num;
       _ptr += OBJ_NUM_HEAD_SIZE;
       *(reinterpret_cast<Allocator**>(_ptr)) = this;
       _ptr += OBJ_PTR_HEAD_SIZE;

       ClassName *_construct_ptr = reinterpret_cast<ClassName*>(_ptr);
       for (size_t i = 0; i < Num; i++)
       {
         new(_construct_ptr + i) ClassName(std::forward<P>(args)...);
       }
       return _construct_ptr;
     }

   template<typename ClassName>
     void delete_obj(ClassName *_destruct_ptr)
     {
       if (_destruct_ptr == nullptr)
         return;
       char *_alloced_ptr = 
         reinterpret_cast<char*>(_destruct_ptr) + READ_OBJ_NUM_OFFSET;
       const size_t _obj_num = *(reinterpret_cast<size_t*>(_alloced_ptr));
       for (size_t i = 0; i < _obj_num; i++)
       {
         (_destruct_ptr + i)->~ClassName();
       }
       free_memory(_alloced_ptr);
     }

 public:
   // |obj pointer|xxxx
   void* alloc(size_t size) {
     char* _ptr = alloc_memory(size + OBJ_PTR_HEAD_SIZE);
     if (_ptr == nullptr) 
       return nullptr;
     *(reinterpret_cast<Allocator**>(_ptr)) = this;
     return _ptr + OBJ_PTR_HEAD_SIZE;
   }

   void* realloc(void *ptr, size_t size) {
     if (ptr == nullptr) 
       return nullptr;
     char *_ptr = realloc_memory(static_cast<char*>(ptr) + READ_OBJ_PTR_OFFSET,
                                 size + OBJ_PTR_HEAD_SIZE);
     if (_ptr == nullptr)
       return nullptr;
     return _ptr + OBJ_PTR_HEAD_SIZE;
   }

   void free(void *ptr) {
     if (ptr == nullptr)
       return;
     free_memory(static_cast<char*>(ptr) + READ_OBJ_PTR_OFFSET);
   }

 public:
   Allocator() noexcept = default;
   virtual ~Allocator() noexcept = default;

 public:
   static Allocator* get_allocator_ptr(void *ptr) {
     if (ptr == nullptr)
       return nullptr;
     return *(reinterpret_cast<Allocator**>(
         static_cast<char*>(ptr) + READ_OBJ_PTR_OFFSET));
   }
};

}

namespace zy
{

inline void* g_alloc(Allocator *allocator, size_t size)
{
  if (allocator == nullptr)
    return nullptr;
  return allocator->alloc(size);
}

inline void* g_realloc(void *ptr, size_t size)
{
  Allocator *allocator = nullptr; 
  if (ptr == nullptr || 
    (allocator = Allocator::get_allocator_ptr(ptr)) == nullptr)
    return nullptr;

  return allocator->realloc(ptr, size);
}

inline void g_free(void *ptr)
{
  Allocator *allocator = nullptr; 
  if (ptr == nullptr ||
    (allocator = Allocator::get_allocator_ptr(ptr)) == nullptr)
    return;
  allocator->free(ptr);
}

template<typename ClassName, size_t Num = 1, typename... P>
ClassName* g_new(Allocator *allocator, P&&... args)
{
  if (allocator == nullptr)
    return nullptr;
  ClassName *_ptr = allocator->create_obj<ClassName, Num>(std::forward<P>(args)...);
  return _ptr;
}

template<typename ClassName>
void g_delete(ClassName *ptr)
{
  Allocator *allocator = nullptr;
  if (ptr == nullptr || 
    (allocator = Allocator::get_allocator_ptr(ptr)) == nullptr)
    return;
  allocator->delete_obj(ptr);
}

}

#endif // _ZY_ALLOCATOR_

//
// Created by 霍伟元 on 2022/10/1.
//

#ifndef CPPTEC_OBJECINTERFACE_H
#define CPPTEC_OBJECINTERFACE_H

namespace zy
{

template<typename BaseT>
class ObjectInterface
{
// public:
//  ObjectInterface() = default;
//  ObjectInterface(const ObjectInterface<BaseT> &obj) = default;
//  ObjectInterface(ObjectInterface<BaseT> &&obj) = default;
//  virtual ~ObjectInterface() = default;
//
// public:
//  ObjectInterface<BaseT>& operator=(const ObjectInterface<BaseT> &obj) = default;
//  ObjectInterface<BaseT>& operator=(ObjectInterface<BaseT> &&obj) = default;

 public:
  virtual void clear() = 0;
};

}
#endif //CPPTEC_OBJECINTERFACE_H

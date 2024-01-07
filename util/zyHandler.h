#ifndef ZYHANDLER_H
#define ZYHANDLER_H

namespace zy
{
template<typename T>
class Handler
{
  public:
    virtual ~Handler() = default;

  public:
    virtual int handing(T *data) = 0;
};
}

#endif // ZYHANDLER_H

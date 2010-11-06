#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <boost/utility/result_of.hpp>
#include <limits>
#include <iostream>
class Square{
public:
  template<typename Signature>
  struct result;
  
  template<typename This, typename T>
  struct result<This(T)> { typedef T type; };


  template<typename T>  
  T operator()(const T& x) const{
    return x * x;
  }
};


struct ForwardDiff;

struct BackwardDiff;

struct CentralDiff;

template <typename Func>
class Functor{
public:
  Func func;
  
  Functor(Func func_);

  template<typename Signature>
  struct result;
  
  template<typename This, typename T>
  struct result<This(T)> { typedef T type; };

  
  template<typename T> 
  typename boost::result_of< Func(T) >::type operator()(const T& x) const{
    return func(x);
  }
};
    
template<typename T>
Functor<T>::Functor(T func_):func(func_){}

template<typename Func>
class DFunctor{
public:
  Func func;

  DFunctor(Func func_);
  template<typename Signature>
  struct result;
  
  template<typename This, typename T>
  struct result<This(T)> { typedef T type; };

  template<typename T>
  typename boost::result_of< Func(T) >::type operator()(const T& x) const{
    typedef typename boost::result_of<Func(T)>::type result_type;
    result_type epsilon = std::numeric_limits<T>::epsilon()*10e6;
    return (func(x+epsilon)-func(x-epsilon))/( 2. * epsilon);
  }
};  

template<typename T>
DFunctor<T>::DFunctor(T func_):func(func_){}


#endif

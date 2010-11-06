#include "function.hpp"
#include "functor.hpp"
#include <iostream>
//#include <boost/lambda/lambda.hpp>
#include <cxxabi.h>


#include <boost/function_types/is_function.hpp>
#include <boost/function_types/is_callable_builtin.hpp>

int Square_Int(int x){
  return x*x;
};

int main(int argc, char** argv){

  Square fn;
  
  typedef boost::result_of<Square(double) >::type type;

  std::cout << fn(10) << std::endl;
  int status;
  std::cout <<  abi::__cxa_demangle(typeid(type).name(), 0, 0, &status) << std::endl;

  typedef boost::result_of<Functor<Square >(double) >::type T1;
  
  std::cout <<  abi::__cxa_demangle(typeid(T1).name(), 0, 0, &status) << std::endl;

  typedef Functor<Square> Func;

  std::cout << Func(Square())(10.01) << std::endl;

  std::cout << DFunctor<Square>(Square())(10.01) << std::endl;
  
  
  return 0;
}

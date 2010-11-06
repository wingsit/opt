#include <iostream>
#include <boost/function.hpp>
#include <cmath>
#include <vector>
#include <functional>

using namespace std;

double newton_method(double init,		     
		     boost::function<double(double x)> dfun,
		     boost::function<double(double x)> d2fun)
{
  static double epsilon = 10e-6;
  while(fabs(dfun(init)) > epsilon)
    init -= dfun(init)/d2fun(init);

  return init;

};


struct UnaryFunction{
  typedef boost::function<double(double x)> Func;
  Func fun;
  double epsilon;
  
  UnaryFunction(Func fun_):fun(fun_){epsilon = 10e-6;};

  double at(const double& x) const{
    return fun(x);
  };

  double operator()(const double& x) const{
    return at(x);
  };
  
  double DD(const double& x) const{
    return (D(x+epsilon) - D(x-epsilon))/(2*epsilon);
  };  

  double DDD(const double& x) const{
    return (DD(x+epsilon) - DD(x-epsilon))/(2*epsilon);
  };  
  double D(const double& x) const{
    return (fun(x+epsilon) - fun(x-epsilon))/ ( 2 * epsilon);
  };

  Func f(){
    return fun;
  }

  struct Derivative{
    Func fun;
    double epsilon;
    Derivative(Func fun_):fun(fun_){epsilon = 10e-7;};
    
    double operator()(const double& x) const{
      return (fun(x+epsilon) - fun(x-epsilon))/(2 * epsilon);
    };
  };

  Func Df(){return Derivative(fun);};
  Func DDf(){return Derivative(UnaryFunction(Derivative(fun)));};
  Func DDDf(){return Derivative(UnaryFunction(Derivative(UnaryFunction(Derivative(fun)))));};
  /*
  Func NthDf(unsigned n){ 
    return NthDf_impl(fun, n);
  }

  Func NthDf_impl(UnaryFunction fun, unsigned n){
    if(n == 0){
      return f();
    }else if(n == 1){
      return Derivative(fun);
    }else if(n >= 2){
      return NthDf_impl(UnaryFunction(Derivative(fun)), n-1);
    }
  };
  */  

};
  

struct polynomial:unary_function<double,double>{
  std::vector<double> coef;
  polynomial(std::vector<double> coef_):coef(coef_){};
  
  double operator()(const double& x) const{
    double sum = 0;
    for(int i = 0; i < coef.size(); ++i)
      {
	sum += pow(x, i) * coef[coef.size()-i-1];
      }
    return sum;
  };
};


int main()
{


  typedef boost::function<double(double x)> Func;  
  std::vector<double> coef;
  coef.push_back(1.5);
  coef.push_back(.25);
  coef.push_back(-2.23);

  // std::cout << newton_method( 0, 
  polynomial poly(coef);
  UnaryFunction func(poly);
 
  cout << func.at(1) << endl;
  cout << func.D(1) << endl;
  cout << func.DD(1) << endl;

  cout << func.Df()(1) << endl;
  cout << func.DDf()(1) << endl;
  cout << func.DDDf()(1) << endl;

  cout << newton_method(1, func.Df(), func.DDf()) << endl;
  
  return 1;


}

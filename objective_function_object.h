#ifndef OBJECTIVE_FUNCTION_OBJECT_H
#define OBJECTIVE_FUNCTION_OBJECT_H
#include <eigen/eigen>
//#include <memory>
#include <cassert>
#include <boost/shared_ptr.hpp>

using namespace Eigen;


template<typename VecType, typename ResultType>
class FuncImpl{
public:
  virtual ResultType operator()(const VecType&) const = 0;  
  virtual FuncImpl * Clone() const = 0;
  virtual ~FuncImpl(){}
  typedef VecType arg_type;
  typedef ResultType result_type;
};

template<typename VecType>
class JacobianImpl{
public:
  virtual VecType operator()(const VecType&) const = 0;
  virtual JacobianImpl * Clone() const = 0;
  virtual ~JacobianImpl(){}
  typedef VecType arg_type;
  typedef VecType result_type;
};
  
template<typename VecType, typename MatrixType>
class HessianImpl{
public:
  virtual MatrixType operator()(const VecType&) const = 0;
  virtual void operator()(const VecType&, MatrixType&) const =0;
  virtual HessianImpl * Clone() const = 0;
  virtual ~HessianImpl(){}
  typedef VecType arg_type;
  typedef MatrixType result_type;
};  

template<typename ArgType, typename ResultType>
class Functor{
protected:
  typedef FuncImpl<ArgType, ResultType> Func;
  boost::shared_ptr<Func> fun_;
public:  
  Functor(boost::shared_ptr<Func> fun):fun_(fun){};
  Functor& operator=(boost::shared_ptr<Func> fun){
    fun_ = fun;
    return *this;
  };
  ResultType operator()(const ArgType x) const{
    return (*fun_)(x);
  }
};
  
//Objective function should provide implementation on evaluation of function
// and the Jacobian (vector-wise)
// ResultType must support index-based access
// ArgType must be Floating point type
template<typename ArgType, typename MatrixType, typename ResultType>
class ObjFunc{
public:
  typedef FuncImpl<ArgType,ResultType> Func;
  typedef JacobianImpl<ArgType> JacobianFunc;
  typedef HessianImpl<ArgType, MatrixType> HessianFunc;
  typedef ArgType JacobianType;
  //  typedef ArgType MatrixType;
private:
  boost::shared_ptr<Func> fun;
  boost::shared_ptr<JacobianFunc> jacobian;
  boost::shared_ptr<HessianFunc> hessian_;
  //  typedef boost::function<ArgType(ArgType x)> JacobianFunc;

public:
  explicit ObjFunc(boost::shared_ptr<Func> fun_,
		   boost::shared_ptr<JacobianFunc> jacobian_)
    :fun(fun_), jacobian(jacobian_), hessian_(NULL){};

  explicit ObjFunc(boost::shared_ptr<Func> fun_,
		   boost::shared_ptr<JacobianFunc> jacobian_,
		   boost::shared_ptr<HessianFunc> hessian_)
    :fun(fun_), jacobian(jacobian_), hessian_(hessian_){};

  ObjFunc(const ObjFunc& rhs):fun(rhs.getObjective()), jacobian(rhs.getJacobian()), hessian_(rhs.getHessian()){};
  
  inline ResultType operator()(const ArgType & x)const{
    return (*fun)(x);  
  };

  inline JacobianType gradient(const ArgType& x)const{
    return (*jacobian)(x);
  };
  
  inline void gradient(const ArgType& x, ArgType& result_vec) const{
    result_vec = (*jacobian)(x);
  };
  
  inline void hessian(const ArgType& x, MatrixType& result_hessian) const{
    (*hessian_)(x, result_hessian);
  };

  inline MatrixType hessian(const ArgType& x) const{
    MatrixType result_hessian(x.size(), x.size());
    this->hessian(x, result_hessian);
    return result_hessian;
  }; 

  boost::shared_ptr<Func> getObjective() const{
    return fun;
  };

  boost::shared_ptr<JacobianFunc> getJacobian() const{
    return jacobian;
  };

  boost::shared_ptr<HessianFunc> getHessian() const{
    return hessian_;
  };
  
};
struct Direction:FuncImpl<double, double>{
  VectorXd x_, p_;
  typedef ObjFunc<VectorXd, MatrixXd, double> Func;
  Func f_;
  Direction(const VectorXd& x, const VectorXd& p, const Func& f):x_(x), p_(p), f_(f){};
  double operator()(const double& a)const{
    return f_(x_+a*p_);
  };
  ~Direction(){}
  Direction* Clone() const{
    return new Direction(x_, p_, f_);
  };  
};



#endif

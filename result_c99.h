struct ResultWrapperBase;
#define __RESULT_C99_COMPLIANCE Result(ResultWrapperBase const* w)

#include "result.h"

/**********************************************/
/*           MAIN MACRO EXPRESSIONS           */
/**********************************************/

// Standard C99-compliant version of the TRY macro. The main difference is
// that it doesn't use GNU statement expression but the variable name
// should be listed as a first argument.
#define TRY_2(varname, Expr_) \
  ResultWrapperBase const & result_tmp_ ## varname = wrapResult((Expr_)); \
  if (!result_tmp_ ## varname.ok) \
    return ERROR(CONV_OBJ_TO_TYPE(&result_tmp_ ## varname, (Expr_)).getError()); \
  varname = CONV_OBJ_TO_TYPE(&result_tmp_ ## varname, (Expr_)).getData()

// This macro converts the given object
// 'obj' to the return type of expression 'Expr_'
// See https://www.artima.com/articles/conditional-love-foreach-redux
// for the additional technical details.
#define CONV_OBJ_TO_TYPE(obj, Expr_) (true ? obj : (Expr_))


/**********************************************/
/*              RESULT WRAPPER                */
/**********************************************/

struct ResultWrapperBase
{
  /**
   * true  -- Result contains expected return value.
   * false -- Result contains error.
   */
  bool ok;
};

template <class T>
struct ResultWrapper : ResultWrapperBase
{
  ResultWrapper(T const &res) :
    res(res) { ok = res.isOk(); }

  /** Copy of Result */
  T res;
};

template <typename T, typename E>
Result<T, E>::Result(ResultWrapperBase const * w)
{
  typedef ResultWrapper< Result<T, E> > Wrapper;
  const Result<T, E> &other = ((Wrapper*)w)->res;
  data = other.data;
  err_val = other.err_val;
  ok = other.ok;
}

/**
 * Necessary because C++99 doesn't have 'auto' keyword,
 * thus we need type-agnostic wrapper
 */
template <typename T, typename E>
ResultWrapper< Result<T, E> > wrapResult(Result<T, E> const &res)
{
  return ResultWrapper< Result<T, E> >(res);
}


#include <string>
#include <stdio.h>
#include <iostream>

/**
 * Rust-like error handling. See test.cc for usage examples.
 */

/**********************************************/
/*           MAIN MACRO EXPRESSIONS           */
/**********************************************/

// Return expected value (first template arg of Result) if no errors.
//
// Otherwise, terminate function immediately and return
// error object (second template arg of Result)
//
// (uses GNU statement expression, available in almost all modern compilers)
#define TRY(Expr_) \
  ({ \
    typeof(Expr_) res = (Expr_); \
    if (!res.isOk()) return ERROR(res.getError()); \
    res.getData(); \
  })

// Send error message with exact file/line where error occured
#define ERROR(msg) error(msg, __FILE__, __LINE__)

// Use GCC extension to notify user about ignored Result objects.
// (in C++17, [[nodiscard]] can be used instead)
#define RESULT_NODISCARD __attribute__((warn_unused_result))

/**********************************************/
/*               ERROR OBJECTS                */
/**********************************************/

template <typename E>
struct Error
{
  Error(E e) : err_val(e) {}
  E err_val;
};
template <typename E> Error<E> error(E e) { return Error<E>(e); }

Error<std::string> error(std::string msg, std::string file, int line)
{
  char buf[255];
  std::string full_msg = msg;
  full_msg += "\n  in ";
  full_msg += file;
  full_msg += ":";
  full_msg += (snprintf(buf, 255, "%d", line), buf);
  return Error<std::string>(full_msg);
}

/**********************************************/
/*            MAIN RESULT OBJECT              */
/**********************************************/

// Macro so that C99 header can add some additional
// conversion logic.
#ifndef __RESULT_C99_COMPLIANCE
#define __RESULT_C99_COMPLIANCE
#endif

/**
 * Template class for Rust-like error handling.
 * See https://softwareengineering.stackexchange.com/questions/352702/error-handling-considerations/352745
 * for the original suggestion.
 *
 * T - type of expected value
 * E - type of error contents (usually std::string)
 *
 */
template <typename T, typename E = std::string>
struct Result
{
  Result(const Result<T, E> &other) :
    data(other.data), err_val(other.err_val), ok(other.ok) {}

  // typedef ResultWrapper< Result<T, E> > Wrapper;
  // /**
  //  * Constructor that unpacks Result from ResultWrapper
  //  *
  //  * TODO: Use initializer list because T can't be initialized
  //  *       in constructor body if it is const reference.
  //  */
  // Result(ResultWrapperBase const &w)
  // {
  //   const Result<T, E> &other = ((Wrapper const &)w).res;
  //   data = other.data;
  //   err_val = other.err_val;
  //   ok = other.ok;
  // }

  __RESULT_C99_COMPLIANCE;

  template <typename T2>
  Result(T2 t) : data(t), ok(true) {}

  /** */
  template <typename E2>
  Result(Error<E2> e) : data(), err_val(e.err_val), ok(false) {}

  template <typename T2, typename E2>
  Result(Result<T2, E2> other) :
    data(other.data), err_val(other.err_val), ok(other.ok) {}

  const T & getData() const { if (ok) return data; else throw std::logic_error(err_val); }
  const E & getError() const { return err_val; }
  bool isOk() const { return ok; }

  private:
  T data;
  E err_val;
  bool ok;
};


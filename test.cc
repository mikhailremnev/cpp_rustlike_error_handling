#include "result_c99.h"

/**
 *
 * Usage example:
 *
 * ```
 *
 * // RESULT_NODISCARD means that GCC will show warnings for unchecked
 * // errors in Result ('warning: ignoring return value of...').
 *
 * Result<int, std::string> sqrtResult(int x) RESULT_NODISCARD;
 * Result<int, std::string> sqrtLoop(int x) RESULT_NODISCARD;
 *
 * Result<int, std::string> sqrtResult(int x)
 * {
 *   if (x < 0) return ERROR("Error: x is negative");
 *   std::cout << "x = " << sqrt(x) << std::endl;
 *   return x;
 * }
 *
 * Result<int, std::string> sqrtLoop(int x)
 * {
 *   int y = TRY(sqrtResult(x));
 *
 *   // Won't reach this part of the code if x is negative
 *
 *   for (int i = 0; i < y; i++) {
 *     printf("%d\n", i);
 *   }
 *
 *   return y;
 * }
 *
 * int main()
 * {
 *   // GCC will print warning here, because
 *   // possible error is ignored.
 *   sqrtLoop(16);
 *
 *   Result<int, std::string> result = sqrtLoop(-16);
 *
 *   return result.isOk() ? 0 : 1;
 * }
 *
 * ```
 *
 */

Result<int, std::string> resultExample1(int x)
{
  if (x < 0) return ERROR("Error: x is negative");
  std::cout << "x = " << x << std::endl;
  return x;
}

Result<int, std::string> resultExample2(int x)
{
  int y = TRY(resultExample1(x));
  std::cout << "y = " << y << std::endl;
  return y;
}

Result<int, std::string> resultExample3(int x)
{
  int y; TRY_2(y, resultExample1(x));
  std::cout << "y = " << y << std::endl;
  return y;
}

bool resultTest()
{
  int val1;
  int val1_expected = 15;

  std::string val2;
  std::string val2_expected =
    "Error: x is negative\n"
    "  in result.cc:20";

  val1 = resultExample2( 15).getData();
  val2 = resultExample2(-15).getError();

  return (val1 == val1_expected) && (val2 == val2_expected);
}

int importantRetval() RESULT_NODISCARD;
int importantRetval()
{
  return 2;
}


int main()
{
  importantRetval();

  resultExample2(9);
  Result<int> y = resultExample3(-15);
  if (!y.isOk()) std::cout << y.getError() << std::endl;
  resultTest();
  return 0;
}


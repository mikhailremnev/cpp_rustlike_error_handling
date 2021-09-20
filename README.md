Template class for Rust-like error handling, based on the suggestion from
https://softwareengineering.stackexchange.com/questions/352702/error-handling-considerations/352745

Basically, instead of exceptions, you can implement functions that return
Variant class that contains either return value or error details, if
function call has failed for some reason. 

Moreover, you can use `TRY` macro to chain multiple calls like this:

```
// If establishConnection() fails, getTemperature() will not be called.
double temperature = TRY( TRY(establishConnection())->getTemperature() );
```

There are four main differences:

1. Trying to call `getData` (or typecast) the Result object with
   `isOk() == false`will throw `std::logic_error` exception.
2. I had to drop C++11 and C++17 features to provide C99 support. (technically
   it is possible to detect which standard is being used from preprocessor
   directives and use the correct set of features but I was lazy)
3. If GNU statement expressions are not supported by your compiler,
   it is possible to use `TRY_2` macro that doesn't depend on those.
   It was implemented based on the ideas from this article:
   https://www.artima.com/articles/conditional-love-foreach-redux
4. Instead of `[[nodiscard]]` from C++17, I'm using GCC-specific
   `warn_unused_result` to show compiler warnings for ignored
   errors.

List of removed features from newer C++ standards:
* Add std::move statements for C++11 version
* Use 'auto' for C++11 version
* Use [[nodiscard]] for Result in C++17 version

Note that it might be possible to use union for `data` and
`err_val` class members to reduce memory usage. I'm not sure
if it is always possible.


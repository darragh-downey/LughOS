# LughOS Secure Coding Rules

All C code must follow these rules to ensure security and reliability.

## 1. Avoid Unsafe Functions

- **Prohibited**: `strcpy`, `strcat`, `sprintf`, `gets`, `scanf`, `vsprintf`.
- **Use Instead**: `strncpy`, `strncat`, `snprintf`, `fgets`, `sscanf`, `vsnprintf`.

## 2. Enforce Bounds Checking

- Validate array indices and buffer sizes.

## 3. Memory Management

- Check `malloc`/`calloc` results.
- Avoid raw pointer arithmetic.

## 4. Input Validation

- Validate all inputs to prevent overflows.

## 5. Error Handling

- Handle errors explicitly.

## 6. Static Analysis

- Code must pass Cppcheck with `--enable=all`.

## Enforcement

- Compiler flags: `-Werror`, `-fstack-protect-strong`, `-D_FORTIFY_SOURCE=2`.
- CI: GitHub Actions fails on violations.

---

# NASA JPL Institutional Coding Standard for C (LOC-1 to LOC-4)

## LOC-1: Language Compliance

1. **Language Conformance**: All C code shall conform to ISO/IEC 9899:1999(E). No undefined/unspecified behavior. No `#pragma` except `#error`/`#warning`.
2. **Routine Checking**: Compile with all warnings enabled, no errors/warnings. Use static analysis tools (e.g., `gcc -Wall -pedantic`).

## LOC-2: Predictable Execution

3. **Loop Bounds**: All loops shall have statically determinable upper bounds. Annotate non-terminating server loops with `/* @non-terminating@ */`.
4. **No Recursion**: No direct/indirect recursion.
5. **No Dynamic Memory After Initialization**: No dynamic allocation (e.g., `malloc`) after task init.
6. **IPC for Task Communication**: Use IPC for task comms, no callbacks. IPC messages contain data only (no pointers).
7. **No Task Delays for Synchronization**: Do not use task delays for synchronization.
8. **Explicit Ownership of Shared Data**: Shared data has a single owning task; ownership transferred via IPC.
9. **Restrict Semaphores and Locks**: Avoid semaphores/locks for shared data. If used, avoid nesting; lock/unlock in same function.
10. **Memory Protection**: Use memory protection if available; otherwise, use safety margins/barriers.
11. **Simple Control Flow**: No `goto`, `setjmp`, `longjmp`.
12. **Enum Initialization**: Only the first enum member may be initialized with `=`, unless all are explicitly initialized.

## LOC-3: Defensive Coding

13. **Limited Scope**: Declare data at smallest scope. Avoid shadowing. Prefer `const`, `enum`, pure functions. Place `extern` in headers.
14. **Check Return Values**: Check/use return values of non-void functions, or cast to `(void)` if unused.
15. **Validate Parameters**: Check parameters at start of public functions; for non-public, caller or callee must check.
16. **Use Assertions**: Use static/dynamic assertions for sanity checks. Functions >10 lines should have at least one assertion. Assertions must be side-effect free and trigger error recovery.
17. **Use Typedefs for Types**: Use typedefs indicating size/signedness (e.g., `I32`, `U16`) instead of basic types.
18. **Explicit Order of Evaluation**: Use parentheses to make order of evaluation explicit in compound expressions.
19. **No Side Effects in Expressions**: Boolean expressions shall have no side effects.

## LOC-4: Code Clarity

20. **Limit Preprocessor Use**: Restrict preprocessor to file inclusion/simple macros. No token pasting, varargs, recursive macros. Limit conditional compilation to header guards.
21. **No Macros in Functions**: Do not `#define` macros within functions/blocks.
22. **No Undefining Macros**: Do not use `#undef`.
23. **Preprocessor Directives in Same File**: `#else`, `#elif`, `#endif` must be in same file as their `#if`/`#ifdef`.
24. **One Statement Per Line**: No more than one statement/declaration per line (except `for` loop expressions).
25. **Short Functions**: Functions should be ≤60 lines, ≤6 parameters.
26. **Limit Indirection**: Object declarations should have ≤2 levels of indirection.
27. **Limit Dereferencing**: Statements should have ≤2 levels of dereferencing per object.
28. **No Hidden Dereferences**: Do not hide pointer dereference ops in macros/typedefs.
29. **No Non-Constant Function Pointers**: Do not use non-constant function pointers.
30. **No Function Pointer Casts**: Do not convert function pointers to other types (except integral types).
31. **Preprocessor Order**: `#include` directives only preceded by other preprocessor directives or comments.

---

## SEI CERT C Coding Standard: Rules and Recommendations

The SEI CERT C Coding Standard is organized into 15 chapters, each with rules (mandatory, e.g., ARR30-C) and recommendations (advisory, e.g., ARR00-C). Rules are required for compliance; recommendations improve quality and security. Below is a comprehensive list, grouped by topic, with identifiers and brief descriptions. For full details, see the [SEI CERT C Coding Standard wiki](https://wiki.sei.cmu.edu/confluence/display/c/SEI+CERT+C+Coding+Standard).

### 1. Preprocessor (PRE)

**Rules:**

- PRE30-C: Do not create a universal character name through concatenation.
- PRE31-C: Avoid side effects in arguments to unsafe macros.
- PRE32-C: Do not use preprocessor directives in invocations of function-like macros.

**Recommendations:**

- PRE00-C: Prefer inline or static functions to function-like macros.
- PRE01-C: Use parentheses within macros around parameter names.
- PRE02-C: Macro replacement lists should be parenthesized.
- PRE10-C: Wrap multi-statement macros in a do-while loop.
- PRE11-C: Do not conclude a single statement macro definition with a semicolon.
- PRE12-C: Do not define unsafe function-like macros.

### 2. Declarations and Initialization (DCL)

**Rules:**

- DCL30-C: Declare objects with appropriate storage durations.
- DCL31-C: Declare identifiers before using them.
- DCL36-C: Do not declare an identifier with conflicting linkage classifications.
- DCL37-C: Do not declare or define a reserved identifier.
- DCL38-C: Use the correct syntax when declaring a flexible array member.
- DCL39-C: Avoid information leakage when passing a structure across a trust boundary.
- DCL40-C: Do not create incompatible declarations of the same function or object.
- DCL41-C: Do not declare variables inside a switch statement before the first case label.

**Recommendations:**

- DCL00-C: Const-qualify immutable objects.
- DCL01-C: Do not reuse variable names in subscopes.
- DCL02-C: Use visually distinct identifiers.
- DCL03-C: Use a static assertion to test the value of a constant expression.
- DCL04-C: Do not declare more than one variable per declaration.
- DCL05-C: Use typedefs to improve code readability.
- DCL06-C: Use meaningful symbolic constants to represent literal values.
- DCL07-C: Include the appropriate type information in function declarators.
- DCL08-C: Properly encode relationships in constant definitions.
- DCL09-C: Declare functions that return errno with a return type of errno_t.
- DCL10-C: Maintain the contract between the writer and caller of variadic functions.
- DCL11-C: Understand the type issues associated with variadic functions.
- DCL12-C: Implement abstract data types using opaque types.
- DCL13-C: Declare function parameters that are pointers to values not changed by the function as const.
- DCL15-C: Declare file-scope objects or functions that do not need external linkage as static.
- DCL16-C: Use "L," not "l," when specifying long integer constants.
- DCL19-C: Minimize the scope of variables and functions.
- DCL20-C: Explicitly specify void when a function accepts no parameters.
- DCL21-C: Understand the storage of compound literals.
- DCL22-C: Use volatile for data that cannot be cached.
- DCL23-C: Guarantee that mutually visible identifiers are unique.

### 3. Expressions (EXP)

**Rules:**

- EXP30-C: Do not depend on the order of evaluation for side effects.
- EXP32-C: Do not access a volatile object through a nonvolatile reference.
- EXP33-C: Do not read uninitialized memory.
- EXP34-C: Do not dereference null pointers.
- EXP35-C: Do not modify objects with temporary lifetime.
- EXP36-C: Do not cast pointers into more strictly aligned pointer types.
- EXP37-C: Call functions with the correct number and type of arguments.
- EXP39-C: Do not access a variable through a pointer of an incompatible type.
- EXP40-C: Do not modify constant objects.
- EXP42-C: Do not compare padding data.
- EXP43-C: Avoid undefined behavior when using restrict-qualified pointers.
- EXP44-C: Do not rely on side effects in operands to sizeof, _Alignof, or _Generic.
- EXP45-C: Do not perform assignments in selection statements.
- EXP46-C: Do not use a bitwise operator with a Boolean-like operand.
- EXP47-C: Do not call va_arg with an argument of the incorrect type.

**Recommendations:**

- EXP00-C: Use parentheses for precedence of operation.
- EXP02-C: Be aware of the short-circuit evaluation of logical expressions.
- EXP03-C: Do not assume the size of a structure is the sum of the sizes of its members.
- EXP05-C: Do not cast away a const qualification.
- EXP08-C: Ensure pointer arithmetic is used correctly.
- EXP09-C: Use sizeof to determine the size of a type or variable.
- EXP10-C: Do not depend on the order of evaluation of subexpressions or the order in which side effects take place.
- EXP11-C: Do not make assumptions regarding the layout of structures with bit-fields.
- EXP12-C: Do not ignore values returned by functions.
- EXP13-C: Treat relational and equality operators as if they were nonassociative.
- EXP15-C: Do not place a semicolon on the same line as an if, for, or while statement.
- EXP16-C: Do not compare function pointers to constant values.
- EXP19-C: Use braces for the body of an if, for, or while statement.
- EXP20-C: Perform explicit tests to determine success, true and false, and equality.

### 4. Integers (INT)

**Rules:**

- INT30-C: Ensure that unsigned integer operations do not wrap.
- INT31-C: Ensure that integer conversions do not result in lost or misinterpreted data.
- INT32-C: Ensure that operations on signed integers do not result in overflow.
- INT33-C: Ensure that division and remainder operations do not result in divide-by-zero errors.
- INT34-C: Do not shift an expression by a negative number of bits or by greater than or equal to the number of bits that exist in the operand.
- INT35-C: Do not attempt to convert a negative value to an unsigned integer type.
- INT36-C: Converting a pointer to integer or integer to pointer.

**Recommendations:**

- INT00-C: Understand the data model used by your implementation(s).
- INT02-C: Understand integer conversion rules.
- INT04-C: Enforce limits on integer values originating from untrusted sources.
- INT07-C: Use only explicitly signed or unsigned char type for numeric values.
- INT08-C: Verify that all integer values are in range.
- INT09-C: Ensure enumeration constants map to unique values.
- INT10-C: Do not assume a positive remainder when using the % operator.
- INT12-C: Do not make assumptions about the type of a plain int bit-field when used in an expression.
- INT13-C: Use bitwise operators only on unsigned operands.
- INT14-C: Avoid performing bitwise and arithmetic operations on the same data.
- INT15-C: Use intmax_t or uintmax_t for formatted I/O on programmer-defined integer types.
- INT16-C: Do not make assumptions about representation of signed integers.
- INT18-C: Evaluate integer expressions in a larger size before comparing or assigning to that size.

### 5. Floating Point (FLP)

**Rules:**

- FLP30-C: Do not use floating-point variables as loop counters.
- FLP32-C: Prevent or detect domain and range errors in math functions.
- FLP34-C: Ensure that floating-point conversions do not result in loss of data or precision.
- FLP36-C: Preserve precision when converting integral values to floating-point type.
- FLP37-C: Do not use object representations to compare floating-point values.

**Recommendations:**

- FLP00-C: Understand the limitations of floating-point numbers.
- FLP03-C: Detect and handle floating-point errors.
- FLP04-C: Check floating-point inputs for exceptional values.
- FLP06-C: Convert integers to floating point for floating-point operations.
- FLP07-C: Prefer almost-equal comparisons over direct equality for floating-point numbers.

### 6. Arrays (ARR)

**Rules:**

- ARR30-C: Do not form or use out-of-bounds pointers or array subscripts.
- ARR32-C: Ensure size arguments for variable length arrays are in a valid range.
- ARR36-C: Do not subtract or compare two pointers that do not refer to the same array.
- ARR37-C: Do not add or subtract an integer to a pointer to a non-array object.
- ARR38-C: Guarantee that library functions do not form invalid pointers.
- ARR39-C: Do not add or subtract a scaled integer to a pointer.

**Recommendations:**

- ARR00-C: Understand implementation-specific details regarding the representation and alignment of arrays.
- ARR01-C: Do not apply the sizeof operator to a pointer when taking the size of an array.
- ARR02-C: Explicitly state array bounds, even if implicitly defined by an initializer.

### 7. Characters and Strings (STR)

**Rules:**

- STR30-C: Do not attempt to modify string literals.
- STR31-C: Guarantee that storage for strings has sufficient space for character data and the null terminator.
- STR32-C: Do not pass a non-null-terminated character sequence to a library function that expects a string.
- STR34-C: Cast characters to unsigned char before converting to larger integer sizes.
- STR37-C: Arguments to character-handling functions must be representable as an unsigned char.
- STR38-C: Do not confuse narrow and wide character strings and functions.

**Recommendations:**

- STR00-C: Represent characters using an appropriate type.
- STR02-C: Sanitize data passed to complex subsystems.
- STR03-C: Do not inadvertently truncate strings.
- STR04-C: Use plain char for characters in the basic character set.
- STR05-C: Use pointers to const when referring to string literals.
- STR06-C: Do not assume that strtok() leaves the parse string unchanged.
- STR07-C: Use the bounds-checking interfaces for string manipulation.
- STR09-C: Don’t assume numeric values for expressions with type plain character.
- STR10-C: Do not concatenate different types of string literals.
- STR11-C: Do not specify the bound of a character array initialized with a string literal.

### 8. Memory Management (MEM)

**Rules:**

- MEM30-C: Do not access freed memory.
- MEM31-C: Free dynamically allocated memory when no longer needed.
- MEM33-C: Allocate and copy structures containing a flexible array member dynamically.
- MEM34-C: Only free memory allocated dynamically.
- MEM35-C: Allocate sufficient memory for an object.
- MEM36-C: Do not modify the alignment of objects by calling realloc.

**Recommendations:**

- MEM00-C: Allocate and free memory in the same module, at the same level of abstraction.
- MEM01-C: Store a new value in pointers immediately after free().
- MEM02-C: Immediately cast the result of a memory allocation function call into a pointer to the allocated type.
- MEM03-C: Clear sensitive information stored in reusable resources.
- MEM04-C: Beware of zero-length allocations.
- MEM05-C: Avoid large stack allocations.
- MEM06-C: Ensure that sensitive data is not written out to disk.
- MEM07-C: Ensure that the arguments to calloc(), when multiplied, do not wrap.
- MEM08-C: Use realloc() only to resize dynamically allocated arrays.
- MEM09-C: Use the correct type for the corresponding allocation function.
- MEM10-C: Define and use a pointer validation function.
- MEM11-C: Do not assume infinite heap space.
- MEM12-C: Consider using a goto chain when leaving a function on error when using and releasing resources.

### 9. Input Output (FIO)

**Rules:**

- FIO30-C: Exclude user input from format strings.
- FIO32-C: Do not perform operations on devices that are only appropriate for files.
- FIO34-C: Distinguish between characters read from a file and EOF or WEOF.
- FIO37-C: Do not assume that fgets() or fgetws() returns a nonempty string when successful.
- FIO38-C: Do not copy a FILE object.
- FIO39-C: Do not alternately input and output from a stream without an intervening flush or positioning call.
- FIO40-C: Reset strings on fgets() or fgetws() failure.
- FIO41-C: Do not call getc(), putc(), getwc(), or putwc() with a stream argument that has side effects.
- FIO42-C: Close files when they are no longer needed.
- FIO44-C: Only use values for fsetpos() that are returned from fgetpos().
- FIO45-C: Avoid TOCTOU race conditions while accessing files.
- FIO46-C: Do not access a closed file.
- FIO47-C: Use valid format strings.

**Recommendations:**

- FIO00-C: Take care when creating format strings.
- FIO01-C: Be careful using functions that use file names for identification.
- FIO02-C: Canonicalize path names originating from tainted sources.
- FIO03-C: Do not make assumptions about fopen() and file creation.
- FIO05-C: Identify files using multiple file attributes.
- FIO06-C: Create temporary files with appropriate permissions.
- FIO08-C: Take care when calling remove() on an open file.
- FIO09-C: Be careful with binary data when transferring data across systems.
- FIO10-C: Take care when using the rename() function.
- FIO11-C: Take care when specifying the mode parameter of fopen().
- FIO12-C: Prefer pread() to lseek() followed by read().
- FIO13-C: Never push back anything other than one read character using ungetc().
- FIO14-C: Understand the difference between text mode and binary mode with file streams.
- FIO15-C: Ensure that file operations are performed in a secure directory.
- FIO17-C: Do not rely on an ending null character when using fread().
- FIO18-C: Never expect fwrite() to terminate the writing process with a null character.
- FIO19-C: Do not use fseek() and ftell() to compute the size of a regular file.
- FIO20-C: Avoid unintentional truncation when using fgets() or fgetws().
- FIO21-C: Do not create temporary files in shared directories.
- FIO22-C: Close files before spawning processes.

### 10. Environment (ENV)

**Rules:**

- ENV30-C: Do not modify the object referenced by the return value of certain functions.
- ENV31-C: Do not rely on an environment pointer following an operation that may invalidate it.
- ENV32-C: All exit handlers must return normally.
- ENV33-C: Do not call system().
- ENV34-C: Do not store pointers returned by certain functions.

**Recommendations:**

- ENV00-C: Do not store the pointer to the string returned by getenv().
- ENV01-C: Do not make assumptions about the size of an environment variable.
- ENV02-C: Beware that environment variable values may contain arbitrary characters.
- ENV03-C: Sanitize the environment when invoking external programs.
- ENV04-C: Do not call system() if you do not need a command processor.

### 11. Signals (SIG)

**Rules:**

- SIG30-C: Call only asynchronous-safe functions within signal handlers.
- SIG31-C: Do not access shared objects in signal handlers.
- SIG34-C: Do not call signal() from within interruptible signal handlers.
- SIG35-C: Do not return from a computational exception signal handler.

**Recommendations:**

- SIG00-C: Mask signals handled by noninterruptible signal handlers.
- SIG01-C: Understand implementation-defined behavior when using signals.
- SIG02-C: Avoid using signals to implement normal functionality.

### 12. Error Handling (ERR)

**Rules:**

- ERR30-C: Set errno to zero before calling a library function known to set errno, and check errno only after the function returns a value indicating failure.
- ERR32-C: Do not rely on indeterminate values of errno.
- ERR33-C: Do not throw an exception across a C callback function.

**Recommendations:**

- ERR00-C: Adopt and implement a consistent and comprehensive error-handling policy.
- ERR01-C: Use ferror() rather than errno to check for file operation errors.
- ERR02-C: Avoid in-band error indicators.
- ERR03-C: Use runtime-constraint handlers when calling the bounds-checking interfaces.
- ERR04-C: Choose an appropriate termination strategy.
- ERR05-C: Application-independent code should provide error detection without dictating error handling.
- ERR06-C: Understand the termination behavior of assert() and abort().
- ERR07-C: Prefer functions that support error checking over equivalent functions that don’t.

### 13. Application Programming Interfaces (API)

**Rules:**

- API30-C: Do not cast away const qualification in a function parameter if the function modifies the object.
- API31-C: Provide a mechanism to terminate variadic functions when an error is encountered.
- API32-C: Ensure that function parameters have appropriate types.

**Recommendations:**

- API00-C: Functions should validate their parameters.
- API01-C: Use standard library functions instead of custom implementations when possible.
- API02-C: Provide a consistent interface for library functions.
- API03-C: Create consistent interfaces and capabilities for functions that share common behaviors or properties.
- API04-C: Provide a way to prevent functions from throwing exceptions.
- API05-C: Use conformant array parameters in function prototypes.
- API06-C: Use caution with functions that can lead to resource exhaustion.
- API07-C: Avoid implementation-defined behavior in standard library functions.
- API08-C: Avoid passing string literals to functions expecting non-const char pointers.

### 14. Concurrency (CON)

**Rules:**

- CON30-C: Clean up thread-specific storage.
- CON31-C: Do not destroy a mutex while it is locked.
- CON32-C: Prevent data races when accessing bit-fields from multiple threads.
- CON33-C: Avoid race conditions when using library functions.
- CON34-C: Declare objects shared between threads with appropriate storage durations.
- CON35-C: Avoid deadlock by locking in a predefined order.
- CON36-C: Wrap functions that can spuriously wake up in a loop.
- CON37-C: Do not call signal() in a multithreaded program.
- CON38-C: Preserve thread safety and liveness when using condition variables.
- CON39-C: Do not join or detach a thread that was previously joined or detached.
- CON40-C: Do not refer to an atomic variable twice in an expression.
- CON41-C: Wrap functions that can fail with a mutex in a loop.
- CON43-C: Do not allow data races in multithreaded code.

**Recommendations:**

- CON00-C: Avoid assuming that concurrent accesses to shared objects are atomic.
- CON01-C: Acquire and release synchronization primitives in the same module, at the same level of abstraction.
- CON02-C: Do not use volatile to synchronize access to shared objects.
- CON03-C: Ensure visibility of shared objects when controlling access with synchronization primitives.
- CON04-C: Protect shared objects with the appropriate synchronization primitive.
- CON05-C: Do not perform operations that can block while holding a lock.
- CON06-C: Ensure that every mutex has an owner responsible for its creation and destruction.
- CON07-C: Ensure that each thread that dereferences a pointer to a shared object does so only while the object’s lifetime is still valid.
- CON08-C: Ensure that a thread that accesses a shared object does not do so after the object’s lifetime has ended.
- CON09-C: Avoid the ABA problem when using compare-and-swap operations.

### 15. Miscellaneous (MSC, POS, WIN)

**Rules:**

- MSC30-C: Do not use the rand() function for generating pseudorandom numbers.
- MSC32-C: Properly seed pseudorandom number generators.
- MSC33-C: Do not pass invalid data to the asctime() function.
- MSC37-C: Ensure that control never reaches the end of a non-void function.
- MSC38-C: Do not treat a predefined identifier as an object if it might only be implemented as a macro.
- MSC39-C: Do not call va_arg() on a va_list that has an indeterminate value.
- MSC40-C: Do not violate constraints.
- MSC41-C: Never hard code sensitive information.
- POS30-C: Use the readlink() function properly.
- POS34-C: Do not call putenv() with a pointer to an automatic variable as the argument.
- POS35-C: Avoid race conditions while checking for the existence of a symbolic link.
- POS36-C: Observe correct revocation order while relinquishing privileges.
- POS37-C: Ensure that privilege relinquishment is successful.
- POS38-C: Beware of race conditions when using fork and file descriptors.
- POS39-C: Use the correct byte ordering when transferring data between systems.
- POS44-C: Do not use real-time signals.
- POS47-C: Do not use threads that can be canceled asynchronously.
- POS48-C: Do not unlock or destroy another POSIX thread’s mutex.
- POS49-C: When data must be accessed by multiple threads, provide a mutex to protect it.
- POS50-C: Declare objects shared between POSIX threads with appropriate storage durations.
- POS51-C: Avoid deadlock with POSIX threads by locking in a predefined order.
- POS52-C: Do not perform operations that can block while holding a POSIX mutex.
- POS53-C: Do not use more than one mutex for concurrent waiting operations on a condition variable.
- POS54-C: Detect and handle POSIX library errors.
- WIN30-C: Properly pair allocation and deallocation functions.

**Recommendations:**

- MSC00-C: Compile cleanly at high warning levels.
- MSC01-C: Strive for logical completeness.
- MSC04-C: Use comments consistently and in a readable fashion.
- MSC05-C: Do not manipulate time_t typed values directly.
- MSC06-C: Beware of compiler optimizations.
- MSC07-C: Detect and remove dead code.
- MSC09-C: Character encoding: Use subset of ASCII for safety critical applications.
- MSC10-C: Character encoding: UTF8-related issues.
- MSC11-C: Incorporate diagnostic tests using assertions.
- MSC12-C: Detect and remove code that has no effect or is never executed.
- MSC13-C: Treat string literals as const-qualified data.
- MSC14-C: Do not introduce unnecessary platform dependencies.
- MSC15-C: Do not depend on undefined behavior.
- MSC17-C: Finish every set of statements associated with a case label with a break statement.
- MSC20-C: Do not use a switch statement to transfer control into a complex block.
- MSC21-C: Use robust loop termination conditions.
- MSC22-C: Use the setjmp() or longjmp() functions only when necessary and appropriate.
- MSC23-C: Beware of vendor-specific library extensions that can conflict with future standards.
- MSC24-C: Do not use deprecated or obsolescent functions.
- POS00-C: Avoid race conditions with multiple threads.
- POS01-C: Check for the existence of a file in a time-of-check, time-of-use critical section without holding an appropriate lock.
- POS02-C: Follow the principle of least privilege when assigning permissions to a new process.
- POS04-C: Avoid using PTHREAD_MUTEX_NORMAL type mutex locks.
- POS05-C: Limit access to files by creating a jail.
- POS06-C: Pass only POSIX thread–specific data to thread-specific functions.
- POS07-C: Understand that the behavior of unsetenv() is implementation defined with respect to a NULL parameter.
- POS08-C: Understand that the behavior of POSIX functions can vary when a file is opened for writing.
- POS09-C: Understand the difference between POSIX threads and regular process threads when relinquishing the CPU.
- WIN00-C: Be specific when dynamically allocating memory on Windows platforms.
- WIN01-C: Do not use the Windows _wsystem() function.
- WIN02-C: Consider using safe string functions when dealing with WCHAR strings.
- WIN03-C: Understand when to call GetLastError() versus retrieving the errno value.
- WIN04-C: Consider using the Windows CRT for superior functionality to the C Standard.
- WIN05-C: Understand the error code used by Windows APIs.

---

*For full details, see the JPL_Coding_Standard_C.pdf and the SEI CERT C Secure Coding Standard documentation and wiki.*

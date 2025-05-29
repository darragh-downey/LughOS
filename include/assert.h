/**
 * @file assert.h
 * @brief Assertions for LughOS
 *
 * Implementation of runtime assertions for LughOS according to 
 * NASA Power of Ten rule 10: Assert critical invariants
 * 
 * This implementation provides both:
 * 1. Standard C assert() macro for compatibility
 * 2. NASA Power of Ten compliant ASSERT(), PRECOND(), POSTCOND(), INVARIANT() macros
 */

#ifndef LUGHOS_ASSERT_H
#define LUGHOS_ASSERT_H

#include "lugh.h"

/**
 * Assert that a condition is true
 * If the condition is false, logs an error and halts the system
 * 
 * @param cond Condition to check
 * @param message Message to log if condition is false
 * @param file Source file where assertion was made
 * @param line Line number where assertion was made
 */
void assert_impl(bool cond, const char* message, const char* file, int line);

/**
 * Standard C assert() macro for compatibility with existing code
 * This maps to our NASA compliant implementation
 * 
 * Per SEI CERT ERR00-C: "Assert the validity of assumptions"
 * and NASA Power of Ten rule 10: "Assert critical invariants"
 */
#ifdef NDEBUG
/* When NDEBUG is defined, assert() becomes a no-op per standard C */
#define assert(cond) ((void)0)
#else
/* Our assert maps to the NASA-compliant implementation */
#define assert(cond) assert_impl((cond), #cond, __FILE__, __LINE__)
#endif

/**
 * Assert macro for checking invariants
 * Per NASA Power of Ten rule 10
 */
#ifdef DEBUG
#define ASSERT(cond, message) assert_impl((cond), (message), __FILE__, __LINE__)
#else
#define ASSERT(cond, message) ((void)0)
#endif

/**
 * Precondition check for function parameters
 * Per NASA Power of Ten rule 10
 */
#define PRECOND(cond, message) assert_impl((cond), "Precondition failed: " message, __FILE__, __LINE__)

/**
 * Postcondition check for function results
 * Per NASA Power of Ten rule 10
 */
#define POSTCOND(cond, message) assert_impl((cond), "Postcondition failed: " message, __FILE__, __LINE__)

/**
 * Invariant check for data structures
 * Per NASA Power of Ten rule 10
 */
#define INVARIANT(cond, message) assert_impl((cond), "Invariant failed: " message, __FILE__, __LINE__)

#endif /* LUGHOS_ASSERT_H */

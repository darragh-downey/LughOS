#ifndef SANDBOX_H
#define SANDBOX_H

/**
 * Sandbox Environment Header for LughOS
 *
 * Defines the interfaces for the sandbox testing environment
 * used to safely test updates before committing them.
 *
 * Complies with:
 * - SEI CERT ERR33-C: Detect errors and handle appropriately
 * - JPL Rule 14: Check return values
 * - NASA Rule 1: Simple control flow
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * Apply an update in a sandbox environment for testing
 * 
 * Creates an isolated test environment to run the update
 * to check for faults or security issues before committing.
 * 
 * @param image Pointer to the update binary 
 * @param size Size of the update binary
 * @return true if the update behaved correctly in sandbox
 */
bool sandbox_apply(const uint8_t *image, size_t size);

/**
 * Run validation tests on an updated component
 * 
 * @param path Path to the updated component
 * @return true if all tests passed
 */
bool run_tests(const char *path);

#endif /* SANDBOX_H */

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

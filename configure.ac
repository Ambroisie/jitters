# Disallow unexpanded macros
m4_pattern_forbid([^(AM|AC|AX|LT)_])

# Initialize the project name, version, and maitainer address
AC_INIT([jitters], [0.1-dev], [bruno@belanyi.fr])
# Only use C for this project
AC_LANG([C])

# Auxiliary files
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([build-aux/m4])

# Non-recursive Make build system
AM_INIT_AUTOMAKE([foreign subdir-objects
                  dist-bzip2 no-dist-gzip
                  silent-rules
                  -Wall -Werror])
# Use silent rules unless `make V=1` is used
AM_SILENT_RULES([yes])
AC_CONFIG_SRCDIR([src/jitters.c])
AC_CONFIG_FILES([Makefile])

# Activate release-mode when there is no dash in version
AX_IS_RELEASE([dash-version])
# Build in debug-mode by default, except when in release
AX_CHECK_ENABLE_DEBUG([yes])

# Standard C99 setup
AC_PROG_CC_C99
AC_PROG_CC_STDC

# Warnings
AX_COMPILER_FLAGS([], [], [], [-Wall -Wextra -pedantic])

# Use Flex/Lex
AM_PROG_LEX
# Use Bion/Yacc
AC_PROG_YACC
# Use libraries
AM_PROG_AR
AC_PROG_RANLIB

AC_OUTPUT

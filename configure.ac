# Disallow unexpanded macros
m4_pattern_forbid([^(AM|AC|AX|LT)_])

# Initialize the project name, version, and maitainer address
AC_INIT([jitters], [1.1], [bruno@belanyi.fr])
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
AC_CONFIG_HEADERS([src/config.h])

# Activate release-mode when there is no dash in version
AX_IS_RELEASE([dash-version])
# Build in debug-mode by default, except when in release
AX_CHECK_ENABLE_DEBUG([yes])

# Standard C99 setup
AC_PROG_CC_C99
AC_PROG_CC_STDC

# Warnings
AX_APPEND_COMPILE_FLAGS([-Wall -Wextra -pedantic])
AS_IF(
    [test "x$ax_enable_debug" = "xyes"],
    [AX_APPEND_COMPILE_FLAGS([-Werror])]
)

# Use Address-santizer in debug, if it is available
AS_IF(
    [test "x$ax_enable_debug" = "xyes"],
    [AX_APPEND_COMPILE_FLAGS([-fsanitize=address])]
)

# Use Flex/Lex
AM_PROG_LEX
# Use Bion/Yacc
AC_PROG_YACC
# Use libraries
AM_PROG_AR
AC_PROG_RANLIB

# We need GNU argp for argument parsing
AC_CHECK_HEADERS(
    [argp.h],
    [],
    [AC_MSG_ERROR([GNU Argp is needed to parse arguments])]
)

# Check for Criterion
PKG_CHECK_MODULES(
    [CRITERION],
    [criterion],
    [HAVE_CRITERION=1],
    [HAVE_CRITERION=0]
)
# Make a conditional
AM_CONDITIONAL([USING_CRITERION], [test "$HAVE_CRITERION" -eq 1])
# Substitute the defined flags
AM_COND_IF(
    [USING_CRITERION],
    [AC_SUBST([CRITERION_CFLAGS]) AC_SUBST([CRITERION_LIBS])]
)

# Use Autotools TAP driver
AC_REQUIRE_AUX_FILE([tap-driver.sh])

AC_OUTPUT

NULL =

# Extra files to include in 'make dist'
EXTRA_DIST = \
    bootstrap \
    $(NULL)

# Initialise those variables for use in included files
BUILT_SOURCES =
noinst_LIBRARIES =
# We want to remove those files when `clean` rule is being executed
CLEANFILES = $(BUILT_SOURCES)

include %D%/src/local.am
include %D%/tests/local.am

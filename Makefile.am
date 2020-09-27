NULL =
# Program and sources
bin_PROGRAMS = jitters
jitters_SOURCES = \
    src/jitters.c \
    $(NULL)

# Add warnings
jitters_CFLAGS = $(WARN_CFLAGS)
jitters_LDFLAGS = $(WARN_LDFLAGS)

# Extra files to include in 'make dist'
EXTRA_DIST = \
    bootstrap \
    $(NULL)

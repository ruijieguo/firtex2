#!/bin/sh

# Make sure that we are in the right directory.
if test ! -f configure.ac; then
    cat >&2 <<__EOF__

You must run bootstrap from the root of the distribution.

__EOF__
    exit 1
fi

if [ -n "$AUTOMAKE" ] || [ -n "$ACLOCAL" ] ; then
    if [ -z "$ACLOCAL" ] || [ -z "$AUTOMAKE" ] ; then
        _present="AUTOMAKE"
        _missing="ACLOCAL"

        [ -n "$ACLOCAL" ] && _present="ACLOCAL" && _missing="AUTOMAKE"

        cat >&2 <<__EOF__

[ERROR]: You are providing the path to ${_present}
through your environment but no reference to ${_missing}.
To fix this error please specify ${_missing} too.

As an example, if you are using automake-1.12 and have
available aclocal-1.12 you will want to do the following:

    AUTOMAKE="/usr/local/bin/automake-1.12" \\
    ACLOCAL="/usr/local/bin/aclocal-1.12"   \\
    ./bootstrap

Your current environment has:
    AUTOMAKE="$AUTOMAKE"
    ACLOCAL="$ACLOCAL"

__EOF__
        exit 1
    fi
else
    AUTOMAKE="$(which automake)"
fi

autoreconf --install -Wall --verbose "${@}"

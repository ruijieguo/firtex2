#!/bin/sh

FX_BOOST_PREFIX='--with-boost='
FX_THRIFT_PREFIX='--with-thrift='
FX_LIBEVENT_PREFIX='--with-libevent='
FX_DEBUG_PREFIX='--with-debug'

FX_BOOST_HOME=`pwd`/external
FX_LIBEVENT_HOME=`pwd`/external/libevent
FX_THRIFT_HOME=`pwd`/external/thrift
FX_DEBUG_BUILD=no

FX_BUILTIN_BOOST=yes
FX_BUILTIN_LIBEVENT=yes
FX_BUILTIN_THRIFT=yes

SHELL=/bin/sh

for i in $*
do
    if [ "${i:0:${#FX_BOOST_PREFIX}}" = "$FX_BOOST_PREFIX" ] ; then
	FX_BOOST_HOME=${i#*"$FX_BOOST_PREFIX"}
	FX_BUILTIN_BOOST=no
    fi
    if [ "${i:0:${#FX_LIBEVENT_PREFIX}}" = "$FX_LIBEVENT_PREFIX" ] ; then
	FX_LIBEVENT_HOME=${i#*"$FX_LIBEVENT_PREFIX"}
	FX_BUILTIN_LIBEVENT=no
    fi
    if [ "${i:0:${#FX_THRIFT_PREFIX}}" = "$FX_THRIFT_PREFIX" ] ; then
	FX_BUILTIN_THRIFT=no
    fi
    if [ "${i:0:${#FX_DEBUG_PREFIX}}" = "$FX_DEBUG_PREFIX" ] ; then
	FX_DEBUG_BUILD=yes
    fi
done
THRIFT_CPPFLAGS=""

if test x$FX_BUILTIN_THRIFT = xyes; then
    THRIFT_CPPFLAGS="-I$FX_LIBEVENT_HOME -I$FX_LIBEVENT_HOME/include -I$FX_LIBEVENT_HOME/include/event2 -I$FX_BOOST_HOME -I/usr/local"
    if test x$FX_DEBUG_BUILD != xno; then
	THRIFT_CPPFLAGS="$THRIFT_CPPFLAGS -g -D_DEBUG"
    fi
    THRIFT_LDFLAGS="-L$FX_LIBEVENT_HOME/.libs"

    AC_CONFIGURE_ARGS="--with-boost=$FX_BOOST_HOME '$THRIFT_CPPFLAGS' 'LDFLAGS=-L$FX_LIBEVENT_HOME/.libs' 'LD_LIBRARY_PATH=$FX_LIBEVENT_HOME/.libs'"
fi

if test x$FX_BUILTIN_LIBEVENT = xyes; then
    cd ./external/libevent
    echo "Entering directory "`pwd`
    ./configure $*
    echo "Compiling in directory"`pwd`
    make -j2
    echo "Leaving directory "`pwd`
    cd ../../
fi

if test x$FX_BUILTIN_THRIFT = xyes; then
    cd ./external/thrift
    echo "Entering directory "`pwd`
    OLD_CPPFLAGS=$CPPFLAGS
    OLD_LDFLAGS=$LDFLAGS
    export CPPFLAGS="$THRIFT_CPPFLAGS"
    export LDFLAGS="$THRIFT_LDFLAGS"
    ${SHELL} ./configure --disable-option-checking --with-boost=$FX_BOOST_HOME
    echo "Compiling in directory"`pwd`
    make -j2
    echo "Leaving directory "`pwd`
    export CPPFLAGS=$OLD_CPPFLAGS
    export LDFLAGS=$OLD_LDFLAGS
    cd ../../
fi

# compile main code
echo "Entering directory "`pwd`
libtoolize --force
aclocal
autoheader
automake -a
autoconf

# compile contrib
cd ./contrib
echo "Entering directory "`pwd`
/bin/sh ./bootstrap.sh $*
echo "Leaving directory "`pwd`
cd ../

./configure $*

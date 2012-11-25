#!/bin/sh

libtoolize --force
aclocal
autoheader
automake -a
autoconf

#! /bin/sh
# 
# Simple script to rebuild the "auto" configurations

aclocal                     \
&& autoheader               \
&& automake --add-missing   \
&& autoconf

/bin/rm -r autom4te.cache

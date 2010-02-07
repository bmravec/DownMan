#!/bin/bash

autoheader && aclocal && libtoolize --copy --force && autoconf && automake --add-missing --copy

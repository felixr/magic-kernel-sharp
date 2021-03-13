# John Costella's Magic Kernel Sharp

This repository is a copy of John Costella's reference implementation of his
Magic Kernel Sharp. See http://www.johncostella.com/magic

## Building 

`make` will make the `default` target which includes debug executables in
`build/dbg/bin`.

To make optimized executables, use `make all`; these will be in `build/opt/bin`.

`make test` will run unit tests in debug mode; use `make otest` for optimized.

If you get errors in linking, note that a few standard libraries need to be
installed, which vary for different distros (google if you get stuck):

- For errors with -lz, you need to install something like zlib, zlib-dev, or
  zliblg-dev.
- For errors with -lpng, you need to install something like libpng or
  libpng-devel.
- For errors with -ljpeg, you need to install something like libjpeg or
  libjpeg-devel.
- You should not get errors with -lpthread on any distro.

For information about the "jpC" paradigm used in this code, see lib/jpc.h.

All code is in ANSI C (C89).

## License

Distributed under the MIT License. See `LICENSE` for more information.


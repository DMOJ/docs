# Colored Compiler Messages

The DMOJ judge component understands ANSI codes, and can display them in HTML form inside submission status pages for more accessible compiler errors and messages.

![colored compiler messages in action](https://sc-cdn.scaleengine.net/i/c1b33b24338f680a6d2356b0c734789c.png)

This document assumes you have an environment variable `$JUDGE_HOME` set to the home directory of the judge user.

## Enabling Colors in GCC 4.9+

GCC has native support for message coloring since version 4.9. It can be enabled by passing the flag `-fdiagnostics-color=always` to the GCC executable. You can enable this for the judge user with an alias.

```sh
# Change the judge user .bashrc to /etc/bash.bashrc if you desire colors to be enabled globally
echo 'alias gcc=gcc -fdiagnostics-color=always' >> $JUDGE_HOME/.bashrc
```

## Enabling Colors in GCC < 4.9

Colors can be enabled in GCC versions prior to 4.9 through the `colorgcc` package. If `colorgcc` doesn't work for you, there are alternative packages out there that accomplish a similar result.

```sh
sudo apt-get install colorgcc
```

Then, specify the `colorgcc` executable in place of the GCC executors in the judge configuration file. `colorgcc` will determine which compiler to use based on the file extension of the file being compiled.

### Using `colorgcc` alongside a GCC 4.9+ installation

It may be the case that you have multiple GCC versions installed, and that `colorgcc` will attempt to colorize GCC 4.9+ output instead of GCC < 4.9. By default, `colorgcc` uses the GCC executables in `$PATH`, but this behaviour can be overriden through a `.colorgccrc` file placed in the judge `$HOME`.

```sh
echo 'color-g++: /usr/bin/g++' >> $JUDGE_HOME/.colorgccrc
echo 'color-gcc: /usr/bin/gcc' >> $JUDGE_HOME/.colorgccrc
echo 'color-c++: /usr/bin/c++' >> $JUDGE_HOME/.colorgccrc
echo 'color-cc:  /usr/bin/cc'  >> $JUDGE_HOME/.colorgccrc
```

This will allow you to specify, for example, `color-g++` in the judge configuration file for a pre-4.9 GCC.

## Clang

Clang has native coloring support, enabled by default.

## Colors for Other Languages

Got colors working for other languages? We'd love to know! Send us a pull request, or open a Github issue.

# Judge Configuration
The DMOJ judge is configured with a YAML file, which contains the runtimes, problems folders, and other information.

A sample configuration file is available [here](https://github.com/DMOJ/docs/blob/master/sample_files/judge_conf.yml).

## Runtimes

The runtimes are configured with a `runtime` node. While most runtimes can be automatically configured with `dmoj-autoconf`, those that are not in the `$PATH` variable will not be automatically detected, and will need to be manually configured.

## Problems

The problems are configured with `problem_storage_root`. There are three ways to configure this node:

1. A single string: all grandchildren directories which include an `init.yml` will be treated as a problem directory.
2. A list: all children of each element in the list will be treated as a problem directory.
3. A dictionary: the key is an integer and the value is a string. The n-th level children of the string will be considered problem directories, where `n` is the entry's key.

## ID

The judge's display name is configured with an `id` node. This is a string, and it should match the one on the site interface.

## Key

This key is used to validate your judge's connection to the bridge, and as such, should match the one on the site interface. This is configured with a `key` node.

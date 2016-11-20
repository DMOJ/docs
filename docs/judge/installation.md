This installation guide is for Linux based machines (WSL included). For installation on Windows click [here](windows_installation).

## Installing the prerequisite

```
$ sudo apt install git python-dev python-pip build-essential
$ sudo pip install cython
$ git clone https://github.com/DMOJ/judge
$ cd judge
$ sudo python setup.py develop
```

## Making the configuration file

Start by taking the `runtime` block from the output of the command `python -m dmoj.executors.autoconfig` and put it in a new file `config.yml`. Now add a `problem_storage_root` option where you specify where your problem data is located. Here is how your configuration should look like:

```
problem_storage_root: /judge/problems
runtime:
   ...
```

Now you should be able to run `dmoj-cli -c config.yml`. For additional configuration options, look at the example available [here](https://github.com/DMOJ/docs/blob/master/sample_judge_conf.yml).

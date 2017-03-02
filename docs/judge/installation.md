This installation guide is for Linux based machines (WSL included). For installation on, Windows see the [Windows install instructions](windows_installation).

## Installing the prerequisite

```
$ sudo apt install git python-dev python-pip build-essential
$ sudo pip install cython
$ git clone https://github.com/DMOJ/judge
$ cd judge
$ sudo python setup.py develop
```

## Making the configuration file

Start by taking the `runtime` block from the output of the command `python -m dmoj.executors.autoconfig` and put it in a new file `config.yml`. Next, add a `problem_storage_root` node where you specify where your problem data is located. 

```
problem_storage_root: /judge/problems
runtime:
   ...
```

You should now be able to run `dmoj-cli -c config.yml` to enter a CLI environment for the judge. For additional configuration options, an [example configuration file](https://github.com/DMOJ/docs/blob/master/sample_judge_conf.yml) is provided.

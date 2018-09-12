This installation guide is for Linux based machines (WSL included). For installation on Windows, see the [Windows install instructions](windows_installation).

## Installing the prerequisites

```
$ apt install git python-dev python-pip build-essential
$ git clone https://github.com/DMOJ/judge
$ cd judge
$ pip install -r requirements.txt
$ python setup.py develop
```

## Configuring the judge

To start, you should set up a `.dmojrc` file containing executor information.

```bash
$ dmoj-autoconf > ~/.dmojrc
```

Next, edit the file to add a `problem_storage_root` node where you specify where your problem data is located. 

```yaml
problem_storage_root:
  - /judge/problems
runtime:
   ...
```

You should now be able to run `dmoj-cli` to enter a CLI environment for the judge. If you used a different path than `~/.dmojrc`, you may specify it with the `-c` flag. For additional configuration options, an [example configuration file](https://github.com/DMOJ/docs/blob/master/sample_files/judge_conf.yml) is provided.

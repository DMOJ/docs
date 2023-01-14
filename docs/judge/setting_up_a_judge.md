# Setting up a judge

This guide goes through the process of installing a judge and connecting it to
the site. It is intended for Linux-based machines (WSL included); Windows is
not supported.

It is assumed that the site installation instructions have been followed, and
that a bridge instance is running.

## Site-side setup

First, add a judge on the admin page, located under
[/admin/judge/](https://dmoj.ca/admin/judge/). Provide the
name of the judge and the authentication key for the judge. You may use the
`Regenerate` button to generate an authentication key.

In `local_settings.py`, find the `BRIDGED_JUDGE_ADDRESS`. This is the address
you will be connecting a judge to. By default, this should be `localhost:9999`.
If you are connecting from a different machine, you will need to change
`localhost` to an actual IP. Also, ensure that this port is **open**, or you
will receive cryptic error messages when attempting to connect a judge.

Finally, ensure the bridge is running. You should see something similar to the
following lines.

```shell-session
$ supervisorctl status
bridged RUNNING pid <pid>, uptime <uptime>
```

## Judge-side setup

The DMOJ supports installing judges through Docker and a PyPI package. We
recommend the Docker installation if you are able to use Docker, since we have
dealt with the hard problem of getting many runtimes co-existing on the same
machine and keeping them up-to-date. The PyPI package is also supported, and
may give you more control at the expense of more administrative complexity.

### With Docker

We maintain Docker images with all runtimes we support in the
[runtimes-docker](https://github.com/DMOJ/runtimes-docker) project.

Runtimes are split into three tiers of decreasing support. Tier 1 includes
Python 2/3, C/C++ (GCC only), Java 8, and Pascal. Tier 3 contains all the
runtimes we run on [dmoj.ca](https://dmoj.ca). Tier 2 contains some in-between
mix; read the `Dockerfile` for each tier for details. These images are rebuilt
and tested every week to contain the latest runtime versions.

The session below starts a CLI instance you can use to test the judge before
connecting it to the site. It expects problems to be placed on the host under
`/mnt/problems`, and judge-specific configuration to be in
`/mnt/problems/judge.yml`.

```shell-session
$ git clone --recursive https://github.com/DMOJ/judge.git
$ cd judge/.docker
$ make judge-tier1
$ docker run \
    -v /mnt/problems:/problems \
    --cap-add=SYS_PTRACE \
    dmoj/judge-tier1:latest \
    cli -c /problems/judge.yml
```

The session below spawns a tier 1 judge image. It expects the relevant
environment variables to be set, and the network device to be `enp1s0`. You can
modify this to suit your installation requirements.

```shell-session
$ docker run \
    --name judge \
    -p "$(ip addr show dev enp1s0 | perl -ne 'm@inet (.*)/.*@ and print$1 and exit')":9998:9998 \
    -v /mnt/problems:/problems \
    --cap-add=SYS_PTRACE \
    -d \
    --restart=always \
    dmoj/judge-tier1:latest \
    run -p "$PORT" -c /problems/judge.yml \
    "$IP" "$JUDGE_NAME" "$JUDGE_AUTHENTICATION_KEY"
```

`$PORT` and `$IP` should be the port and IP that was specified in
`BRIDGED_JUDGE_ADDRESS` of the site's `local_settings.py`.

`$JUDGE_NAME` is the judge name as specified in the site, and
`$JUDGE_AUTHENTICATION_KEY` is the judge authentication key.

### Through PyPI

#### Installing the prerequisites

```shell-session
$ apt install python3-dev python3-pip build-essential libseccomp-dev
$ pip3 install dmoj
```

#### Configuring the judge

Start by taking the `runtime` block from the output of the command
`dmoj-autoconf` and put it in a new file `judge.yml`. Next, add a
`problem_storage_root` node where you specify where your problem data is
located. Your `judge.yml` file should look something like below.

```yaml
id: <judge name>
key: <judge authentication key>
problem_storage_root:
  - /mnt/problems
runtime:
   ...
```

You should now be able to run `dmoj-cli -c judge.yml` to enter a CLI
environment for the judge. For additional configuration options, an [example
configuration
file](https://github.com/DMOJ/docs/blob/master/sample_files/judge_conf.yml) is
provided.

You should now be able to connect the judge to the site.

```shell-session
$ dmoj -c judge.yml -p "$PORT" "$IP"
```

`$PORT` and `$IP` should be the port and IP that was specified in
`BRIDGED_JUDGE_ADDRESS` of the site's `local_settings.py`. If the port is 9999,
you can exclude the `-p PORT` argument, as the judge will default to port 9999.

If you are using the defaults in `local_settings.py`, the following command
should suffice.

```shell-session
$ dmoj -c judge.yml localhost
```

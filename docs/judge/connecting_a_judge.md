# Connecting a Judge to the Site

This guide is for connecting a judge to the site.

## Site-Side

First, add a judge on the admin page, located at [https://dmoj.ca/admin/judge](https://dmoj.ca/admin/judge). Provide the name of the judge and the authentication key for the judge. You may use the `Regenerate` button to generate an authentication key.

In `local_settings.py`, find the `BRIDGED_JUDGE_ADDRESS`. This is the address you will be connecting a judge to. By default, this should be `localhost:9999`. If you are connecting from a different machine, you will need to change `localhost` to an actual IP. Also, ensure that this port is **open**, or you will receive cryptic error messages when attempting to connect a judge.

Finally, ensure the bridge is running. You should see something similar to the following lines.
```shell-session
$ supervisorctl status
bridged RUNNING pid <pid>, uptime <uptime>
```

## Judge-Side

### With Docker

We maintain Docker images with all runtimes we support in the [runtimes-docker](https://github.com/DMOJ/runtimes-docker) project.

Runtimes are split into three tiers of decreasing support. Tier 1 includes
Python 2/3, C/C++ (GCC only), Java 8, and Pascal. Tier 3 contains all the
runtimes we run on [dmoj.ca](https://dmoj.ca). Tier 2 contains some in-between
mix; read the `Dockerfile` for each tier for details. These images are rebuilt
and tested every week to contain the latest runtime versions.

The script below spawns a tier 1 judge image. It expects the relevant
environment variables to be set, the network device to be `enp1s0`, problems
to be placed under `/mnt/problems`, and judge-specific configuration to be in
`/mnt/problems/judge.yml`.

```shell-session
$ git clone --recursive https://github.com/DMOJ/judge.git
$ cd judge/.docker
$ make judge-tier1
$ exec docker run \
    --name judge \
    -p "$(ip addr show dev enp1s0 | perl -ne 'm@inet (.*)/.*@ and print$1 and exit')":9998:9998 \
    -v /mnt/problems:/problems \
    --cap-add=SYS_PTRACE \
    -d \
    --restart=always \
    dmoj/judge-tier1:latest \
    run -p "$PORT" -s -c /problems/judge.yml \
    "$IP" "$JUDGE_NAME" "$JUDGE_AUTHENTICATION_KEY"
```

`$PORT` is the bridge port, `$IP` is the bridge IP, `$JUDGE_NAME` is the judge name, and `$JUDGE_AUTHENTICATION_KEY` is the judge authentication key.

### Directly on the machine

Ensure you have [installed and configured the judge](/judge/linux_installation.md).

Add the following yaml block to your `config.yml`.
```yaml
id: judge name
key: judge authentication key
```

Connect to the site.
```shell-session
$ dmoj -c config.yml -p "$PORT" "$IP"
```
`$PORT` and `$IP` should be the port and IP that was specified in `BRIDGED_JUDGE_ADDRESS`. If the port is 9999, you can exclude the `-p PORT` argument, as the judge will default to port 9999.

If you are using the defaults in `local_settings.py`, the following command should suffice.
```shell-session
$ dmoj -c config.yml localhost
```

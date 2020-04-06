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

Ensure you have [installed and configured the judge](/judge/linux_installation.md).

Add the following yaml block to your `config.yml`.
```yaml
id: judge name
key: judge authentication key
```

Connect to the site.
```shell-session
$ dmoj -c config.yml -p PORT IP
```
`PORT` and `IP` should be the port and IP that was specified in `BRIDGED_JUDGE_ADDRESS`. If the port is 9999, you can exclude the `-p PORT` argument, as the judge will default to port 9999.

If you are using the defaults in `local_settings.py`, the following command should suffice.
```shell-session
$ dmoj -c config.yml localhost
```

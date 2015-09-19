## SSL Proxying for User Content

User-generated content (e.g., comments) poses a threat to site security, and may cause mixed-content warnings. If your site is served over HTTPS, this may be suboptimal - routing user content through an HTTPS server can help.

The DMOJ site provides support for this through the [Github Camo](https://github.com/atmos/camo) project.

## Requirements

* CoffeeScript (`apt-get install coffeescript`)

## Installing Camo to /code

```shell
$ cd /code
$ git clone https://github.com/atmos/camo.git camo
```

Now, Camo may be started by running `/code/camo/server.coffee`.

```shell
$ PORT="<port>" CAMO_KEY="<key>" coffee /code/camo/server.coffee
```

* Camo will listen on `<port>`.
* `<key>` is the HMAC secret key used for digests. Set it to anything you want.

## Configuring DMOJ to use Camo

To enable the use of Camo in the DMOJ site, you need to specify a couple of variables in your `local_settings.py`.

```python
# The URL on which Camo is listening
CAMO_URL = "https://example.com:<port>"
# The key you specified for running Camo
CAMO_KEY = "<key>"
# Domains to exclude from Camo proxying. Typically, these would be your own domains which you use
# for content delivery, and you know to already be secure.
CAMO_EXCLUDE = ("https://dmoj.ml", "https://dmoj.ca")
# Whether Camo should use HTTPS (you probably want this)
CAMO_HTTPS = True
```

Restart DMOJ for the changes to take effect. After restarting, you may have to purge Django's cache before seeing any changes.

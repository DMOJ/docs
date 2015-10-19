This guide is intended for Linux machines, specifically those running Debian or derivatives (e.g. Ubuntu). Your mileage may vary with other distros.

# Requirements
* Python 2
* `virtualenv` (`pip install virtualenv`)
* `pip`
* `git`
* MySQL or MariaDB database in CentOS 7
* Ruby and `gem`
* NodeJS and `npm`
* RabbitMQ server (`apt-get install rabbitmq-server`)

## Step 1
### Clone the repository and create a virtualenv for the site
This step is simple. Choose a location to install the site to, clone the repository and create a virtualenv. In this document, we'll install the site to `/code/site/`.

```sh
$ mkdir /code/
$ cd /code/
$ git clone https://github.com/DMOJ/site.git
$ virtualenv site
$ cd /code/site/
$ git submodule update --init
$ source bin/activate
```

## Step 2
### Installing the requirements
This step is a bit tricky, since usually you won't have all the required libraries already installed.

```sh
$ pip install -r requirements.txt
```

If, for example, you are missing `Python.h`, you should run `apt-get install python-dev` to fix the error. In this step, Google is your best friend to resolving errors.

## Step 3
### Local settings for Django
It's recommended that you add your settings in `dmoj/local_settings.py` rather than modifying `dmoj/settings.py` directly. `settings.py` will automatically read `local_settings.py` and `exec` it, so write your configuration there.

Below is a template of what `local_settings.py` should contain. You may peruse `settings.py` for a list of possible configuration options.

```python
SITE_NAME = 'DMOJ'
SITE_LONG_NAME = 'Don Mills Online Judge'
SITE_ADMIN_EMAIL = 'admin@dmoj.ca'

ACE_URL = '//dmoj.ml/ace/'

# COMPRESS_ROOT != STATIC_ROOT
COMPRESS_ROOT = '/root/arena/site/compress'

INSTALLED_APPS += (
    'django_select2',
)

# Add database credentials
DATABASES = {
     'default': {
        'ENGINE': 'django.db.backends.mysql',
        'NAME': 'dmoj',
        'USER': 'root',
        'PASSWORD': 'pass',
        'HOST': '127.0.0.1',
    }
}

BRIDGED_JUDGE_HOST = '0.0.0.0'
BRIDGED_DJANGO_HOST = '0.0.0.0'

DEFAULT_USER_TIME_ZONE = 'Europe/Bucharest'

# Add cache. You can use memcached.
CACHES = {
    'default': {
        'BACKEND': 'django.core.cache.backends.filebased.FileBasedCache',
        'LOCATION': '/var/tmp/django_cache',
    }
}
```
You can customize this template to your liking.

<!--*TODO*: wkhtmltopdf installation instructions.-->

## Step 4
### Installing RabbitMQ
The DMOJ uses the RabbitMQ AMQP framework to communicate with the event server (for live updates) and bridge server (for judging).

First, install `rabbitmq-server`.

```sh
$ apt-get install rabbitmq-server
```

Now, we need to enable the administration panel so we can add a user for DMOJ.

```
$ rabbitmq-plugins enable rabbitmq_management
```

This will start the administration panel on `<host>:15672`. In your browser, log in with the user `guest` and the password `guest`. Navigate to the *Admin* view, add a vhost, and create a privileged user on it. After doing so, you should change the password of the guest account.

Finally, we need to tell DMOJ about the RabbitMQ vhost. We can do so by setting `JUDGE_AMQP_PATH` in `local_settings.py` to `amqp://user:password@host:port/vhost`.

## Step 5
### Compiling the SASS-y stylesheets

Compiling the CSS from SASS is a fairly painless procedure. For added cross-browser compatibility, we also run the generated CSS through Pleeease through the script `make_style.sh`. You'll need to install both SASS and Pleeease before proceeding.

```sh
$ gem install sass
$ npm install -h pleeease-cli
```

Then, run `./make_style.sh`, located in `/code/site`.

## Step 6 and more
*TODO*: Run project, and add a reverse proxy (NGINX).

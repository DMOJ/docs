# Installing the DMOJ Site   
This tutorial is based on my experience installing DMOJ Django based site. This guide is intended for linux machines. Might work on other systems, but I offer no guarantee.   
This tutorial is supposed to work on a Ubuntu machine.
I will use `<text>` as a command for your preferred text editor. This can be vim, nano etc.

## Requirements
* Python 2
* `virtualenv` (`pip install virtualenv`)
* `pip`
* `git`
* MySQL or MariaDB database in CentOS 7
* Ruby and `gem`
* NodeJS and `npm`
* RabbitMQ server (`apt-get install rabbitmq-server`)

## Step 1 - create a directory and virtualenv for the site
This step is simple. Choose a location to install the site to, and export this location to an environment variable for easier access later on. In this document, we'll install the site to `/code/site/`.

```sh
$ mkdir /code/
$ virtualenv site
$ export DMOJ_HOME=/code/site/
$ cd /code/site/
$ source /bin/activate
```

The command line should look like `(env)walle256@server ~:`

## Step 2 - cloning the repository
Now we need to clone the latest DMOJ source files. Make sure you are in `/code/site/` for the following commands.

```sh
$ git clone https://github.com/DMOJ/site.git .
$ git submodule update --init
```

## Step 3 - installing the requirements
This step is a bit tricky, since usually you won't have all the required libraries.

```sh
$ pip install -r requirements.txt
```

If, for example, you are missing `Python.h`, you should run `apt-get install python-dev` to fix the error. In this step, Google is your best friend to resolving errors.

## Step 4 - local settings for Django
It's recommended that you add your settings in `dmoj/local_settings.py` rather than modifying `dmoj/settings.py` directly. `settings.py` will automatically read `local_settings.py` and `exec` it, so write your configuration there.

Below is a template of what `local_settings.py` should contain.

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

## Step 5
*TL:DR*:
* Install RabbitMQ
* Create a user and a vhost for the site.
* Set `JUDGE_AMQP_PATH` to `amqp://user:password@host:port/vhost`

Not *TL:DR*       
1. Start by enabling the management plugin. `rabbitmq-plugins enable rabbitmq_management`.    
2. Open your browser and go to: `host:15672`.     
3. Login with user guest and password guest.     
4. Now go to Admin, and add a vhost.     
5. Then create a user with rights on that vhost.       
6. (Optional) Change the password of the guest account.       
7. Now set `JUDGE_AMQP_PATH` to `amqp://user:password@host:port/vhost`.       

## Step 6

For building the CSS, you need to install SASS 'gem install sass' and Pleeease 'npm install -g pleeease-cli'.
Now run `./make_style.sh`.

#Step 8 and more
*TODO*: Run project, and add a reverse proxy (NGINX).

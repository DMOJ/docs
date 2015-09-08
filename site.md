#Notes   
This tutorial is based on my experience installing DMOJ Django based site. This guide is intended for linux machines. Might work on other systems, but I offer no guarantee.   
This tutorial is supposed to work on a Ubuntu machine.
I will use <text> as a command for your preferred text editor. This can be vim, nano etc.

#Requirements
* Python 2
* pip
* git
* Mysql or MariaDB in CentOS 7
* Ruby and gem
* NodeJS and npm

#Step 1
Install virtualenv by using the following command 'sudo pip install virtualenv'

#Step 2
Create a virtualenv by using the following command 'virtualenv env'.
This environment will be enabled using the following command in the folder you ran 'virtualenv env': 'source env/bin/activate'

Now the command line should look like this:

```
(env)walle256@server ~:
```

#Step 3
Run: 'git clone https://github.com/DMOJ/site', 'cd site', and then 'git submodule update --init'

#Step 4
Run: 'pip install -r requirements.txt'
This step is a bit tricky, usually, you don't have all the libs that are required, but, for example if you don't have 'Python.h', run 'apt-get install python-dev'.
In this step, Google is your best friend.

#Step 5
It's recommended that you add your settings in 'dmoj/local_settings.py', so start by writing '<text> dmoj/local_settings.py'.
I will give you a template of what should local_settings.py should contain.

```python
SITE_NAME = 'DMOJ'
ACE_URL = '//dmoj.ml/ace/'

#This is where problem PDFs are created.
PROBLEM_PDF_CACHE = "/root/arena/site/pdf/"
WEBKIT_PDF = True

#COMPRESS_ROOT != STATIC_ROOT
COMPRESS_ROOT = '/root/arena/site/compress'

INSTALLED_APPS += (
    'django_select2',
)

#Add database credentials
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

#Add cache. You can use memcached.
CACHES = {
    'default': {
        'BACKEND': 'django.core.cache.backends.filebased.FileBasedCache',
        'LOCATION': '/var/tmp/django_cache',
    }
}
```
You can customize this template to your liking.

#Step 6
*TODO*: Add RabbitMQ.

#Step 7

For building the CSS, you need to install SASS 'gem install sass' and Pleeease 'npm install -g pleeease-cli'.
Now run './make_style.sh'.

#Step 8 and more
*TODO*: Run project, and add a reverse proxy (NGINX).

# Installing the site

## Installing the prerequisites

```shell-session
$ apt update
$ apt install git gcc g++ make python3-dev python3-pip libxml2-dev libxslt1-dev zlib1g-dev gettext curl redis-server
$ curl -sL https://deb.nodesource.com/setup_18.x | sudo -E bash -
$ apt install nodejs
$ npm install -g sass postcss-cli postcss autoprefixer
```

## Creating the database

Next, we will set up the database using MariaDB. The DMOJ is only tested to work with MySQL, and it is unlikely to work with anything else. Please visit [the MariaDB site](https://mariadb.org/download/?t=repo-config) and follow the download instructions.

When asked, you should select the latest MariaDB version.

```shell-session
$ apt update
$ apt install mariadb-server libmysqlclient-dev
```

The next step is to set up the database itself. You should execute the commands listed below to create the necessary database and user.

```shell-session
$ sudo mysql
mariadb> CREATE DATABASE dmoj DEFAULT CHARACTER SET utf8mb4 DEFAULT COLLATE utf8mb4_general_ci;
mariadb> GRANT ALL PRIVILEGES ON dmoj.* TO 'dmoj'@'localhost' IDENTIFIED BY '<mariadb user password>';
mariadb> exit
```

## Installing prerequisites

Now that you are done, you can start installing the site. First, create a virtual environment and activate it. Here, we'll create a virtual environment named `dmojsite`.

```shell-session
$ python3 -m venv dmojsite
$ . dmojsite/bin/activate
```

You should see `(dmojsite)` prepended to your shell. Henceforth, `(dmojsite)` commands assume you are in the code directory, with the virtual environment active.

?> The virtual environment will help keep the modules needed separate from the system package manager, and save you many headaches when updating. Read more about virtual environments [here](https://docs.python.org/3/tutorial/venv.html).

Now, fetch the site source code. If you plan to install a judge [from PyPI](https://pypi.org/project/dmoj/), check out a matching version of the site repository. For example, for judge v4.0.0:

```shell-session
(dmojsite) $ git clone https://github.com/DMOJ/site.git
(dmojsite) $ cd site
(dmojsite) $ git checkout v4.0.0  # only if planning to install a judge from PyPI, otherwise skip this step
(dmojsite) $ git submodule init
(dmojsite) $ git submodule update
```

Install Python dependencies into the virtual environment.

```shell-session
(dmojsite) $ pip3 install -r requirements.txt
(dmojsite) $ pip3 install mysqlclient
```

You will now need to configure `dmoj/local_settings.py`. You should make a copy of [this sample settings file](https://github.com/DMOJ/docs/blob/master/sample_files/local_settings.py) and read through it, making changes as necessary. Most importantly, you will want to update MariaDB credentials.

?>  Leave debug mode on for now; we'll disable it later after we've verified that the site works. <br> <br>
    Generally, it's recommended that you add your settings in `dmoj/local_settings.py` rather than modifying `dmoj/settings.py` directly. `settings.py` will automatically read `local_settings.py` and load it, so write your configuration there.

Now, you should verify that everything is going according to plan.

```shell-session
(dmojsite) $ python3 manage.py check
```

## Compiling assets

The DMOJ uses `sass` and `autoprefixer` to generate the site stylesheets. The DMOJ comes with a `make_style.sh` script that may be run to compile and optimize the stylesheets.

```shell-session
(dmojsite) $ ./make_style.sh
```

Now, collect static files into `STATIC_ROOT` as specified in `dmoj/local_settings.py`.

```shell-session
(dmojsite) $ python3 manage.py collectstatic
```

You will also need to generate internationalization files.

```shell-session
(dmojsite) $ python3 manage.py compilemessages
(dmojsite) $ python3 manage.py compilejsi18n
```

## Setting up database tables

We must generate the schema for the database, since it is currently empty.

```shell-session
(dmojsite) $ python3 manage.py migrate
```

Next, load some initial data so that your install is not entirely blank.

```shell-session
(dmojsite) $ python3 manage.py loaddata navbar
(dmojsite) $ python3 manage.py loaddata language_small
(dmojsite) $ python3 manage.py loaddata demo
```

!>  Keep in mind that the `demo` fixture creates a superuser account with a username and password of `admin`. If your
    site is exposed to others, you should change the user's password or remove the user entirely.

You should create an admin account with which to log in initially.

```shell-session
(dmojsite) $ python3 manage.py createsuperuser
```

## Setting up Celery

The DMOJ uses Celery workers to perform most of its heavy lifting, such as batch rescoring submissions. We will use Redis as its broker, though note that other brokers that Celery supports will work as well.

Start up the Redis server, which is needed by the Celery workers.

```shell-session
$ service redis-server start
```

Configure `local_settings.py` by uncommenting `CELERY_BROKER_URL` and `CELERY_RESULT_BACKEND`. By default, Redis listens on localhost port 6379, which is reflected in `local_settings.py`. You will need to update the addresses if you changed Redis's settings.

We will test that Celery works soon.

## Running the server

At this point, you should attempt to run the server, and see if it all works.

```shell-session
(dmojsite) $ python3 manage.py runserver 0.0.0.0:8000
```

You should Ctrl-C to exit after verifying.

!>  **Do not use `runserver` in production!** <br> <br>
    We will set up a proper webserver using nginx and uWSGI soon.

You should also test to see if `bridged` runs.

```shell-session
(dmojsite) $ python3 manage.py runbridged
```

If there are no errors after about 10 seconds, it probably works.
You should Ctrl-C to exit.

Next, test that the Celery workers run.

```shell-session
(dmojsite) $ celery -A dmoj_celery worker
```

You can Ctrl-C to exit.

## Setting up uWSGI

`runserver` is insecure and not meant for production workloads, and should not be used beyond testing.
In the rest of this guide, we will be installing `uwsgi` and `nginx` to serve the site, using `supervisord`
to keep `site` and `bridged` running. It's likely other configurations may work, but they are unsupported.

First, copy our `uwsgi.ini` ([link](https://github.com/DMOJ/docs/blob/master/sample_files/uwsgi.ini)). You should change the paths to reflect your install.

You need to install `uwsgi`.

```shell-session
(dmojsite) $ pip3 install uwsgi
```

To test, run:

```shell-session
(dmojsite) $ uwsgi --ini uwsgi.ini
```

If it says workers are spawned, it probably works.
You should Ctrl-C to exit.

## Setting up supervisord

You should now install `supervisord` and configure it.

```shell-session
$ apt install supervisor
```

Copy our `site.conf` ([link](https://github.com/DMOJ/docs/blob/master/sample_files/site.conf)) to `/etc/supervisor/conf.d/site.conf`, `bridged.conf` ([link](https://github.com/DMOJ/docs/blob/master/sample_files/bridged.conf)) to `/etc/supervisor/conf.d/bridged.conf`, `celery.conf` ([link](https://github.com/DMOJ/docs/blob/master/sample_files/celery.conf)) to `/etc/supervisor/conf.d/celery.conf` and fill in the fields.

Next, reload `supervisord` and check that the site, bridged, and celery have started.

```shell-session
$ supervisorctl update
$ supervisorctl status
```

If all three processes are running, everything is good! Otherwise, peek at the logs and see what's wrong.

## Setting up nginx

Now, it's time to set up `nginx`.

```shell-session
$ apt install nginx
```

You should copy the sample `nginx.conf` ([link](https://github.com/DMOJ/docs/blob/master/sample_files/nginx.conf)), edit it and place it in wherever it is supposed to be for your nginx install.

?>  Typically, `nginx` site files are located in `/etc/nginx/conf.d`.
    Some installations might place it at `/etc/nginx/sites-available` and require a symlink in `/etc/nginx/sites-enabled`.

Next, check if there are any issues with your nginx setup.

```shell-session
$ nginx -t
```

If not, reload the `nginx` configuration.

```shell-session
$ service nginx reload
```

You should be good to go. Visit the site at where you set it up to verify.

If it does not work, check `nginx` logs and `uwsgi` log `stdout`/`stderr` for details.

?> Now that your site is installed, remember to set `DEBUG` to `False` in `local_settings`. Leaving it `True` is a security risk.

## Configuration of event server

Create `config.js`. This assumes you use `nginx`, or there be dragons.
You may need to shuffle ports if they are already used.

```shell-session
(dmojsite) $ cat > websocket/config.js
module.exports = {
    get_host: '127.0.0.1',
    get_port: 15100,
    post_host: '127.0.0.1',
    post_port: 15101,
    http_host: '127.0.0.1',
    http_port: 15102,
    long_poll_timeout: 29000,
};
```

`get_port` should be the same as the port for `/event/` in `nginx.conf`.
`http_port` should be the same as the port for `/channels/` in `nginx.conf`.
`post_port` should be the same as the port in `EVENT_DAEMON_POST` in `local_settings`.
You need to configure `EVENT_DAEMON_GET` and `EVENT_DAEMON_POLL`.
You need to uncomment the relevant section in the `nginx` configuration.

Need to install the dependencies.

```shell-session
(dmojsite) $ npm install qu ws simplesets
(dmojsite) $ pip3 install websocket-client
```

Now copy `wsevent.conf` ([link](https://github.com/DMOJ/docs/blob/master/sample_files/wsevent.conf)) to `/etc/supervisor/conf.d/wsevent.conf`, changing paths, and then update supervisor and nginx.

```shell-session
$ supervisorctl update
$ supervisorctl restart bridged
$ supervisorctl restart site
$ service nginx restart
```

## Installing the prerequisites

```
$ sudo apt install git gcc g++ make python-dev libxml2-dev libxslt1-dev zlib1g-dev ruby-sass gettext curl
$ wget -q --no-check-certificate -O- https://bootstrap.pypa.io/get-pip.py | sudo python
$ sudo pip install virtualenv
$ wget -O- https://deb.nodesource.com/setup_4.x | sudo -E bash -
$ sudo apt install nodejs
$ sudo npm install -g pleeease-cli
```

## Creating the database

Next, we will set up the database using MySQL. The DMOJ is only tested to work with MySQL, and it is unlikely to work with anything else.

```
$ wget http://dev.mysql.com/get/mysql-apt-config_0.6.0-1_all.deb
$ sudo dpkg -i mysql-apt-config_0.6.0-1_all.deb
```
When asked, you should select the latest MySQL version.

```
$ sudo apt update
$ sudo apt install mysql-server libmysqlclient-dev
```

You will required to create a root password for MySQL. It's a good idea to remember it!

Next, we should set up the database itself. You will be asked for the root password you just set, after which you should execute the commands listed below to create the necessary database.

```
$ mysql -uroot -p
mysql> CREATE DATABASE dmoj DEFAULT CHARACTER SET utf8mb4 DEFAULT COLLATE utf8mb4_general_ci;
mysql> GRANT ALL PRIVILEGES ON dmoj.* to 'dmoj'@'localhost' IDENTIFIED BY '<password>';
mysql> exit
```

## Installing the site

Now that you are done, you can start installing the site. First, create a `virtualenv` and activate it. Here, we'll create a `virtualenv` named `dmojsite`.

```
$ virtualenv dmojsite
$ . dmojsite/bin/activate
```
You should see `(dmojsite)` prepended to your shell. Henceforth, `(dmojsite)` commands assumes you are in the code directory, with `virtualenv` active.

!!! note
    The `virtualenv` will help keep the modules needed separate from the system package manager, and save you many headaches when updating. Read more about `virtualenv`s [here](#).


Now, fetch the site source code.

```
(dmojsite) $ git clone https://github.com/DMOJ/site.git
(dmojsite) $ cd site
(dmojsite) $ git submodule init
(dmojsite) $ git submodule update
```

Install Python dependencies into the `virtualenv`.

```
(dmojsite) $ pip install -r requirements.txt
(dmojsite) $ pip install mysqlclient
```

You will now need to configure `dmoj/local_settings.py`. You should make a copy [of this sample settings file](https://github.com/DMOJ/docs/blob/master/sample_files/local_settings.py) and read through it, making changes as necessary. Most importantly, you will want to update MySQL credentials.

!!! info
    Leave debug mode on for now; we'll disable it later after we've verified that the site works.
    
    Generally, it's recommended that you add your settings in `dmoj/local_settings.py` rather than modifying `dmoj/settings.py` directly. `settings.py` will automatically read `local_settings.py` load it, so write your configuration there.

Now, you should verify that everything is going according to plan.

```
(dmojsite) $ python manage.py check
```

Setup database tables.

```
(dmojsite) $ python manage.py migrate
```

DMOJ uses `sass` and `pleeease` to generate the site stylesheets. DMOJ comes with a `make_style.sh` script that may be ran to compile and optimize the stylesheets.

```
(dmojsite) $ ./make_style.sh
```

Now collect static files into `STATIC_ROOT` as specified in `dmoj/local_settings.py`.

```
(dmojsite) $ python manage.py collectstatic
```

You will also need to generate i18n files.

```
(dmojsite) $ python manage.py compilemessages
(dmojsite) $ python manage.py compilejsi18n
```

Next, load some initial data so that your install is not entirely blank.

```
(dmojsite) $ python manage.py loaddata navbar
(dmojsite) $ python manage.py loaddata language_small
```

You should create an admin account with which to log in initially.

```
(dmojsite) $ python manage.py createsuperuser
```

At this point, you should attempt to run the server, and see if it all works.

```
(dmojsite) $ python manage.py runserver 0.0.0.0:8000
```

You should Ctrl-C to exit after verifying.

!!! danger
    **Do not use `runserver` in production!**

    We will set up a proper webserver using Nginx and UWsgi soon.

You should also test to see if `bridged` runs.

```
(dmojsite) $ python manage.py runbridged
```

If there are no errors after about 10 seconds, it probably works.

You should Ctrl-C to exit.

Your job is half done.
You should now switch to a real python application server.
The rest of this guide assumes you are using `uwsgi` and `nginx` to serve the site.
It also assumes that you will be using `supervisord` to manage site and bridged.
There be dragons should you use something else.

Now, copy `uwsgi-template.ini` to `uwsgi.ini` and edit it.
You should at least change the paths to reflect your install.

You need to install `uwsgi`. The easiest way:

```
(dmojsite) $ curl http://uwsgi.it/install | bash -s default $PWD/uwsgi
```

To test:

```
(dmojsite) $ ./uwsgi --ini uwsgi.ini
```

If it says workers are spawned, it probably works.
You should Ctrl-C to exit.

If it all works out, you should now install `supervisord` and configure it.

```
$ sudo apt install supervisor
```

Copy our `site.conf` to `/etc/supervisor/conf.d/site.conf`, and fill in the fields.
Copy our `bridged.conf` to `/etc/supervisor/conf.d/bridged.conf`, and fill in the fields.

Reload supervisor configuration.

```
$ sudo supervisorctl update
```

Wait a few seconds, and run:

```
$ sudo supervisorctl status
```

If both processes are running, everything is good!
Otherwise peek at the logs and see what's wrong.

Now it's time to set up `nginx`.

```
$ sudo apt install nginx
```

Typically, `nginx` site files are located in `/etc/nginx/conf.d`.
Some installations might place it at `/etc/nginx/sites-available`
and require a symlink in `/etc/nginx/sites-enabled`.
You should copy the sample `nginx.conf`, edit it and place it in wherever
it is supposed to be.

Now reload the `nginx` configuration.

```
$ sudo service nginx reload
```

You should be good to go. Visit the site at where you set it up.
If it doesn't, check `nginx` logs and `uwsgi` log `stdout`/`stderr`.

!!! info
    Now that your site is installed, remember to set `DEBUG` to `False` in
    `local_settings`. Leaving it `True` is a security risk.

## Configuration of event server
Create `config.js`. This assumes you use `nginx`, or there be dragons.
You may need to shuffle ports if they are already used.

```
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

`get_port` should be the same as the port for `/event/` in `nginx.conf`
`http_port` should be the same as the port for `/channels/` in `nginx.conf`
`post_port` should be the same as the port in `EVENT_DAEMON_POST` in `local_settings`.
You need to configure `EVENT_DAEMON_GET` and `EVENT_DAEMON_POLL`.
You need to uncomment the relevant section in the `nginx` configuration.

Need to install the dependencies.

```
(dmojsite) $ npm install qu ws simplesets
(dmojsite) $ pip install websocket-client
```

Now copy `wsevent.conf` to `/etc/supervisor/conf/wsevent.conf`, changing paths.
and then update supervisor.

```
$ sudo supervisorctl update
```

Now do remember to reload `nginx`, restart bridge and site.

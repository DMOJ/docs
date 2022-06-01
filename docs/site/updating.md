# Updating the Site

The DMOJ is under active development, so occasionally you may wish to update. This is a fairly simple process.

!>  The DMOJ development team makes no commitment to backward compatibility. An update migration
    might add, change, or delete data from your install. Always back up before attempting an update. <br> <br>
    If in doubt, feel free to [contact us on Slack](https://slack.dmoj.ca).

First, switch to the site virtual environment, and pull the latest changes.

```shell-session
(dmojsite) $ git pull origin master
```

Dependencies may have changed since the last time you updated, so install any missing ones now.

```shell-session
(dmojsite) $ pip3 install -r requirements.txt
```

The database schema might also have changed, so update it.

```shell-session
(dmojsite) $ python3 manage.py migrate
(dmojsite) $ python3 manage.py check
```

Finally, update any static files that may have changed.

```shell-session
(dmojsite) $ ./make_style.sh
(dmojsite) $ python3 manage.py collectstatic
(dmojsite) $ python3 manage.py compilemessages
(dmojsite) $ python3 manage.py compilejsi18n
```

That's it! You may wish to condense the above steps into a script you can run at a later time.

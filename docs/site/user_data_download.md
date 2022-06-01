# User Data Downloads

The DMOJ allows users to download their data. At the time of writing, only user comments and submission data can be downloaded.

By default, this feature is disabled. To enable, uncomment the relevant settings in `local_settings.py`.

```python
## Data download settings.
# Uncomment to allow users to download their data.
DMOJ_USER_DATA_DOWNLOAD = True

# Directory to cache user data downloads.
# It is the administrator's responsibility to clean up old files.
DMOJ_USER_DATA_CACHE = '/home/dmoj-uwsgi/datacache'

# Path to use for nginx's X-Accel-Redirect feature.
# Should be an internal location mapped to the above directory.
DMOJ_USER_DATA_INTERNAL = '/datacache'

# How often a user can download their data.
DMOJ_USER_DATA_DOWNLOAD_RATELIMIT = datetime.timedelta(days=1)
```

Also, uncomment the relevant section in your Nginx configuration if you wish to take
advantage of Nginx's [X-Accel-Redirect](https://www.nginx.com/resources/wiki/start/topics/examples/x-accel/#x-accel-redirect)
feature.

```nginx
# Uncomment if you are allowing user data downloads and want to serve it faster.
# This location name should be set to DMOJ_USER_DATA_INTERNAL.
location /datacache {
    internal;
    root <path to data cache directory, without the final /datacache>;
    # Default from local_settings.py:
    #root /home/dmoj-uwsgi/;
}
```

These data files are not cleaned up automatically. Although each user can have at most one data file
stored on the server at a time, you may want to clean up old files. A cron job should suffice:

```
0 */4 * * * find /home/dmoj-uwsgi/datacache/ -type f -mtime 2 -delete
```

This cron job will delete files older than 2 days every 4 hours. You are recommended to tweak these
values according to your ratelimit.

You should now find a link on your _Edit profile_ that allows you to download your data,
along with various configuration options.

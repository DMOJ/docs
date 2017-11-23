## Installing reCAPTCHA Spam Registration Prevention

If you run DMOJ for any prolonged period of time, eventually spambots will begin registering in large numbers.

DMOJ can integrate with reCAPTCHA to filter out spam registrations with a little setup.

## Getting an API Key

First, head to the [reCAPTCHA admin site](https://www.google.com/recaptcha/admin).
Select "reCAPTCHA V2", specify your domain, and click through to get an API key pair.

In `local_settings.py`, set `RECAPTCHA_PUBLIC_KEY` to the site key, and `RECAPTCHA_PRIVATE_KEY` to the secret key.

## Installing reCAPTCHA Support

First, install `django-recaptcha2` in the site virtualenv.

```
(dmojsite) $ pip install django-recaptcha2
```

Finally, open `local_settings.py` in your editor of choice, and add `snowpenguin.django.recaptcha2` to the end of`INSTALLED_APPS`.

Restart DMOJ for the changes to take effect. You should now have an "I'm not a robot" checkbox on registration.

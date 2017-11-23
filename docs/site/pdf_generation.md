# PDF Generation of Problem Statements
The DMOJ supports rendering problem statements to PDF. This can be useful in the case of on-site contests, where contestants
are distributed paper versions of the problems.

For example, [here](https://dmoj.ca/problem/ioi14p1/pdf) is a generated PDF of [this problem](https://dmoj.ca/problem/ioi14p1).

## Installing PhantomJS
Grab a PhantomJS build from [the download page](http://phantomjs.org/download.html), and extract it. For these static
builds, there is no need to do anything further. You can run it to verify it's working.

```
$ phantomjs --version
2.1.1
```

## Configuring DMOJ to use PhantomJS
Configuring DMOJ to generate PDFs with PhantomJS requires the addition of a couple of lines to your `local_settings.py`.

```py
# Cache location for generated PDFs. You should consider using something more persistant
# than /tmp, since PDF generation is an expensive operation.
PROBLEM_PDF_CACHE = '/tmp'
# Path to wherever you extracted the PhantomJS binary.
PHANTOMJS = '/usr/local/bin/phantomjs'
```

Restart DMOJ for the changes to take effect.

!!! note
    The DMOJ uses a Segoe UI font when viewed on Windows browsers. If running PhantomJS on a Linux server, installing
    Segoe UI fonts on it will provide optimal rendering quality &mdash; otherwise, a fallback font will be used and
    statements will look subpar.

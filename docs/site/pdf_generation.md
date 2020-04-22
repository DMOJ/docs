# PDF Generation of Problem Statements
The DMOJ supports rendering problem statements to PDF. This can be useful in the case of on-site contests, where
contestants are distributed paper versions of the problems.

For example, [here](https://dmoj.ca/problem/ioi14p1/pdf) is a generated PDF of
[this problem](https://dmoj.ca/problem/ioi14p1).

While Puppeteer is used for PDF generation in production, Selenium is the recommended PDF renderer.

!> The DMOJ also supports PDF generation using PhantomJS and SlimerJS. However, they are not used in production, are
outdated, and may be deprecated in the future.

## Installation
Install [Selenium](https://www.selenium.dev/) in the same Python virtualenv as the site, and install the Chromium
browser. Querying the browser version is necessary for the next step but also serves as a way to make sure Chromium
installed correctly.

```shell
$ pip install selenium
$ apt install chromium-browser
$ chromium-browser --version
Chromium 80.0.3987.163 Built on Ubuntu , running on Ubuntu 18.04
```

Selenium requires [ChromeDriver](https://chromedriver.chromium.org/downloads) to run Chromium, so download and unzip the
correct version. Only the major version number is important (e.g. 80). An example install procedure might be as follows:

```shell
$ wget https://chromedriver.storage.googleapis.com/80.0.3987.106/chromedriver_linux64.zip
$ unzip chromedriver_linux64.zip
$ install chromedriver /usr/local/bin/
$ rm chromedriver_linux64.zip chromedriver
```

?>  To follow these instructions, installing `unzip` may be required with `apt install unzip`.

?>  The DMOJ uses a Segoe UI font when viewed on Windows browsers. If running Selenium on a Linux server, installing
    Segoe UI fonts on it will provide optimal rendering quality &mdash; otherwise, a fallback font will be used and
    statements will look subpar.

## Configuration
Configuring DMOJ to generate PDFs with Selenium can be done by adding the following lines to your `local_settings.py`.

```py
# Enable Selenium PDF generation
USE_SELENIUM = True
# Optional paths to Chromium and ChromeDriver
SELENIUM_CUSTOM_CHROME_PATH = None
SELENIUM_CHROMEDRIVER_PATH = 'chromedriver'

# Cache location for generated PDFs. You should consider using something more persistant
# than /tmp, since PDF generation is an expensive operation.
DMOJ_PDF_PROBLEM_CACHE = '/tmp'
```

Restart DMOJ for the changes to take effect.

?>  Selenium requires that the chromedriver and Chromium installations are in your system's `PATH` variable, so the
    `SELENIUM_CUSTOM_CHROME_PATH` and `SELENIUM_CHROMEDRIVER_PATH` variables are entirely optional on standard
    installations. However, they may be set to troubleshoot issues.

## Updating
When updating packages, the `chromium-browser` package may update to a newer version, while the `chromedriver` binary
lags behind. Check to make sure that the major versions of the two are the same after updating.

```shell
$ chromium-browser --version
Chromium 80.0.3987.163 Built on Ubuntu , running on Ubuntu 18.04
$ chromedriver --version
ChromeDriver 80.0.3987.106 (f68069574609230cf9b635cd784cfb1bf81bb53a-refs/branch-heads/3987@{#882})
```

If these versions do not match, follow the [installation procedure](#installation) to get a newer version of
ChromeDriver.

## Troubleshooting
### "View as PDF" button doesn't show up
If a "View as PDF" button does not show up on the problem page, make sure that the `USE_SELENIUM` variable is set to
`True` and that `selenium` can be properly imported.

```shell
$ . siteenv/bin/activate
$ python
>>> import selenium
```

If an `ImportError` is thrown, please follow the [installation procedure](#installation).

### "View as PDF" button shows up
If a "View as PDF" button shows up, but generation fails, an error log should be displayed in the browser. This log will
also be captured by the `judge.problem.pdf` Django log handler. Depending on the error, setting the optional variables
`SELENIUM_CUSTOM_CHROME_PATH` to the path of the Chromium binary and `SELENIUM_CHROMEDRIVER_PATH` to the path of the 
ChromeDriver binary may alleviate the problem.

One common cause of PDF generation failures is mismatched Chromium and ChromeDriver versions. To find out if this
affects you and how to fix it, follow the instructions in the [updating section](#updating).

For other errors, take a look at the [Selenium documentation](https://www.selenium.dev/documentation/en/webdriver/),
specifically the
[common exceptions](https://www.selenium.dev/selenium/docs/api/py/common/selenium.common.exceptions.html) section.

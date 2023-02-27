# PDF generation of problem statements

The DMOJ supports rendering problem statements to PDF. This can be useful in the case of on-site contests, where
contestants receive paper versions of the problems.

For example, [here](https://dmoj.ca/problem/ioi14p1/pdf) is a generated PDF of
[this problem](https://dmoj.ca/problem/ioi14p1).

You can use Puppeteer or Selenium to render PDFs of problems. Puppeteer is used in production and
Selenium is documented below.

## Installation

Install [Selenium](https://www.selenium.dev/) in the same Python virtual environment as the site. You will also need to
install [ChromeDriver](https://chromedriver.chromium.org/downloads), as it's a special version of the Chromium engine
needed by Selenium to create PDFs.

```shell-session
$ pip3 install selenium
$ apt install chromium-driver
```

?>  The DMOJ uses a Segoe UI font when viewed on Windows browsers. If running Selenium on a Linux server, installing
    Segoe UI fonts on it will provide optimal rendering quality &mdash; otherwise, a fallback font will be used and
    statements will look subpar.

## Configuration

Configuring DMOJ to generate PDFs with Selenium can be done by adding the following lines to your `local_settings.py`.

```python
# Enable Selenium PDF generation.
USE_SELENIUM = True
# Optional paths to Chromium and ChromeDriver.
SELENIUM_CUSTOM_CHROME_PATH = None
SELENIUM_CHROMEDRIVER_PATH = 'chromedriver'

# Cache location for generated PDFs. You should consider using something more persistent
# than /tmp, since PDF generation is an expensive operation.
DMOJ_PDF_PROBLEM_CACHE = '/tmp'
```

Restart DMOJ for the changes to take effect.

?>  Selenium requires that the ChromeDriver and Chromium installations are in your system's `PATH` variable, so the
    `SELENIUM_CUSTOM_CHROME_PATH` and `SELENIUM_CHROMEDRIVER_PATH` variables are entirely optional on standard
    installations. However, they may be set to troubleshoot issues.

## Troubleshooting

### "View as PDF" button doesn't show up

If a "View as PDF" button does not show up on the problem page, make sure that the `USE_SELENIUM` variable is set to
`True` and that `selenium` can be properly imported.

```shell-session
$ . dmojsite/bin/activate
$ python3
>>> import selenium
```

If an `ImportError` is thrown, please follow the [installation procedure](#installation).

### "View as PDF" button shows up

If a "View as PDF" button shows up, but generation fails, an error log should be displayed in the browser. This log will
also be captured by the `judge.problem.pdf` Django log handler. Depending on the error, setting the optional variables
`SELENIUM_CUSTOM_CHROME_PATH` to the path of the Chromium binary and `SELENIUM_CHROMEDRIVER_PATH` to the path of the
ChromeDriver binary may alleviate the problem.

For other errors, take a look at the [Selenium documentation](https://www.selenium.dev/documentation/webdriver/),
specifically the
[common exceptions](https://www.selenium.dev/selenium/docs/api/py/common/selenium.common.exceptions.html) section.

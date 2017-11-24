# LaTeX Math with Wikimedia Mathoid
The DMOJ platform is capable of rendering LaTeX math for constraints and formulas that may appear in problem statements. For example,
[this problem](https://dmoj.ca/problem/fibonacci) makes extensive use of this functionality to present crisp math rendering in browsers.

The DMOJ makes use of the [Wikimedia Mathoid](https://github.com/wikimedia/mathoid) project to render math.

## Installing Mathoid
Follow the [installation instructions](https://github.com/wikimedia/mathoid) of Mathoid. Moving forward, we'll assume that
you are running Mathoid on `localhost:8888`.

## Configuring DMOJ to use Mathoid
Assuming Mathoid  is installed, configuring DMOJ to generate math with it requires the addition of a few lines
to `local_settings.py.`

```python
# The URL Mathoid is running on
MATHOID_URL = 'http://localhost:8888'
# A directory accessible by the user running Mathoid, as well as the web (nginx) user.
# For optimal performance, change this to something more persistant than /tmp
MATHOID_CACHE_ROOT = '/tmp/mathoid_cache'
# The URL base MATHOID_CACHE_ROOT is configured to be served under in your webserver. For
# example, if /tmp/mathoid_cache/render.png exists, example.com/mathoid/render.png should
# serve it.
MATHOID_CACHE_URL = '//example.com/mathoid/'
```

Restart DMOJ for the changes to take effect. After restarting, you may have to purge Django's cache before seeing any changes.

## Using Mathoid math in problem statements
A snippet of a problem statement using Mathoid ot render math is shown below.

```md
The Fibonacci sequence is a well known sequence of numbers in which

$$F(n) = \begin{cases} 0, & \text{if } n = 0 \\ 1, & \text{if } n = 1 \\ F(n-2) + F(n-1), & \text{if } n \ge 2 \end{cases}$$

Given a number ~N~ ~(1 \le N \le 10^{19})~, find the ~N^{th}~ Fibonacci number, modulo ~1\,000\,000\,007~ ~(= 10^9 + 7)~.<br/>
**Note:** For 30% of the marks of this problem, it is guaranteed that ~(1 \le N \le 1\,000\,000)~.
```

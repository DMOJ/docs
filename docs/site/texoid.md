# Rendering LaTeX diagrams in problem statements

The DMOJ platform is capable of rendering LaTeX documents onto problem statements. This can be useful for things like
drawing graphs with ease, porting over PDF resources, and so on. An example of this functionality can be seen live
[here](https://dmoj.ca/problem/ds4), [here](https://dmoj.ca/problem/ccoqr16p3), and
[here](https://dmoj.ca/problem/ccoqr16p1).

DMOJ supports this through a related project, [Texoid](https://github.com/DMOJ/texoid). Texoid interfaces with texlive
to provide a REST endpoint for LaTeX rendering.

## Installing Texoid

First, clone the Texoid repository, and install it and its dependencies into a new virtualenv.

```shell-session
$ git clone https://github.com/DMOJ/texoid.git
$ cd texoid
$ python3 -m venv env
$ . env/bin/activate
$ pip install -e .
```

Texoid relies on LaTeX distribution to render documents to DVI format, `dvisvgm` to convert to SVGs, and ImageMagick to
convert  SVGs into PNGs. On a typical Debian or Ubuntu machine, you can fetch everything you need with:

```shell-session
$ apt install texlive-latex-base texlive-binaries imagemagick
```

## Running Texoid

To start the Texoid server, run:

```shell-session
$ export LATEX_BIN=<path to latex>
$ export DVISVGM_BIN=<path to dvisvgm>
$ export CONVERT_BIN=<path to convert>
$ env/bin/texoid --port=<port>
```

The environment variables are not necessary if all three executables are present in `$PATH`, as they should be if you
followed the installation instructions above. Here, convert refers to ImageMagick's `convert` tool.

To test, start Texoid with `--port=8886`. Then, we can request a render of a simple LaTeX document.

```latex
\documentclass{standalone}
\begin{document}
$E=mc^2$
\end{document}
```

The response should contain JSON, with SVG and a Base64-encoded PNG inside.

```shell-session
$ curl --data 'q=%5Cdocumentclass%7Bstandalone%7D%0A%5Cbegin%7Bdocument%7D%0A%24E%3Dmc%5E2%24%0A%5Cend%7Bdocument%7D' http://localhost:8886
{
    "success": true,
    "svg": "<?xml version='1.0'?><svg...</svg>",
    "png": "iVBORw0KGgoA...kSuQmCC\n",
    "meta": {
        "width": "48",
        "height": "10"
    }
}
```

## Configuring DMOJ to use Texoid

Once Texoid is installed, configuring DMOJ to generate LaTeX diagrams with it requires the addition of a few lines to
`local_settings.py`.

```python
# The URL Texoid is running on.
TEXOID_URL = 'http://localhost:8886'

# A directory accessible by the user running Texoid, as well as the web (nginx)
# user.
#
# For optimal performance (since launching texlive is expensive), change this
# to something more persistent than /tmp.
TEXOID_CACHE_ROOT = '/tmp/texoid_cache'

# The URL base TEXOID_CACHE_ROOT is configured to be served under in your
# webserver. For example, if /tmp/texoid_cache/render.png exists,
# example.com/texoid/render.png should serve it.
TEXOID_CACHE_URL = '//example.com/texoid/'
```

Restart DMOJ for the changes to take effect. After restarting, you may have to purge Django's cache before seeing any changes.

## Using LaTeX diagrams in problem statements

To invoke Texoid to generate LaTeX diagrams, wrap your LaTeX code in `<latex>` blocks.

```markdown
## This is a LaTeX Demo

The diagram below is **real LaTeX!**

<latex>
\documentclass{standalone}
\begin{document}
$E=mc^2$
\end{document}
</latex>
```

Typically, `\documentclass{standalone}` works best for inlining diagrams.

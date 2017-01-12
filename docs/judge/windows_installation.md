## Setting up the Judge on Windows

### Windows 10 Anniversary Update
This is the recommended way to run the DMOJ on Windows, but requires the Windows Subsystem for Linux (WSL) that was added in the Windows 10 Anniversary Update. You will [need to enable the subsystem](https://msdn.microsoft.com/en-us/commandline/wsl/install_guide) before proceeding.

Open Bash by tying `bash` in command prompt, or running it directly.

We'll need a couple of packages to get started, after which we can begin setting up the judge.

```
$ sudo apt install git python-dev python-pip
$ sudo pip install cython
$ git clone https://github.com/DMOJ/judge
$ cd judge
$ sudo python setup.py develop
```

And that's it. You can type `dmoj-cli` in your shell to check that the CLI executable has been created.

### All Other Windows

Instructions for installing on a regular Windows machine are a bit more involved. First, either install `git` and `clone` the DMOJ source code as shown in the previous instructions, or just download the zip from https://github.com/DMOJ/judge and extract it into your preferred directory.

You will need a Python 2.7 installation. Make sure you have `cython` installed, by running `pip install cython`. Assuming a command prompt in the judge directory, run:

```
$ python setup.py develop
```

Unless you have VC2015 correctly configured, the command will fail as it does not succeed in building the necessary sandbox binaries. However, we provide these binaries at http://binaries.dmoj.ca/x86/ for x86 Python and http://binaries.dmoj.ca/x64/ for x64 Python. You can check which version of Python you are running via the first line of the `python` interpreter. In the example below, we are running a 64 bit Python, so we should download from the x64 folder.

```
Python 2.7.11 (v2.7.11:6d1b6a68f775, Dec  5 2015, 20:40:30) [MSC v.1500 64 bit (AMD64)] on win32
```

`_wbox.pyd` goes into `dmoj/wbox`, and `_checker.pyd` optionally goes into `dmoj/checkers`.

Running `develop` again should succeed, and a `dmoj-cli.exe` should be placed in `C:/Python27/Scripts/`. You can try running it to see if it works &mdash; you should see a help text if so. **Note that the DMOJ Windows judge requires administrator privileges to run: you will need to start cmd.exe as having admin privileges explicitly.**

### Using the CLI to Test Problems

Now, we need to configure some languages. Still in the judge directory, run:

```
$ python -u dmoj/executors/autoconfig.py
```

Autoconfig will print out some debug info followed by a YAML declaration for a `runtime` node. Copy it out and add it to a file called `judge.yml`.

Running `dmoj-cli -c judge.yml` should put the judge through selftests. At any point in time, you may add more languages (e.g., through `apt` in WSL), and then use `autoconfig` to generate the necessary configuration.

Next, you will need to set a `problem_storage_root` node to tell DMOJ where to look for problem data.

Setting `problem_storage_root` to a directory informs the judge to treat the children of its children (its grandchildren) as problem directories. In the example below, `/judge/problems/default/helloworld` will be treated as a problem folder (so long as it contains an `init.yml`), while `/judge/problems/helloworld` will not, even if it does contain an init file.

```yaml
problem_storage_root: /judge/problems
runtime:
   ...
```

Running `dmoj-cli` again will allow you to start submitting problems you have configured via the `submit` command. Use the `help` command to get a listing of supported commands. Use `<command> --help` to get more info on a specific command.

#### Note for Windows, non-WSL
You will need to set `tempdir` to a directory accessible by all users. Typically, a folder directly in `C:`, e.g. `C:/judge-temp`, suffices. If you would like to further isolate temporary judge data, you can use a virtual hard drive (VHD) and mount it to another letter drive.

**The Windows judge will not run without a `tempdir` set, though you will get cryptic errors.**

To run the judge, simply do `python main.py <bridge hostname> -p <bridge port> [-c <config path>]`.
But before you run the judge, you must configure it.
You are recommended to pass the absolute path to `judge.yml` on the command line.

### judge.yml

`judge.yml` is the only configuration file required for a DMOJ judge. It specifies directories to search for
problems, and also defines the execution environment.

For an example `judge.yml` containing all supported key-value pairs, see [sample_judge_conf.yml](https://github.com/DMOJ/docs/blob/master/sample_judge_conf.yml)

### selftest_sandboxing node

It may be the case that your judges take a very long time to start up due to self-testing. One way to speed up the boot process once you're confident that the sandboxing is configured correctly is to specify `selftest_sandboxing: false` in `judge.yml`. Doing so will disable all sandboxing during testing. Beware! This may void your warranty, but may end up increasing your mileage.

### Authentication

All judges are authenticated to the bridge in order to avoid rogue judges returning false data.

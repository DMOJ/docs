# Judge Configuration

To run the judge, simply do `python main.py <bridge hostname> -p <bridge port> [-c <config path>]`.
But before you run the judge, you must configure it. You are recommended to pass the absolute path
to `judge.yml` on the command line.

## `judge.yml`

`judge.yml` is the only configuration file required for a DMOJ judge. It specifies directories to search for
problems, and also defines the execution environment.

For an example `judge.yml` containing all supported key-value pairs, see [sample_judge_conf.yml](/sample_judge_conf.yml)

### Authentication

All judges are authenticated to the bridge in order to avoid rogue judges returning false data.

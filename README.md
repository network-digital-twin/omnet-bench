# omnet-bench
Implements a benchmark environment using OMNeT++.

## Introduction
Essentially, the implementation lies within the [`src/zte_qos/`](src/zte_qos/) folder.

## Get Started
This section clarifies the steps needed to run the benchmark without building the OMNeT++ project.

### Requirements
To run the benchmark, the OMNeT++ environment is not mandatory. However, a Python environment is needed to execute the topology generation script and the result analysis script.

The Python dependencies are listed as follows:
1. `PyYAML==6.0.1`: loading/dumping of YAML files, required by the [topology generator](src/zte_qos/tools/topo_gen/requirements.txt);
2. `pandas==2.2.0`: loading/dumping of CSV files, required by the [result analyzer](src/zte_qos/tools/analysis/requirements.txt);
3. `pyarrow==15.0.0`: future dependency of `pandas`.

### Steps
xxx
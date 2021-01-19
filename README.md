- [Build & Run](#build---run)
  * [Build](#build)
  * [Running tests](#running-tests)
  * [Running scenario](#running-scenario)
  * [Creating your own scenario](#creating-your-own-scenario)

## Build & Run

The project was tested on linux (to be concrete on ubuntu 19.04), but should run also on unix like operating systems.

### Build
```bash
mkdir build
cd build
cmake ..
make
```

### Running tests
In `build` directory after building the project just run
```bash
./tests
```

### Running scenario
Given scenario file under `scenario_path` and `streamlet` binary file, running:
```bash
./streamlet false scenario_path > forest_state # false to disable logs
```
will run the scenario and output the forest state of the honest nodes at the end of scenario to the file forest_state.
To visualize the graph we recommend the program `dot` from *Graphviz*.

```bash
dot -Tpng < forest_state > forest_state.png
```

### Creating your own scenario
To create new scenario to run just create yaml file with following entries
```yml
scenario-name: "Test Config"  # Just name given to the scenario
rounds: 10                    # Scenario will run for #rounds
honest-nodes: 10              # Number of honest nodes following Streamlet protocol
dummy-nodes: 10               # Count of dummy nodes which do nothing
dishonest-nodes:              # Count of nodes trying to make ruckus in the protocol
scheduler-type: "simple"      # one of ["simple", "partitioning"], simple will deliver messages from last round. Partitionig will partition the network and for #synchronize-every-n keep two subnetworks separated
service-type: "repeating"     # for now only option is "repeating"
repeat-n-times: 12            # Optional, default to 1, number of times the same node will be the epoch leader.
synchronize-every-n: 10       # Optional, default to 1, number of rounds between each synchronization time in partitioning scheduler.

```

- [Build & Run](#build---run)
  * [Build](#build)
  * [Running Tests](#running-tests)
  * [Running scenario](#running-scenario)
  * [Creating Your Own Scenario](#creating-your-own-scenario)

## Build & Run

The project was tested on linux (to be concrete on ubuntu 19.04), but should run also on unix like operating systems.

### Build
```bash
mkdir build
cd build
cmake ..
make
```

### Running Tests
In `build` directory after building the project just run
```bash
./tests
```

### Running scenario
Given scenario file under `scenario_path` and `streamlet` binary file, running:
```bash
./streamlet false scenario_path > forest_state
```
will run the scenario and outptu forest state of the honest nodes at the end of scenarop to the file forest_state.

### Creating Your Own Scenario
To create new scenario to run just create yaml file with following entries
```yml
scenario-name: "Test Config"  # Just name given to the scenario
rounds: 10                    # Scenario will run for #rounds amount time.
honest-nodes: 10              # Count of honest nodes following streamlet protocol
dummy-nodes: 10               # Count of dummy nodes which do nothing
dishonest-nodes:              # Count of nodes trying to make ruckus in the protocol
scheduler-type: "simple"      # one of ["simple", "partitioning"], simple will deliver messages from last round. Partitionig will partition the network and for #synchronie-every-n keep two subnetworks separated
service-type: "repeating"     # for now only option is "repeating"
repeat-n-times: 12            # Opotional, default to 1, number of times the same node will be the epoch leader.
synchronize-every-n: 10       # Optional, default to 1, number of rounds between each synchronization time in partitioning scheduler.

```
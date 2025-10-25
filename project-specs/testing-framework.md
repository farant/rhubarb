some libraries this will depend on that need to be made

- string library (struct prefixed with length like casey muratori)
- string utilities
- string builder
- hash table
- hashing library?
- ini parser
- arg parser?
- something for environment variables
- probably something that can define a config and get it from file or env variables or args
- things for printing strings with colors to shell
- probably some filesystem stuff or at least a platform independence layer
- probably some stuff for spawning processes (at least platform independence layer)


features that we want

- test suites
- tests within suites
- ability to run specific suites or tests via some cli tool
- ability to not run certain suites or tests with cli tool
- run all tests even if some fail
- see which tests failed
- setup and terdown per test & per suite
- parameterized tests (data driven)
- show which assertion failed, actual vs expected values
- timing for how long took to run is nice
- mark tests as skip or pending
- organized output with total run/passed/failed/skipped and which failed
- quick way to just re-run failed tests
- proper exit codes
- verbose mode or just show failed tests
- watch mode (with filter option)

i'm also curious if it might be worthwhile to have some kind of database that stores some information about each test run so you can kind of see stats over time of success/failure of tests, number of tests, etc.


so as far as the architecture we have:

- test runner
- some libraries to support the test runner 
- the actual test library that tests use
- the test suites themselves

i think that there will be a ini config file

the test suites will be able to run in such a way, maybe with a flag like --register, so that they don't run but register themselves into a test registery file, and probably with another flag like --list-tests that outputs the tests in some format to stdout for the benefit of the test runner along with other information about the names, etc of tests and test suites (this is necessary so the test runner can filter tests by name and stuff i think?)

- test runner will need to be able to compile test files with their dependencies
- build artifacts need to go somewhere (probably a temp location?)
- test runner needs to keep track of which ones compile and which don't
- probably one test file not compiling should not stop all tests from running if others do compile
- the test runner needs to run compiled artifacts
- it needs to aggregate results from the tests it ran
- probably needs to do some cleanup afterwards


since we are not trying to use gcc specific extensions and want strict c89 i think the most straightforward way to approach test registration is to manually create a main function in each test file that has a data structure with test suites and test information and the individual test functions referenced. not sure a better way to do this in a better way without meta programming or code generation for now.

this means that we will have a separate binary for each test file, it doesn't make sense to manually maintain a test_all.c file with a single main that has all the tests registered in it.



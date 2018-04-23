# DNS Performance Tracker

### How to use

* Clone repository
* Refer to [install.md](install.md) to set up dependencies and database
* Configure and compile
```
cd dns-performance-tracker
./waf configure
./waf
```
Optionally, configure with '--openssl-include', '--mysql-include' and '--with-tests' to tell waf where to look for openssl and mysqlclient headers, and build gtest unittests or not.
* Update configuration, 'build/db.conf' (copied from 'conf/db.sample.conf') contains database connection information
* Run
```
cd build
./dns_performance_tracker
```
To see command line options available, do
```
./dns_performance_tracker -h
```
-i flag configures interval between queries. Note that the actual interval will be time supplied as in this flag + total query times for all domains combined.

### What to expect

Under the default logging level, upon each query, the program will output entries for each domain including average query time, standard deviation, total number of queries, and the timestamp of the first query.

The same information is stored in MySQL database, and the program will keep collecting stats where it left off, if it's killed and restarted.

### Code Architecture

* Two units of releases are created, 'query' and 'report'
  * 'query' is high level business object, which uses ldns to interact with given domains, and a reporter interface to report and update statistics.
  * 'report' provides a reporting interface, and a MySQL DB accessor as one concrete implementation. 'report' defines the data model of individual stat reports, and that of DB interaction in MySQL accessor's case.

### DB Schema Design

Two tables are created
* One 'stats' table for DNS performance statistics associated with each domain, keyed by the domain name. This table keeps track of total successful and failed requests, total duration of those requests that succeeded, variance so far, and the timestamps of first and the last query.
* Another 'journal' table keeps track individual queries: at which time a query is made to which domain, if the query succeeded, and how much time the query took.

'Journal' table is purely for journaling, and can be turned off without affecting the user-facing behavior of the program. New variance (standard deviation) and average values are purely decided upon 'stats' table, using an online algorithm for variance calculation.

### Appendix

* This project uses waf as build system, and provides one sample unit test on QueryUtil component. Code style roughly follows that of [BDE](https://github.com/bloomberg/bde)
* Mock, CI and vagrant distribution were considered, but were not pursued due to lack of time
* A multi-threaded implementation was considered but not pursued, since no concurrent jobs were identified


zhehao@remap.ucla.edu
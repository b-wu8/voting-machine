# A "Very Secure" Voting System
## Root the (Ballot) Box

It's not secure at all by default. This was (_not_) "written" with best practices
in mind, but to demonstrate common vulnerabilities in online systems, such as webpages
and databases. This project contains five fatal vulnerabilities that could result in 
escalation of non-root user privileges, ability to modify election database, ability
to insert malicious scripts, and much more beyond. 

Please note that this project is NOT TO BE USED for any kind of reference. It is 
purely used for educational purposes and fun for studying network and system 
security. 

## Running the Front-End

The front-end interface to the voting machine is a series of CGI
applications, written in both Bash and in Python. From the root of the
folder, run

```sh
$ make cgi
```

This starts a CGI-aware web server running on port 8000 on the local machine.
Then, using a web browser, go to `http://localhost:8000/cgi-bin/home.cgi` to
access the interface. The default password for the login system is `admin`.
If you wish to add additional CGI scripts, add them to the `cgi-bin/`
directory, making sure you mark them as executable (`chmod 755 file.cgi`). To
exit the CGI server, send a keyboard interupt (`<Ctrl>+C`).

## Running the Back-End

The back-end is a set of C and Python programs that access a database. We
have provided the database software, `sqlite3`, along with `etovucca`, the
interface to it. `sqlite3` is a SQL system much like MySQL (which we used in
class), but simpler. To get started, build the `etovucca` interface. From the
directory root, run:

```sh
$ make
```

This will build both the interface and the `sqlite3` database engine.

Next, you will have to initialize the database file, `rtbb.sqlite3`. Run
(from the directory root):

```sh
$ make initdb
```
This will create the database based on a setup script that we have already
provided. You can then run the `make cgi` command to interface with the
front-end via a web browser, or run the `./etovucca` binary.

If you want to interface with the database directly (using SQL statements),
run:

```sh
$ ./sqlite3 rtbb.sqlite3
```

Information on the CLI can be found [here](https://sqlite.org/cli.html).
Also, please note the `./sqlite3` in the command. The SEED VM has its own
version of `sqlite3`, but you should use the one bundled in this repository.


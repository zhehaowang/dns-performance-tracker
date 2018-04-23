# Install

### Mac OSX 10.13.3

```sh
brew install ldns
brew install mysql++
```

```sh
brew services start mysql
```

Optionally, if compiling with test
* Install gtest

### Set up database

```sh
mysql -uxxx -p
```

```sh
mysql>source [path to repo_root/schema/record.sql]
```

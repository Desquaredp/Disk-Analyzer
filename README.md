# Elastic Disk Analyzer (CLI)
 
 Disk Analyzer that uses elastic arrays to store the fetched data of a group of files within a directory. 
 This data can be accessed in a desired order using flags.



## Building

To compile and run:

```bash
make
./da <falgs> 
```

## Program Options


```bash
$ ./da -h
Disk Analyzer (da): analyzes disk space usage
Usage: ./da [-ahs] [-l limit] [directory]

If no directory is specified, the current working directory is used.

Options:
    * -a              Sort the files by time of last access (descending)
    * -h              Display help/usage information
    * -l limit        Limit the output to top N files (default=unlimited)
    * -s              Sort the files by size (default, ascending)

```


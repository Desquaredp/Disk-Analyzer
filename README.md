# Elastic Disk Analyzer (CLI)
 
 Disk Analyzer that uses elastic arrays to store the fetched data of a group of files within a directory. 
 This data can be accessed in a desired order using flags.


TODO: Remove the link above. Your README should not depend on a link to the spec.

TODO: Replace this section with a short (1-3 paragraph) description of the project. What it does, how it does it, and any features that stand out. If you ever need to refer back to this project, the description should jog your memory.

## Building

To compile and run:

```bash
make
./da <falgs> 
```

## Program Options

TODO: Provide an overview of program options and their descriptions, if applicable. If the program does not accept any options, delete this section.

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


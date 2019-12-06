# Introduction

!!! notice
    The manual installation guide is not intended to describe *the* Islandora 8 installation but rather *an* Islandora 8 installation. The server created using this guide is not hardened, will not be easily scalable, and the components may not be configured in a way you consider easy to work with. A production instance of Islandora 8 should be installed and maintained by a professional with an understanding of Linux and server administration.

This guide will contain generalized steps on installation and configuration of the various components, but will contain specific example commands for executing these steps on an Ubuntu 18.04 Server.

## Some Prerequisite Knowledge

This guide assumes the user has some knowledge:

- A general idea of how to work on the command-line of a Linux server using Bash. Commands are described in detail, but servers are volatile, and knowledge is still assumed in case anything happens outside of your expectations.
- An understanding of how to modify files from the command line. Configurations will often need to be created or modified in order to get things up and running. This might involve using an application like `nano` or `vi`/`vim`, or creating these files locally and uploading them to the server. It should also generally be assumed that most of these configuration files will have to be created or edited using `sudo`, and that permissions and ownership may need to be specified on these files.

## Conventions Used in This Guide

### Chronological Organization

The steps in this guide are listed in chronological order of installation and configuration. Some sections will reference variables and rely on components installed in previous sections. It does not account for skipping over or otherwise changing the order of installation steps; bear this in mind if you decide to do things out of the provided order.

### Replacement Variables

It is expected that the person setting up the site may want to use different usernames, passwords, and other such variables than the ones presented by default in this guide. Additionally, some defaults can't be provided, such as up-to-date version information for externally-provided components. In such cases, the replacement variables will be placed in all capital letters, and a description of the variables, any possible defaults, and how to get up-to-date information, will be listed below.

### Bash Commands

!!! notice
    Command blocks are *always* assumed to start at the home folder of the user originally created during the server installation. **They are never run as `root`**; if root access is required, `sudo` will be specified, and if files are created belonging to `root` that should not belong to `root`, `chmod` and `chown` will be run against them immediately afterwards to ensure correct permissions. If commands need to be run from a different working directory, an absolute path will be specified to use with `cd`. If you're concerned about whether or not a code block can be run from your current working directory, run `cd ~` before executing any commands in it.

Commands to be run on the command line will be placed in code blocks, with one command per line, and any replacement variables below, e.g.,

```shell
sudo run --this-command
python3 run.py /this/other/command --with-param PARAMETER
```
- `PARAMETER`: `some_sensible_default`, perhaps with an explanation of why, or how to determine alternatives

### Editing Files In Place

When an individual file needs to be modified in place (as opposed to replacing it outright), a Before and After quote will be provided that identifies one or more lines in the file, what the default installed version of that file looks like on that line, and what the line should look like after it has been modified, like so:

`/path/to/file_being_modified`

**Before**:
> 174 | Here is what line 174 in the file looked like before

> 175 | And here is what the following line looked like in the file before

**After**:
> 174 | Here is what line 174 should look like after modification

> 175 | And here is what the following line should look like after modification: VARIABLE

- `VARIABLE`: `some_value`, perhaps with an explanation of why

It should be noted that configuration files and the like are subject to change by the organizations that maintain their respective applications; this guide generally recommends installing the latest version of these applications, as these generally include security updates. It is expected that the implementer will be able to search through a file and find specific lines in the case where the maintaining organization has moved it in a subsequent patch. In most cases, configuration files will be provided outright to avoid these scenarios.

### Adding or Replacing Files

When a file needs to be added or replaced, it will be described in three sections:

- A line that describes the path to the file, as well as the owner, group, and umask for the file; it is assumed that the person following the guide will use `chmod` and `chown` appropriately to apply the owner, group, and umask
- The entire contents of the file in a code block, including any portions that need to be replaced with specific values
- Those replacement values

`/the/path/to/some/file.php | owner:group/umask`
```php
<?php

/**
 * Here is the entire contents of a PHP file
 */
function do_something($to_this) {
  $to_this += THE_NUMBER_TO_ADD_TO_THIS;
}

?>
```
- `THE_NUMBER_TO_ADD_TO_THIS`: 12, perhaps with an explanation of why, or other numbers that may be appropriate

### Troubleshooting

The most common issues you will likely run into when manually provisioning a server are:

- Files or directories are not owned by the user who needs access to them, and can therefore not be written to. Check the ownership of files using `ls -la`, and ensure their ownership using `chown USER` for files, and `chown -R USER` for directories
- Replacement variables were left in place in files specified by the guide. Ensure any replacement variables such as server addresses and passwords are swapped out when writing files to the server

For any other issues, don't hesitate to email the [mailing list](islandora@googlegroups.com) to ask for help. If you think that a part of the installation documentation is incorrect or could be improved, please create an issue in the [http://github.com/Islandora/documentation/issues](documentation issues queue) and give it a `documentation` tag. Bear in mind that this guide is built for Ubuntu 18.04 and attempts to give generalized instructions; you will likely naturally encounter situations where your own environment needs to differ from the guide.

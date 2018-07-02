## Setting `HUNTER_ROOT` variable

By default, if you haven't set a `HUNTER_ROOT` environment variable for your system, the install files will be located in this directory.

If you prefer to share your Hunter C++ libraries in a single directory across your system, you should set the `HUNTER_ROOT` variable. If you're not sure how to do this you can check [here](https://github.com/ruslo/hunter/wiki/Requirements#hunter_root).

If you're using a Mac, you may need to use `~/.bash_profile` instead of `~/.bashrc`. If neither file exists, run:

```bash
touch ~/.bashrc
```

Then open the file and add the `export` line mentioned in the link to the end.

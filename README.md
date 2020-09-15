# sloth

**sloth** is a small utility that reads stdin and echoes it back out very, _very_ slowly.

By default, it will sleep for 10ms between input bytes, but you can specify a duration with an argument:

```
cat list.txt | sloth 25  # sleep for 25 milliseconds per byte

cat list.txt | sloth 3s  # sleep for 3 seconds per byte
```

Supported suffixes:

- `ms` (milliseconds)
- `s` (seconds)
- `m` (minutes)
- `h` (hours)

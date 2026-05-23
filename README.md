# AmxxArch

![AMX Mod X](https://img.shields.io/badge/AMX%20Mod%20X-1.9%2B-blue)
![Platform](https://img.shields.io/badge/platform-Linux%20x86%20%7C%20Windows%20x86-brightgreen)
![C++](https://img.shields.io/badge/C%2B%2B-20-orange)
![7--Zip](https://img.shields.io/badge/backend-7--Zip-lightgrey)

**AmxxArch** is an AMX Mod X module for archive extraction, powered by an embedded 7-Zip backend.

## License

AmxxArch is licensed under the [MIT License](LICENSE).

7-Zip license information is available in [`licenses/7zip.txt`](licenses/7zip.txt).

## API

### `arch_extract`

```pawn
native arch_extract(
  const archive_path[],
  const output_dir[],
  const callback[],
  ArchFormat:format = ARCH_FORMAT_AUTO,
  const password[] = ""
);
```

Starts an extraction job.

`format = ARCH_FORMAT_AUTO` detects the archive type from the archive filename. Use an explicit `ARCH_FORMAT_*` value if the filename has no useful extension or if you want to avoid auto detection.

`password` can be omitted for non-encrypted archives.

Returns a positive job id on success, or a negative `ArchError` code on failure.

Callback signature:

```pawn
public callback(job_id, ArchEvent:event, const entry[], processed, total, const message[])
```

### `arch_cancel`

```pawn
native arch_cancel(job_id);
```

Requests cancellation for a running job.

Returns `1` if the job still exists and cancellation was requested. Already queued file, progress, and done callbacks can be suppressed by the module, but files that were already written to disk are not rolled back.

### Extracted file list

When `ARCH_EVENT_DONE` is fired, the module still keeps the finished job alive while your callback is running. That means you can ask it which files were extracted without scanning the output folder yourself.

```pawn
native arch_get_file_count(job_id);
native arch_get_file_name(job_id, index, buffer[], maxlen);
native arch_get_file_path(job_id, index, buffer[], maxlen);
```

`arch_get_file_name` returns the entry name from the archive.

`arch_get_file_path` returns the real extracted path on disk.

These natives are meant to be used inside `ARCH_EVENT_DONE`. After the callback finishes, the job is cleaned up automatically.

## Supported formats

| Format | Enum | Common extensions |
| --- | --- | --- |
| Auto detect | `ARCH_FORMAT_AUTO` | Detected from filename. |
| TAR | `ARCH_FORMAT_TAR` | `.tar` |
| GZip | `ARCH_FORMAT_GZIP` | `.gz`, `.gzip` |
| BZip2 | `ARCH_FORMAT_BZIP2` | `.bz2` |
| XZ | `ARCH_FORMAT_XZ` | `.xz` |
| TAR + GZip | `ARCH_FORMAT_TAR_GZIP` | `.tar.gz`, `.tgz` |
| TAR + BZip2 | `ARCH_FORMAT_TAR_BZIP2` | `.tar.bz2`, `.tbz2`, `.tbz` |
| TAR + XZ | `ARCH_FORMAT_TAR_XZ` | `.tar.xz`, `.txz` |
| ZIP | `ARCH_FORMAT_ZIP` | `.zip` |
| RAR | `ARCH_FORMAT_RAR` | `.rar` |
| 7-Zip | `ARCH_FORMAT_7Z` | `.7z` |

## Events

| Event | Meaning |
| --- | --- |
| `ARCH_EVENT_STARTED` | Extraction job started. |
| `ARCH_EVENT_FILE` | A file entry is being extracted. `entry[]` contains the entry name. |
| `ARCH_EVENT_PROGRESS` | Progress update. `processed` and `total` contain byte counters when available. |
| `ARCH_EVENT_DONE` | Extraction finished successfully. |
| `ARCH_EVENT_ERROR` | Extraction failed. `message[]` contains the reason. |
| `ARCH_EVENT_CANCELLED` | Job was cancelled. |

## Errors

| Error | Value | Meaning |
| --- | ---: | --- |
| `ARCH_ERROR_OK` | `0` | No error. |
| `ARCH_ERROR_INVALID_PARAMS` | `-1` | Bad native arguments. |
| `ARCH_ERROR_BAD_FORMAT` | `-2` | Bad explicit format value. |
| `ARCH_ERROR_UNKNOWN_FORMAT` | `-3` | Format could not be detected or is not supported. |
| `ARCH_ERROR_BAD_CALLBACK` | `-4` | Callback function was not found. |
| `ARCH_ERROR_SHUTTING_DOWN` | `-5` | Module is shutting down. |
| `ARCH_ERROR_FILE_NOT_FOUND` | `-6` | Archive file does not exist. |

## Example

```pawn
#include <amxmodx>
#include <amxxarch>

public plugin_init() {
  register_plugin("AmxxArch Example", "1.0", "AmxxArch")

  new const job_id = arch_extract(
    "cstrike/addons/amxmodx/data/test.zip",
    "cstrike/addons/amxmodx/data/out",
    "on_archive_event"
  )
  server_print("job %d started", job_id)
}

public on_archive_event(job_id, ArchEvent:event, const entry[], processed, total, const message[]) {
  switch (event) {
    case ARCH_EVENT_FILE:
      server_print("extracting: %s", entry)

    case ARCH_EVENT_DONE:
      server_print("archive extracted")

    case ARCH_EVENT_ERROR:
      server_print("extract failed: %s", message)
  }
}
```

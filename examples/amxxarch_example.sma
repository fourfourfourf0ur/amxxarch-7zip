#include <amxmodx>
#include <amxxarch>

public plugin_init() {
  register_plugin("AmxxArch Example", "1.0", "AmxxArch")

  new const job_id = arch_extract(
    "cstrike/addons/amxmodx/data/test.zip",
    "cstrike/addons/amxmodx/data/out",
    "on_archive_event"
  )

  if (job_id < 0) {
    print_start_error(job_id)
    return
  }

  server_print("job %d started", job_id)
}

public on_archive_event(job_id, ArchEvent:event, const entry[], processed, total, const message[]) {
  switch (event) {
    case ARCH_EVENT_STARTED: {
      server_print("extraction started")
    }

    case ARCH_EVENT_FILE: {
      server_print("extracting: %s", entry)
    }

    case ARCH_EVENT_PROGRESS: {
      if (total > 0)
        server_print("progress: %d/%d", processed, total)
    }

    case ARCH_EVENT_DONE: {
      server_print("archive extracted")
      print_extracted_files(job_id)
    }

    case ARCH_EVENT_ERROR: {
      server_print("extract failed: %s", message)
    }

    case ARCH_EVENT_CANCELLED: {
      server_print("extraction cancelled")
    }
  }
}

print_extracted_files(job_id) {
  new const count = arch_get_file_count(job_id)
  if (!count) {
    server_print("no extracted files")
    return
  }

  new name[256]
  new path[256]

  server_print("extracted files: %d", count)

  for (new i = 0; i < count; i++) {
    if (!arch_get_file_name(job_id, i, name, charsmax(name)))
      continue

    if (!arch_get_file_path(job_id, i, path, charsmax(path)))
      continue

    server_print("file: %s", name)
    server_print("path: %s", path)
  }
}

print_start_error(error) {
  switch (error) {
    case ARCH_ERROR_INVALID_PARAMS:
      server_print("bad native arguments")

    case ARCH_ERROR_BAD_FORMAT:
      server_print("bad archive format value")

    case ARCH_ERROR_UNKNOWN_FORMAT:
      server_print("unknown archive format")

    case ARCH_ERROR_BAD_CALLBACK:
      server_print("callback function was not found")

    case ARCH_ERROR_SHUTTING_DOWN:
      server_print("module is shutting down")

    case ARCH_ERROR_FILE_NOT_FOUND:
      server_print("archive file not found")

    default:
      server_print("failed to start extraction: %d", error)
  }
}
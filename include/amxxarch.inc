#if defined _amxxarch_included
  #endinput
#endif
#define _amxxarch_included

#pragma library amxxarch

enum ArchFormat {
  ARCH_FORMAT_AUTO = 0,
  ARCH_FORMAT_TAR,
  ARCH_FORMAT_GZIP,
  ARCH_FORMAT_BZIP2,
  ARCH_FORMAT_XZ,
  ARCH_FORMAT_TAR_GZIP,
  ARCH_FORMAT_TAR_BZIP2,
  ARCH_FORMAT_TAR_XZ,
  ARCH_FORMAT_ZIP,
  ARCH_FORMAT_RAR,
  ARCH_FORMAT_7Z
};

enum ArchEvent {
  ARCH_EVENT_STARTED = 1,
  ARCH_EVENT_FILE,
  ARCH_EVENT_PROGRESS,
  ARCH_EVENT_DONE,
  ARCH_EVENT_ERROR,
  ARCH_EVENT_CANCELLED
};

enum ArchError {
  ARCH_ERROR_OK = 0,
  ARCH_ERROR_INVALID_PARAMS = -1,
  ARCH_ERROR_BAD_FORMAT = -2,
  ARCH_ERROR_UNKNOWN_FORMAT = -3,
  ARCH_ERROR_BAD_CALLBACK = -4,
  ARCH_ERROR_SHUTTING_DOWN = -5,
  ARCH_ERROR_FILE_NOT_FOUND = -6
};

/**
 * Starts extraction.
 *
 * Callback signature:
 *   public callback(job_id, ArchEvent:event, const entry[], processed, total, const message[])
 *
 * format = ARCH_FORMAT_AUTO detects by archive filename.
 * password can be omitted for non-encrypted archives.
 * Returns positive job id on success, or ArchError negative code on failure.
 */
native arch_extract(const archive_path[], const output_dir[], const callback[], ArchFormat:format = ARCH_FORMAT_AUTO, const password[] = "");

/**
 * Requests cancellation for a running job.
 *
 * Returns 1 if the job still exists and cancellation was requested.
 * Already queued file/progress/done callbacks can be suppressed by the module,
 * but files that were already written to disk are not rolled back.
 */
native arch_cancel(job_id);

/**
 * Returns number of extracted files for a job.
 *
 * Intended to be used inside ARCH_EVENT_DONE callback.
 * Returns 0 if the job is unknown or no files were extracted.
 */
native arch_get_file_count(job_id);

/**
 * Gets extracted file entry name by index.
 *
 * Entry name is the path inside archive.
 * Intended to be used inside ARCH_EVENT_DONE callback.
 * Returns 1 on success, 0 on failure.
 */
native arch_get_file_name(job_id, index, buffer[], maxlen);

/**
 * Gets extracted file output path by index.
 *
 * Output path is the real path where the file was extracted.
 * Intended to be used inside ARCH_EVENT_DONE callback.
 * Returns 1 on success, 0 on failure.
 */
native arch_get_file_path(job_id, index, buffer[], maxlen);

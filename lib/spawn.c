#include "spawn.h"
#include <string.h>

void spawn_simple(char *command) {
  int result;

  if ((result = try_spawn_simple(command)) != 0) {
    throw("Command '%s' returned error code %d", command, result);
  }
}

int try_spawn_simple(char *command) {
  return system(command);
}

void spawn(
    char *command,
    bool blocking,
    char **pout_filename,
    char **perr_filename,
    char **pcompleted_filename) {
  int result;

  if ((result = try_spawn(
          command,
          blocking,
          pout_filename,
          perr_filename,
          pcompleted_filename)) != 0) {
    throw("Command '%s' returned error code %d", command, result);
  }
}

int try_spawn(
    char *command,
    bool blocking,
    char **pout_filename,
    char **perr_filename,
    char **pcompleted_filename) {
  char *out_filename = null;
  char *err_filename = null;
  char *completed_filename = null;
  char *full_command = null;
  int result;

  string_new_temp_filename(&out_filename);
  string_new_temp_filename(&err_filename);
  string_new_temp_filename(&completed_filename);

  string_new_f(
      &full_command,
      "exec 1>%s; exec 2>%s; %s%s && touch %s%s",
      out_filename,
      err_filename,
      blocking ? "" : "nohup ",
      command,
      completed_filename,
      blocking ? "" : " &");
  result = system(full_command);
  string_delete(&full_command);

  if (pout_filename != null) {
    donate(&out_filename, pout_filename);
  }

  if (perr_filename != null) {
    donate(&err_filename, perr_filename);
  }

  if (pcompleted_filename != null) {
    donate(&completed_filename, pcompleted_filename);
  }

  string_delete(&completed_filename);
  string_delete(&err_filename);
  string_delete(&out_filename);

  return result;
}

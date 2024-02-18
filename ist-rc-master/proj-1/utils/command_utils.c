#include "command_utils.h"

int check_status(const int command, const char *status) {
  const int aur_status_len = 3, dlr_status_len = 2, bkr_status_len = 3,
            rsr_status_len = 3, lfd_status_len = 1, ddr_status_len = 2;

  const char *aur_status[] = {"OK", "NOK", "NEW"};
  const char *dlr_status[] = {"OK", "NOK"};
  const char *bkr_status[] = {"OK", "EOF", "ERR"};
  const char *rsr_status[] = {"OK", "EOF", "ERR"};
  const char *lfd_status[] = {"NOK"};
  const char *ddr_status[] = {"OK", "NOK"};

  const char **container;
  int container_len;

  switch (command) {
    case AUR_COMMAND:
      container = aur_status;
      container_len = aur_status_len;
      break;

    case DLR_COMMAND:
      container = dlr_status;
      container_len = dlr_status_len;
      break;

    case BKR_COMMAND:
      container = bkr_status;
      container_len = bkr_status_len;
      break;

    case RSR_COMMAND:
      container = rsr_status;
      container_len = rsr_status_len;
      break;

    case LFD_COMMAND:
      container = lfd_status;
      container_len = lfd_status_len;
      break;

    case DDR_COMMAND:
      container = ddr_status;
      container_len = ddr_status_len;
      break;
  }

  int checker = is_str_in_container(container, container_len, status);
  if (checker == -1) {
    return S_NOT_STATUS;
  }

  return parse_status(container[checker]);
}

int parse_status(const char *status) {
  if (strcmp(status, "OK") == 0) {
   return S_OK;

  } else if (strcmp(status, "NOK") == 0) {
   return S_NOK;

  } else if (strcmp(status, "EOF") == 0) {
   return S_EOF;

  } else if (strcmp(status, "ERR") == 0) {
   return S_ERR;

  } else if (strcmp(status, "NEW") == 0) {
   return S_NEW;
  }

  return S_NOT_STATUS;
}

int check_command(const char *command) {
  if (strcmp(command, "AUT") == 0) {
   return AUT_COMMAND;

  } else if (strcmp(command, "AUR") == 0) {
   return AUR_COMMAND;

  } else if (strcmp(command, "DLU") == 0) {
   return DLU_COMMAND;

  } else if (strcmp(command, "DLR") == 0) {
   return DLR_COMMAND;

  } else if (strcmp(command, "BCK") == 0) {
   return BCK_COMMAND;

  } else if (strcmp(command, "BKR") == 0) {
   return BKR_COMMAND;

  } else if (strcmp(command, "RST") == 0) {
   return RST_COMMAND;

  } else if (strcmp(command, "RSR") == 0) {
   return RSR_COMMAND;

  } else if (strcmp(command, "LSD") == 0) {
   return LSD_COMMAND;

  } else if (strcmp(command, "LDR") == 0) {
   return LDR_COMMAND;

  } else if (strcmp(command, "LSF") == 0) {
   return LSF_COMMAND;

  } else if (strcmp(command, "LFD") == 0) {
   return LFD_COMMAND;

  } else if (strcmp(command, "DEL") == 0) {
   return DEL_COMMAND;

  } else if (strcmp(command, "DDR") == 0) {
   return DDR_COMMAND;

  } else if (strcmp(command, "UPL") == 0) {
   return UPL_COMMAND;

  } else if (strcmp(command, "UPR") == 0) {
   return UPR_COMMAND;

  } else if (strcmp(command, "RSB") == 0) {
   return RSB_COMMAND;

  } else if (strcmp(command, "RBR") == 0) {
   return RBR_COMMAND;

  } else {
   return ERR_COMMAND;
  }
}

char *get_command(const int command_code) {
  switch (command_code) {
    case AUT_COMMAND:
      return "AUT";
      break;

    case AUR_COMMAND:
      return "AUR";
      break;

    case DLU_COMMAND:
      return "DLU";
      break;

    case BCK_COMMAND:
      return "BCK";
      break;

    case BKR_COMMAND:
      return "BKR";
      break;

    case RST_COMMAND:
      return "RST";
      break;

    case RSR_COMMAND:
      return "RSR";
      break;

    case LSD_COMMAND:
      return "LSD";
      break;

    case LDR_COMMAND:
      return "LDR";
      break;

    case LSF_COMMAND:
      return "LSF";
      break;

    case LFD_COMMAND:
      return "LFD";
      break;

    case DEL_COMMAND:
      return "DEL";
      break;

    case DDR_COMMAND:
      return "DDR";
      break;

    case UPL_COMMAND:
      return "UPL";
      break;

    case UPR_COMMAND:
      return "UPR";
      break;

    case RSB_COMMAND:
      return "RSB";
      break;

    case RBR_COMMAND:
      return "RBR";
      break;

    default:
      fprintf(stdout, "Unknown command");
      break;
  }

  return NULL;
}

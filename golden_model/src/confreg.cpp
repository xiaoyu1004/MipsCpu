#include "confreg.h"
#include "utils.h"

int Confreg::get_word(int addr) {
  if ((addr & CONF_MASK) != CONF_BASE) {
    fatal_msg("invalid conf addr");
  }

  return 0;
}

void Confreg::set_word(int addr, int wdata) {
  if ((addr & CONF_MASK) != CONF_BASE) {
    fatal_msg("invalid conf addr");
  }
}

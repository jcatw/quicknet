#include <stdint.h>

#include "networknode.h"
#include "bstreap.h"
#include "krapivsky.h"

int main(int argc, char **argv) {
  krapivsky_model_t *km;
  km = krapivsky_bstreap_simulate_lnu(0.2, 3.5, 1.8, 1000);
}

#include "wokwi-api.h"
#include <stdlib.h>

typedef struct {
  pin_t pin_j;
  pin_t pin_k;
  pin_t pin_clk;
  pin_t pin_q;
  pin_t pin_qbar;
  uint32_t val_q;
  uint32_t val_qbar;
} chip_state_t;

static void update_output(chip_state_t *chip) {
  uint32_t j = pin_read(chip->pin_j);
  uint32_t k = pin_read(chip->pin_k);
  if (j == HIGH && k == LOW) {
    chip->val_q = HIGH;
    chip->val_qbar = LOW;
  } else if (j == LOW && k == HIGH) {
    chip->val_q = LOW;
    chip->val_qbar = HIGH;
  } else if (j == HIGH && k == HIGH) {
    if (chip->val_q == HIGH) {
      chip->val_q = LOW;
      chip->val_qbar = HIGH;
    } else {
      chip->val_q = HIGH;
      chip->val_qbar = LOW;
    }
  }
  pin_write(chip->pin_q, chip->val_q);
  pin_write(chip->pin_qbar, chip->val_qbar);
}

void chip_pin_change(void *user_data, pin_t pin, uint32_t value) {
  update_output(user_data); 
}

void chip_init() {
  chip_state_t *chip = malloc(sizeof(chip_state_t));
  chip->pin_j = pin_init("J", INPUT);
  chip->pin_k = pin_init("K", INPUT);
  chip->pin_clk = pin_init("clk", INPUT);
  chip->pin_q = pin_init("Q", OUTPUT);
  chip->pin_qbar = pin_init("Q_bar", OUTPUT);
  chip->val_q = LOW;
  chip->val_qbar = HIGH;
  pin_write(chip->pin_q, chip->val_q);
  pin_write(chip->pin_qbar, chip->val_qbar);
  const pin_watch_config_t watch_config = {
    .edge = RISING,
    .pin_change = chip_pin_change,
    .user_data = chip,
  };
  pin_watch(chip->pin_clk, &watch_config);
}

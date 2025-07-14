#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  pin_t pin_j;
  pin_t pin_k;
  pin_t pin_clk;
  pin_t pin_q;
  pin_t pin_qbar;
  uint32_t attr_id;
  bool val_q;
} chip_state_t;

void chip_pin_change(void *user_data, pin_t pin, uint32_t value) {
  chip_state_t *chip = (chip_state_t*)user_data;
  uint8_t j = pin_read(chip->pin_j);
  uint8_t k = pin_read(chip->pin_k);
  uint8_t id = attr_read(chip->attr_id);
  if (j == LOW && k == HIGH) {
    chip->val_q = false;
    pin_write(chip->pin_q, (uint32_t)chip->val_q);
    pin_write(chip->pin_qbar, (uint32_t)!chip->val_q);
  } else if (j == HIGH && k == LOW) {
    chip->val_q = true;
    pin_write(chip->pin_q, (uint32_t)chip->val_q);
    pin_write(chip->pin_qbar, (uint32_t)!chip->val_q);
  } else if (j == HIGH && k == HIGH) {
    chip->val_q = !chip->val_q;
    pin_write(chip->pin_q, (uint32_t)!chip->val_q);
    pin_write(chip->pin_qbar, (uint32_t)chip->val_q);
  } else {
    chip->val_q = chip->val_q;
    pin_write(chip->pin_q, (uint32_t)chip->val_q);
    pin_write(chip->pin_qbar, (uint32_t)!chip->val_q);
  }
  //printf("Chip %d: Q= %d, J = %d, K=%d\n",id, pin_read(chip->pin_q), j, k);
}

void chip_init() {
  chip_state_t *chip = malloc(sizeof(chip_state_t));
  chip->pin_j = pin_init("J", INPUT);
  chip->pin_k = pin_init("K", INPUT);
  chip->pin_clk = pin_init("clk", INPUT);
  chip->pin_q = pin_init("Q", OUTPUT_LOW);
  chip->pin_qbar = pin_init("Q_bar", OUTPUT_HIGH);
  chip->val_q = false;
  chip->attr_id = attr_init("id", 1);
  const pin_watch_config_t watch_config = {
    .edge = RISING,
    .pin_change = chip_pin_change,
    .user_data = chip,
  };
  pin_watch(chip->pin_clk, &watch_config);
}

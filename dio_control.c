/*
  dio_control.c - digital i/o control methods
  Part of Grbl v0.9

  Copyright (c) 2012-2014 Sungeun K. Jeon

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/  

#include "system.h"
#include "dio_control.h"
#include "protocol.h"
#include "gcode.h"


void dio_init()
{
  DIGITAL_IO_DDR |= (1 << DIGITAL_IO_BIT0);
  DIGITAL_IO_DDR |= (1 << DIGITAL_IO_BIT1);
  DIGITAL_IO_DDR |= (1 << DIGITAL_IO_BIT2);
  DIGITAL_IO_DDR |= (1 << DIGITAL_IO_BIT3);
  dio_stop();
}

void dio_stop()
{
  DIGITAL_IO_DDR &= ~(1 << DIGITAL_IO_BIT0);
  DIGITAL_IO_DDR &= ~(1 << DIGITAL_IO_BIT1);
  DIGITAL_IO_DDR &= ~(1 << DIGITAL_IO_BIT2);
  DIGITAL_IO_DDR &= ~(1 << DIGITAL_IO_BIT3);
}

void dio_immediate_run(uint8_t mode, uint8_t pin)
{
  if (sys.state == STATE_CHECK_MODE) { return; }

  protocol_auto_cycle_start();   //temp fix for M8 lockup
  protocol_buffer_synchronize(); // Ensure coolant turns on when specified in program.

  uint8_t bit = DIGITAL_IO_BIT3;
  switch (pin) {
    case 0: bit = DIGITAL_IO_BIT0; break;
    case 1: bit = DIGITAL_IO_BIT1; break;
    case 2: bit = DIGITAL_IO_BIT2; break;
    case 3: bit = DIGITAL_IO_BIT3; break;
  }

  if (mode == DIGITAL_OUTPUT_IMMEDIATE_ENABLE) {
    DIGITAL_IO_DDR |= (1 << bit);
  } else if (mode == DIGITAL_OUTPUT_IMMEDIATE_DISABLE) {
    DIGITAL_IO_DDR &= ~(1 << bit);
  } else {
    dio_stop();
  }
}

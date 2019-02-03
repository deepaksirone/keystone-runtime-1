//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "regs.h"
#include "sbi.h"
#include "timex.h"
#include "interrupt.h"
#include "printf.h"

#define DEFAULT_CLOCK_DELAY 40000

void init_timer(void)
{
  sbi_set_timer(get_cycles64() + DEFAULT_CLOCK_DELAY);
}

void handle_timer_interrupt()
{
  sbi_stop_enclave(0);
  unsigned long next_cycle = get_cycles64() + DEFAULT_CLOCK_DELAY;
  sbi_set_timer(next_cycle);
  return;
}

void handle_interrupts(struct encl_ctx_t* regs)
{
  unsigned long cause = regs->scause;

  switch(cause) {
    case INTERRUPT_CAUSE_TIMER:
      handle_timer_interrupt();
      break;
    /* ignore other interrupts */
    case INTERRUPT_CAUSE_SOFTWARE:
    case INTERRUPT_CAUSE_EXTERNAL:
    default:
      printf("[FATAL] Eyrie runtime could not handle the interrupt: 0x%lx\n", cause);
      sbi_stop_enclave(0);
      //SBI_CALL_1(SBI_SM_EXIT_ENCLAVE,-1);
      return;
  }
}

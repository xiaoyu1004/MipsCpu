#ifndef CONFREG_H
#define CONFREG_H

#define LED_ADDR  0xffff0000
#define SEG_ADDR  0xffff0001

#define CONF_BASE 0xffff0000
#define CONF_MASK 0xffff0000

class Confreg {
  public:
    int get_word(int addr);

    void set_word(int addr, int wdata);
};

#endif
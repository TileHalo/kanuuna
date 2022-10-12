#include <SPI.h>
#include <mcp2515.h>

#include "canusb.h"
#include "util.h"

#define BAUD 9600
#define QLEN 8

struct can_queue {
  struct can_frame q[QLEN];
  byte head, tail; /* Next to pop, next to push. Mod QLEN*/
};


byte can_queue_push(struct can_frame msg, struct can_queue *q);
struct can_frame can_queue_pop(struct can_queue *q);

void int_handler();
void write_to_serial(const struct can_frame cf);
bool read_from_serial();

const int baud = BAUD;
bool interrupt = false;

MCP2515 mcp(10);
struct can_frame can_msg, cmsg;
struct can_queue queue;

byte can_queue_push(struct can_frame msg, struct can_queue *q) {
  q->q[q->tail] = msg;
  q->tail = (q->tail + 1) % QLEN;
  return q->tail;
}

struct can_frame can_queue_pop(struct can_queue *q) {
  struct can_frame ret;

  ret = q->q[q->head];
  q->head = (q->head + 1) % QLEN;
  return ret;
}

void int_handler() {
  interrupt = true;
}
void write_to_serial(const struct can_frame cf) {
  /* char buf[19];
  int i;
  memset(&buf[0], 0, sizeof(buf));
  for (i = 7; i <= 0; i--) {
    buf[i] = byte_to_hex(cf.can_id >> i*4);
  }
  buf[8] = byte_to_hex(cf.can_dlc);
  for (i = 0; i < cf.can_dlc; i++) {
    buf[9 + i*2] = byte_to_hex(cf.data[i] >> 4);
    buf[9 + i*2 + 1] = byte_to_hex(cf.data[i]);
  }
  buf[9 + i*2 + 2] = '\r';
  Serial.print(buf); */
  Serial.print("t");
  Serial.print(can_msg.can_id, HEX);
  Serial.print(can_msg.can_dlc, HEX);
  for (int i = 0; i < can_msg.can_dlc; i++) {
    Serial.print(can_msg.data[i], HEX);
  }
  Serial.write('\r');
}

bool read_from_serial() {

  char msg[19];
  String s;

  memset(&msg[0], 0, sizeof(msg));
  s = Serial.readStringUntil('\r');
  s.toCharArray(msg, 19);
  Serial.print(parse_canusb(msg, mcp));

  return true;
}


void setup() {
  while(!Serial);
  Serial.begin(baud);
  mcp.reset();
  mcp.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp.setNormalMode();
  /* pinMode(2, INPUT_PULLUP); */
  /* attachInterrupt(0, int_handler, FALLING); */
  queue.head = 0;
  queue.tail = 0;
}

void loop() {
  MCP2515::ERROR err;
  /* if (queue.head != queue.tail) {
    cmsg = can_queue_pop(&queue);
    write_to_serial(cmsg);
  } */
  if (Serial.available() > 0) {
    if (!read_from_serial()) {
      Serial.println("Error");
      exit(1);
    }
  }
  err = mcp.readMessage(&can_msg);
  if (err == MCP2515::ERROR_OK) {
    write_to_serial(can_msg);
    /* can_queue_push(can_msg, &queue); */
  } else if (err != MCP2515::ERROR_NOMSG) {
    Serial.print("\a");
  }
}

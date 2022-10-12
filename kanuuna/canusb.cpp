#include <mcp2515.h>
#include "canusb.h"
#include "util.h"
#include <stdlib.h>

/* Parse CAN_ID into the structure. Can use both normal and extended formats */
static unsigned int parse_id(const char *iid, unsigned int id_len);

static unsigned int parse_data(const char *msg, struct can_frame *fr);

static unsigned int parse_id(const char *iid, unsigned int id_len) {
  unsigned int id;

  id = 0;
  for (int i = 0; i < id_len; i++) {
    id |= hex_to_nib(iid[i]) << ((id_len - 1) * 4 - i * 4);
  }

  return id;
}

static unsigned int parse_data(const char *msg, struct can_frame *fr) {

  fr->can_dlc = hex_to_nib(msg[0]);
  for (int i = 0; i < fr->can_dlc; i++) {
    fr->data[i] = hex_to_nib(msg[i * 2 + 1]) << 4;
    fr->data[i] |= hex_to_nib(msg[i * 2 + 2]);
  }

  return 0;
}

const char *parse_canusb(const char *msg, MCP2515 mcp) {
  struct can_frame fr;
  // Serial.println(msg[1]);
  // return msg;
  switch (msg[0]) {
  case 'S':
    switch (msg[1]) {
    case '0':
      if (mcp.setBitrate(CAN_10KBPS, MCP_8MHZ) != MCP2515::ERROR_OK)
        return "\a";
      return "\r";
    case '1':
      if (mcp.setBitrate(CAN_20KBPS, MCP_8MHZ) != MCP2515::ERROR_OK)
        return "\a";
      return "\r";
    case '2':
      if (mcp.setBitrate(CAN_50KBPS, MCP_8MHZ) != MCP2515::ERROR_OK)
        return "\a";
      return "\r";
    case '3':
      if (mcp.setBitrate(CAN_100KBPS, MCP_8MHZ) != MCP2515::ERROR_OK)
        return "\a";
      return "\r";
    case '4':
      if (mcp.setBitrate(CAN_125KBPS, MCP_8MHZ) != MCP2515::ERROR_OK)
        return "\a";
      return "\r";
    case '5':
      if (mcp.setBitrate(CAN_250KBPS, MCP_8MHZ) != MCP2515::ERROR_OK)
        return "\a";
      return "\r";
    case '6':
      if (mcp.setBitrate(CAN_500KBPS, MCP_8MHZ) != MCP2515::ERROR_OK)
        return "\a";
      return "\r";
    case '7':
      return "\a";
    case '8':
      if (mcp.setBitrate(CAN_1000KBPS, MCP_8MHZ) != MCP2515::ERROR_OK)
        return "\a";
      return "\r";
    default:
      return "\a";
    }
  case 's':
    return "\a";
  case 'O':
    if (mcp.setNormalMode() != MCP2515::ERROR_OK)
      return "\a";
    return "\r";
  case 'D':
    if (mcp.setLoopbackMode() != MCP2515::ERROR_OK)
      return "\a";
    return "\r";
  case 'C':
    if (mcp.reset() != MCP2515::ERROR_OK)
      return "\a";
    return "\r";
  case 't':
    fr.can_id = parse_id(&msg[1], 3);
    parse_data(&msg[4], &fr);
    if (mcp.sendMessage(&fr) != MCP2515::ERROR_OK)
      return "\a";
    return "z\r";
  case 'T':
    fr.can_id = parse_id(&msg[1], 8) | CAN_EFF_FLAG;
    parse_data(&msg[9], &fr);
    if (mcp.sendMessage(&fr) != MCP2515::ERROR_OK)
      return "\a";
    return "Z\r";
  case 'r':
    fr.can_id = parse_id(&msg[1], 3);
    fr.can_id |= CAN_RTR_FLAG;
    fr.can_dlc = hex_to_nib(msg[4]);
    if (mcp.sendMessage(&fr) != MCP2515::ERROR_OK)
      return "\a";
    return "z\r";
  case 'R':
    fr.can_id = parse_id(&msg[1], 8);
    fr.can_id |= CAN_RTR_FLAG | CAN_EFF_FLAG;
    fr.can_dlc = hex_to_nib(msg[9]);
    if (mcp.sendMessage(&fr) != MCP2515::ERROR_OK)
      return "\a";
    return "Z\r";
  case 'F':
    return "F00\r";
  case 'M':
    return "\a";
  case 'm':
    return "\a";
  case 'V':
    return "V1013\r";
  case 'N':
    return "\a";
  case 'Z':
    return "\a";
  case 'L':
    if (mcp.setListenOnlyMode() != MCP2515::ERROR_OK)
      return "\a";
    return "\a";
  default:
    return "\a";
  }
}

// vi: ft=arduino

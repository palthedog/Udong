#ifndef BUTTON_CONFIG_H
#define BUTTON_CONFIG_H

/*
#include <ArduinoJson.h>

enum ButtonType {
  PushButton,
  DPadButton,
  // TriggerButton,
  // AnalogTriggerButton,
};

struct PushButtonSelector {
  int push_button_id;
};

inline void convertFromJson(JsonVariantConst var, PushButtonSelector& dst) {
  dst.push_button_id = var["push_button_id"];
}

inline bool convertToJson(const PushButtonSelector& src, JsonVariant dst) {
  dst["push_button_id"] = src.push_button_id;
  return true;
}

enum DPadDirection { Up, Right, Down, Left };
struct DPadButtonSelector {
  DPadDirection direction;
  DPadButtonSelector() : direction(Up) {
  }

  DPadButtonSelector(DPadDirection dir) : direction(dir) {
  }
};

inline void convertFromJson(JsonVariantConst var, DPadButtonSelector& dst) {
  if (var["direction"] == "up") {
    dst.direction = Up;
  } else if (var["direction"] == "right") {
    dst.direction = Right;
  } else if (var["direction"] == "down") {
    dst.direction = Down;
  } else if (var["direction"] == "left") {
    dst.direction = Left;
  }
}

inline bool convertToJson(const DPadButtonSelector& src, JsonVariant dst) {
  switch (src.direction) {
    case Up:
      dst["direction"] = "up";
      break;
    case Right:
      dst["direction"] = "right";
      break;
    case Down:
      dst["direction"] = "down";
      break;
    case Left:
      dst["direction"] = "left";
      break;
  }
  return true;
}

struct ButtonId {
  ButtonType type;

  union Selector {
    PushButtonSelector push_button;
    DPadButtonSelector d_pad;

    Selector(const PushButtonSelector& p) : push_button(p) {
    }
    Selector(const DPadButtonSelector& d) : d_pad(d) {
    }
  } selector;

  ButtonId() : ButtonId(PushButtonSelector{0}) {
  }

  ButtonId(const PushButtonSelector& p) : type(PushButton), selector(p) {
  }

  ButtonId(const DPadButtonSelector& d) : type(DPadButton), selector(d) {
  }
};

// const std::vector<ButtonId>& getAllButtonIds();

inline void convertFromJson(JsonVariantConst var, ButtonId& dst) {
  String type_str = var["type"];
  if (type_str == "push") {
    dst.type = ButtonType::PushButton;
    dst.selector.push_button = var["push_button"];
  } else if (type_str == "d-pad") {
    dst.type = ButtonType::DPadButton;
    dst.selector.d_pad = var["d_pad"];
  } else {
    Serial.printf("ERROR: unknown button type: %s\n", type_str.c_str());
  }
}

inline bool convertToJson(const ButtonId& src, JsonVariant dst) {
  switch (src.type) {
    case PushButton:
      dst["type"] = "push";
      dst["push_button"] = src.selector.push_button;
      break;
    case DPadButton:
      dst["type"] = "d-pad";
      dst["d_pad"] = src.selector.d_pad;
      break;
  }
  return true;
}

struct ButtonAssignment {
  int switch_id;
  ButtonId button_id;
};

inline void convertFromJson(JsonVariantConst var, ButtonAssignment& dst) {
  dst.switch_id = var["switch_id"];
  dst.button_id = var["button_id"];
}

inline bool convertToJson(const ButtonAssignment& src, JsonVariant dst) {
  dst["switch_id"] = src.switch_id;
  dst["button_id"] = src.button_id;
  return true;
}
*/
#endif

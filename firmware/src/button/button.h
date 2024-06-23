#ifndef BUTTON_H
#define BUTTON_H

enum ButtonType {
  PushButton,
  DPadButton,
};

struct PushButtonSelector {
  int push_button_id;
};

struct DPadButtonSelector {
  enum DPadDirection { Up, Right, Down, Left };
  DPadDirection direction;
};

struct ButtonId {
  ButtonType type;

  union Selector {
    PushButtonSelector push_button;
    DPadButtonSelector d_pad;
  };
};

#endif

Import("env")

if "test" in env.GetBuildType():
  exit(0)

board_config = env.BoardConfig()
USB_VID = "0x16c0"
USB_PID = "0x27dc"

board_config.update("build.arduino.earlephilhower.usb_vid", USB_VID)
board_config.update("build.arduino.earlephilhower.usb_pid", USB_PID)

# should be array of VID:PID pairs
board_config.update("build.hwids", [
  [USB_VID, USB_PID],
])

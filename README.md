# Trouble Shooting

## Failed to install packages (at arduino-pico.git)
```error
VCSBaseException: VCS: Could not process command ['git', 'clone', '--recursive', 'https://github.com/earlephilhower/arduino-pico.git', 'C:\\Users\\safii\\.platformio\\.cache\\tmp\\pkg-installing-l4vwgb1f']
```
You have to enable long-path name on Windows
See: https://arduino-pico.readthedocs.io/en/latest/platformio.html#important-steps-for-windows-users-before-installing

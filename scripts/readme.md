# scripts

this subdirectory contains build toolchain setup and other auxiliary scripts

## windows install

right-click "install-msys2.ps1" and click "Run with PowerShell" in the context
menu. this will open powershell, install msys2, and close the powershell window
once it's done installing.

open "MSYS2 MINGW64" from the windows start menu, and use the `cd` command to
navigate to this folder. drive letters (`C:\`) are written like `/c/` in msys2,
and paths use forward slashes instead of backward slashes. e.g. `cd
/c/Users/Loek/Documents/project-weerstation/scripts`.

run `sh install-packages-mingw.sh`, and wait for the text "installing packages
done!" to appear.

you've now installed the necessary tools to compile, upload and debug the stm32
firmware. shortcuts for commonly used commands are implemented as visual studio
code tasks, so you don't have to be a terminal ninja.

### notes

- if you're experiencing libusb-related issues while using st-link, try using
  [zadig](https://zadig.akeo.ie/) to update the usb driver.
- make sure to use the 64-bit version of msys2 (titled "MSYS2 MINGW64" in your
  start menu)

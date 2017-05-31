#!/usr/bin/env sh

# Build wxWidgets
if [ ! "$(ls -A ${HOME}/.wxwidgets)" ]; then
  wget -q -O wxwidgets.tar.bz2 "https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.0/wxWidgets-3.1.0.tar.bz2" > /dev/null
  tar -xjf wxwidgets.tar.bz2
  cd wxWidgets-3.1.0
  ./configure --prefix=${HOME}/.wxwidgets --enable-unicode --disable-shared --enable-mediactrl=no --enable-webview --enable-webview-webkit --silent
  make -j2 > /dev/null
  make install > /dev/null
  cd ..
else 
  printf 'wxWidgets will not be built (%s/.wxwidgets not empty).\n' "$HOME"
fi
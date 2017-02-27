# Kuku Kube Bot

This project is a bot for [kuku kube](http://kuku-kube.com) game, in order to achieve high scores. It is specially designed for this specific version of the game. However, it can easily be adjusted for any another version with same implementation but different graphics. Available only for linux based operating systems. Configuration required before each use, see the SETTINGS block in code.

## Compile
gcc -Wall kuku-bot.c -lX11 -lXtst -o kuku-bot

## Showcase
[youtube](https://www.youtube.com/watch?v=Yp1kWO3cJL8)

## Warning
This c bot takes control of your mouse pointer for a minute.

It is highly recommended to create a new keyboard shortcut so you can stop it, in case something goes wrong.
example: "killall kuku_bot"

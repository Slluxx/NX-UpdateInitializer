# NX-UpdateInitializer
 
This homebrew is a frankenstein version of [NX-Update-Checker](https://github.com/16BitWonder/NX-Update-Checker) and [UpThemAll](https://github.com/HookedBehemoth/UpThemAll)

NX-Update-Checker is used to determine what games on your console have updates available and writes them to SDCard. UpThemAll is used to loop through all installed titles and trigger an automatic update (the kind of update you would do when launching the game).

NX-UpdateInitializer takes the best of both. It checks for updates and only triggers the updates on the games that are installed on the console and only if updates are available for that game. This prevents requests beeing made for NSP forwarders and titles that dont have updates - which should reduce the telemetry beeing send to nintendo.

This only works if you dont have 90DNS or incognito. It does NOT help to avoid bans. Please dont use this tool if you pirate your games!

This is just a herlper tool to trigger update downloads for all your games.
A tiny C++ WinAPI program for parsing a file with birthday dates and showing notifications.

You can git it's binaries [here](https://yadi.sk/d/3JUvq5LgegeYy).

Usage:
1. Create a database-file, eg. ``C:\Users\kostya\BitTorrentSynced\birthdays.txt`` with a format like this:

    10.11. Wife
    15.09. Mother
    09.02. Father
    14.04. Grandma
    15.05. Dog
    16.05. Get acknowlegded
    03.08. Wedding

2. When run ``Birthdays.exe C:\Users\kostya\BitTorrentSynced\birthdays.txt``
3. Notifications should appear if some event will happen today or tomorrow.

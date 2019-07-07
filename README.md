# GamerSaver
Cloud-based management of saves for games. Developed using Qt and C++.

Currently integrated with Google Drive as the cloud storage platform
* Needs Google account to sign in and use app!

# Status
* Undergoing rewrite

# Current Features
* List local save games for supported games

# Roadmap
## V0.1.0
* Allow extensibility with custom games list
* OAuth 2.0 Authorization and login for Google Drive support
* Create support for loading a picture icon for the logged in user

## V0.2.0
* Check access and permission to write to GamerSaver folder on user Drive account

## V0.3.0
* Implement export and import save features

## Long-Term Features
* Add extensibility (custom games support through user-created text file)
* Theme
* Switch to more profesional cloud storage platform if enough warrant it

# Supported Games
## Bethesda games
### Elder Scrolls Franchise
* Oblivion
* Skyrim
* Skyrim Special Edition

### Fallout Franchise
* Fallout 3
* Fallout New Vegas

# Installation
* Unsupported right now
* Will likely use GitHub's Release system in future when app is more deployable
* Or installation through a `ninja install` step

# Compile-time Dependencies
* QtWidgets
* QtNetwork
* QtCore
* meson
* ninja

# Building
1. `git clone https://github.com/nathanblair/GamerSaver.git`
2. `cd GamerSaver`
3. `meson build`
4. `ninja -C build`

# Testing
1. **After running `meson build`** `ninja -C build test`

# Contributing
1. Create an issue with your feature request/bug report
2. Clone to local dev environment if not done already
3. Make adjustments to implement new feature/fix bug
4. Submit Pull Request (PR)
5. Profit??

# Resetting Password
* Reauthorize with the desired username. This will prompt authentication from Google and, assuming you haven't left your logged-in Google sessions available on your browser for others to use, this means of authentication is viewed as sufficient for the scope of this application.

# Notes
## Notes on permissions, authorization, and security
Everything in this application runs locally. The OAuth redirect URI (the thing that allows Google to grant consent to GamerSaver to access your Google Drive account) is directed to your local loopback address (i.e. not accessible to the internet) and the app never stores information about your Google Credentials or login state.

The only power GamerSaver has is listed in the scope of the consent screen you will see upon first signing in. And to elaborate on that effect, GamerSaver will not be allowed to completely erase all of your Google Drive data willy-nilly or other such disastrous operation. Its scope of access involves only access to a folder delegated to it by the API.

In short, this application isn't trying to spy on you, collect your information, sell your information to third parties, or any combination of these nefarious schemes. This application is just wholesome usefulness. It should be a breath of fresh air. It is also fully open source; if you have any doubts about your security and privacy while using, feel free to check out and modify the source!

# Credits and Thanks
This application would not have been possible without the support of the [Qt Dev Documentation](http://doc.qt.io/) and the amazing support and answers available on [StackOverflow](https://stackoverflow.com/questions/tagged/c%2B%2B). Also, the great site [LearnCPP](http://www.learncpp.com/) was without equal in serving as a reference to refreshing my C++ skills.

Thank you to the amazing [NexusMods](http://www.nexusmods.com/games/?) community. The Nexus has been my lifeblood for the past 12 years now and I can't imagine my life of gaming without it. Robin and the team, you guys are the best.

Tannin42, thank you for the inspiration to build this application using Qt and C++. ModOrganizer remains my motivation for how to make software that works well, provides great features, extensive tools, and an intuitive user experience.

And most of all, thank ***you*** for reading this, downloading GamerSaver, using GamerSaver, and experiencing what I hope has been a refreshingly simple and straightforward software experience.


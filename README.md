# GamerSaver
Cloud-based management of saves for games. Developed using Qt and C++.

Currently integrated with Google Drive as the cloud storage platform
* Needs Google account to sign in and use app!


# Current Features
* List local save games for supported games
* OAuth 2.0 Authorization and login for Google Drive support

# Status
* Active development

# Upcoming Features
* Create support for loading a picture icon for the logged in user
* Check access and permission to write to GamerSaver folder on user Drive account
* Implement export and import save features
* Finish integrating Google Drive API calls for getting and pushing files
* Better encryption for user password and credential storing - I care about security and privacy!


# Long-Term Features
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
* Until then, you can build this yourself from source using Qt framework
* If you're not sure how to do that, for now, you'll have to do some research


# Contributing
1. Create an issue with your feature request/bug report
2. Clone to local dev environment
3. Make adjustments to implement new feature/fix bug
4. TEST [no test modules supplied, you'll have to make your own :( ]
5. Submit Pull Request (PR)
6. Profit??


# Bugs and Known Issues
* There is no timer or timeout monitoring for the loopback OAuth server. As such, closing out of the opened browser window will never trigger the application to stop waiting for a loopback response and authorization will never finish.
	* To fix this, you'll need to relaunch the application.
* No password recovery options are available. A password reset effect can be achieved by reauthorization with the desired username. This will prompt authentication from Google and, assuming you haven't left your logged-in Google sessions available on your browser for others to use, this means of authentication is viewed as sufficient for the scope of this application.


# Notes
## Notes on permissions, authorization, and security
Everything in this application runs locally. The user authentication database is not stored in some web-accessible database. It is stored completely on your machine, where I nor anyone else could ever access it. Even the OAuth redirect URI (the thing that allows Google to grant consent to GamerSaver to access your Google Drive account) is directed to your local loopback address (i.e. not accessible to the internet).

The user management aspect of this application, stored locally on your computer in the application's authentication database, is solely to make your Google Account access more secure. I collect none of this data. In fact, I don't have access to it even if I wanted to. GamerSaver does not "phone home"; its only home is right there on your personal computer!

The only power GamerSaver has is listed in the scope of the consent screen you will see upon first signing in. And to elaborate on that effect, GamerSaver will not be allowed to completely erase all of your Google Drive data willy-nilly or other such disastrous operation. Its scope of access involves only access to a folder delegated to it by the API.

In short, this is probably one of the few applications out there that isn't trying to spy on you, collect your information, sell your information to third parties, or any combination of these nefarious schemes. This application is just wholesome usefulness. It should be a breath of fresh air. It is also fully open source; if you have any doubts about your security and privacy while using, feel free to check out and modify the source!

# Credits and Thanks
This application would not have been possible without the support of the [Qt Dev Documentation](http://doc.qt.io/) and the amazing support and answers available on [StackOverflow](https://stackoverflow.com/questions/tagged/c%2B%2B). Also, the great site [LearnCPP](http://www.learncpp.com/) was without equal in serving as a reference to refreshing my C++ skills.

Thank you to the amazing [NexusMods](http://www.nexusmods.com/games/?) community. The Nexus has been my lifeblood for the past 12 years now and I can't imagine my life of gaming without it. Robin and the team, you guys are the best.

Tannin42, thank you for the inspiration to build this application using Qt and C++. ModOrganizer remains my motivation for how to make software that works well, provides great features, extensive tools, and an intuitive user experience.

And most of all, thank ***you*** for reading this, downloading GamerSaver, using GamerSaver, and experiencing what I hope has been a refreshingly simple and straightforward software experience.

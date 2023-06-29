# OpenLambda Project

## Overview

OpenLambda is a shared game code base for [OGS](https://gitlab.com/headcrab-junkyard/OGS) and [OGSNext](https://gitlab.com/headcrab-junkyard/OGSNext) game engines. 
It provides you with an engine and [SDK](https://gitlab.com/headcrab-junkyard/ogs-sdk) license-compatible game/mod development solution and allows you to create 
new mods (or even completely new games) for these engines

## Contributing

You're not obligated to do that, but if want to help the project, feel free to contribute. All contributions are welcome and even the slightest 
help will be appreciated. Pull/Merge Requests with code and documentation are especially appreciated, but you also can help by report about the 
issues you've found. The actual list of the issues can be found [here](https://gitlab.com/headcrab-junkyard/openlambda/-/issues). You can post any 
issue regarding the project you've discovered there (things that don't work properly or at all or something that needs an adjustment/improvements 
in your opinion)

Read below to learn how you can help improving the project:

* Fork the repo
* Get familiar with the development workflow, [Coding Guidelines](), [Code of Conduct](CODE_OF_CONDUCT.md) and learn how to sign your work
* Find an issue to work on, deal with it and submit a [Pull/Merge Request](https://gitlab.com/headcrab-junkyard/openlambda/-/merge_requests/new)
	* First time contributing to open source? Pick [a good first issue](https://gitlab.com/headcrab-junkyard/openlambda/-/issues?label_name%5B%5D=good+first+issue) to get you familiar with the contributing process
	* First time contributing to the project? Pick [a beginner friendly issue](https://gitlab.com/headcrab-junkyard/openlambda/-/issues?label_name%5B%5D=beginners) to get you familiar with the codebase and our contributing process
	* Want to become a Committer? Solve an issue showing that you understand the project's objectives and architecture. Here is [a good list to start](https://gitlab.com/headcrab-junkyard/openlambda/-/issues?label_name%5B%5D=help+wanted)
* Could not find an issue? Look for bugs, typos, and missing features

### Cans and Cant's

* You CAN incorporate a piece of HLSDK code by rewriting (retyping it line-by-line, symbol-by-symbol) it, but you CAN'T just copy-paste something from 
the source project here. The [HLSDK license](https://github.com/ValveSoftware/halflife/blob/master/LICENSE) is incompatible with [GPLv3](LICENSE)
* You CAN incorporate your own code pieces (the pieces you're have authored, i.e. those that you've written yourself) into the codebase and from that time they 
will be covered under the [GPLv3](LICENSE) terms
* You CAN use any game logic sources from the original Quake/Quake 2/Quake 3 and Doom 3 to improve the project's codebase or in order to implement a missing 
feature
* You CAN use the codebase as a base for a completely new game. The platform will allow you to use a completely custom game protocol on both client and 
server sides. You're also not limited by the legacy engine limits in such case and can do whatever you want
* You CAN use the codebase as a base for any mod (re)implementation but remember that this is a general-purpose game/mod base in the first place and 
in case you've made a decision to remake some mod on this platform you'll be responsible to maintain the thing. And remember that you need to fully rewrite 
it and not just copy-paste the sources from the original project (again, license incompatibility)
* You CAN expect to be mentioned as a contributor inside the source files and other files/parts you've contributed to
* You CAN expect to get help with your issues regarding the project but only if you've followed the rules and guidelines

## Feedback

* Star the [repo](https://gitlab.com/headcrab-junkyard/openlambda) to show your support and interest
* Ask a question, request a new feature and file a bug with [GitLab issues](https://gitlab.com/headcrab-junkyard/openlambda/-/issues/new)

## License

* [GNU GPLv3](LICENSE) for most of the codebase
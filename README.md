<p align="center">
  <a href="https://github.com/sys41x4/halkaDM">
    <img alt="Halka DM" src="/assets/logo.png" width="200" />
  </a>
</p>
<h1 align="center">
  Halka DM
</h1>

<p align="center">
<a href="https://github.com/sys41x4/halkaDM/blob/main/LICENSE" target="blank">
<img src="https://img.shields.io/github/license/sys41x4/halkaDM?style=flat-square" alt="halkaDM license" />
</a>
<a href="https://github.com/sys41x4/halkaDM/fork" target="blank">
<img src="https://img.shields.io/github/forks/sys41x4/halkaDM?style=flat-square" alt="halkaDM forks"/>
</a>
<a href="https://github.com/sys41x4/halkaDM/stargazers" target="blank">
<img src="https://img.shields.io/github/stars/sys41x4/halkaDM?style=flat-square" alt="halkaDM stars"/>
</a>
<a href="https://github.com/sys41x4/halkaDM/issues" target="blank">
<img src="https://img.shields.io/github/issues/sys41x4/halkaDM?style=flat-square" alt="halkaDM issues"/>
</a>
<a href="https://github.com/sys41x4/halkaDM/pulls" target="blank">
<img src="https://img.shields.io/github/issues-pr/sys41x4/halkaDM?style=flat-square" alt="halkaDM pull-requests"/>
</a>

</a>
</p>


<p align="center"><img src="/assets/cover_image.png" alt="halkaDM png"  width="auto" /></p>


<p align="center">
    <a href="https://github.com/sys41x4/halkaDM/issues/new/choose">Report Bug</a>
    ·
    <a href="https://github.com/sys41x4/halkaDM/issues/new/choose">Request Feature</a>
</p>

<p align="center">
<i>Loved the Project? Please consider <a href="https://paypal.me/sys41x4/10">donating</a>  💸 to help it improve!</i>
</p>

<p align="center">
<a href="https://www.paypal.me/sys41x4"><img src="https://img.shields.io/badge/support-PayPal-blue?logo=PayPal&style=flat-square&label=Donate" alt="sponsor Halka DM"/>
</a>
<a href='https://ko-fi.com/sys41x4' target='_blank'><img height='23' width="100" src='https://cdn.ko-fi.com/cdn/kofi3.png?v=2' alt='Buy Coffee for Arijit Bhowmick' />
</a>
<a href="https://www.buymeacoffee.com/sys41x4" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/default-orange.png" alt="Buy Me A Coffee" height="23" width="100" style="border-radius:1px" />
</p>

<p align="center">
<a href="https://sys41x4.github.io" target="blank">
<img src="https://img.shields.io/website?url=https%3A%2F%2Fsys41x4.github.io&logo=github&style=flat-square" />
</a>
<a href="https://twitter.com/intent/tweet?text=Wow:&url=https://github.com/sys41x4/halkaDM">
<img src="https://img.shields.io/twitter/url?style=social&url=https://github.com/sys41x4/halkaDM" alt="tweet Halka DM"/>
</a>
</p>

---
**Author & Developer :** <a href="https://github.com/Arijit-Bhowmick">ARIJIT BHOWMICK</a> [<a href="https://github.com/sys41x4">sys41x4</a>]
---
*Desc:* Halka DM is a Desktop Manager build with an intention of simplicity and security in mind, consuming minimal system resource to support low resource bounded systems.

**halkaDM:micro : Currently in Development<-->Testing Phase**
---

| S.No. | package | type | source | status
| - | - | - | - | - |
| 1 | halkaDM:gui | GUI | /source/gui/* | - |
| 2 | halkaDM:micro | TUI | /source/micro/* | under-development |
| 3 | halkaDM:nano | TUI | /source/nano/* | - |


Current Project status:<br>
`<[branch/dev]<--->[branch/testing]>`*-->[branch/alpha]-->[branch/beta]--><[branch/stable]<-->[branch/main]>*

PR's are welcome at **dev** Branch


Branches : 
[branch/main](https://github.com/sys41x4/halkaDM/tree/main) | 
[branch/stable](https://github.com/sys41x4/halkaDM/tree/stable) | 
[branch/beta](https://github.com/sys41x4/halkaDM/tree/beta) | 
[branch/alpha](https://github.com/sys41x4/halkaDM/tree/alpha) | 
[branch/testing](https://github.com/sys41x4/halkaDM/tree/testing) | 
[branch/dev](https://github.com/sys41x4/halkaDM/tree/dev)

---

```
[TESTING DETAILS]

--[Software Dependencies]--

date [/usr/bin/date]
calender [/usr/bin/cal]
cpu status [/usr/bin/mpstat]
network status [/usr/sbin/tcpdump]
available desktop environment [/usr/share/xsessions]
jpg->ASCII ART [/usr/bin/jp2a]

--[sudo apt install]--
libncurses5-dev libncursesw5-dev
libpam0g-dev
ncal
sysstat
tcpdump

----------------------------------------------------
Currently Only Desktop Environtments are supported located at
1. X-Sessions : /usr/share/xsessions/
2. Wayland-Sessions : /usr/share/wayland-sessions/

----------------------------------------------------
[Testing]

Currently tested only for Debian Based Distribution

-- Install software & library dependencies before further process--

[Create Executable]
gcc ./source/micro/halkaDM.micro.cpp -o ./bin/micro/halkaDM.micro -lcurses -lpam -lxcb

[ Setup ] {use the below setup process untill makefile and documentation are officially created}

install:
cd source/micro/
sudo install -dZ /etc/halkaDM/halkaDM.micro
sudo install -DZ ../../../bin/micro/halkaDM.micro -t /usr/bin
sudo install -DZ res/xsetup.sh -t /etc/halkaDM/halkaDM.micro
sudo install -DZ res/wsetup.sh -t /etc/halkaDM/halkaDM.micro
sudo install -dZ /etc/halkaDM/halkaDM.micro/lang
sudo install -DZ res/lang/* -t /etc/halkaDM/halkaDM.micro/lang
sudo install -DZ res/pam.d/halkaDM.micro -m 644 -t /etc/pam.d

installsystemd:
sudo install -DZ res/halkaDM.service -m 644 -t /usr/lib/systemd/system

uninstall:
sudo rm -rf /etc/halkaDM/halkaDM.micro
sudo rm -f /usr/bin/halkaDM.micro
sudo rm -f /usr/lib/systemd/system/halkaDM.service
sudo rm -f /etc/pam.d/halkaDM.micro
sudo rm -f /etc/init.d/halkaDM.micro

tty -> 2
As the application is running in tty2 and if you got stuck in other tty's
then you can switch to tty2 with CTRL+ALT+F2 and use the application

[ Hardware ]
Currently it is being testing in Ubuntu & Debian 11 bullseye & with jwm/icewm/fluxbox WM
    Distributor ID: Ubuntu
    Description:    Ubuntu 20.04.4 LTS
    Release:        20.04
    Codename:       focal
```

---

<p align="center">
    <img src="https://kounter.tk/badge/sys41x4.github_halkaDM?label=&color=23262a&style=for-the-badge&cntSuffix=%20Halka%20DM%20Repository%20Views" alt="halkaDM GitHub Repo View Count" vertical-align="middle"></img>
</p>
  
---
### Contributors

<a href="https://github.com/sys41x4/halkaDM/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=sys41x4/halkaDM" />
</a>

---
<h3 align="center">
  🙏 Support 🙏
</h3>

<p align="center">
<a href="https://www.paypal.me/sys41x4"><img src="https://img.shields.io/badge/support-PayPal-blue?logo=PayPal&style=flat-square&label=Donate" alt="sponsor Halka DM"/>
</a>
<a href='https://ko-fi.com/sys41x4' target='_blank'><img height='23' width="100" src='https://cdn.ko-fi.com/cdn/kofi3.png?v=2' alt='Buy Coffee for Arijit Bhowmick' />
</a>
<a href="https://www.buymeacoffee.com/sys41x4" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/default-orange.png" alt="Buy Me A Coffee" height="23" width="100" style="border-radius:1px" />
</p>

<p align="center">
<a href="https://sys41x4.github.io" target="blank">
<img src="https://img.shields.io/website?url=https%3A%2F%2Fsys41x4.github.io&logo=github&style=flat-square" />
</a>
<a href="https://twitter.com/intent/tweet?text=Wow:&url=https://github.com/sys41x4/halkaDM">
<img src="https://img.shields.io/twitter/url?style=social&url=https://github.com/sys41x4/halkaDM" alt="tweet Halka DM"/>
</a>
</p>

<hr>
<p align="center">
Developed with ❤️ in India 🇮🇳 
</p>

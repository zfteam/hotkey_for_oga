what is this:

controll volume and backlight everywhere for odroid go advance



build:

```bash
git clone https://github.com/zfteam/hotkey_for_oga.git
cd hotkey_for_oga
make
````


install:

edit the file `/etc/rc.local` in `sudo` mode, and add a line

```bash
/home/odroid/hotkey_for_oga/hotkey_for_oga &
````


configure:
```bash
mkdir ~/.confg/hotkey_for_oga
cp config.ini ~/.config/hotkey_for_oga
````
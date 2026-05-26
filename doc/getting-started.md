---
colorlinks: true
---
# Sonata getting started guide

This guide tells you how to get started with the Sonata board.
If you have any issues in following this guide please contact the lowRISC team at [info@lowrisc.org](mailto:info@lowrisc.org).
If you are attending a *workshop*, please feel free to speak to one of the organisers who will be pleased to help.

The Sonata software build environment can be setup under Windows, macOS and Linux.
We use a tool called [Nix](https://nixos.org/) to manage the build environment on all platforms.
You need to install Nix but don't need to know anything else about it to follow these instructions.

If you have a Sonata board, you can also set it up with the latest release.
If you are attending a *workshop* and are borrowing a board, this step will have already been done for you.
Read the [updating the sonata system guide](https://lowrisc.github.io/sonata-system/doc/guide/updating-system.html) for instructions on how to do this.
You only need to follow the first two steps listed there.
Don't worry if you don't have a board, because you can still follow the rest of this getting started guide.

Only Windows requires specific instructions, Nix handles everything you need on Linux and macOS.
So if you are not using Windows jump straight to [Installing Nix](#installing-nix).

## Windows-specific setup

To obtain a Linux environment on Windows, you can choose to start a virtual machine or use Windows Subsystem for Linux (WSL).
Microsoft provides [a detailed guide on how to install WSL](https://learn.microsoft.com/en-us/windows/wsl/install).
For latest systems this would just be a single command:

```bat
wsl --install
```

---

> You might need to enable virtualisation in the BIOS if it's not enabled by default.

---

If you are running the command without admin privileges, user account control (UAC) popups will appear a few times asking to allow changes to be made to the device.
Click "yes" to approve.

After the command's completion, it should say that Ubuntu is installed.
Reboot your machine for the changes to take effect.
After rebooting, Ubuntu should be available in your start menu.
Click it to start.
For the first time, it prompts you to select a Unix username and password.
Follow the Linux (Ubuntu) steps for the rest of this guide.

---

> If you have installed your WSL a long time ago, `systemd` may not have been enabled by default.
> It is recommended to enable `systemd`.
> Follow the following guide on [using systemd to manage Linux services with WSL](https://learn.microsoft.com/en-us/windows/wsl/systemd).

---

## Installing Nix

The Nix package manager is used to create reproducible builds and consistent development environments.
For Linux systems, we recommend the following command from the official [documentation](https://nixos.org/download/):
```sh
sh <(curl --proto '=https' --tlsv1.2 -L https://nixos.org/nix/install) \
  --daemon
```
For other systems such as macOS, Windows (WSL2), Docker and others, follow the official [documentation](https://nixos.org/download/).

To use Nix from the terminal you need to open up a new terminal for it to be added to your path.

Make sure the experimental features ["flakes"](https://nixos.wiki/wiki/Flakes) and ["nix-command"](https://nixos.wiki/wiki/Nix_command) are enabled
by adding the following to `/etc/nix/nix.conf` or `~/.config/nix/nix.conf`:
```console
experimental-features = nix-command flakes
```

### Setup Nix cache

To make use of the lowRISC Nix cache, so you don't have to rebuild binaries yourself, you need to mark yourself as a trusted user.
To do this, you add your username to the trusted users in `/etc/nix/nix.conf`, e.g. `trusted-users = root username`.
*You can also add all users from a certain group instead of a single user by using an `@` symbol before the group name, e.g. `@sudo` or `@wheel`.*

---

> For Ubuntu users (including WSL users), this means adding this line to the `/etc/nix/nix.conf`:
> ```
> trusted-users = root @sudo
> ```
>
> You need to restart the nix-daemon afterwards for the change to be picked up:
> ```sh
> sudo systemctl restart nix-daemon
> ```

---

> For macOS users, this means adding this line to the `/etc/nix/nix.conf`:
> ```
> trusted-users = root @admin
> ```
>
> You then need to restart your Mac for the changes to take effect.

---

## Enter the CHERIoT development environment

Running the following will put you in a shell environment with all the applications needed to build the CHERIoT RTOS.

```sh
# Enter the shell with
nix develop github:lowRISC/sonata-software
# Exit the shell with
exit
```

These applications are layered on top of your usual environment.
You can see what was added with: `echo $PATH`

If you see the warning that substituter is ignored, cancel the process with Ctrl+C and check to see that [trusted-users is setup properly](#setup-nix-cache).
Nix can and will build everything from source if it can't find a cached version, so letting it continue will cause LLVM-CHERIoT to be built from scratch on your machine.
This is what the warning looks like if you have not configured the cache correctly:

```
do you want to allow configuration setting 'extra-substituters' to be set to
'https://nix-cache.lowrisc.org/public/' (y/N)? y
do you want to permanently mark this value as trusted (y/N)? y
do you want to allow configuration setting 'extra-trusted-public-keys' to be set to
'nix-cache.lowrisc.org-public-1:O6JLD0yXzaJDPiQW1meVu32JIDViuaPtGDfjlOopU7o=' (y/N)? y
do you want to permanently mark this value as trusted (y/N)? y
warning: ignoring untrusted substituter 'https://nix-cache.lowrisc.org/public/',
you are not a trusted user.
```

## Your first build

Clone the Sonata software repository, *making sure to recursively clone submodules as well*, then navigate into it.

```sh
git clone --branch v1.1 \
    --recurse-submodule \
    https://github.com/lowRISC/sonata-software.git
cd sonata-software
```

Enter the nix development development environment if you haven't already.
*Note that because we are in the repository we don't need to provide any arguments to `nix develop`.*

```sh
nix develop
```

Then build the examples with the following command.

```sh
xmake build -P examples
```

After running this you should see the build run to completion and report success, the critical lines indicating a successful build are:

```sh
Converted to uf2, output size: 92672, start address: 0x20000000
Wrote 92672 bytes to ../build/cheriot/cheriot/release/proximity_test.slot3.uf2
Converted to uf2, output size: 126976, start address: 0x20000000
Wrote 126976 bytes to ../build/cheriot/cheriot/release/snake_demo.slot3.uf2
Converted to uf2, output size: 161792, start address: 0x20000000
Wrote 161792 bytes to ../build/cheriot/cheriot/release/sonata_simple_demo.slot3.uf2
[100%]: build ok, spent 0.986s
```

*Note output size and times may differ.*

If you have got a successful build, congratulations!
Your environment is ready to go for Sonata software development.
Get in touch with lowRISC on [info@lowrisc.org](mailto:info@lowrisc.org) if you have any issues.

For reference the output (from a build run on a Linux machine) looks like:

```console
$ xmake build -P examples
checking for platform ... cheriot
checking for architecture ... cheriot
...
Converted to uf2, output size: 126976, start address: 0x20000000
Wrote 126976 bytes to ../build/cheriot/cheriot/release/snake_demo.slot3.uf2
[100%]: build ok, spent 13.04s
warning: ./cheriot-rtos/sdk/xmake.lua:116: unknown language value 'c2x', it may be 'c23'
warning: add -v for getting more warnings ..
```

If you are following this guide as preparation for a *workshop*, you are now all set up and don't need to go any further.
If you are here on your own, you can now try [running software](https://lowrisc.github.io/sonata-software/doc/guide/running-software.html).

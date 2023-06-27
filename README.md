<!-- Project title -->
<div align="center">
    <a href="https://git.io/typing-svg"><img src="https://readme-typing-svg.herokuapp.com?font=Fira+Code&pause=1000&center=true&width=435&lines=Netshow+command" alt="Project Name Typing" /></a>
</div>

# About
Netshow command is a binary written in C language that returns a list of interfaces on your system and their IP addresses (IPv4 and IPv6).

# Getting started

- Clone this repository locally
```bash
$ git clone https://github.com/alexispages/netshow.git
```
- Permit execution of the **netshow** binary file:
```bash
$ chmod +x netshow
```
- Execute the netshow binary with proper options

# Compile binary file locally

You can build netshow binary by using gcc:

```bash
$ gcc netshow.c -o netshow
```

# Options

Netshow offer tow functionnalities:
- Display network interfaces on the machine with the option **-a**
```bash
$ ./netshow -a
System interfaces :

1: lo
2: eno1
3: virbr0
4: docker0
```
- Display of IPv4 and IPv6 addresses associated with an interface by using **-i *interface_name***
```bash
$ ./netshow -i lo
lo :

inet 127.0.0.1/8
inet6 ::1/128
```

# Contributor

<a href="https://www.linkedin.com/in/alexis-pag%C3%A8s"><img alt="Alexis PAGES LinkedIn" src="https://img.shields.io/badge/LinkedIn-Alexis%20PAGES-blue?logo=LinkedIn&style=for-the-badge"></a>

# License

- [GPL v3](./LICENSE)

<p align="right"><a href="#about">back to top ⬆️</a></p>

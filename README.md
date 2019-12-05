# NectarSun Power Board

NectarSun Power board source code project. Builds on Linux systems

## Getting Started

You should be able to run the project after cloning the repository, updating submodules, and deploying the mbed-os library.

### Prerequisites

* mbed-cli        (version)
* Python 2.7      (2.7.15)
* gcc             (7.3.0)
* g++             (7.3.0)
* Ubuntu          (18.04)
* GNU make        (4.1)
* https://github.com/xpack-dev-tools/arm-none-eabi-gcc-xpack/releases/v8.3.1-1.3/
* pip             (9.0.1)
* mercurial
* git

```
`sudo apt-get install python2.7 python-pip git mercurial`
```

### Installing

1. make sure you have Python 2.7, gcc (7.3.0), make (4.1), pip, mercurial
2. install mbed-cli `pip install mbed-cli`
3. download the ARM GCC compiler (xpack-dev-tools v8.3.1-1.3), install it at `/opt/path`
4. run `mbed-cli`
5. install missing python packages required by the project
```
cd your_dir/power_board/mbed-os/
pip install -r requirements.txt
```
6. download mbed-os:
```
cd your_dir/power_board/
mbed-deploy
```
7. update submodules
8. try to compile: 
```
cd your_dir/power_board/NS-project/
make build
```

## Running the tests

Explain how to run the automated tests for this system

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### Coding style tests

```
cd your_dir/power_board/NS-project/
make format
```

## Deployment

use ST-link flasher.

## Built With

* [mbed-os](https://os.mbed.com/docs/mbed-os/v5.14/introduction/index.html) - embedded C framework

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## Authors

* **Billie Thompson** - *Initial work* - [PurpleBooth](https://github.com/PurpleBooth)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Hat tip to anyone whose code was used
* Inspiration
* etc
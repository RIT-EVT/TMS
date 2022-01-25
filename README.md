# DEV1-TMS

## Introduction

### DEV1-TMS

The DEV1-TMS is the Thermal Management System for the RIT-EVT Dirt
Electric Vehicle (DEV1). The firmware for the DEV1-TMS is written for
the STM32f302r8 microcontroller that is designed into the DEV1-TMS.

The DEV1-TMS will have the following responsibilities.

1. Track thermals across DEV1
2. Operate thermal control systems, including 2 fans and the pump
for a water-cooling loop
3. Expose the system over the DEV1 CAN network

For a more detailed description of the requirements, refer to the
[DEV1-TMS Software Requirements Specification](https://dev1-tms.readthedocs.io/en/latest/srs.html).

For more information on the BMS over all and the software level API,
refer to the
[Read the Docs DEV1-TMS page](https://dev1-tms.readthedocs.io/en/latest/index.html).

### Documentation

Documentation is handled via Sphinx. To build the documentation, navigate
to the `docs/` folder and run `make html`. You can then open the generated
`docs/build/html/index.html`.

To generate a PDF of the Software Requirements Specification, run the command
`rinoh docs/source/srs.rst --output docs`. You can then view the PDF version
of the SRS in `docs/srs.pdf`. The SRS is identical to the one generated via
`make html`.

### Related Projects

The DEV1-BMS is one component of the larger DEV1 project, you can find related
projects in the [RIT-EVT](https://github.com/RIT-EVT) Github page.
Additionally, the STM32f302r8 driver code is handled via the
[EVT-core](https://github.com/RIT-EVT/EVT-core) project.

# Cloudflare DNS Updater

This project is a simple C-based tool to update a DNS record in Cloudflare with the public IP of the running machine.

## Motivation

This task can be done with a few lines of bash script, but I wanted to practice some points:

- Refresh my C/C++ knowledge. The project was coded in C11, enabling warnings as errors and _pedantic_ mode.
- Use CMake to create the project structure because I didn't want to fight against Makefiles and PHONY rules.
- Write some tests for C code. I found `greatest.h`, a single header file with definitions to create test suites in C.

## Prerequisites

- CMake 3.12 or higher
- libcurl development libraries

## Project Structure

The project is organized into four main modules:

- **lib**: Implements most of the core functionality.
- **test**: Contains tests for the code implemented in the `lib` module.
- **cli**: Implements the command-line tool.
- **scripts**: Utilities to prepare the project, build, and test.

This structure helps in maintaining a clear separation of concerns and makes the project easier to manage and extend.

## Building the Project

1. Clone the repository:
    ```sh
    git clone <repository-url>
    cd <repository-directory>
    ```

2. Prepare the project:
    ```sh
    mkdir build
    cd build
    ```

3. Run CMake to configure the project:
    ```sh
    cmake ..
    ```

4. Build the project:
    ```sh
    make
    ```

## Running Tests

To run the tests, just run the script to build and run the `test` project:
```sh
scripts/build_and_test.sh
```

> I use to run this script with some FS watcher like [watchexec](https://github.com/watchexec/watchexec).

## CLI Usage

Commands
- **check**: Get public IP from [ifconfig.me](https://ifconfig.me).
- **update**: Update Cloudflare DNS record with the current public IP.
    - `--zone` / `-z`: Zone ID (your domain).
    - `--record` / `-r`: Record ID (record of your domain to update).
    - `--token` / `-t`: API Token with DNS edit permission.
        - Note: Token can also be defined in the `CLOUDFLARE_API_TOKEN` environment variable.

## Special Thanks

Special thanks to the following projects for their contributions:

- [greatest](https://github.com/silentbicycle/greatest): A C testing library.
- [curl](https://curl.se/): A command-line tool and library for transferring data with URLs.

## Contributing

Contributions are welcome! Please open an issue or submit a pull request.

## License

This project is licensed under the MIT License.

# Video Transcoding with OpenMP

This project demonstrates how to transcode videos into different resolutions using the FFmpeg library, leveraging parallel processing with OpenMP. The program accepts multiple resolution targets and transcodes the input video into those resolutions concurrently.

## Table of Contents

- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [How It Works](#how-it-works)
- [Example](#example)
- [Contributing](#contributing)
- [License](#license)

## Features

- **Parallel Processing:** Utilizes OpenMP to transcode videos into multiple resolutions concurrently.
- **Customizable Resolutions:** Accepts a list of target resolutions for transcoding.
- **Logging:** Logs the start and end times of processing each resolution, along with any errors.

## Prerequisites

- **GCC Compiler:** Required for compiling the program with OpenMP support.
- **FFmpeg:** Must be installed on your system for video transcoding.
- **Make:** For compiling and running the program using Makefile.

### Installing FFmpeg

On Ubuntu:

```sh
sudo apt update
sudo apt install ffmpeg
```

On MacOS:

```sh
brew install ffmpeg
```

## Installation

1. Clone this repository:

   ```sh
   git clone https://github.com/yourusername/video-transcoding-omp.git
   cd video-transcoding-omp
   ```

2. Compile the program:

   ```sh
   make
   ```

## Usage

After compiling, you can run the program using the `make run` command:

```sh
make run
```

Alternatively, you can run the executable directly with custom arguments:

```sh
./my_program 1920x1080 1280x720 input_video.mp4
```

### Command-Line Arguments

- **Resolution Arguments:** Specify one or more resolutions in the format `WIDTHxHEIGHT`.
- **Input File:** The last argument must be the path to the input video file.

### Example

```sh
./my_program 1920x1080 1280x720 640x480 input_video.mp4
```

This will transcode `input_video.mp4` into `1920x1080`, `1280x720`, and `640x480` resolutions, producing files named `output_1.mp4`, `output_2.mp4`, and `output_3.mp4`.

## How It Works

- **OpenMP Parallelism:** The program uses OpenMP to divide the list of resolutions among multiple threads. Each thread processes a different resolution, using FFmpeg to transcode the video.
  
- **FFmpeg Command:** The program generates FFmpeg commands dynamically based on the provided resolutions and executes them using the `system()` function.

- **Logging:** Each thread logs its processing details, including the start and end time for each resolution, to a file named `build_log.txt`.

### Code Structure

- **`main.c`**: The entry point of the program, responsible for handling input arguments, setting up OpenMP threads, and calling the `transcode_video` function.
  
- **`transcoder.c`**: Contains the implementation of the `transcode_video` function, which constructs and executes FFmpeg commands.

- **`transcoder.h`**: Declares the `transcode_video` function.

- **`Makefile`**: Defines rules for compiling the program and running tests.

## Contributing

Contributions are welcome! Please fork this repository, create a feature branch, and submit a pull request.


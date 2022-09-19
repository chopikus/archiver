# archiver 📁
`archiver` is a compressing/decompressing tool made for educational purposes.

Being written in C++ and using [Huffman coding techinque](https://en.wikipedia.org/wiki/Huffman_coding) for data compression, this tool uses extremely small amount of RAM, while having pretty decent compressing/decompressing speeds.

`archiver` can be used as an every-day tool, however it requires some attention from the end-user. (just like the `dd` command 😉).
## Installation
0. Requirements: `make`, `CMake`, `GTest`.
1. Clone this repository -- `git clone https://github.com/chopikus/archiver.git`.
2. Open the cloned folder -- `cd archiver`.
3. Create new folder for building the tool, and open it -- `mkdir build && cd build`.
4. Run the following command: `cmake .. && make`. 
After that the executable file named `archiver` should appear.

## Usage
* `archiver -c ARCHIVE file1 [file2...]` -- compress files `file1`, `file2`, ... into an archive named `ARCHIVE`;
* `archiver -d ARCHIVE` -- decompress archive named `ARCHIVE` into the current folder;
* `archiver -h` -- show help message.

## **Sections below are not fully written yet**.
## Benchmarks
#### Video compression
[This](https://www.youtube.com/watch?v=rTgj1HxmUbg) rickroll video was used for benchmarking (downloaded in 360p).
* Initial file size -- 63.1 MB
* Archive size -- 62.8 MB (space saving: 0.4%)
* Compressing time -- 11.6s (speed: ~5.5MB/s)
* Decompressing time -- 28.9s (speed: ~2.2MB/s)
#### Text compression
Testing compression of book "War and Peace" written by Leo Tolstoy.
* File format -- `txt`
* Initial file size -- 5.3 MB
* Compressed size -- 2.6 MB (space saving: 52.9%)
* Compressing time -- 0.49s (speed: ?)
* Decompressing time -- ? (speed: ?)
## Details
### Running tests
0. Requirements: `make`, `CMake`, `GTest`.
1. Clone this repository -- `git clone https://github.com/chopikus/archiver.git`.
2. Open the cloned folder -- `cd archiver`.
3. Create new folder for running tests, and open it -- `mkdir build && cd build`.
4. Run the following command: `cmake .. && make tests`. 

### Optimizations
TODO

### File format
TODO

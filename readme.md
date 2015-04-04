# ITQ tool

Squish the file size of Impulse Tracker (IT) mods by converting the samples to
Ogg Vorbis format.

The majority of the code is a fork of libmodplug 0.8.8.5 — only the encoding
component and frontend tool were added.

## What 4

I needed to ship mods with very large samples, and uncompressed WAVs take up a
fair bucket.

## How 4

```sh
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## File format

The file format of an ITQ file is almost exactly the same as that of an IT
file. The sample header section has two added fields:

```c
DWORD nbytes;     // Number of bytes in compressed sample
CHAR samplecodec; // OGG Vorbis, FLAC, etc.
```

The sample data sections, which usually contain raw audio, contain their
respective encoded streams. The `nbytes` field was added because their offsets
in the file are impossible to know otherwise.

The `samplecodec` field indicates the codec used. Only Vorbis is supported at
this time, but this field affords future additions.

## Shopping list

- [x] Encode .it into .itq
- [ ] Decode .itq into .it
- [ ] Vorbis quality option
- [ ] Flexible output filename options

## Subjectively relevant links

- [Modipulate](https://github.com/MrEricSir/Modipulate) — Library using ITQ
- [libmodplug homepage](http://modplug-xmms.sourceforge.net/)
- [IT format specification](http://schismtracker.org/wiki/ITTECH.TXT)

## License

All code in the `modplug` directory is public domain (see `modplug/COPYING`).

The ITQ frontend tool and modifications to libmodplug are public domain.

libvorbis and libogg use the BSD license.

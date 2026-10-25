# Playlist duration-probe regression test

Configure the existing desktop CMake build with `-DBUILD_VIDEOPLAYER_TESTS=ON`,
then build only the native wrapper and test:

```sh
cmake --build <build-directory> --target qtascdocumentscore videoplayer_probe_test
```

Run with the same Qt libraries/plugins used by the desktop application:

```sh
APP=/path/to/desktopeditors
LD_LIBRARY_PATH="$APP:$APP/converter" QT_PLUGIN_PATH="$APP" \
  QT_QPA_PLATFORM=offscreen QT_MEDIA_BACKEND=ffmpeg \
  /path/to/videoplayer_probe_test /absolute/path/to/video.mp4
```

The test exercises the actual `QVideoPlaylist` implementation and checks that an
invalid source completes once, ten successive valid probes report nonzero
durations, and resetting the source does not crash or stall later probes. It uses
temporary links and does not modify the input video.

The crash occurs when a duration callback synchronously clears the media source:
Qt's FFmpeg backend destroys its playback engine during the callback and then
continues initialization with that engine. Cleanup must instead run after the
backend callback returns. The pending flag prevents duplicate completion and
keeps the next probe from starting before cleanup finishes.

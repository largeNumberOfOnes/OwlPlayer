# THIS FILE IS AUTOGEN!!!

objects/app.o: src/app.cpp src/app.h src/had/had.h src/had/had_keys.h \
  src/had/had_types.h src/had/had_audio.h src/had/audioFile.h \
  src/had/had_logger.h /usr/include/pipewire-0.3/pipewire/pipewire.h \
  /usr/include/spa-0.2/spa/support/plugin.h \
  /usr/include/spa-0.2/spa/utils/defs.h \
  /usr/include/spa-0.2/spa/utils/dict.h \
  /usr/include/pipewire-0.3/pipewire/array.h \
  /usr/include/pipewire-0.3/pipewire/client.h \
  /usr/include/spa-0.2/spa/param/param.h \
  /usr/include/spa-0.2/spa/param/buffers.h \
  /usr/include/spa-0.2/spa/param/profile.h \
  /usr/include/spa-0.2/spa/param/port-config.h \
  /usr/include/spa-0.2/spa/param/route.h \
  /usr/include/pipewire-0.3/pipewire/proxy.h \
  /usr/include/spa-0.2/spa/utils/hook.h \
  /usr/include/spa-0.2/spa/utils/list.h \
  /usr/include/pipewire-0.3/pipewire/protocol.h \
  /usr/include/pipewire-0.3/pipewire/context.h \
  /usr/include/pipewire-0.3/pipewire/core.h \
  /usr/include/pipewire-0.3/pipewire/properties.h \
  /usr/include/spa-0.2/spa/utils/string.h \
  /usr/include/pipewire-0.3/pipewire/loop.h \
  /usr/include/spa-0.2/spa/support/loop.h \
  /usr/include/spa-0.2/spa/support/system.h \
  /usr/include/pipewire-0.3/pipewire/utils.h \
  /usr/include/spa-0.2/spa/pod/pod.h \
  /usr/include/spa-0.2/spa/utils/type.h \
  /usr/include/pipewire-0.3/pipewire/permission.h \
  /usr/include/pipewire-0.3/pipewire/conf.h \
  /usr/include/pipewire-0.3/pipewire/device.h \
  /usr/include/pipewire-0.3/pipewire/buffers.h \
  /usr/include/spa-0.2/spa/node/node.h \
  /usr/include/spa-0.2/spa/buffer/buffer.h \
  /usr/include/spa-0.2/spa/buffer/meta.h \
  /usr/include/spa-0.2/spa/node/event.h \
  /usr/include/spa-0.2/spa/pod/event.h \
  /usr/include/spa-0.2/spa/node/command.h \
  /usr/include/spa-0.2/spa/pod/command.h \
  /usr/include/pipewire-0.3/pipewire/mem.h \
  /usr/include/pipewire-0.3/pipewire/factory.h \
  /usr/include/pipewire-0.3/pipewire/keys.h \
  /usr/include/pipewire-0.3/pipewire/log.h \
  /usr/include/spa-0.2/spa/support/log.h \
  /usr/include/pipewire-0.3/pipewire/link.h \
  /usr/include/pipewire-0.3/pipewire/main-loop.h \
  /usr/include/pipewire-0.3/pipewire/map.h \
  /usr/include/pipewire-0.3/pipewire/module.h \
  /usr/include/pipewire-0.3/pipewire/node.h \
  /usr/include/pipewire-0.3/pipewire/port.h \
  /usr/include/pipewire-0.3/pipewire/stream.h \
  /usr/include/pipewire-0.3/pipewire/filter.h \
  /usr/include/spa-0.2/spa/node/io.h \
  /usr/include/pipewire-0.3/pipewire/thread-loop.h \
  /usr/include/pipewire-0.3/pipewire/data-loop.h \
  /usr/include/spa-0.2/spa/support/thread.h \
  /usr/include/pipewire-0.3/pipewire/type.h \
  /usr/include/pipewire-0.3/pipewire/version.h src/had/had_interface.h \
  src/fileManager.h src/setup.h src/eventQueue.h src/player.h \
  src/spectre.h src/switchPanel.h
	clang++-14 -g -std=c++20 -c src/app.cpp -I/usr/include/pipewire-0.3 -I/usr/include/spa-0.2 -D_REENTRANT -o objects/app.o

objects/config.o: src/config.cpp src/config.h
	clang++-14 -g -std=c++20 -c src/config.cpp -I/usr/include/pipewire-0.3 -I/usr/include/spa-0.2 -D_REENTRANT -o objects/config.o

objects/eventQueue.o: src/eventQueue.cpp src/eventQueue.h src/had/had.h \
  src/had/had_keys.h src/had/had_types.h src/had/had_audio.h \
  src/had/audioFile.h src/had/had_logger.h \
  /usr/include/pipewire-0.3/pipewire/pipewire.h \
  /usr/include/spa-0.2/spa/support/plugin.h \
  /usr/include/spa-0.2/spa/utils/defs.h \
  /usr/include/spa-0.2/spa/utils/dict.h \
  /usr/include/pipewire-0.3/pipewire/array.h \
  /usr/include/pipewire-0.3/pipewire/client.h \
  /usr/include/spa-0.2/spa/param/param.h \
  /usr/include/spa-0.2/spa/param/buffers.h \
  /usr/include/spa-0.2/spa/param/profile.h \
  /usr/include/spa-0.2/spa/param/port-config.h \
  /usr/include/spa-0.2/spa/param/route.h \
  /usr/include/pipewire-0.3/pipewire/proxy.h \
  /usr/include/spa-0.2/spa/utils/hook.h \
  /usr/include/spa-0.2/spa/utils/list.h \
  /usr/include/pipewire-0.3/pipewire/protocol.h \
  /usr/include/pipewire-0.3/pipewire/context.h \
  /usr/include/pipewire-0.3/pipewire/core.h \
  /usr/include/pipewire-0.3/pipewire/properties.h \
  /usr/include/spa-0.2/spa/utils/string.h \
  /usr/include/pipewire-0.3/pipewire/loop.h \
  /usr/include/spa-0.2/spa/support/loop.h \
  /usr/include/spa-0.2/spa/support/system.h \
  /usr/include/pipewire-0.3/pipewire/utils.h \
  /usr/include/spa-0.2/spa/pod/pod.h \
  /usr/include/spa-0.2/spa/utils/type.h \
  /usr/include/pipewire-0.3/pipewire/permission.h \
  /usr/include/pipewire-0.3/pipewire/conf.h \
  /usr/include/pipewire-0.3/pipewire/device.h \
  /usr/include/pipewire-0.3/pipewire/buffers.h \
  /usr/include/spa-0.2/spa/node/node.h \
  /usr/include/spa-0.2/spa/buffer/buffer.h \
  /usr/include/spa-0.2/spa/buffer/meta.h \
  /usr/include/spa-0.2/spa/node/event.h \
  /usr/include/spa-0.2/spa/pod/event.h \
  /usr/include/spa-0.2/spa/node/command.h \
  /usr/include/spa-0.2/spa/pod/command.h \
  /usr/include/pipewire-0.3/pipewire/mem.h \
  /usr/include/pipewire-0.3/pipewire/factory.h \
  /usr/include/pipewire-0.3/pipewire/keys.h \
  /usr/include/pipewire-0.3/pipewire/log.h \
  /usr/include/spa-0.2/spa/support/log.h \
  /usr/include/pipewire-0.3/pipewire/link.h \
  /usr/include/pipewire-0.3/pipewire/main-loop.h \
  /usr/include/pipewire-0.3/pipewire/map.h \
  /usr/include/pipewire-0.3/pipewire/module.h \
  /usr/include/pipewire-0.3/pipewire/node.h \
  /usr/include/pipewire-0.3/pipewire/port.h \
  /usr/include/pipewire-0.3/pipewire/stream.h \
  /usr/include/pipewire-0.3/pipewire/filter.h \
  /usr/include/spa-0.2/spa/node/io.h \
  /usr/include/pipewire-0.3/pipewire/thread-loop.h \
  /usr/include/pipewire-0.3/pipewire/data-loop.h \
  /usr/include/spa-0.2/spa/support/thread.h \
  /usr/include/pipewire-0.3/pipewire/type.h \
  /usr/include/pipewire-0.3/pipewire/version.h src/had/had_interface.h
	clang++-14 -g -std=c++20 -c src/eventQueue.cpp -I/usr/include/pipewire-0.3 -I/usr/include/spa-0.2 -D_REENTRANT -o objects/eventQueue.o

objects/fileManager.o: src/fileManager.cpp src/fileManager.h src/had/had.h \
  src/had/had_keys.h src/had/had_types.h src/had/had_audio.h \
  src/had/audioFile.h src/had/had_logger.h \
  /usr/include/pipewire-0.3/pipewire/pipewire.h \
  /usr/include/spa-0.2/spa/support/plugin.h \
  /usr/include/spa-0.2/spa/utils/defs.h \
  /usr/include/spa-0.2/spa/utils/dict.h \
  /usr/include/pipewire-0.3/pipewire/array.h \
  /usr/include/pipewire-0.3/pipewire/client.h \
  /usr/include/spa-0.2/spa/param/param.h \
  /usr/include/spa-0.2/spa/param/buffers.h \
  /usr/include/spa-0.2/spa/param/profile.h \
  /usr/include/spa-0.2/spa/param/port-config.h \
  /usr/include/spa-0.2/spa/param/route.h \
  /usr/include/pipewire-0.3/pipewire/proxy.h \
  /usr/include/spa-0.2/spa/utils/hook.h \
  /usr/include/spa-0.2/spa/utils/list.h \
  /usr/include/pipewire-0.3/pipewire/protocol.h \
  /usr/include/pipewire-0.3/pipewire/context.h \
  /usr/include/pipewire-0.3/pipewire/core.h \
  /usr/include/pipewire-0.3/pipewire/properties.h \
  /usr/include/spa-0.2/spa/utils/string.h \
  /usr/include/pipewire-0.3/pipewire/loop.h \
  /usr/include/spa-0.2/spa/support/loop.h \
  /usr/include/spa-0.2/spa/support/system.h \
  /usr/include/pipewire-0.3/pipewire/utils.h \
  /usr/include/spa-0.2/spa/pod/pod.h \
  /usr/include/spa-0.2/spa/utils/type.h \
  /usr/include/pipewire-0.3/pipewire/permission.h \
  /usr/include/pipewire-0.3/pipewire/conf.h \
  /usr/include/pipewire-0.3/pipewire/device.h \
  /usr/include/pipewire-0.3/pipewire/buffers.h \
  /usr/include/spa-0.2/spa/node/node.h \
  /usr/include/spa-0.2/spa/buffer/buffer.h \
  /usr/include/spa-0.2/spa/buffer/meta.h \
  /usr/include/spa-0.2/spa/node/event.h \
  /usr/include/spa-0.2/spa/pod/event.h \
  /usr/include/spa-0.2/spa/node/command.h \
  /usr/include/spa-0.2/spa/pod/command.h \
  /usr/include/pipewire-0.3/pipewire/mem.h \
  /usr/include/pipewire-0.3/pipewire/factory.h \
  /usr/include/pipewire-0.3/pipewire/keys.h \
  /usr/include/pipewire-0.3/pipewire/log.h \
  /usr/include/spa-0.2/spa/support/log.h \
  /usr/include/pipewire-0.3/pipewire/link.h \
  /usr/include/pipewire-0.3/pipewire/main-loop.h \
  /usr/include/pipewire-0.3/pipewire/map.h \
  /usr/include/pipewire-0.3/pipewire/module.h \
  /usr/include/pipewire-0.3/pipewire/node.h \
  /usr/include/pipewire-0.3/pipewire/port.h \
  /usr/include/pipewire-0.3/pipewire/stream.h \
  /usr/include/pipewire-0.3/pipewire/filter.h \
  /usr/include/spa-0.2/spa/node/io.h \
  /usr/include/pipewire-0.3/pipewire/thread-loop.h \
  /usr/include/pipewire-0.3/pipewire/data-loop.h \
  /usr/include/spa-0.2/spa/support/thread.h \
  /usr/include/pipewire-0.3/pipewire/type.h \
  /usr/include/pipewire-0.3/pipewire/version.h src/had/had_interface.h \
  src/setup.h
	clang++-14 -g -std=c++20 -c src/fileManager.cpp -I/usr/include/pipewire-0.3 -I/usr/include/spa-0.2 -D_REENTRANT -o objects/fileManager.o

objects/Fourier_transform.o: src/Fourier_transform.cpp src/Fourier_transform.h
	clang++-14 -g -std=c++20 -c src/Fourier_transform.cpp -I/usr/include/pipewire-0.3 -I/usr/include/spa-0.2 -D_REENTRANT -o objects/Fourier_transform.o

objects/main.o: src/main.cpp src/had/had.h src/had/had_keys.h src/had/had_types.h \
  src/had/had_audio.h src/had/audioFile.h src/had/had_logger.h \
  /usr/include/pipewire-0.3/pipewire/pipewire.h \
  /usr/include/spa-0.2/spa/support/plugin.h \
  /usr/include/spa-0.2/spa/utils/defs.h \
  /usr/include/spa-0.2/spa/utils/dict.h \
  /usr/include/pipewire-0.3/pipewire/array.h \
  /usr/include/pipewire-0.3/pipewire/client.h \
  /usr/include/spa-0.2/spa/param/param.h \
  /usr/include/spa-0.2/spa/param/buffers.h \
  /usr/include/spa-0.2/spa/param/profile.h \
  /usr/include/spa-0.2/spa/param/port-config.h \
  /usr/include/spa-0.2/spa/param/route.h \
  /usr/include/pipewire-0.3/pipewire/proxy.h \
  /usr/include/spa-0.2/spa/utils/hook.h \
  /usr/include/spa-0.2/spa/utils/list.h \
  /usr/include/pipewire-0.3/pipewire/protocol.h \
  /usr/include/pipewire-0.3/pipewire/context.h \
  /usr/include/pipewire-0.3/pipewire/core.h \
  /usr/include/pipewire-0.3/pipewire/properties.h \
  /usr/include/spa-0.2/spa/utils/string.h \
  /usr/include/pipewire-0.3/pipewire/loop.h \
  /usr/include/spa-0.2/spa/support/loop.h \
  /usr/include/spa-0.2/spa/support/system.h \
  /usr/include/pipewire-0.3/pipewire/utils.h \
  /usr/include/spa-0.2/spa/pod/pod.h \
  /usr/include/spa-0.2/spa/utils/type.h \
  /usr/include/pipewire-0.3/pipewire/permission.h \
  /usr/include/pipewire-0.3/pipewire/conf.h \
  /usr/include/pipewire-0.3/pipewire/device.h \
  /usr/include/pipewire-0.3/pipewire/buffers.h \
  /usr/include/spa-0.2/spa/node/node.h \
  /usr/include/spa-0.2/spa/buffer/buffer.h \
  /usr/include/spa-0.2/spa/buffer/meta.h \
  /usr/include/spa-0.2/spa/node/event.h \
  /usr/include/spa-0.2/spa/pod/event.h \
  /usr/include/spa-0.2/spa/node/command.h \
  /usr/include/spa-0.2/spa/pod/command.h \
  /usr/include/pipewire-0.3/pipewire/mem.h \
  /usr/include/pipewire-0.3/pipewire/factory.h \
  /usr/include/pipewire-0.3/pipewire/keys.h \
  /usr/include/pipewire-0.3/pipewire/log.h \
  /usr/include/spa-0.2/spa/support/log.h \
  /usr/include/pipewire-0.3/pipewire/link.h \
  /usr/include/pipewire-0.3/pipewire/main-loop.h \
  /usr/include/pipewire-0.3/pipewire/map.h \
  /usr/include/pipewire-0.3/pipewire/module.h \
  /usr/include/pipewire-0.3/pipewire/node.h \
  /usr/include/pipewire-0.3/pipewire/port.h \
  /usr/include/pipewire-0.3/pipewire/stream.h \
  /usr/include/pipewire-0.3/pipewire/filter.h \
  /usr/include/spa-0.2/spa/node/io.h \
  /usr/include/pipewire-0.3/pipewire/thread-loop.h \
  /usr/include/pipewire-0.3/pipewire/data-loop.h \
  /usr/include/spa-0.2/spa/support/thread.h \
  /usr/include/pipewire-0.3/pipewire/type.h \
  /usr/include/pipewire-0.3/pipewire/version.h src/had/had_interface.h \
  src/setup.h src/app.h src/fileManager.h src/eventQueue.h src/player.h \
  src/spectre.h src/switchPanel.h
	clang++-14 -g -std=c++20 -c src/main.cpp -I/usr/include/pipewire-0.3 -I/usr/include/spa-0.2 -D_REENTRANT -o objects/main.o

objects/player.o: src/player.cpp src/player.h src/had/had.h src/had/had_keys.h \
  src/had/had_types.h src/had/had_audio.h src/had/audioFile.h \
  src/had/had_logger.h /usr/include/pipewire-0.3/pipewire/pipewire.h \
  /usr/include/spa-0.2/spa/support/plugin.h \
  /usr/include/spa-0.2/spa/utils/defs.h \
  /usr/include/spa-0.2/spa/utils/dict.h \
  /usr/include/pipewire-0.3/pipewire/array.h \
  /usr/include/pipewire-0.3/pipewire/client.h \
  /usr/include/spa-0.2/spa/param/param.h \
  /usr/include/spa-0.2/spa/param/buffers.h \
  /usr/include/spa-0.2/spa/param/profile.h \
  /usr/include/spa-0.2/spa/param/port-config.h \
  /usr/include/spa-0.2/spa/param/route.h \
  /usr/include/pipewire-0.3/pipewire/proxy.h \
  /usr/include/spa-0.2/spa/utils/hook.h \
  /usr/include/spa-0.2/spa/utils/list.h \
  /usr/include/pipewire-0.3/pipewire/protocol.h \
  /usr/include/pipewire-0.3/pipewire/context.h \
  /usr/include/pipewire-0.3/pipewire/core.h \
  /usr/include/pipewire-0.3/pipewire/properties.h \
  /usr/include/spa-0.2/spa/utils/string.h \
  /usr/include/pipewire-0.3/pipewire/loop.h \
  /usr/include/spa-0.2/spa/support/loop.h \
  /usr/include/spa-0.2/spa/support/system.h \
  /usr/include/pipewire-0.3/pipewire/utils.h \
  /usr/include/spa-0.2/spa/pod/pod.h \
  /usr/include/spa-0.2/spa/utils/type.h \
  /usr/include/pipewire-0.3/pipewire/permission.h \
  /usr/include/pipewire-0.3/pipewire/conf.h \
  /usr/include/pipewire-0.3/pipewire/device.h \
  /usr/include/pipewire-0.3/pipewire/buffers.h \
  /usr/include/spa-0.2/spa/node/node.h \
  /usr/include/spa-0.2/spa/buffer/buffer.h \
  /usr/include/spa-0.2/spa/buffer/meta.h \
  /usr/include/spa-0.2/spa/node/event.h \
  /usr/include/spa-0.2/spa/pod/event.h \
  /usr/include/spa-0.2/spa/node/command.h \
  /usr/include/spa-0.2/spa/pod/command.h \
  /usr/include/pipewire-0.3/pipewire/mem.h \
  /usr/include/pipewire-0.3/pipewire/factory.h \
  /usr/include/pipewire-0.3/pipewire/keys.h \
  /usr/include/pipewire-0.3/pipewire/log.h \
  /usr/include/spa-0.2/spa/support/log.h \
  /usr/include/pipewire-0.3/pipewire/link.h \
  /usr/include/pipewire-0.3/pipewire/main-loop.h \
  /usr/include/pipewire-0.3/pipewire/map.h \
  /usr/include/pipewire-0.3/pipewire/module.h \
  /usr/include/pipewire-0.3/pipewire/node.h \
  /usr/include/pipewire-0.3/pipewire/port.h \
  /usr/include/pipewire-0.3/pipewire/stream.h \
  /usr/include/pipewire-0.3/pipewire/filter.h \
  /usr/include/spa-0.2/spa/node/io.h \
  /usr/include/pipewire-0.3/pipewire/thread-loop.h \
  /usr/include/pipewire-0.3/pipewire/data-loop.h \
  /usr/include/spa-0.2/spa/support/thread.h \
  /usr/include/pipewire-0.3/pipewire/type.h \
  /usr/include/pipewire-0.3/pipewire/version.h src/had/had_interface.h
	clang++-14 -g -std=c++20 -c src/player.cpp -I/usr/include/pipewire-0.3 -I/usr/include/spa-0.2 -D_REENTRANT -o objects/player.o

objects/setup.o: src/setup.cpp src/setup.h src/had/had.h src/had/had_keys.h \
  src/had/had_types.h src/had/had_audio.h src/had/audioFile.h \
  src/had/had_logger.h /usr/include/pipewire-0.3/pipewire/pipewire.h \
  /usr/include/spa-0.2/spa/support/plugin.h \
  /usr/include/spa-0.2/spa/utils/defs.h \
  /usr/include/spa-0.2/spa/utils/dict.h \
  /usr/include/pipewire-0.3/pipewire/array.h \
  /usr/include/pipewire-0.3/pipewire/client.h \
  /usr/include/spa-0.2/spa/param/param.h \
  /usr/include/spa-0.2/spa/param/buffers.h \
  /usr/include/spa-0.2/spa/param/profile.h \
  /usr/include/spa-0.2/spa/param/port-config.h \
  /usr/include/spa-0.2/spa/param/route.h \
  /usr/include/pipewire-0.3/pipewire/proxy.h \
  /usr/include/spa-0.2/spa/utils/hook.h \
  /usr/include/spa-0.2/spa/utils/list.h \
  /usr/include/pipewire-0.3/pipewire/protocol.h \
  /usr/include/pipewire-0.3/pipewire/context.h \
  /usr/include/pipewire-0.3/pipewire/core.h \
  /usr/include/pipewire-0.3/pipewire/properties.h \
  /usr/include/spa-0.2/spa/utils/string.h \
  /usr/include/pipewire-0.3/pipewire/loop.h \
  /usr/include/spa-0.2/spa/support/loop.h \
  /usr/include/spa-0.2/spa/support/system.h \
  /usr/include/pipewire-0.3/pipewire/utils.h \
  /usr/include/spa-0.2/spa/pod/pod.h \
  /usr/include/spa-0.2/spa/utils/type.h \
  /usr/include/pipewire-0.3/pipewire/permission.h \
  /usr/include/pipewire-0.3/pipewire/conf.h \
  /usr/include/pipewire-0.3/pipewire/device.h \
  /usr/include/pipewire-0.3/pipewire/buffers.h \
  /usr/include/spa-0.2/spa/node/node.h \
  /usr/include/spa-0.2/spa/buffer/buffer.h \
  /usr/include/spa-0.2/spa/buffer/meta.h \
  /usr/include/spa-0.2/spa/node/event.h \
  /usr/include/spa-0.2/spa/pod/event.h \
  /usr/include/spa-0.2/spa/node/command.h \
  /usr/include/spa-0.2/spa/pod/command.h \
  /usr/include/pipewire-0.3/pipewire/mem.h \
  /usr/include/pipewire-0.3/pipewire/factory.h \
  /usr/include/pipewire-0.3/pipewire/keys.h \
  /usr/include/pipewire-0.3/pipewire/log.h \
  /usr/include/spa-0.2/spa/support/log.h \
  /usr/include/pipewire-0.3/pipewire/link.h \
  /usr/include/pipewire-0.3/pipewire/main-loop.h \
  /usr/include/pipewire-0.3/pipewire/map.h \
  /usr/include/pipewire-0.3/pipewire/module.h \
  /usr/include/pipewire-0.3/pipewire/node.h \
  /usr/include/pipewire-0.3/pipewire/port.h \
  /usr/include/pipewire-0.3/pipewire/stream.h \
  /usr/include/pipewire-0.3/pipewire/filter.h \
  /usr/include/spa-0.2/spa/node/io.h \
  /usr/include/pipewire-0.3/pipewire/thread-loop.h \
  /usr/include/pipewire-0.3/pipewire/data-loop.h \
  /usr/include/spa-0.2/spa/support/thread.h \
  /usr/include/pipewire-0.3/pipewire/type.h \
  /usr/include/pipewire-0.3/pipewire/version.h src/had/had_interface.h
	clang++-14 -g -std=c++20 -c src/setup.cpp -I/usr/include/pipewire-0.3 -I/usr/include/spa-0.2 -D_REENTRANT -o objects/setup.o

objects/spectre.o: src/spectre.cpp src/spectre.h src/had/had.h src/had/had_keys.h \
  src/had/had_types.h src/had/had_audio.h src/had/audioFile.h \
  src/had/had_logger.h /usr/include/pipewire-0.3/pipewire/pipewire.h \
  /usr/include/spa-0.2/spa/support/plugin.h \
  /usr/include/spa-0.2/spa/utils/defs.h \
  /usr/include/spa-0.2/spa/utils/dict.h \
  /usr/include/pipewire-0.3/pipewire/array.h \
  /usr/include/pipewire-0.3/pipewire/client.h \
  /usr/include/spa-0.2/spa/param/param.h \
  /usr/include/spa-0.2/spa/param/buffers.h \
  /usr/include/spa-0.2/spa/param/profile.h \
  /usr/include/spa-0.2/spa/param/port-config.h \
  /usr/include/spa-0.2/spa/param/route.h \
  /usr/include/pipewire-0.3/pipewire/proxy.h \
  /usr/include/spa-0.2/spa/utils/hook.h \
  /usr/include/spa-0.2/spa/utils/list.h \
  /usr/include/pipewire-0.3/pipewire/protocol.h \
  /usr/include/pipewire-0.3/pipewire/context.h \
  /usr/include/pipewire-0.3/pipewire/core.h \
  /usr/include/pipewire-0.3/pipewire/properties.h \
  /usr/include/spa-0.2/spa/utils/string.h \
  /usr/include/pipewire-0.3/pipewire/loop.h \
  /usr/include/spa-0.2/spa/support/loop.h \
  /usr/include/spa-0.2/spa/support/system.h \
  /usr/include/pipewire-0.3/pipewire/utils.h \
  /usr/include/spa-0.2/spa/pod/pod.h \
  /usr/include/spa-0.2/spa/utils/type.h \
  /usr/include/pipewire-0.3/pipewire/permission.h \
  /usr/include/pipewire-0.3/pipewire/conf.h \
  /usr/include/pipewire-0.3/pipewire/device.h \
  /usr/include/pipewire-0.3/pipewire/buffers.h \
  /usr/include/spa-0.2/spa/node/node.h \
  /usr/include/spa-0.2/spa/buffer/buffer.h \
  /usr/include/spa-0.2/spa/buffer/meta.h \
  /usr/include/spa-0.2/spa/node/event.h \
  /usr/include/spa-0.2/spa/pod/event.h \
  /usr/include/spa-0.2/spa/node/command.h \
  /usr/include/spa-0.2/spa/pod/command.h \
  /usr/include/pipewire-0.3/pipewire/mem.h \
  /usr/include/pipewire-0.3/pipewire/factory.h \
  /usr/include/pipewire-0.3/pipewire/keys.h \
  /usr/include/pipewire-0.3/pipewire/log.h \
  /usr/include/spa-0.2/spa/support/log.h \
  /usr/include/pipewire-0.3/pipewire/link.h \
  /usr/include/pipewire-0.3/pipewire/main-loop.h \
  /usr/include/pipewire-0.3/pipewire/map.h \
  /usr/include/pipewire-0.3/pipewire/module.h \
  /usr/include/pipewire-0.3/pipewire/node.h \
  /usr/include/pipewire-0.3/pipewire/port.h \
  /usr/include/pipewire-0.3/pipewire/stream.h \
  /usr/include/pipewire-0.3/pipewire/filter.h \
  /usr/include/spa-0.2/spa/node/io.h \
  /usr/include/pipewire-0.3/pipewire/thread-loop.h \
  /usr/include/pipewire-0.3/pipewire/data-loop.h \
  /usr/include/spa-0.2/spa/support/thread.h \
  /usr/include/pipewire-0.3/pipewire/type.h \
  /usr/include/pipewire-0.3/pipewire/version.h src/had/had_interface.h \
  src/Fourier_transform.h
	clang++-14 -g -std=c++20 -c src/spectre.cpp -I/usr/include/pipewire-0.3 -I/usr/include/spa-0.2 -D_REENTRANT -o objects/spectre.o

objects/switchPanel.o: src/switchPanel.cpp src/switchPanel.h src/had/had.h \
  src/had/had_keys.h src/had/had_types.h src/had/had_audio.h \
  src/had/audioFile.h src/had/had_logger.h \
  /usr/include/pipewire-0.3/pipewire/pipewire.h \
  /usr/include/spa-0.2/spa/support/plugin.h \
  /usr/include/spa-0.2/spa/utils/defs.h \
  /usr/include/spa-0.2/spa/utils/dict.h \
  /usr/include/pipewire-0.3/pipewire/array.h \
  /usr/include/pipewire-0.3/pipewire/client.h \
  /usr/include/spa-0.2/spa/param/param.h \
  /usr/include/spa-0.2/spa/param/buffers.h \
  /usr/include/spa-0.2/spa/param/profile.h \
  /usr/include/spa-0.2/spa/param/port-config.h \
  /usr/include/spa-0.2/spa/param/route.h \
  /usr/include/pipewire-0.3/pipewire/proxy.h \
  /usr/include/spa-0.2/spa/utils/hook.h \
  /usr/include/spa-0.2/spa/utils/list.h \
  /usr/include/pipewire-0.3/pipewire/protocol.h \
  /usr/include/pipewire-0.3/pipewire/context.h \
  /usr/include/pipewire-0.3/pipewire/core.h \
  /usr/include/pipewire-0.3/pipewire/properties.h \
  /usr/include/spa-0.2/spa/utils/string.h \
  /usr/include/pipewire-0.3/pipewire/loop.h \
  /usr/include/spa-0.2/spa/support/loop.h \
  /usr/include/spa-0.2/spa/support/system.h \
  /usr/include/pipewire-0.3/pipewire/utils.h \
  /usr/include/spa-0.2/spa/pod/pod.h \
  /usr/include/spa-0.2/spa/utils/type.h \
  /usr/include/pipewire-0.3/pipewire/permission.h \
  /usr/include/pipewire-0.3/pipewire/conf.h \
  /usr/include/pipewire-0.3/pipewire/device.h \
  /usr/include/pipewire-0.3/pipewire/buffers.h \
  /usr/include/spa-0.2/spa/node/node.h \
  /usr/include/spa-0.2/spa/buffer/buffer.h \
  /usr/include/spa-0.2/spa/buffer/meta.h \
  /usr/include/spa-0.2/spa/node/event.h \
  /usr/include/spa-0.2/spa/pod/event.h \
  /usr/include/spa-0.2/spa/node/command.h \
  /usr/include/spa-0.2/spa/pod/command.h \
  /usr/include/pipewire-0.3/pipewire/mem.h \
  /usr/include/pipewire-0.3/pipewire/factory.h \
  /usr/include/pipewire-0.3/pipewire/keys.h \
  /usr/include/pipewire-0.3/pipewire/log.h \
  /usr/include/spa-0.2/spa/support/log.h \
  /usr/include/pipewire-0.3/pipewire/link.h \
  /usr/include/pipewire-0.3/pipewire/main-loop.h \
  /usr/include/pipewire-0.3/pipewire/map.h \
  /usr/include/pipewire-0.3/pipewire/module.h \
  /usr/include/pipewire-0.3/pipewire/node.h \
  /usr/include/pipewire-0.3/pipewire/port.h \
  /usr/include/pipewire-0.3/pipewire/stream.h \
  /usr/include/pipewire-0.3/pipewire/filter.h \
  /usr/include/spa-0.2/spa/node/io.h \
  /usr/include/pipewire-0.3/pipewire/thread-loop.h \
  /usr/include/pipewire-0.3/pipewire/data-loop.h \
  /usr/include/spa-0.2/spa/support/thread.h \
  /usr/include/pipewire-0.3/pipewire/type.h \
  /usr/include/pipewire-0.3/pipewire/version.h src/had/had_interface.h
	clang++-14 -g -std=c++20 -c src/switchPanel.cpp -I/usr/include/pipewire-0.3 -I/usr/include/spa-0.2 -D_REENTRANT -o objects/switchPanel.o

objects/audioFile.o: src/had/audioFile.cpp src/had/audioFile.h \
  src/had/had_logger.h src/had/had_types.h
	clang++-14 -g -std=c++20 -c src/had/audioFile.cpp -I/usr/include/pipewire-0.3 -I/usr/include/spa-0.2 -D_REENTRANT -o objects/audioFile.o

objects/had_audio.o: src/had/had_audio.cpp src/had/audioFile.h \
  src/had/had_logger.h src/had/had_types.h src/had/had_audio.h \
  /usr/include/pipewire-0.3/pipewire/pipewire.h \
  /usr/include/spa-0.2/spa/support/plugin.h \
  /usr/include/spa-0.2/spa/utils/defs.h \
  /usr/include/spa-0.2/spa/utils/dict.h \
  /usr/include/pipewire-0.3/pipewire/array.h \
  /usr/include/pipewire-0.3/pipewire/client.h \
  /usr/include/spa-0.2/spa/param/param.h \
  /usr/include/spa-0.2/spa/param/buffers.h \
  /usr/include/spa-0.2/spa/param/profile.h \
  /usr/include/spa-0.2/spa/param/port-config.h \
  /usr/include/spa-0.2/spa/param/route.h \
  /usr/include/pipewire-0.3/pipewire/proxy.h \
  /usr/include/spa-0.2/spa/utils/hook.h \
  /usr/include/spa-0.2/spa/utils/list.h \
  /usr/include/pipewire-0.3/pipewire/protocol.h \
  /usr/include/pipewire-0.3/pipewire/context.h \
  /usr/include/pipewire-0.3/pipewire/core.h \
  /usr/include/pipewire-0.3/pipewire/properties.h \
  /usr/include/spa-0.2/spa/utils/string.h \
  /usr/include/pipewire-0.3/pipewire/loop.h \
  /usr/include/spa-0.2/spa/support/loop.h \
  /usr/include/spa-0.2/spa/support/system.h \
  /usr/include/pipewire-0.3/pipewire/utils.h \
  /usr/include/spa-0.2/spa/pod/pod.h \
  /usr/include/spa-0.2/spa/utils/type.h \
  /usr/include/pipewire-0.3/pipewire/permission.h \
  /usr/include/pipewire-0.3/pipewire/conf.h \
  /usr/include/pipewire-0.3/pipewire/device.h \
  /usr/include/pipewire-0.3/pipewire/buffers.h \
  /usr/include/spa-0.2/spa/node/node.h \
  /usr/include/spa-0.2/spa/buffer/buffer.h \
  /usr/include/spa-0.2/spa/buffer/meta.h \
  /usr/include/spa-0.2/spa/node/event.h \
  /usr/include/spa-0.2/spa/pod/event.h \
  /usr/include/spa-0.2/spa/node/command.h \
  /usr/include/spa-0.2/spa/pod/command.h \
  /usr/include/pipewire-0.3/pipewire/mem.h \
  /usr/include/pipewire-0.3/pipewire/factory.h \
  /usr/include/pipewire-0.3/pipewire/keys.h \
  /usr/include/pipewire-0.3/pipewire/log.h \
  /usr/include/spa-0.2/spa/support/log.h \
  /usr/include/pipewire-0.3/pipewire/link.h \
  /usr/include/pipewire-0.3/pipewire/main-loop.h \
  /usr/include/pipewire-0.3/pipewire/map.h \
  /usr/include/pipewire-0.3/pipewire/module.h \
  /usr/include/pipewire-0.3/pipewire/node.h \
  /usr/include/pipewire-0.3/pipewire/port.h \
  /usr/include/pipewire-0.3/pipewire/stream.h \
  /usr/include/pipewire-0.3/pipewire/filter.h \
  /usr/include/spa-0.2/spa/node/io.h \
  /usr/include/pipewire-0.3/pipewire/thread-loop.h \
  /usr/include/pipewire-0.3/pipewire/data-loop.h \
  /usr/include/spa-0.2/spa/support/thread.h \
  /usr/include/pipewire-0.3/pipewire/type.h \
  /usr/include/pipewire-0.3/pipewire/version.h \
  /usr/include/spa-0.2/spa/param/audio/raw.h \
  /usr/include/spa-0.2/spa/param/audio/format-utils.h \
  /usr/include/spa-0.2/spa/pod/parser.h \
  /usr/include/spa-0.2/spa/pod/iter.h \
  /usr/include/spa-0.2/spa/pod/vararg.h \
  /usr/include/spa-0.2/spa/pod/builder.h \
  /usr/include/spa-0.2/spa/param/audio/format.h \
  /usr/include/spa-0.2/spa/param/format.h \
  /usr/include/spa-0.2/spa/param/audio/dsp.h \
  /usr/include/spa-0.2/spa/param/audio/iec958.h \
  /usr/include/spa-0.2/spa/param/audio/dsd.h \
  /usr/include/spa-0.2/spa/param/audio/mp3.h \
  /usr/include/spa-0.2/spa/param/audio/aac.h \
  /usr/include/spa-0.2/spa/param/audio/vorbis.h \
  /usr/include/spa-0.2/spa/param/audio/wma.h \
  /usr/include/spa-0.2/spa/param/audio/ra.h \
  /usr/include/spa-0.2/spa/param/audio/amr.h \
  /usr/include/spa-0.2/spa/param/audio/alac.h \
  /usr/include/spa-0.2/spa/param/audio/flac.h \
  /usr/include/spa-0.2/spa/param/audio/ape.h \
  /usr/include/spa-0.2/spa/param/format-utils.h \
  /usr/include/spa-0.2/spa/param/audio/raw-utils.h \
  /usr/include/spa-0.2/spa/param/audio/dsp-utils.h \
  /usr/include/spa-0.2/spa/param/audio/iec958-utils.h \
  /usr/include/spa-0.2/spa/param/audio/dsd-utils.h \
  /usr/include/spa-0.2/spa/param/audio/mp3-utils.h \
  /usr/include/spa-0.2/spa/param/audio/aac-utils.h \
  /usr/include/spa-0.2/spa/param/audio/vorbis-utils.h \
  /usr/include/spa-0.2/spa/param/audio/wma-utils.h \
  /usr/include/spa-0.2/spa/param/audio/ra-utils.h \
  /usr/include/spa-0.2/spa/param/audio/amr-utils.h \
  /usr/include/spa-0.2/spa/param/audio/alac-utils.h \
  /usr/include/spa-0.2/spa/param/audio/flac-utils.h \
  /usr/include/spa-0.2/spa/param/audio/ape-utils.h \
  /usr/include/spa-0.2/spa/param/props.h
	clang++-14 -g -std=c++20 -c src/had/had_audio.cpp -I/usr/include/pipewire-0.3 -I/usr/include/spa-0.2 -D_REENTRANT -o objects/had_audio.o

objects/had_interface.o: src/had/had_interface.cpp src/had/had_interface.h \
  src/had/had_keys.h src/had/had_logger.h src/had/had_types.h
	clang++-14 -g -std=c++20 -c src/had/had_interface.cpp -I/usr/include/pipewire-0.3 -I/usr/include/spa-0.2 -D_REENTRANT -o objects/had_interface.o


comp: objects/app.o objects/config.o objects/eventQueue.o objects/fileManager.o objects/Fourier_transform.o objects/main.o objects/player.o objects/setup.o objects/spectre.o objects/switchPanel.o objects/audioFile.o objects/had_audio.o objects/had_interface.o
	clang++-14 -g -std=c++20 objects/app.o objects/config.o objects/eventQueue.o objects/fileManager.o objects/Fourier_transform.o objects/main.o objects/player.o objects/setup.o objects/spectre.o objects/switchPanel.o objects/audioFile.o objects/had_audio.o objects/had_interface.o -lsndfile -lmpg123 -lasound -lncursesw -I/usr/include/pipewire-0.3 -I/usr/include/spa-0.2 -D_REENTRANT -lpipewire-0.3 -o execs/output.out

run:
	alacritty -e execs/output.out

clear:
	cd objects; rm *.o

car: comp run


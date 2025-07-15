# Base image with Emscripten SDK
FROM emscripten/emsdk:latest

# Install CMake 3.27+ manually (required by raylib)
ADD https://github.com/Kitware/CMake/releases/download/v3.27.0/cmake-3.27.0-linux-x86_64.tar.gz /tmp/
RUN tar -xzf /tmp/cmake-3.27.0-linux-x86_64.tar.gz -C /opt \
 && ln -s /opt/cmake-3.27.0-linux-x86_64/bin/* /usr/local/bin \
 && rm /tmp/cmake-3.27.0-linux-x86_64.tar.gz

# Install perl (required for HTML cleanup using regex)
RUN apt-get update && apt-get install -y perl

# Copy project into container
WORKDIR /app
COPY . .

# Initialize git submodules (e.g. raylib, raygui)
RUN git submodule update --init --recursive

# Build project
WORKDIR /app/build
RUN emcmake cmake .. -DCMAKE_BUILD_TYPE=MinSizeRel \
 && cmake --build . --parallel

# Clean up generated HTML
RUN sed -i 's|<span><input type=checkbox id=resize>Resize canvas</span>||g' webnoise.html \
 && sed -i "s|<span><input type=button onclick='Module.requestFullscreen(document.getElementById(\"pointerLock\").checked,document.getElementById(\"resize\").checked)'value=Fullscreen></span>||g" webnoise.html

# Change background color
RUN sed -i 's/\(body{[^}]*padding:none\)/\1;background-color: #181a1b/' webnoise.html

# Collect final output artifacts
RUN mkdir -p /out \
 && cp webnoise.wasm /out/ \
 && cp webnoise.js /out/ \
 && cp webnoise.html /out/index.html
